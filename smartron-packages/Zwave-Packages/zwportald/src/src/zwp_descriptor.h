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
 * @file    zwp_descriptor.h
 *
 * @brief   Descriptor management of Z-Wave Network hierarchy
 * @details Handles the descriptors at Node, Endpoint and Interface level in a Z-Wave network.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-07-10
 * - Initial version
 */

#ifndef ZWP_DESCRIPTOR_H_
#define ZWP_DESCRIPTOR_H_

/**
 * \addtogroup  zwportaldzwave
 * @{
 */

#include "zwp_descriptor_define.h"
#include "zwp_interface_state_define.h"
#include "zwp_status.h"
#include "zwu_array_list.h"
#include "zwp_network.h"
#include "zwu_time.h"

 /**
 * @brief Network descriptor class
 */
typedef struct _zwp_desc_IF_ep_name
{
	//zwifd_t         desc_interface;				/**< Interface descriotor */
	zwifd_p         desc_interface;				/**< Interface descriotor */
	char		    name[ZW_LOC_STR_MAX + 1];   /**< user configured name string of the endpoint */
	char		    loc[ZW_LOC_STR_MAX + 1];	/**< user configured location string of the endpoint */
}zwp_desc_IF_ep_name_t;

/**
 * @name Getters
 * @{
 */
/**
 * @brief Get last updated time of network descriptor
 * @param[in]     desc_net          Network descriptor object
 * @return        Last updated time
 */
zwu_time_t zwp_desc_get_network_utime(zwp_desc_net_t desc_net);
/**
 * @}
 */

/**
 * @brief Get interface descriptor from Node ID, Endpoint ID and Interface ID
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_id           Node ID
 * @param[in]     endpoint_id       Endpoint ID
 * @param[in]     interface_id      Interface ID
 * @param[out]    desc_interface_p  Interface descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_get_desc_interface(zwp_desc_net_t desc_net, int node_id, int endpoint_id, int interface_id, zwifd_p *desc_interface_p);

/**
 * @brief Get endpoint descriptor from Node ID and Endpoint ID
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_id           Node ID
 * @param[in]     endpoint_id       Endpoint ID
 * @param[out]    desc_endpoint_p   Endpoint descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_get_desc_endpoint(zwp_desc_net_t desc_net, int node_id, int endpoint_id, zwepd_p *desc_endpoint_p);

/**
 * @brief Get node descriptor from Node ID
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_id           Node ID
 * @param[out]    desc_node_p       Node descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_get_desc_node(zwp_desc_net_t desc_net, int node_id, zwnoded_p *desc_node_p);

/**
 * @brief Get user specific information at interface level
 * @param[in]     desc_interface    Interface descriptor object
 * @return        User specific information at interface level
 */
void *zwp_desc_interface_get_user(zwifd_p desc_interface);

/**
 * @brief Set user specific information at interface level
 * @param[in]     desc_interface    Interface descriptor object
 * @param[in]     user              User specific information at interface level
 * @return        Nothing
 */
void zwp_desc_interface_set_user(zwifd_p desc_interface, void *user);

/**
 * @brief Get free handler \link zwp_interface_state_free_t \endlink for interface level state
 * @param[in]     desc_interface    Interface descriptor object
 * @return        Free handler for interface level state
 */
zwp_interface_state_free_t zwp_desc_interface_get_state_free_handler(zwifd_p desc_interface);

/**
 * @brief Set free handler \link zwp_interface_state_free_t \endlink for interface level state
 * @param[in]     desc_interface    Interface descriptor object
 * @param[in]     state_free_handler Free handler for interface level state
 * @return        Nothing
 */
void zwp_desc_interface_set_state_free_handler(zwifd_p desc_interface, zwp_interface_state_free_t state_free_handler);

/**
* @brief Get the net pointer from interface descriptor
* @param[in]     desc_interface    Interface descriptor object
* @return        the net pointer at interface level
*/
zwp_net_t zwp_desc_interface_get_net(zwifd_p desc_interface);

/**
 * @brief Get user specific information at node level
 * @param[in]     desc_node         Node descriptor object
 * @return        User specific information at node level
 */
void *zwp_desc_node_get_user(zwnoded_p desc_node);

/**
 * @brief Set user specific information at node level
 * @param[in]     desc_node         Node descriptor object
 * @param[in]     user              User specific information at node level
 * @return        Nothing
 */
void zwp_desc_node_set_user(zwnoded_p desc_node, void *user);

/**
 * @brief Get secure state of the node
 * @param[in]     desc_node         Node descriptor object
 * @retval        0                 Non-secure
 * @retval        1                 Secure
 */
uint8_t zwp_desc_node_get_secure_state(zwnoded_p desc_node);

/**
 * @brief Free interface level user specific information of all interfaces under a node
 * @param[in]     desc_node         Node descriptor object
 * @return        Nothing
 */
void zwp_desc_interface_state_under_node_destroy(zwnoded_p desc_node);

/**
 * @brief Free interface level user specific information of all interfaces under a network
 * @param[in]     desc_net          Network descriptor object
 * @return        Nothing
 */
void zwp_desc_interface_state_under_net_destroy(zwp_desc_net_t desc_net);

/**
 * @brief Free node level user specific information of all nodes under a network
 * @param[in]     desc_net          Network descriptor object
 * @return        Nothing
 */
void zwp_desc_node_state_under_net_destroy(zwp_desc_net_t desc_net);

/**
@brief Return an array of interfaces and their respective endpoints based on interface id.
@param[in]	desc_net			Network descriptor object
@param[in]	interface_id		Interface ID
@param[out]	if_ep_name_list_p	An array of interfaces with respective endpoint name. List elements of type <em> zwp_desc_IF_ep_name_t</em>
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_desc_get_device_from_interface_id(zwp_desc_net_t desc_net,
												   uint16_t interface_id, 
												   zwu_array_list_t *if_ep_name_list_p);

/**
* @brief Free list of interface and endpoint names got from zwp_desc_get_device_from_interface_id()
* @return        Nothing
*/
void zwp_desc_free_if_ep_name_list(zwu_array_list_t if_ep_name_list);

/**
@brief Get the corresponding endpoint name and location from interface descriptor.
@param[in]	desc_interface		Interface descriptor object
@param[out]	pEpNameLoc			Pointer to the corresponding endpoint's name and location
@return ZWP_STATUS_XXX
*/
zwp_status_t zwp_desc_get_endpoint_nameloc_from_interface(zwifd_p desc_interface,
	zw_nameloc_t *pEpNameLoc);

/**
 * @brief Get list of interfaces under an endpoint
 * @param[in]     desc_endpoint     Endpoint descriptor object
 * @param[out]    interface_list_p  List of interfaces. List elements of type <em> zwifd_t * </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_free_interface_list() to free resources
 */
zwp_status_t zwp_desc_get_interface_list(zwepd_p desc_endpoint, zwu_array_list_t *interface_list_p);

/**
 * @brief Free list of interfaces got from zwp_desc_get_interface_list()
 * @return        Nothing
 */
void zwp_desc_free_interface_list(zwu_array_list_t interface_list);

/**
 * @brief Get list of endpoints under a node
 * @param[in]     desc_node         Node descriptor object
 * @param[out]    endpoint_list_p   List of endpoints. List elements of type <em> zwepd_t * </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_free_endpoint_list() to free resources
 */
zwp_status_t zwp_desc_get_endpoint_list(zwnoded_p desc_node, zwu_array_list_t *endpoint_list_p);

/**
 * @brief Free list of endpoints got from zwp_desc_get_endpoint_list()
 * @return        Nothing
 */
void zwp_desc_free_endpoint_list(zwu_array_list_t endpoint_list);

/**
 * @brief Get list of endpoints under a network
 * @param[in]     desc_net          Network descriptor object
 * @param[out]    node_endpoint_list_p  List of endpoints. List elements of type <em> zwepd_t * </em>
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_free_node_endpoint_list() to free resources
 */
zwp_status_t zwp_desc_get_node_endpoint_list(zwp_desc_net_t desc_net, zwu_array_list_t *node_endpoint_list_p);

/**
 * @brief Free list of endpoints got from zwp_desc_get_node_endpoint_list()
 * @return        Nothing
 */
void zwp_desc_free_node_endpoint_list(zwu_array_list_t node_endpoint_list);

/**
 * @brief Get list of nodes under a network
 * @param[in]     desc_net          Network descriptor object
 * @param[out]    node_list_p       List of nodes. Each list element points to a memory allocation for <em> zwnoded_t </em> and \link zwp_node_state_t \endlink appended together
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_free_node_list() to free resources
 */
zwp_status_t zwp_desc_get_node_list(zwp_desc_net_t desc_net, zwu_array_list_t *node_list_p);

/**
 * @brief Free list of nodes got from zwp_desc_get_node_list()
 * @return        Nothing
 */
void zwp_desc_free_node_list(zwu_array_list_t node_list);

/**
 * @brief Add (if not present) or update (if present) a node to the network
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_desc_new     HCAPI Node descriptor
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_add_or_update_node(zwp_desc_net_t desc_net, zwnoded_p node_desc_new);

/**
 * @brief Remove a node from the network
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_id           Node ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_remove_node(zwp_desc_net_t desc_net, int node_id);

/**
 * @brief Refresh the node level user specific information
 * @param[in]     desc_net          Network descriptor object
 * @param[in]     node_id           Node ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
zwp_status_t zwp_desc_refresh_node(zwp_desc_net_t desc_net, int node_id);

/**
 * @brief Allocate and initialize the descriptor tree for a given network
 * @param[out]    desc_net_p        Network descriptor object
 * @param[in]     net               Network handle
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_desc_net_exit() to free resources
 */
zwp_status_t zwp_desc_net_init(zwp_desc_net_t *desc_net_p, zwp_net_t net);

/**
 * @brief Uninitialize and free the descriptor tree for a given network
 * @param[in]     desc_net          Network descriptor object
 * @return        Nothing
 * @see zwp_desc_net_init
 */
void zwp_desc_net_exit(zwp_desc_net_t desc_net);
/** }@ */
#endif /* ZWP_DESCRIPTOR_H_ */
