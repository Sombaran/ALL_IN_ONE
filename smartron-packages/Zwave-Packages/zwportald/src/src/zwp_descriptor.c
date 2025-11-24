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
 * @file    zwp_descriptor.c
 *
 * @brief   Descriptor management of Z-Wave Network hierarchy
 * @details Handles the descriptors at Node, Endpoint and Interface level in a Z-Wave network.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-07-10
 * - Initial version
 *
 * @version 1.1 - 2014-01-24
 * - Version command class version 2  --Sasidhar
 */

#include <stdint.h>
#include <stddef.h>

#include "zwp_log.h"
#if defined(ZWP_DEBUG_DESCRIPTOR)
#include "zwp_log_enable.h"
#else
#include "zwp_log_disable.h"
#endif

#include "zwp_descriptor.h"
#include "zwp_node_state.h"
#include "zwp_interface_state.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_thread_mutex.h"
#include "zwp_hcapi.h"

/** Get a structures address from a given member address */
#define container_of(ptr, type, member) ({                      \
        const void *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/** Forward declaration for network descriptor container class */
typedef struct _zwp_desc_container *zwp_desc_container_net_t;
/** Forward declaration for node descriptor container class */
typedef struct _zwp_desc_container *zwp_desc_container_node_t;
/** Forward declaration for endpoint descriptor container class */
typedef struct _zwp_desc_container *zwp_desc_container_endpoint_t;
/** Forward declaration for interface descriptor container class */
typedef struct _zwp_desc_container_interface *zwp_desc_container_interface_t;

/** List of unsupported command classes */
static uint8_t unsupported_cc[] = {
									COMMAND_CLASS_CLIMATE_CONTROL_SCHEDULE,
									COMMAND_CLASS_CLOCK,
									COMMAND_CLASS_INDICATOR,
									COMMAND_CLASS_METER_PULSE,
									COMMAND_CLASS_METER_TBL_CONFIG,
									COMMAND_CLASS_METER_TBL_MONITOR,
									COMMAND_CLASS_PROTECTION,
									COMMAND_CLASS_SIMPLE_AV_CONTROL,
									COMMAND_CLASS_THERMOSTAT_SETBACK,
									COMMAND_CLASS_MULTI_CMD
								};

/**
 * @brief Network descriptor class
 */
struct _zwp_desc_net
{
	zwp_desc_container_net_t         desc_container_net;         /**< Network descriptor container */

	zwu_time_t                       utime;                      /**< Last updated time of the network */

	zwp_net_t                        net;                        /**< Back pointer to Network */
};

/**
 * @brief Generic descriptor container class
 */
struct _zwp_desc_container
{
	struct _zwp_desc_container       *next;                      /**< Reference to the next sibling */
	void                             *child;                     /**< Reference to the first child (lower level) */
	void                             *user;                      /**< User data - Node/Endpoint level info */
	char                             desc[0];                    /**< Place holder for the descriptor that tail gates */
};

/**
 * @brief Interface descriptor container class
 */
struct _zwp_desc_container_interface
{
	struct _zwp_desc_container_interface  *next;                 /**< Reference to the next sibling */
	void                                  *user;                 /**< User data - Interface level info */
	zwp_interface_state_free_t            state_free_handler;    /**< Function handler to free interface level state information */
	zwp_net_t							  net;                   /**< Back pointer to Network */
	char                                  desc[0];               /**< Place holder for the descriptor that tail gates */
};

/** Macro to get node descriptor container object from HCAPI node descriptor */
#define DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node)  container_of(desc_node, struct _zwp_desc_container, desc)
/** Macro to get endpoint descriptor container object from HCAPI endpoint descriptor */
#define DESC_ENDPOINT_TO_DESC_CONTAINER_ENDPOINT(desc_endpoint)  container_of(desc_endpoint, struct _zwp_desc_container, desc)
/** Macro to get interface descriptor container object from HCAPI interface descriptor */
#define DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface)  container_of(desc_interface, struct _zwp_desc_container_interface, desc)

static zwp_status_t zwp_desc_container_endpoint_alloc(zwp_desc_net_t desc_net, zwnoded_t *noded, zwp_desc_container_endpoint_t *desc_container_endpoint_p);
static void zwp_desc_container_endpoint_free(zwp_desc_container_endpoint_t desc_container_endpoint);
static zwp_status_t zwp_desc_container_node_alloc(zwp_desc_net_t desc_net, zwnet_p zwnet, zwp_desc_container_node_t *desc_container_node_p);
static void zwp_desc_container_node_free(zwp_desc_container_node_t desc_container_node);
/**
 * @name Getters
 * @{
 */
zwu_time_t zwp_desc_get_network_utime(zwp_desc_net_t desc_net)
{
	return desc_net->utime;
}
/**
 * @}
 */

/*
 * Returns 1 if cmd_class is in the unsupported_list, otherwise returns 0
 *
 */
/**
 * @brief Check if the command class is not supported
 * @param[in]     cmd_class         Command class
 * @retval        0                 Command class is supported
 * @retval        1                 Command class is not supported
 */
static uint8_t zwp_desc_is_interface_unsupported(uint8_t cmd_class)
{
	int i = 0;
	int unspported_list_len = sizeof(unsupported_cc);
	for(i=0; i<unspported_list_len; i++)
	{
		if(unsupported_cc[i] == cmd_class)
		{
			return 1; //cmd_class is in the supported list
		}
	}

	return 0; //It is supported interface

}

zwp_status_t zwp_desc_get_desc_interface(zwp_desc_net_t desc_net, int node_id, int endpoint_id, int interface_id, zwifd_p *desc_interface_p)
{
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwp_desc_container_interface_t interface;
	zwnoded_p desc_node;
	zwepd_p desc_endpoint;
	zwifd_p desc_interface;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_p)node->desc;

		if (desc_node->nodeid == node_id)
		{
			for (endpoint = node->child; endpoint; endpoint = endpoint->next)
			{
				desc_endpoint = (zwepd_p)endpoint->desc;

				if (desc_endpoint->epid == endpoint_id)
				{
					for (interface = endpoint->child; interface; interface = interface->next)
					{
						desc_interface = (zwifd_p)interface->desc;

						if (desc_interface->cls == interface_id)
						{
							ZWP_LOG(ZWP_LOG_DEBUG, "Found (NodeId, EndpointId, InterfaceId): (%u, %u, %u|0x%04x)",
									desc_node->nodeid,
									desc_endpoint->epid,
									desc_interface->cls,
									desc_interface->cls);

							*desc_interface_p = desc_interface;

							return ZWP_STATUS_OK;
						}
					}
				}
			}
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to find (NodeId, EndpointId, InterfaceId): (%d, %d, %d|0x%04x)",
			node_id, endpoint_id, interface_id, interface_id);

	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_desc_get_desc_endpoint(zwp_desc_net_t desc_net, int node_id, int endpoint_id, zwepd_p *desc_endpoint_p)
{
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwnoded_p desc_node;
	zwepd_p desc_endpoint;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_p)node->desc;

		if (desc_node->nodeid == node_id)
		{
			for (endpoint = node->child; endpoint; endpoint = endpoint->next)
			{
				desc_endpoint = (zwepd_p)endpoint->desc;

				if (desc_endpoint->epid == endpoint_id)
				{
					ZWP_LOG(ZWP_LOG_DEBUG, "Found (NodeId, EndpointId): (%u, %u)",
							desc_node->nodeid,
							desc_endpoint->epid);

					*desc_endpoint_p = desc_endpoint;

					return ZWP_STATUS_OK;
				}
			}
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to find (NodeId, EndpointId): (%d, %d)",
			node_id, endpoint_id);

	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_desc_get_desc_node(zwp_desc_net_t desc_net, int node_id, zwnoded_p *desc_node_p)
{
	zwp_desc_container_node_t node;
	zwnoded_p desc_node;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_p)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		if (desc_node->nodeid == node_id)
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Found (NodeId): (%u)",
					desc_node->nodeid);

			*desc_node_p = desc_node;

			return ZWP_STATUS_OK;
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to find (NodeId): (%d)",
			node_id);

	return ZWP_STATUS_ERROR;
}

void *zwp_desc_interface_get_user(zwifd_p desc_interface)
{
	zwp_desc_container_interface_t interface;

	interface = DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface);

	return interface->user;
}

void zwp_desc_interface_set_user(zwifd_p desc_interface, void *user)
{
	zwp_desc_container_interface_t interface;

	interface = DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface);

	interface->user = user;
}

zwp_interface_state_free_t zwp_desc_interface_get_state_free_handler(zwifd_p desc_interface)
{
	zwp_desc_container_interface_t interface;

	interface = DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface);

	return interface->state_free_handler;
}

void zwp_desc_interface_set_state_free_handler(zwifd_p desc_interface, zwp_interface_state_free_t state_free_handler)
{
	zwp_desc_container_interface_t interface;

	interface = DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface);

	interface->state_free_handler = state_free_handler;
}

zwp_net_t zwp_desc_interface_get_net(zwifd_p desc_interface)
{
	zwp_desc_container_interface_t interface;

	interface = DESC_INTERFACE_TO_DESC_CONTAINER_INTERFACE(desc_interface);

	return interface->net;
}


void *zwp_desc_node_get_user(zwnoded_p desc_node)
{
	zwp_desc_container_node_t node;

	node = DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node);

	return node->user;
}

void zwp_desc_node_set_user(zwnoded_p desc_node, void *user)
{
	zwp_desc_container_node_t node;

	node = DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node);

	node->user = user;
}

uint8_t zwp_desc_node_get_secure_state(zwnoded_p desc_node)
{
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwp_desc_container_interface_t interface;
	zwifd_t *desc_interface;
	uint8_t secure;

	secure = 0;

	node = DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node);

	for (endpoint = (zwp_desc_container_endpoint_t)node->child;
			endpoint && !secure;
			endpoint = endpoint->next)
	{

		for (interface = (zwp_desc_container_interface_t)endpoint->child;
				interface;
				interface = interface->next)
		{
			desc_interface = (zwifd_t *)interface->desc;

			if(desc_interface && (desc_interface->propty & IF_PROPTY_SECURE))
			{
				secure = 1;
				break;
			}

		}
	}

	return secure;
}

/**
 * @brief Free interface level user specific information of all interfaces under an endpoint
 * @param[in]     desc_endpoint     Endpoint descriptor object
 * @return        Nothing
 */
static void zwp_desc_interface_state_under_endpoint_destroy(zwepd_p desc_endpoint)
{
	zwp_desc_container_endpoint_t endpoint;
	zwp_desc_container_interface_t interface;
	zwifd_p desc_interface;

	endpoint = DESC_ENDPOINT_TO_DESC_CONTAINER_ENDPOINT(desc_endpoint);

	for (interface = (zwp_desc_container_interface_t)endpoint->child; interface; interface = interface->next)
	{
		desc_interface = (zwifd_p)interface->desc;

		zwp_interface_state_destroy(desc_interface);
	}
}

void zwp_desc_interface_state_under_node_destroy(zwnoded_p desc_node)
{
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwepd_p desc_endpoint;

	node = DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node);

	for (endpoint = (zwp_desc_container_endpoint_t)node->child; endpoint; endpoint = endpoint->next)
	{
		desc_endpoint = (zwepd_p)endpoint->desc;

		zwp_desc_interface_state_under_endpoint_destroy(desc_endpoint);
	}
}

void zwp_desc_interface_state_under_net_destroy(zwp_desc_net_t desc_net)
{
	zwp_desc_container_node_t node;
	zwnoded_p desc_node;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_p)node->desc;

		zwp_desc_interface_state_under_node_destroy(desc_node);
	}
}

void zwp_desc_node_state_under_net_destroy(zwp_desc_net_t desc_net)
{
	zwp_desc_container_node_t node;
	zwnoded_p desc_node;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_p)node->desc;

		zwp_node_state_destroy(desc_node);
	}
}

/**
@brief Return an array of interfaces and their respective endpoints based on interface id.
@param[in]	desc_net			Network descriptor object
@param[in]	interface_id		Interface ID
@param[out]	if_ep_name_list_p	An array of interfaces with respective endpoint name. List elements of type <em> zwp_desc_IF_ep_name_t *</em>
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_desc_get_device_from_interface_id(zwp_desc_net_t desc_net,
	uint16_t interface_id,
	zwu_array_list_t *if_ep_name_list_p)
{
	zwp_status_t res = ZWP_STATUS_ERROR;
	zwu_array_list_t if_ep_name_list;
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwp_desc_container_interface_t interface;
	//zwnoded_p desc_node = NULL;
	zwp_desc_IF_ep_name_t  *pstIF_ep_name;
	zwepd_p desc_endpoint;
	zwifd_p desc_interface;

	if (zwu_array_list_create(0, &if_ep_name_list) != ZWP_STATUS_OK) 
	{
		goto l_err_endpoint_list_create;
	}

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		//desc_node = (zwnoded_p)node->desc;

		for (endpoint = node->child; endpoint; endpoint = endpoint->next)
		{
			desc_endpoint = (zwepd_p)endpoint->desc;

			for (interface = endpoint->child; interface; interface = interface->next)
			{
				desc_interface = (zwifd_p)interface->desc;

				if (desc_interface->cls == interface_id)
				{
					//Found!!
					if ((pstIF_ep_name = (zwp_desc_IF_ep_name_t *)ZWP_MALLOC(sizeof(*pstIF_ep_name))) == NULL)
					{
						ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
						goto l_err_desc_interface_alloc;
					}

					pstIF_ep_name->desc_interface = desc_interface;
					//memcpy(&(pstIF_ep_name->desc_interface), interface->desc, sizeof(zwifd_t));

					memcpy(pstIF_ep_name->name, desc_endpoint->name, ZW_LOC_STR_MAX + 1);
					memcpy(pstIF_ep_name->loc, desc_endpoint->loc, ZW_LOC_STR_MAX + 1);

					if (zwu_array_list_add(if_ep_name_list, pstIF_ep_name) != ZWP_STATUS_OK)
					{
						goto l_err_interface_list_add;
					}				

					break;					
				}
			}

		}

	}

	*if_ep_name_list_p = if_ep_name_list;
	return ZWP_STATUS_OK;

l_err_interface_list_add:
	if(pstIF_ep_name)
		ZWP_FREE(pstIF_ep_name);
l_err_desc_interface_alloc:
	zwp_desc_free_if_ep_name_list(if_ep_name_list);
l_err_endpoint_list_create:
	return res;
}

void zwp_desc_free_if_ep_name_list(zwu_array_list_t if_ep_name_list)
{
	int count;
	int i;

	count = zwu_array_list_size(if_ep_name_list);

	for (i = 0; i < count; i++)
	{
		ZWP_FREE(zwu_array_list_get(if_ep_name_list, i));
	}

	zwu_array_list_destroy(if_ep_name_list);
}

/**
@brief Get the corresponding endpoint name and location from interface descriptor.
@param[in]	desc_interface		Interface descriptor object
@param[out]	pEpNameLoc			Pointer to the corresponding endpoint's name and location
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_desc_get_endpoint_nameloc_from_interface(zwifd_p desc_interface,
	zw_nameloc_t *pEpNameLoc)
{
	zwp_status_t res = ZWP_STATUS_ERROR;
	zwp_net_t net;
	zwp_desc_net_t desc_net;
	zwepd_p desc_endpoint = NULL;

	net = zwp_desc_interface_get_net(desc_interface);
	desc_net = zwp_net_get_desc_net(net);

	if (!pEpNameLoc) goto l_err_arg;

	if (zwp_desc_get_desc_endpoint(desc_net, desc_interface->nodeid, desc_interface->epid, &desc_endpoint) != ZWP_STATUS_OK)
	{
		goto l_err_get_desc_ep;
	}

	strcpy(pEpNameLoc->name, desc_endpoint->name);
	strcpy(pEpNameLoc->loc, desc_endpoint->loc);

	res = ZWP_STATUS_OK;

l_err_get_desc_ep:
l_err_arg:
	return res;
}

zwp_status_t zwp_desc_get_interface_list(zwepd_p desc_endpoint, zwu_array_list_t *interface_list_p)
{
	zwu_array_list_t interface_list;
	zwp_desc_container_endpoint_t endpoint;
	zwp_desc_container_interface_t interface;
	zwifd_t *desc_interface;
	int interface_count;
	int i;

	if (zwu_array_list_create(0, &interface_list) != ZWP_STATUS_OK)
	{
		goto l_err_endpoint_list_create;
	}

	interface_count = 0;
	endpoint = DESC_ENDPOINT_TO_DESC_CONTAINER_ENDPOINT(desc_endpoint);

	for (interface = (zwp_desc_container_interface_t)endpoint->child;
			interface;
			interface = interface->next)
	{
		desc_interface = (zwifd_t *)interface->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Interface Id: %d", desc_interface->cls);

		if ((desc_interface = (zwifd_t *)ZWP_MALLOC(sizeof(*desc_interface))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_endpoint_alloc;
		}

		memcpy(desc_interface, interface->desc, sizeof(*desc_interface));

		if (zwu_array_list_add(interface_list, desc_interface) != ZWP_STATUS_OK)
		{
			goto l_err_endpoint_list_add;
		}

		interface_count++;
	}

	*interface_list_p = interface_list;

	return ZWP_STATUS_OK;

l_err_endpoint_list_add:
	ZWP_FREE(desc_interface);
l_err_desc_endpoint_alloc:
	for (i = 0; i < interface_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(interface_list, i));
	}
	zwu_array_list_destroy(interface_list);
l_err_endpoint_list_create:
	return ZWP_STATUS_ERROR;
}

void zwp_desc_free_interface_list(zwu_array_list_t interface_list)
{
	int interface_count;
	int i;

	interface_count = zwu_array_list_size(interface_list);

	for (i = 0; i < interface_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(interface_list, i));
	}

	zwu_array_list_destroy(interface_list);
}

zwp_status_t zwp_desc_get_endpoint_list(zwnoded_p desc_node, zwu_array_list_t *endpoint_list_p)
{
	zwu_array_list_t endpoint_list;
	zwp_desc_container_node_t node;
	zwp_desc_container_endpoint_t endpoint;
	zwepd_t *desc_endpoint;
	int endpoint_count;
	int i;

	if (zwu_array_list_create(0, &endpoint_list) != ZWP_STATUS_OK)
	{
		goto l_err_node_list_create;
	}

	endpoint_count = 0;
	node = DESC_NODE_TO_DESC_CONTAINER_NODE(desc_node);

	for (endpoint = (zwp_desc_container_endpoint_t)node->child;
			endpoint;
			endpoint = endpoint->next)
	{
		desc_endpoint = (zwepd_t *)endpoint->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint Id: %d", desc_endpoint->epid);

		if ((desc_endpoint = (zwepd_t *)ZWP_MALLOC(sizeof(*desc_endpoint))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_node_alloc;
		}

		memcpy(desc_endpoint, endpoint->desc, sizeof(*desc_endpoint));

		if (zwu_array_list_add(endpoint_list, desc_endpoint) != ZWP_STATUS_OK)
		{
			goto l_err_node_list_add;
		}

		endpoint_count++;
	}

	*endpoint_list_p = endpoint_list;

	return ZWP_STATUS_OK;

l_err_node_list_add:
	ZWP_FREE(desc_endpoint);
l_err_desc_node_alloc:
	for (i = 0; i < endpoint_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(endpoint_list, i));
	}
	zwu_array_list_destroy(endpoint_list);
l_err_node_list_create:
	return ZWP_STATUS_ERROR;
}

void zwp_desc_free_endpoint_list(zwu_array_list_t endpoint_list)
{
	int endpoint_count;
	int i;

	endpoint_count = zwu_array_list_size(endpoint_list);

	for (i = 0; i < endpoint_count; i++)
	{
		ZWP_FREE(zwu_array_list_get(endpoint_list, i));
	}

	zwu_array_list_destroy(endpoint_list);
}

zwp_status_t zwp_desc_get_node_endpoint_list(zwp_desc_net_t desc_net, zwu_array_list_t *node_list_p)
{
	zwu_array_list_t node_endpoint_list;
	zwp_desc_container_node_t node;
	zwp_desc_container_node_t endpoint;
	zwnoded_t *desc_node;
	zwepd_t *desc_endpoint;
	zwepd_p node_endpoint_entry;
	int node_endpoint_count;
	int i;

	if (zwu_array_list_create(0, &node_endpoint_list) != ZWP_STATUS_OK)
	{
		goto l_err_node_list_create;
	}

	node_endpoint_count = 0;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		for (endpoint = (zwp_desc_container_endpoint_t)node->child;
				endpoint;
				endpoint = endpoint->next)
		{
			desc_endpoint = (zwepd_t *)endpoint->desc;

			ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint Id: %d", desc_endpoint->epid);

			// Allocate memory for zwnoded_t and zwepd_t
			if ((node_endpoint_entry = (zwepd_p) ZWP_MALLOC(sizeof(*node_endpoint_entry))) == NULL)
			{
				ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
				goto l_err_desc_node_endpoint_alloc;
			}

			memcpy(node_endpoint_entry, endpoint->desc, sizeof(*node_endpoint_entry));

			if (zwu_array_list_add(node_endpoint_list, node_endpoint_entry) != ZWP_STATUS_OK)
			{
				goto l_err_node_endpoint_list_add;
			}

			node_endpoint_count++;
		}
	}

	*node_list_p = node_endpoint_list;

	return ZWP_STATUS_OK;

l_err_node_endpoint_list_add:
	ZWP_FREE(node_endpoint_entry);
l_err_desc_node_endpoint_alloc:
	for (i = 0; i < node_endpoint_count; i++)
	{
		node_endpoint_entry = (zwepd_p) zwu_array_list_get(node_endpoint_list, i);

		ZWP_FREE(node_endpoint_entry);
	}
	zwu_array_list_destroy(node_endpoint_list);
l_err_node_list_create:
	return ZWP_STATUS_ERROR;
}

void zwp_desc_free_node_endpoint_list(zwu_array_list_t node_endpoint_list)
{
	int node_count;
	zwepd_p node_endpoint_entry;
	int i;

	node_count = zwu_array_list_size(node_endpoint_list);

	for (i = 0; i < node_count; i++)
	{
		node_endpoint_entry = (zwepd_p) zwu_array_list_get(node_endpoint_list, i);

		ZWP_FREE(node_endpoint_entry);
	}

	zwu_array_list_destroy(node_endpoint_list);
}

zwp_status_t zwp_desc_get_node_list(zwp_desc_net_t desc_net, zwu_array_list_t *node_list_p)
{
	zwu_array_list_t node_list;
	zwp_desc_container_node_t node;
	zwnoded_t *desc_node;
	char *node_entry;
	zwp_node_state_t *node_state_p;
	int node_count;
	int i;

	if (zwu_array_list_create(0, &node_list) != ZWP_STATUS_OK)
	{
		goto l_err_node_list_create;
	}

	node_count = 0;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		// Allocate memory for zwnoded_t and zwp_node_state_t
		if ((node_entry = (char *) ZWP_MALLOC(sizeof(zwnoded_t) + sizeof(*node_state_p))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_node_alloc;
		}

		memcpy(node_entry, node->desc, sizeof(zwnoded_t));

		/* Handling node level data */
		node_state_p = (zwp_node_state_t *)(node_entry + sizeof(zwnoded_t));

		if (zwp_node_state_refresh(desc_node, node_state_p) != ZWP_STATUS_OK)
		{
			goto l_err_desc_node_refresh_state;
		}

		if (zwu_array_list_add(node_list, node_entry) != ZWP_STATUS_OK)
		{
			goto l_err_node_list_add;
		}

		node_count++;
	}

	*node_list_p = node_list;

	return ZWP_STATUS_OK;

l_err_node_list_add:
	zwp_node_state_free(*node_state_p);
l_err_desc_node_refresh_state:
	ZWP_FREE(node_entry);
l_err_desc_node_alloc:
	for (i = 0; i < node_count; i++)
	{
		node_entry = zwu_array_list_get(node_list, i);

		/* Freeing node level data */
		node_state_p = (zwp_node_state_t *)(node_entry + sizeof(zwnoded_t));

		zwp_node_state_free(*node_state_p);

		ZWP_FREE(node_entry);
	}
	zwu_array_list_destroy(node_list);
l_err_node_list_create:
	return ZWP_STATUS_ERROR;
}

void zwp_desc_free_node_list(zwu_array_list_t node_list)
{
	int node_count;
	char *node_entry;
	zwp_node_state_t *node_state_p;
	int i;

	node_count = zwu_array_list_size(node_list);

	for (i = 0; i < node_count; i++)
	{
		node_entry = (char *) zwu_array_list_get(node_list, i);

		/* Freeing node level data */
		node_state_p = (zwp_node_state_t *)(node_entry + sizeof(zwnoded_t));

		zwp_node_state_free(*node_state_p);

		ZWP_FREE(node_entry);
	}

	zwu_array_list_destroy(node_list);
}

zwp_status_t zwp_desc_add_or_update_node(zwp_desc_net_t desc_net, zwnoded_p node_desc_new)
{
	zwp_desc_container_node_t       node;
	zwp_desc_container_node_t       *node_prev;
	zwp_desc_container_node_t       node_new;
	zwnoded_p                       desc_node;
	zwnoded_p                       desc_node_new;
	zwepd_t                         endpoint_desc;

	if ((node_new = (zwp_desc_container_node_t)ZWP_MALLOC(sizeof(*node_new) + sizeof(zwnoded_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_desc_container_node_alloc;
	}

	node_new->user = NULL;
	node_new->next = node_new->child = NULL;

	if (zwnode_get_ep(node_desc_new, &endpoint_desc) != ZW_ERR_NONE)
	{
		goto l_err_get_endpoint_desc;
	}

	if (zwep_get_node(&endpoint_desc, (zwnoded_t *)node_new->desc) != ZW_ERR_NONE)
	{
		goto l_err_get_node_desc;
	}

	desc_node_new = (zwnoded_t *)node_new->desc;

	// Find the location to insert the new node
	for (node_prev = &desc_net->desc_container_net, node = *node_prev;
			node;
			node_prev = &node->next, node = node->next)
	{
		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		if (desc_node->nodeid == desc_node_new->nodeid)
		{
			break;
		}
	}

	desc_net->utime = zwu_time_now();

	// If node already exists, remove and add the new one
	if (node && (desc_node->nodeid == desc_node_new->nodeid))
	{
		node_new->next = node->next;
		*node_prev = node_new;

		// Free node level data
		zwp_node_state_destroy(desc_node);

		zwp_desc_container_endpoint_free((zwp_desc_container_endpoint_t)node->child);

		ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %u (free) [Update]", desc_node->nodeid);
		ZWP_FREE(node);
	}
	else
	{
		node_new->next = *node_prev;
		*node_prev = node_new;
	}

	if (zwp_desc_container_endpoint_alloc(desc_net, desc_node_new, (zwp_desc_container_endpoint_t *)&node_new->child) != ZWP_STATUS_OK)
	{
		goto l_err_desc_container_endpoint_alloc;
	}

	if (zwp_node_state_create(desc_node_new) != ZWP_STATUS_OK)
	{
		goto l_err_desc_node_create_state;
	}

	return ZWP_STATUS_OK;

	zwp_node_state_destroy(desc_node_new);
l_err_desc_node_create_state:
	zwp_desc_container_endpoint_free((zwp_desc_container_endpoint_t)node_new->child);
l_err_desc_container_endpoint_alloc:
l_err_get_node_desc:
l_err_get_endpoint_desc:
	ZWP_FREE(node_new);
l_err_desc_container_node_alloc:
	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_desc_remove_node(zwp_desc_net_t desc_net, int node_id)
{
	zwp_desc_container_node_t       node;
	zwp_desc_container_node_t       *node_prev;
	zwnoded_t                       *desc_node;

	for (node_prev = &desc_net->desc_container_net, node = *node_prev;
			node;
			node_prev = &node->next, node = node->next)
	{
		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		if (desc_node->nodeid == node_id)
		{
			desc_net->utime = zwu_time_now();

			(*node_prev) = node->next;

			// Free node level data
			zwp_node_state_destroy(desc_node);

			zwp_desc_container_endpoint_free((zwp_desc_container_endpoint_t)node->child);

			ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %u (free)", desc_node->nodeid);
			ZWP_FREE(node);

			return ZWP_STATUS_OK;
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to find match for node id : %d", node_id);

	return ZWP_STATUS_ERROR;
}

zwp_status_t zwp_desc_refresh_node(zwp_desc_net_t desc_net, int node_id)
{
	zwp_desc_container_node_t       node;
	zwnoded_t                       *desc_node;

	for (node = desc_net->desc_container_net; node; node = node->next)
	{
		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node Id: %d", desc_node->nodeid);

		if (desc_node->nodeid == node_id)
		{
			if (zwp_node_state_refresh(desc_node, NULL) != ZWP_STATUS_OK)
			{
				goto l_err_desc_node_refresh_state;
			}

			return ZWP_STATUS_OK;
		}
	}

	ZWP_LOG(ZWP_LOG_ERR, "Failed to find match for node id : %d", node_id);

l_err_desc_node_refresh_state:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Allocate and initialize all interface descriptor containers under an endpoint
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     epd               HCAPI endpoint descriptor
 * @param[out]    desc_container_interface_p  Head of interface descriptor containers list
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_container_interface_free() to free resources
 */
static zwp_status_t zwp_desc_container_interface_alloc(zwp_desc_net_t desc_net, zwepd_t *epd, zwp_desc_container_interface_t *desc_container_interface_p)
{
	int                             hcapi_code;
	zwp_desc_container_interface_t  desc_container_interface;
	zwp_desc_container_interface_t  interface;
	zwp_desc_container_interface_t  interface_next;
	zwifd_t                         *desc_interface;
	zwifd_t               			hcapi_desc_interface;
	zwifd_t                         *desc_interface_tmp;

	desc_container_interface = NULL;

	if ((interface_next = (zwp_desc_container_interface_t)ZWP_MALLOC(sizeof(*interface_next) + sizeof(zwifd_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_desc_container_interface_alloc;
	}

	//Record the head of node tree
	desc_container_interface = interface_next;
	interface = NULL;

	for (hcapi_code = zwep_get_if(epd, (zwifd_t *)interface_next->desc);
			hcapi_code == ZW_ERR_NONE;
			hcapi_code = zwif_get_next(&hcapi_desc_interface, (zwifd_t *)interface_next->desc))
	{

		desc_interface_tmp = (zwifd_t *)interface_next->desc;
		if (zwp_desc_is_interface_unsupported(desc_interface_tmp->cls))
		{
			ZWP_LOG(ZWP_LOG_DEBUG, "Interface Type: %u [skipping]", desc_interface_tmp->cls);

			memcpy(&hcapi_desc_interface, &interface_next->desc, sizeof(hcapi_desc_interface));

			continue;
		}
		interface = interface_next;
		interface->user = NULL;
		interface->state_free_handler = NULL;
		interface->next = NULL;
		interface->net = desc_net->net;
		
		desc_interface = (zwifd_t *)interface->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Interface Type: %u", desc_interface->cls);

		if (zwp_avi_interfaces_setup(zwp_portal_get_avi(zwp_net_get_portal(desc_net->net)), desc_interface,
				zwp_net_get_network_pool(desc_net->net)) != ZWP_STATUS_OK)
		{
			goto l_err_interface_setup;
		}

		if ((interface_next = (zwp_desc_container_interface_t)ZWP_MALLOC(sizeof(*interface_next) + sizeof(zwifd_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_container_interface_alloc;
		}

		//Sibling link at interface level
		interface->next = interface_next;
		memcpy(&hcapi_desc_interface, &interface->desc, sizeof(hcapi_desc_interface));
	}

	if (interface)
	{
		interface->next = NULL;
	}
	else
	{
		//Zero interfaces
		desc_container_interface = NULL;
	}
	//Free the extra interface
	ZWP_FREE(interface_next);

	if (hcapi_code != ZW_ERR_INTF_NOT_FOUND)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "zwep_get_if() or zwif_get_next() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
	}

	*desc_container_interface_p = desc_container_interface;

	return ZWP_STATUS_OK;

l_err_interface_setup:
l_err_desc_container_interface_alloc:
	//Deallocate all interfaces
	interface = desc_container_interface;

	while(interface)
	{
		interface_next = interface->next;

		desc_interface = (zwifd_t *)interface->desc;

		// Free interface level data
		zwp_interface_state_destroy(desc_interface);

		ZWP_LOG(ZWP_LOG_DEBUG, "Interface Type: %u (free)", desc_interface->cls);
		ZWP_FREE(interface);

		interface = interface_next;
	}

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize and free the interface descriptor containers list
 * @param[in]     desc_container_interface Head of interface descriptor containers list
 * @return        Nothing
 * @see zwp_desc_container_interface_alloc
 */
static void zwp_desc_container_interface_free(zwp_desc_container_interface_t desc_container_interface)
{
	zwp_desc_container_interface_t  interface;
	zwp_desc_container_interface_t  interface_next;
	zwifd_t                         *desc_interface;

	interface = desc_container_interface;

	while(interface)
	{
		interface_next = interface->next;

		desc_interface = (zwifd_t *)interface->desc;

		// Free interface level data
		zwp_interface_state_destroy(desc_interface);

		ZWP_LOG(ZWP_LOG_DEBUG, "Interface Type: %u (free)", desc_interface->cls);
		ZWP_FREE(interface);

		interface = interface_next;
	}
}

/**
 * @brief Allocate and initialize all endpoint descriptor containers and all their children
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     noded             HCAPI node handle
 * @param[out]    desc_container_endpoint_p  Head of endpoint descriptor containers list
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_container_endpoint_free() to free resources
 */
static zwp_status_t zwp_desc_container_endpoint_alloc(zwp_desc_net_t desc_net, zwnoded_t *noded, zwp_desc_container_endpoint_t *desc_container_endpoint_p)
{
	int                             hcapi_code;
	zwp_desc_container_endpoint_t   desc_container_endpoint;
	zwp_desc_container_endpoint_t   endpoint;
	zwp_desc_container_endpoint_t   endpoint_next;
	zwepd_t                         *desc_endpoint;

	desc_container_endpoint = NULL;

	if ((endpoint_next = (zwp_desc_container_endpoint_t)ZWP_MALLOC(sizeof(*endpoint_next) + sizeof(zwepd_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_desc_container_endpoint_alloc;
	}

	//Record the head of node tree
	desc_container_endpoint = endpoint_next;
	endpoint = NULL;

	for (hcapi_code = zwnode_get_ep(noded, (zwepd_t *)endpoint_next->desc);
			hcapi_code == ZW_ERR_NONE;
			hcapi_code = zwep_get_next((zwepd_t *)endpoint->desc, (zwepd_t *)endpoint_next->desc))
	{
		endpoint = endpoint_next;
		endpoint->user = NULL;
		endpoint->next = endpoint->child = NULL;

		desc_endpoint = (zwepd_t *)endpoint->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint ID: %u", desc_endpoint->epid);

		if (zwp_desc_container_interface_alloc(desc_net, desc_endpoint, (zwp_desc_container_interface_t *)&endpoint->child) != ZWP_STATUS_OK)
		{
			goto l_err_desc_container_endpoint_alloc;
		}

		if ((endpoint_next = (zwp_desc_container_endpoint_t)ZWP_MALLOC(sizeof(*endpoint_next) + sizeof(zwepd_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_container_endpoint_alloc;
		}

		//Sibling link at endpoint level
		endpoint->next = endpoint_next;
	}

	if (endpoint)
	{
		endpoint->next = NULL;
	}
	else
	{
		//Zero endpoints
		desc_container_endpoint = NULL;
	}
	//Free the extra endpoint
	ZWP_FREE(endpoint_next);

	if (hcapi_code != ZW_ERR_EP_NOT_FOUND)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "zwnode_get_ep() or zwep_get_next() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
	}

	*desc_container_endpoint_p = desc_container_endpoint;

	return ZWP_STATUS_OK;

l_err_desc_container_endpoint_alloc:
	//Deallocate all endpoints
	endpoint = desc_container_endpoint;

	while(endpoint)
	{
		zwp_desc_container_interface_free((zwp_desc_container_interface_t)endpoint->child);

		endpoint_next = endpoint->next;

		desc_endpoint = (zwepd_t *)endpoint->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint ID: %u (free)", desc_endpoint->epid);
		ZWP_FREE(endpoint);

		endpoint = endpoint_next;
	}

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize and free the endpoint descriptor containers list
 * @param[in]     desc_container_endpoint Head of endpoint descriptor containers list
 * @return        Nothing
 * @see zwp_desc_container_endpoint_alloc
 */
static void zwp_desc_container_endpoint_free(zwp_desc_container_endpoint_t desc_container_endpoint)
{
	zwp_desc_container_endpoint_t       endpoint;
	zwp_desc_container_endpoint_t       endpoint_next;

	#if ZWP_LOG_DEBUG_IS_ENABLED
	zwepd_t                             *desc_endpoint;
	#endif

	endpoint = desc_container_endpoint;

	while(endpoint)
	{
		zwp_desc_container_interface_free((zwp_desc_container_interface_t)endpoint->child);

		endpoint_next = endpoint->next;

		#if ZWP_LOG_DEBUG_IS_ENABLED
		desc_endpoint = (zwepd_t *)endpoint->desc;
		ZWP_LOG(ZWP_LOG_DEBUG, "Endpoint ID: %u (free)", desc_endpoint->epid);
		#endif

		ZWP_FREE(endpoint);

		endpoint = endpoint_next;
	}
}

/**
 * @brief Allocate and initialize all node descriptor containers and all their children
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     zwnet             HCAPI network handle
 * @param[out]    desc_container_node_p  Head of node descriptor containers list
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_container_node_free() to free resources
 */
static zwp_status_t zwp_desc_container_node_alloc(zwp_desc_net_t desc_net, zwnet_p zwnet, zwp_desc_container_node_t *desc_container_node_p)
{
	int                             hcapi_code;
	zwp_desc_container_node_t       desc_container_node;
	zwp_desc_container_node_t       node;
	zwp_desc_container_node_t       node_next;
	zwnoded_t                       *desc_node;

	desc_container_node = NULL;

	if ((node_next = (zwp_desc_container_node_t)ZWP_MALLOC(sizeof(*node_next) + sizeof(zwnoded_t))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_desc_container_node_alloc;
	}

	//Record the head of node tree
	desc_container_node = node_next;
	node = NULL;

	for (hcapi_code = zwnet_get_node(zwnet, (zwnoded_t *)node_next->desc);
			hcapi_code == ZW_ERR_NONE;
			hcapi_code = zwnode_get_next((zwnoded_t *)node->desc, (zwnoded_t *)node_next->desc))
	{
		node = node_next;
		node->user = NULL;
		node->next = node->child = NULL;

		desc_node = (zwnoded_t *)node->desc;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %u", desc_node->nodeid);

		if (zwp_desc_container_endpoint_alloc(desc_net, desc_node, (zwp_desc_container_endpoint_t *)&node->child) != ZWP_STATUS_OK)
		{
			goto l_err_desc_container_node_alloc;
		}

		if (zwp_node_state_create(desc_node) != ZWP_STATUS_OK)
		{
			goto l_err_desc_container_node_alloc;
		}

		if ((node_next = (zwp_desc_container_node_t)ZWP_MALLOC(sizeof(*node_next) + sizeof(zwnoded_t))) == NULL)
		{
			ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
			goto l_err_desc_container_node_alloc;
		}

		//Sibling link at node level
		node->next = node_next;
	}

	if (node)
	{
		node->next = NULL;
	}
	else
	{
		//Zero nodes
		desc_container_node = NULL;
	}
	//Free the extra node
	ZWP_FREE(node_next);

	if (hcapi_code != ZW_ERR_NODE_NOT_FOUND)
	{
		ZWP_LOG(ZWP_LOG_WARNING, "zwnet_get_node() or zwnode_get_next() failed with '%s'", zwp_hcapi_error_code_to_string(hcapi_code));
	}

	*desc_container_node_p = desc_container_node;

	return ZWP_STATUS_OK;

l_err_desc_container_node_alloc:
	//Deallocate all nodes
	node = desc_container_node;

	while(node)
	{
		desc_node = (zwnoded_t *)node->desc;

		// Free node level data
		zwp_node_state_destroy(desc_node);

		zwp_desc_container_endpoint_free((zwp_desc_container_endpoint_t)node->child);

		node_next = node->next;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %u (free)", desc_node->nodeid);
		ZWP_FREE(node);

		node = node_next;
	}

	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize and free the node descriptor containers list
 * @param[in]     desc_container_node Head of node descriptor containers list
 * @return        Nothing
 * @see zwp_desc_container_node_alloc
 */
static void zwp_desc_container_node_free(zwp_desc_container_node_t desc_container_node)
{
	zwp_desc_container_node_t       node;
	zwp_desc_container_node_t       node_next;
	zwnoded_t                       *desc_node;

	node = desc_container_node;

	while(node)
	{
		desc_node = (zwnoded_t *)node->desc;

		// Free node level data
		zwp_node_state_destroy(desc_node);

		zwp_desc_container_endpoint_free((zwp_desc_container_endpoint_t)node->child);

		node_next = node->next;

		ZWP_LOG(ZWP_LOG_DEBUG, "Node ID: %u (free)", desc_node->nodeid);
		ZWP_FREE(node);

		node = node_next;
	}
}

/**
 * @brief Allocate and initialize the network descriptor container and all its children
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     zwnet             HCAPI network handle
 * @param[out]    desc_container_net_p  Network descriptor container object
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_container_net_free() to free resources
 */
static zwp_status_t zwp_desc_container_net_alloc(zwp_desc_net_t desc_net, zwnet_p zwnet, zwp_desc_container_net_t *desc_container_net_p)
{
	zwp_desc_container_net_t        desc_container_net;

	desc_container_net = NULL;

	if(!desc_net || !zwnet)
		goto l_err_invalaid_desc_net;

	if (zwp_desc_container_node_alloc(desc_net, zwnet, (zwp_desc_container_node_t *)&desc_container_net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_container_net_alloc;
	}

	*desc_container_net_p = desc_container_net;

	return ZWP_STATUS_OK;

l_err_invalaid_desc_net:
l_err_desc_container_net_alloc:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Uninitialize and free the network descriptor container and all its children
 * @param[in]     desc_container_net Network descriptor container object
 * @return        Nothing
 * @see zwp_desc_container_net_alloc
 */
static void zwp_desc_container_net_free(zwp_desc_container_net_t desc_container_net)
{
		zwp_desc_container_node_free((zwp_desc_container_node_t)desc_container_net);
}

zwp_status_t zwp_desc_net_init(zwp_desc_net_t *desc_net_p, zwp_net_t net)
{
	zwp_desc_net_t desc_net;

	if ((desc_net = (zwp_desc_net_t)ZWP_MALLOC(sizeof(*desc_net))) == NULL)
	{
		ZWP_LOG(ZWP_LOG_ERR, ZWP_MSG_MALLOC_FAILED);
		goto l_err_desc_net_alloc;
	}

	//Back reference to network
	desc_net->net = net;

	if (zwp_desc_container_net_alloc(desc_net, zwp_net_get_zwnet(net), &desc_net->desc_container_net) != ZWP_STATUS_OK)
	{
		goto l_err_desc_container_net_alloc;
	}

	desc_net->utime = zwu_time_now();

	*desc_net_p = desc_net;

	return ZWP_STATUS_OK;

l_err_desc_container_net_alloc:
	ZWP_FREE(desc_net);
l_err_desc_net_alloc:
	return ZWP_STATUS_ERROR;
}

void zwp_desc_net_exit(zwp_desc_net_t desc_net)
{
	if (desc_net)
	{
		desc_net->utime = zwu_time_now();

		zwp_desc_container_net_free(desc_net->desc_container_net);

		ZWP_FREE(desc_net);
	}

	return;
}


