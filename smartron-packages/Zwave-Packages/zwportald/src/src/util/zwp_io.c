/** @copyright
© 2014 SIGMA DESIGNS, INC. THIS IS AN UNPUBLISHED WORK PROTECTED BY SIGMA 
DESIGNS, INC. AS A TRADE SECRET, AND IS NOT TO BE USED OR DISCLOSED EXCEPT AS 
PROVIDED Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. ALL 
RIGHTS RESERVED.
@par
NOTICE: ALL INFORMATION CONTAINED HEREIN IS CONFIDENTIAL AND/OR PROPRIETARY TO 
SIGMA DESIGNS AND MAY BE COVERED BY U.S. AND FOREIGN PATENTS, PATENTS IN 
PROCESS, AND ARE PROTECTED BY TRADE SECRET OR COPYRIGHT LAW. DISSEMINATION OR 
REPRODUCTION OF THE SOURCE CODE CONTAINED HEREIN IS EXPRESSLY FORBIDDEN TO 
ANYONE EXCEPT LICENSEES OF SIGMA DESIGNS  WHO HAVE EXECUTED A SIGMA DESIGNS’ 
Z-WAVE CONTROLLER DEVELOPMENT KIT LIMITED LICENSE AGREEMENT. THE COPYRIGHT 
NOTICE ABOVE IS NOT EVIDENCE OF ANY ACTUAL OR INTENDED PUBLICATION OF THE 
SOURCE CODE. THE RECEIPT OR POSSESSION OF  THIS SOURCE CODE AND/OR RELATED 
INFORMATION DOES NOT CONVEY OR IMPLY ANY RIGHTS  TO REPRODUCE, DISCLOSE OR 
DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL A PRODUCT THAT IT  MAY 
DESCRIBE.
@par
THE SIGMA PROGRAM AND ANY RELATED DOCUMENTATION OR TOOLS IS PROVIDED TO COMPANY 
"AS IS" AND "WITH ALL FAULTS", WITHOUT WARRANTY OF ANY KIND FROM SIGMA. COMPANY 
ASSUMES ALL RISKS THAT LICENSED MATERIALS ARE SUITABLE OR ACCURATE FOR 
COMPANY’S NEEDS AND COMPANY’S USE OF THE SIGMA PROGRAM IS AT COMPANY’S 
OWN DISCRETION AND RISK. SIGMA DOES NOT GUARANTEE THAT THE USE OF THE SIGMA 
PROGRAM IN A THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES ENVIRONMENT WILL 
BE: (A) PERFORMED ERROR-FREE OR UNINTERRUPTED; (B) THAT SIGMA WILL CORRECT ANY 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT ERRORS; (C) THE 
THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICE ENVIRONMENT WILL OPERATE IN 
COMBINATION WITH COMPANY’S CONTENT OR COMPANY APPLICATIONS THAT UTILIZE THE 
SIGMA PROGRAM; (D) OR WITH ANY OTHER HARDWARE, SOFTWARE, SYSTEMS, SERVICES OR 
DATA NOT PROVIDED BY SIGMA. COMPANY ACKNOWLEDGES THAT SIGMA DOES NOT CONTROL 
THE TRANSFER OF DATA OVER COMMUNICATIONS FACILITIES, INCLUDING THE INTERNET, 
AND THAT THE SERVICES MAY BE SUBJECT TO LIMITATIONS, DELAYS, AND OTHER PROBLEMS 
INHERENT IN THE USE OF SUCH COMMUNICATIONS FACILITIES. SIGMA IS NOT RESPONSIBLE 
FOR ANY DELAYS, DELIVERY FAILURES, OR OTHER DAMAGE RESULTING FROM SUCH ISSUES. 
SIGMA IS NOT RESPONSIBLE FOR ANY ISSUES RELATED TO THE PERFORMANCE, OPERATION 
OR SECURITY OF THE THIRD PARTY SERVICE ENVIRONMENT OR CLOUD SERVICES 
ENVIRONMENT THAT ARISE FROM COMPANY CONTENT, COMPANY APPLICATIONS OR THIRD 
PARTY CONTENT. SIGMA DOES NOT MAKE ANY REPRESENTATION OR WARRANTY REGARDING THE 
RELIABILITY, ACCURACY, COMPLETENESS, CORRECTNESS, OR USEFULNESS OF THIRD PARTY 
CONTENT OR SERVICE OR THE SIGMA PROGRAM, AND DISCLAIMS ALL LIABILITIES ARISING 
FROM OR RELATED TO THE SIGMA PROGRAM OR THIRD PARTY CONTENT OR SERVICES. TO THE 
EXTENT NOT PROHIBITED BY LAW, THESE WARRANTIES ARE EXCLUSIVE. SIGMA OFFERS NO 
WARRANTY OF NON-INFRINGEMENT, TITLE, OR QUIET ENJOYMENT. NEITHER SIGMA NOR ITS 
SUPPLIERS OR LICENSORS SHALL BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL OR 
CONSEQUENTIAL DAMAGES OR LOSS (INCLUDING DAMAGES FOR LOSS OF BUSINESS, LOSS OF 
PROFITS, OR THE LIKE), ARISING OUT OF THIS AGREEMENT WHETHER BASED ON BREACH OF 
CONTRACT, INTELLECTUAL PROPERTY INFRINGEMENT, TORT (INCLUDING NEGLIGENCE), 
STRICT LIABILITY, PRODUCT LIABILITY OR OTHERWISE, EVEN IF SIGMA OR ITS 
REPRESENTATIVES HAVE BEEN ADVISED OF OR OTHERWISE SHOULD KNOW ABOUT THE 
POSSIBILITY OF SUCH DAMAGES. THERE ARE NO OTHER EXPRESS OR IMPLIED WARRANTIES 
OR CONDITIONS INCLUDING FOR SOFTWARE, HARDWARE, SYSTEMS, NETWORKS OR 
ENVIRONMENTS OR FOR MERCHANTABILITY, NONINFRINGEMENT, SATISFACTORY QUALITY AND 
FITNESS FOR A PARTICULAR PURPOSE.
@par
The Sigma Program  is not fault-tolerant and is not designed, manufactured or 
intended for use or resale as on-line control equipment in hazardous 
environments requiring fail-safe performance, such as in the operation of 
nuclear facilities, aircraft navigation or communication systems, air traffic 
control, direct life support machines, or weapons systems, in which the failure 
of the Sigma Program, or Company Applications created using the Sigma Program, 
could lead directly to death, personal injury, or severe physical or 
environmental damage ("High Risk Activities").  Sigma and its suppliers 
specifically disclaim any express or implied warranty of fitness for High Risk 
Activities.Without limiting Sigma’s obligation of confidentiality as further 
described in the Z-Wave Controller Development Kit Limited License Agreement, 
Sigma has no obligation to establish and maintain a data privacy and 
information security program with regard to Company’s use of any Third Party 
Service Environment or Cloud Service Environment. For the avoidance of doubt, 
Sigma shall not be responsible for physical, technical, security, 
administrative, and/or organizational safeguards that are designed to ensure 
the security and confidentiality of the Company Content or Company Application 
in any Third Party Service Environment or Cloud Service Environment that 
Company chooses to utilize.
*/
/**
 * @file    zwp_io.c
 *
 * @brief   I/O functions
 * @details Handles Input/Output functions.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-14
 * - Initial version
 */

#include <stdio.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_IO)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_io.h"
#include "zwp_memory.h"
#include "zwp_error.h"

zwp_status_t zwp_file_name_is_safe(const char *file_name)
{
	const char *ptr;
	size_t file_name_length;

	file_name_length = strlen(file_name);

	ptr = file_name;

	/*
	 * Search for the following unsafe names
	 * ..
	 * /..
	 * ../
	 * /../
	 */
	while ((ptr = strstr(ptr, "..")))
	{
		if (((ptr <= file_name) || (*(ptr - 1) == '/'))
				&& (((ptr + 2) >= (file_name + file_name_length)) || (*(ptr + 2) == '/')))
		{
			ZWP_LOG(ZWP_LOG_ERR, "File name '%s' is unsafe", file_name);

			return ZWP_STATUS_ERROR;
		}

		ptr++;
	}

	return ZWP_STATUS_OK;
}

zwp_status_t zwp_read_from_FILE_with_dest_alloc(FILE *file, char **dest_p, int *length, int block_size_hint)
{
	char *dest, *tmp;
	int read;
	int size;
	int bytes;

	bytes = 0;

	dest = NULL;
	read = size = 0;

	if (!block_size_hint)
	{
		block_size_hint = ZWP_FILE_READ_DEFAULT_BLOCK_SIZE;
	}

	while(1)
	{
		size += block_size_hint;

		if ((tmp = (char *)ZWP_REALLOC(dest, size)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_realloc;
		}
		dest = tmp;

		ZWP_LOG(ZWP_LOG_DEBUG, "Before: size: %d read: %d bytes: %d", size, read, bytes);
		if ((bytes = fread(dest + read, 1, block_size_hint, file)) != block_size_hint)
		{
			if (ferror(file))
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to read file: %s", strerror(errno));
				goto l_err_fread;
			}
			else if (feof(file))
			{
				read += bytes;
				break;
			}
			// else: unlikely
		}

		read += bytes;
		ZWP_LOG(ZWP_LOG_DEBUG, "After : size: %d read: %d bytes: %d", size, read, bytes);
	}

	ZWP_LOG(ZWP_LOG_DEBUG, "Final : size: %d read: %d bytes: %d", size, read, bytes);
	dest[read] = '\0';

	*dest_p = dest;
	*length = read;

	return ZWP_STATUS_OK;

l_err_fread:
l_err_realloc:
	ZWP_FREE(dest);
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_read_from_file_path_with_dest_alloc(const char *path, char **dest_p, int *length, int block_size_hint)
{
	FILE *file;
	char *dest;

	if ((file = fopen(path, "rb")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to open file '%s' to read: %s", path, strerror(errno));
		goto l_err_fopen;
	}

	// 'Try' setting block size to the size of the file
	if (!block_size_hint)
	{
		if (fseek(file, 0L, SEEK_END) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to seek file '%s' to the end: %s", path, strerror(errno));
		}
		else
		{
			if ((block_size_hint = ftell(file)) == -1)
			{
				block_size_hint = 0;
			}
			else
			{
				// Extra byte for optional NULL termination
				block_size_hint++;
			}

			if (fseek(file, 0L, SEEK_SET) != 0)
			{
				ZWP_LOG(ZWP_LOG_ERR, "Failed to seek file '%s' back to the top: %s", path, strerror(errno));
				goto l_err_fseek;
			}
		}
	}

	if (zwp_read_from_FILE_with_dest_alloc(file, &dest, length, block_size_hint) != ZWP_STATUS_OK)
	{
		goto l_err_read;
	}

	if (fclose(file) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to close file '%s': %s", path, strerror(errno));
		goto l_err_fclose;
	}

	*dest_p = dest;

	return ZWP_STATUS_OK;

l_err_fclose:
l_err_read:
l_err_fseek:
	fclose(file);
l_err_fopen:
	return ZWP_STATUS_ERROR;
}

void zwp_read_dest_free(char *dest)
{
	ZWP_FREE(dest);
}

zwp_status_t zwu_write_to_FILE(FILE *file, const char *src, int length)
{
	int bytes;

	if ((bytes = fwrite(src, 1, length, file)) != length)
	{
		if (ferror(file))
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to write file: %s", strerror(errno));
			goto l_err_fwrite;
		}
		// else: unlikely
	}

	return ZWP_STATUS_OK;

l_err_fwrite:
	return ZWP_STATUS_ERROR;
}


zwp_status_t zwu_write_to_file_path(const char *path, const char *src, int length)
{
	FILE *file;
	int file_desc;

	if ((file = fopen(path, "wb")) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to open file '%s' to write: %s", path, strerror(errno));
		goto l_err_fopen;
	}

	if (zwu_write_to_FILE(file, src, length) != ZWP_STATUS_OK)
	{
		goto l_err_write;
	}

	//Flush the C buffers before calling fsync
	if (fflush(file) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to fflush file '%s': %s", path, strerror(errno));
		goto l_err_fclose;
	}

	//Flush to physical storage
	file_desc = fileno(file);

	if (file_desc < 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to get fileno '%s': %s", path, strerror(errno));
		goto l_err_fclose;
	}

	if (fsync(file_desc) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to fsync file '%s': %s", path, strerror(errno));
		goto l_err_fclose;
	}

	if (fclose(file) != 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "Failed to close file '%s': %s", path, strerror(errno));
		goto l_err_fclose;
	}

	return ZWP_STATUS_OK;

l_err_fclose:
l_err_write:
	fclose(file);
l_err_fopen:
	return ZWP_STATUS_ERROR;
}

