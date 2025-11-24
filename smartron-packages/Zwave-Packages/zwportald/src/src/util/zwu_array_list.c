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
 * @file    zwu_array_list.c
 *
 * @brief   Array list
 * @details Array list utility
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-23
 * - Initial version
 */

#include "zwu_array_list.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_log.h"

/** Default capacity allocated for an array list */
#define ZWP_ARRAY_LIST_DEFAULT_ALLOC_COUNT       16

/** Array list class */
struct _zwu_array_list
{
	void                        **data;                      /**< Stored data */
	int                         capacity;                    /**< Number of items that can be stored */
	int                         size;                        /**< Number of items actually stored */
};

int zwu_array_list_size(zwu_array_list_t array_list)
{
	return array_list->size;
}

/**
 * @brief Make sure the array list is capable of storing a given number of elements
 * @param[in]     array_list        Array list object
 * @param[in]     min_capacity      Minimum number of elements that the array list should be capable of storing
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwu_array_list_set_min_capacity(zwu_array_list_t array_list, int min_capacity)
{
	void **tmp;
	int capacity;

	if (min_capacity > array_list->capacity)
	{
		// Set to max(capacity + alloc_count, min_capacity)
		capacity = array_list->capacity + ZWP_ARRAY_LIST_DEFAULT_ALLOC_COUNT;
		if (min_capacity > capacity)
		{
			capacity = min_capacity;
		}

		if ((tmp = (void **)ZWP_REALLOC(array_list->data, sizeof(void *) * capacity)) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_array_list_alloc;
		}

		array_list->data = tmp;
		array_list->capacity = capacity;
	}

	return ZWP_STATUS_OK;

l_err_array_list_alloc:
	return ZWP_STATUS_ERROR;
}

/*
 * @note Caller must be aware of array size to prevent out of bounds index
 */
void *zwu_array_list_get(zwu_array_list_t array_list, int index)
{
	return array_list->data[index];
}

zwp_status_t zwu_array_list_add(zwu_array_list_t array_list, void *element)
{
	if (array_list->size >= array_list->capacity)
	{
		if (zwu_array_list_set_min_capacity(array_list, array_list->size + 1) != ZWP_STATUS_OK)
		{
			goto l_err_set_min_capacity;
		}
	}

	array_list->data[array_list->size] = element;
	array_list->size++;

	return ZWP_STATUS_OK;

l_err_set_min_capacity:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwu_array_list_add_at_index(zwu_array_list_t array_list, void *element, int index)
{
	int i;

	if ((index < 0) || (index > array_list->size))
	{
		goto l_err_invalid_index;
	}

	if (array_list->size >= array_list->capacity)
	{
		if (zwu_array_list_set_min_capacity(array_list, array_list->size + 1) != ZWP_STATUS_OK)
		{
			goto l_err_set_min_capacity;
		}
	}

	// Shift elements that are after index
	for (i = array_list->size; i > index; i--)
	{
		array_list->data[i] = array_list->data[i - 1];
	}

	array_list->data[i] = element;
	array_list->size++;

	return ZWP_STATUS_OK;

l_err_set_min_capacity:
l_err_invalid_index:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwu_array_list_remove(zwu_array_list_t array_list, void *element)
{
	int i;
	int j;

	for (i = 0; i < array_list->size; i++)
	{
		if (array_list->data[i] == element)
		{
			for (j = i; j < (array_list->size - 1); j++)
			{
				array_list->data[j] = array_list->data[j + 1];
			}

			array_list->size--;

			return ZWP_STATUS_OK;
		}
	}

	return ZWP_STATUS_ENOENT;
}

zwp_status_t zwu_array_list_remove_at_index(zwu_array_list_t array_list, int index)
{
	int i;

	if ((index < 0) || (index > array_list->size))
	{
		goto l_err_invalid_index;
	}

	// Shift elements that are after index
	for (i = index; i < (array_list->size - 1); i++)
	{
		array_list->data[i] = array_list->data[i + 1];
	}

	array_list->size--;

	return ZWP_STATUS_OK;

l_err_invalid_index:
	return ZWP_STATUS_ERROR;
}

void zwu_array_list_remove_all(zwu_array_list_t array_list)
{
	array_list->size = 0;
}

int zwu_array_list_contains(zwu_array_list_t array_list, void *element)
{
	int i;

	for (i = 0; i < array_list->size; i++)
	{
		if (array_list->data[i] == element)
		{
			return 1;
		}
	}

	return 0;
}

zwp_status_t zwu_array_list_create(int capacity, zwu_array_list_t *array_list_p)
{
	zwu_array_list_t array_list;

	if ((array_list = (zwu_array_list_t)ZWP_MALLOC(sizeof(*array_list))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_array_list_alloc;
	}

	array_list->data = NULL;
	array_list->capacity = 0;
	array_list->size = 0;

	if (capacity <= 0)
	{
		capacity = ZWP_ARRAY_LIST_DEFAULT_ALLOC_COUNT;
	}

	if ((array_list->data = (void **)ZWP_MALLOC(sizeof(void *) * capacity)) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_array_list_data_alloc;
	}

	array_list->capacity = capacity;

	*array_list_p  = array_list;

	return ZWP_STATUS_OK;

l_err_array_list_data_alloc:
	ZWP_FREE(array_list);
l_err_array_list_alloc:
	return ZWP_STATUS_ERROR;
}

void zwu_array_list_destroy(zwu_array_list_t array_list)
{
	ZWP_FREE(array_list->data);
	ZWP_FREE(array_list);
}
