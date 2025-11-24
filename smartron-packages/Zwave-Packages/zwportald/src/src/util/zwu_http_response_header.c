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
 * @file    zwu_http_response_header.c
 *
 * @brief   Curl http response header wrapper.
 * @details Handles HTTP(S) response header.
 *
 * @author  Zelin
 *
 * @version 1.0 - 2016-12-19
 * - Initial version
 */

#include <ctype.h>
#include "zwu_http_response_header.h"
#include "zwp_memory.h"
#include "zwu_log.h"
#include "zwp_error.h"

/**
 * @defgroup zwu_http_response_header Curl http response header
 * @{
 * @ingroup util
 */

typedef struct zwu_http_response_header_node zwu_http_response_header_node_t;
typedef struct zwu_http_response_header_node *zwu_http_response_header_node_p;
/**
 * @brief Curl http response header node
 */
struct zwu_http_response_header_node
{
	char *buffer;                                 /**< Buffer to store header */
	size_t length;                                /**< Buffer length */
	size_t field_name_length;                     /**< Field name length */
	zwu_http_response_header_node_p next;         /**< Pointer to next node */
};

/**
 * @brief Curl http response header class
 */
struct _zwu_http_response_header
{
	zwu_http_response_header_node_p head;    /**< Pointer to head */
	zwu_http_response_header_node_p tail;    /**< Pointer to tail */
};

zwu_status_t zwu_http_response_header_append_field(zwu_http_response_header_t http_response_header, char *buffer, size_t length)
{
	zwu_status_t status;
	zwu_http_response_header_node_p node;
	int i;

	status = ZWU_STATUS_ERROR;

	if (length <= 2 && isspace(buffer[0]))
	{
		status = ZWU_STATUS_OK;
		goto l_err_empty_line;
	}

	if ((node = (zwu_http_response_header_node_p)ZWP_MALLOC(sizeof(zwu_http_response_header_node_t))) == NULL)
	{
		ZWU_LOG(ZWU_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_node_alloc;
	}

	node->buffer = NULL;
	node->length = length;
	node->field_name_length = 0;
	node->next = NULL;

	// Search colon position
	for (i=0; i<length && buffer[i] != ':'; i++)
		;
	if (i < length)
	{
		node->field_name_length = i;
	}

	// Skip \r\n or \n
	if (buffer[length - 2] == '\r' && buffer[length - 1] == '\n')
	{
		node->length -= 2;
	}
	else if (buffer[length - 1] == '\n')
	{
		node->length--;
	}

	// One more byte at the end to store zero terminator.
	if ((node->buffer = (char*)ZWP_MALLOC(node->length+1)) == NULL)
	{
		ZWU_LOG(ZWU_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_buffer_alloc;
	}
	memcpy(node->buffer, buffer, node->length);
	node->buffer[node->length] = '\0';

	// First node
	if (!http_response_header->head)
	{
		http_response_header->head = node;
		http_response_header->tail = node;
	}
	else
	{
		// Append at the end of list
		http_response_header->tail->next = node;
		http_response_header->tail = node;
	}

	status = ZWU_STATUS_OK;
	return status;

	ZWP_FREE(node->buffer);
l_err_buffer_alloc:
	ZWP_FREE(node);
l_err_node_alloc:
l_err_empty_line:
	return status;
}

zwu_status_t zwu_http_response_header_search_field(zwu_http_response_header_t http_response_header, const char *field_name,
	char **field_value_content_p, size_t *field_value_content_length_p)
{
	zwu_http_response_header_node_p node;
	char *field_value_content;
	size_t field_value_content_length;
	int field_value_offset;
	int found_node;

	field_value_content = NULL;
	field_value_content_length = 0;

	// Search from head
	node = http_response_header->head;
	found_node = 0;
	while(node)
	{
		if (node->field_name_length)
		{
			if(node->field_name_length == strlen(field_name)
				&& strncmp(node->buffer, field_name, strlen(field_name)) == 0)
			{
				found_node = 1;
			}
		}

		if (found_node)
		{
			break;
		}
		else
		{
			node = node->next;
		}
	}

	if (!found_node)
	{
		goto l_err_not_found;
	}

	field_value_offset = node->field_name_length + 1;  // field name + ':'
	for(; field_value_offset<node->length && isspace(node->buffer[field_value_offset]); field_value_offset++)
		;  // skip white space before field value

	field_value_content_length = node->length - field_value_offset;
	if (field_value_content_length == 0)
	{
		// Should not happen for HTTP header
	}
	else
	{
		field_value_content = &node->buffer[field_value_offset];
	}

	*field_value_content_p = field_value_content;
	*field_value_content_length_p = field_value_content_length;

	return ZWU_STATUS_OK;

l_err_not_found:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_response_header_get_status_line(zwu_http_response_header_t http_response_header, char **status_line_p)
{
	char *status_line;
	zwu_http_response_header_node_p node;

	node = http_response_header->head;
	if (!node || node->field_name_length)
	{
		goto l_err_invalid_response;
	}

	status_line = node->buffer;

	*status_line_p = status_line;

	return ZWU_STATUS_OK;

l_err_invalid_response:
	return ZWU_STATUS_ERROR;
}

void zwu_http_response_header_release_all_fields(zwu_http_response_header_t http_response_header)
{
	zwu_http_response_header_node_p node;
	zwu_http_response_header_node_p node_backup;

	if (http_response_header)
	{
		// Release node from head to tail
		node = http_response_header->head;
		while(node)
		{
			ZWP_FREE(node->buffer);
			node_backup = node;
			node = node->next;
			ZWP_FREE(node_backup);
		}
		http_response_header->head = NULL;
		http_response_header->tail = NULL;
	}
}

zwu_status_t zwu_http_response_header_create(zwu_http_response_header_t *http_response_header_p)
{
	zwu_http_response_header_t http_response_header;

	if ((http_response_header = (zwu_http_response_header_t)ZWP_MALLOC(sizeof(*http_response_header))) == NULL)
	{
		ZWU_LOG(ZWU_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_http_response_header_alloc;
	}

	http_response_header->head = NULL;
	http_response_header->tail = NULL;

	*http_response_header_p = http_response_header;

	return ZWU_STATUS_OK;

	ZWP_FREE(http_response_header);
l_err_http_response_header_alloc:
	return ZWU_STATUS_ERROR;
}

zwu_status_t zwu_http_response_header_copy(zwu_http_response_header_t http_response_header, zwu_http_response_header_t *http_response_header_p)
{
	zwu_http_response_header_t new_header;
	zwu_http_response_header_node_p node;

	if (zwu_http_response_header_create(&new_header) != ZWU_STATUS_OK)
	{
		goto l_err_http_response_header_create;
	}

	node = http_response_header->head;
	while(node)
	{
		if (zwu_http_response_header_append_field(new_header, node->buffer, node->length) != ZWU_STATUS_OK)
		{
			goto l_err_http_response_header_append_field;
		}
		node = node->next;
	}

	*http_response_header_p = new_header;

	return ZWU_STATUS_OK;

l_err_http_response_header_append_field:
	zwu_http_response_header_destroy(new_header);
l_err_http_response_header_create:
	return ZWU_STATUS_ERROR;
}

void zwu_http_response_header_destroy(zwu_http_response_header_t http_response_header)
{
	if (http_response_header)
	{
		zwu_http_response_header_release_all_fields(http_response_header);
		ZWP_FREE(http_response_header);
	}
}
/**
 * @}
 */
