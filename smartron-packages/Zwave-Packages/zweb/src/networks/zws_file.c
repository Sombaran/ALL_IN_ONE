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
 * @file    zws_file.c
 *
 * @brief   Files
 * @details Handles the the files.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2014-02-12
 * - Initial version
 */

#include <sys/stat.h>

#include "zws_file.h"
#include "zws_log.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_io.h"

/** Root directory path relative to install path */
#define ZWS_ROOT_DIR_RELATIVE_TO_INSTALL_PATH               ".."

/** Host controller application version file name */
#define ZWS_HOST_CONTROLLER_APPLICATION_VERSION_FILE        "VERSION"
/** Engineering UI version file */
#define ZWS_ENGINEERING_UI_VERSION_FILE                     "install/zweb/htdocs/ui/eng/VERSION"
/** TV/STB UI version file */
#define ZWS_TV_UI_VERSION_FILE                              "install/zweb/htdocs/ui/stb/VERSION"
/** PC/Tablet UI version file */
#define ZWS_PC_UI_VERSION_FILE                              "install/zweb/htdocs/ui/pc/VERSION"
/** Phone UI version file */
#define ZWS_PHONE_UI_VERSION_FILE                           "install/zweb/htdocs/ui/phone/VERSION"

/**
 * @brief Get file content for a given file path
 * @param[in]     root_dir_path     Root directory path
 * @param[in]     file_name         File name
 * @param[out]    file_content_p    Content of the file
 * @param[out]    read_p            Number of bytes read
 * @retval        ZWS_STATUS_OK     Success
 * @retval        ZWS_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zws_file_free_file_content() to free resources
 *
 * @par
 * On successful return, the function guarantees at least (read_p + 1) bytes
 * allocated for dest_p. This extra byte is set to '\0'.
 */
static zws_status_t zws_file_get_file_content(char *root_dir_path, char *file_name, char **file_content_p, int *read_p)
{
	char *file_path;
	char *file_content;
	int read;

	if ((file_path = (char *)ZWS_MALLOC(strlen(root_dir_path) + 1
			+ strlen(file_name) + 1)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_version_path_alloc;
	}
	sprintf(file_path, "%s/%s", root_dir_path, file_name);

	if (zws_read_from_file_path_with_dest_alloc(file_path, &file_content, &read, 0) != ZWS_STATUS_OK)
	{
		goto l_err_read_file;
	}

	ZWS_FREE(file_path);

	*file_content_p = file_content;
	*read_p = read;

	return ZWS_STATUS_OK;

	zws_read_dest_free(file_content);
l_err_read_file:
	ZWS_FREE(file_path);
l_err_version_path_alloc:
	return ZWS_STATUS_ERROR;
}

/**
 * @brief Free the content read from a file
 * @param[in]     file_content      File content
 * @return        Nothing
 * @see zws_file_get_file_content
 */
static void zws_file_free_file_content(char *file_content)
{
	zws_read_dest_free(file_content);
}

/*
 * @note Free major_p and minor_p using zws_free_host_controller_application_version
 */
zws_status_t zws_file_get_host_controller_application_version(char *root_dir_path, char **major_p, char **minor_p)
{
	char *file_content;
	int read;
	char *entry;
	char *next_entry;
	char *key;
	char *value;
	char *major;
	char *minor;

	if (zws_file_get_file_content(root_dir_path, ZWS_HOST_CONTROLLER_APPLICATION_VERSION_FILE,
			&file_content, &read) != ZWS_STATUS_OK)
	{
		goto l_err_get_file_content;
	}

	major = NULL;
	minor = NULL;

	for (entry = file_content; entry && entry[0]; entry = next_entry)
	{
		if ((next_entry = strstr(entry, "\r\n")))
		{
			*next_entry = '\0';
			next_entry += 2;
		}
		else if ((next_entry = strstr(entry, "\n")))
		{
			*next_entry = '\0';
			next_entry++;
		}

		ZWS_LOG(ZWS_LOG_DEBUG, "Entry: %s", entry);

		//Split key and value from entry
		key = entry;

		if (!(value = strstr(key, "=")))
		{
			ZWS_LOG(ZWS_LOG_WARNING, "Unable to find key-value delimiter in the entry: %s", entry);
			continue;
		}

		*value='\0';
		value++;

		if (!key[0])
		{
			ZWS_LOG(ZWS_LOG_WARNING, "Empty key in the entry: =%s", value);
			continue;
		}

		if (!strcmp(key, "MAJOR"))
		{
			/* Handle only the first instance, else will run multiple strdup */
			if (major == NULL)
			{
				if ((major = ZWS_STRDUP(value)) == NULL)
				{
					ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
					goto l_err_major_alloc;
				}
			}
		}
		else if (!strcmp(key, "MINOR"))
		{
			/* Handle only the first instance, else will run multiple strdup */
			if (minor == NULL)
			{
				if ((minor = ZWS_STRDUP(value)) == NULL)
				{
					ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
					goto l_err_minor_alloc;
				}
			}
		}
	}

	*major_p = major;
	*minor_p = minor;

	zws_file_free_file_content(file_content);

	return ZWS_STATUS_OK;

l_err_minor_alloc:
l_err_major_alloc:
	ZWS_FREE(minor);
	ZWS_FREE(major);
	zws_file_free_file_content(file_content);
l_err_get_file_content:
	return ZWS_STATUS_ERROR;
}

void zws_file_free_host_controller_application_version(char *major, char *minor)
{
	ZWS_FREE(minor);
	ZWS_FREE(major);
}

/*
 * @note Free major_p and minor_p using zws_free_host_controller_application_version
 */
zws_status_t zws_file_get_ui_version(char *root_dir_path, zws_ui_type_t ui_type, char **version_p)
{
	char *file_path;
	char *file_content;
	int read;
	char *start;
	char *end;
	char *version;

	switch (ui_type)
	{
	case ZWS_UI_ENGINEERING:
		file_path = ZWS_ENGINEERING_UI_VERSION_FILE;
		break;
	case ZWS_UI_TV:
		file_path = ZWS_TV_UI_VERSION_FILE;
		break;
	case ZWS_UI_PC:
		file_path = ZWS_PC_UI_VERSION_FILE;
		break;
	case ZWS_UI_PHONE:
		file_path = ZWS_PHONE_UI_VERSION_FILE;
		break;
	default:
		goto l_err_ui_type_no_match;
	}

	if (zws_file_get_file_content(root_dir_path, file_path,
			&file_content, &read) != ZWS_STATUS_OK)
	{
		goto l_err_get_file_content;
	}

	start = file_content;

	if ((end = strstr(start, "\r\n")))
	{
		*end = '\0';
	}
	else if ((end = strstr(start, "\n")))
	{
		*end = '\0';
	}

	if ((version = ZWS_STRDUP(start)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_version_alloc;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "Version: %s", version);

	*version_p = version;

	zws_file_free_file_content(file_content);

	return ZWS_STATUS_OK;

	ZWS_FREE(version);
l_err_version_alloc:
	zws_file_free_file_content(file_content);
l_err_get_file_content:
l_err_ui_type_no_match:
	return ZWS_STATUS_ERROR;
}

void zws_file_free_ui_version(char *version)
{
	ZWS_FREE(version);
}

/*
 * @note Call zws_free_root_dir() to free root_dir path
 */
zws_status_t zws_file_get_root_dir_path(char **root_dir_path_p)
{
	char *document_root;
	char *root_dir_path;
	struct stat stat_info;

	// Discover schema from ZWARE_INSTALL_PATH env variable
	if ((document_root = getenv("ZWARE_INSTALL_PATH")) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Failed to get ZWARE_INSTALL_PATH env variable");
		goto l_err_get_install_path;
	}

	ZWS_LOG(ZWS_LOG_DEBUG, "ZWARE_INSTALL_PATH from environment variable: %s", document_root);

	if ((root_dir_path = (char *)ZWS_MALLOC(strlen(document_root) + 1
			+ sizeof(ZWS_ROOT_DIR_RELATIVE_TO_INSTALL_PATH))) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_root_dir_alloc;
	}
	sprintf(root_dir_path, "%s/%s", document_root, ZWS_ROOT_DIR_RELATIVE_TO_INSTALL_PATH);

	// Check if file present
	if (stat(root_dir_path, &stat_info) != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Root dir not present at '%s'", root_dir_path);
		goto l_err_dir_not_present;
	}

	*root_dir_path_p = root_dir_path;

	return ZWS_STATUS_OK;

l_err_dir_not_present:
	ZWS_FREE(root_dir_path);
l_err_root_dir_alloc:
l_err_get_install_path:
	return ZWS_STATUS_ERROR;
}

void zws_file_free_root_dir_path(char *root_dir)
{
	ZWS_FREE(root_dir);
}
