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
 * @file    zws_cgi_common_networks.h
 *
 * @brief   CGI Common - Networks Portion
 * @details Handles networks portion common to CGI service.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-08-13
 * - Initial version
 */

#ifndef ZWS_CGI_COMMON_NETWORKS_H_
#define ZWS_CGI_COMMON_NETWORKS_H_

/**
 * \addtogroup  zwebcgiservice
 * @{
 */

#include "zws_status.h"
#include "zw_avro.h"

/** Node ID bitmask */
#define ZWS_CGI_NODE_ID_MASK             0x000000FF
/** Node ID shift */
#define ZWS_CGI_NODE_ID_SHIFT            0
/** Endpoint ID bitmask */
#define ZWS_CGI_ENDPOINT_ID_MASK         0x0000FF00
/** Endpoint ID shift */
#define ZWS_CGI_ENDPOINT_ID_SHIFT        8
/** Interface ID bitmask */
#define ZWS_CGI_INTERFACE_ID_MASK        0xFFFF0000
/** Interface ID shift */
#define ZWS_CGI_INTERFACE_ID_SHIFT       16

/** Sub command not specified */
#define ZWS_CGI_NET_SUB_COMMAND_NONE     (-1)

/** Combo ID not specified */
#define ZWS_CGI_NET_COMBO_ID_NONE        (-1L)
/** Node ID not specified */
#define ZWS_CGI_NODE_ID_NONE             (-1)
/** Endpoint ID not specified */
#define ZWS_CGI_ENDPOINT_ID_NONE         (-1)
/** Interface ID not specified */
#define ZWS_CGI_INTERFACE_ID_NONE        (-1)

/*
 * @note Node descriptor id for node 3 is 3. Endpoint descriptor id for node 3 endpoint 0 is also 3.
 * So to guarantee uniqueness of all descriptor ids endpoint id is incremented/decremented by 1.
 */

/** Get node ID from combo ID */
#define ZWS_CGI_GET_NODE_ID(desc)            (((desc) & ZWS_CGI_NODE_ID_MASK) >> ZWS_CGI_NODE_ID_SHIFT)
/** Get node ID component of combo ID */
#define ZWS_CGI_GET_NODE_DESC_ID(node_id)    (((node_id) << ZWS_CGI_NODE_ID_SHIFT) & ZWS_CGI_NODE_ID_MASK)

/** Get endpoint ID from combo ID */
#define ZWS_CGI_GET_ENDPOINT_ID(desc)        ((((desc) & ZWS_CGI_ENDPOINT_ID_MASK) >> ZWS_CGI_ENDPOINT_ID_SHIFT) - 1)
/** Get endpoint ID component of combo ID */
#define ZWS_CGI_GET_ENDPOINT_DESC_ID(combo_id, endpoint_id)  \
                                             (combo_id) | \
                                             (((endpoint_id + 1) << ZWS_CGI_ENDPOINT_ID_SHIFT) & ZWS_CGI_ENDPOINT_ID_MASK)

/** Get interface ID from combo ID */
#define ZWS_CGI_GET_INTERFACE_ID(desc)       (((desc) & ZWS_CGI_INTERFACE_ID_MASK) >> ZWS_CGI_INTERFACE_ID_SHIFT)
/** Get interface ID component of combo ID */
#define ZWS_CGI_GET_INTERFACE_DESC_ID(combo_id, interface_id)  \
                                             (combo_id) | \
                                             (((interface_id) << ZWS_CGI_INTERFACE_ID_SHIFT) & ZWS_CGI_INTERFACE_ID_MASK)

/** Forward declaration for 'CGI command' class for networks */
enum _zws_cgi_command_networks;

/** CGI network command type */
typedef enum _zws_cgi_command_networks zws_cgi_command_networks_t;

/** CGI network command enumeration */
enum _zws_cgi_command_networks
{
	ZWS_CGI_NETWORKS_COMMAND_NONE,
	ZWS_CGI_NETWORKS_COMMAND_INFO_GET,
	ZWS_CGI_NETWORKS_COMMAND_NODE_LIST_GET
};

/** Forward declaration for 'CGI command' class for nodes */
enum _zws_cgi_command_nodes;

/** CGI node command type */
typedef enum _zws_cgi_command_nodes zws_cgi_command_nodes_t;

/** CGI node command enumeration */
enum _zws_cgi_command_nodes
{
	ZWS_CGI_NODES_COMMAND_NONE,
	ZWS_CGI_NODES_COMMAND_ENDPOINT_LIST_GET
};

/** Forward declaration for 'CGI command' class for endpoints */
enum _zws_cgi_command_endpoints;

/** CGI endpoint command type */
typedef enum _zws_cgi_command_endpoints zws_cgi_command_endpoints_t;

/** CGI endpoint command enumeration */
enum _zws_cgi_command_endpoints
{
	ZWS_CGI_ENDPOINTS_COMMAND_NONE,
	ZWS_CGI_ENDPOINTS_COMMAND_INTERFACE_LIST_GET
};

/** Forward declaration for 'CGI command' class for interfaces */
enum _zws_cgi_command_interfaces;

/** CGI interface command type */
typedef enum _zws_cgi_command_interfaces zws_cgi_command_interfaces_t;

/** CGI interface command enumeration */
enum _zws_cgi_command_interfaces
{
	ZWS_CGI_INTERFACES_COMMAND_NONE,
	ZWS_CGI_INTERFACES_COMMAND_SWITCH_BINARY
};
/** }@ */
#endif /* ZWS_CGI_COMMON_NETWORKS_H_ */
