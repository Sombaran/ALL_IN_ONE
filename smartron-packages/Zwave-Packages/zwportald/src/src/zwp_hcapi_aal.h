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
 * @file    zwp_hcapi_aal.h
 *
 * @brief   HCAPI Application Abstraction Layer
 * @details Provide HCAPI abstration for notification and report callbacks.
 *
 * @author  David Chow
 *
 * @version 1.0 - 2013-11-21
 * - Initial version
 */

#ifndef ZWP_HCAPI_AAL_H_
#define ZWP_HCAPI_AAL_H_

/**
 * \addtogroup  zwportaldzwarec
 * @{
 */

#include "zwp_network_define.h"
#include "zwscn_scene_api.h"

/** HCAPI AAL context*/
typedef struct
{
    zwp_net_t               net;            /**< Network */
	zwnet_p                 zwnet;          /**< Z-Wave network */
    zwscn_init_response_t   cb_fn;          /**< Scene notification and report callback functions */
	zwnet_notify_fn	        notify_cb;      /**< Command status callback */
	zwnet_node_fn	        node_cb;        /**< Node add/del callback */
    zwrep_ts_fn             basic_rpt;      /**< Basic report callback */
    zwrep_switch_fn         switch_rpt;     /**< Binary switch report callback */
    zwrep_ts_fn             level_rpt;      /**< Multi-level switch report callback */
    zwrep_color_sw_get_fn   color_rpt;      /**< Color switch report callback */
    zwrep_dlck_op_fn        dlck_op_rpt;    /**< Door lock operation report callback */
    zwrep_thrmo_setp_fn     thrmo_setp_rpt; /**< Thermostat setpoint report callback */
    zwrep_bsensor_fn        bsensor_rpt;    /**< Binary sensor report callback */
    zwrep_sensor_fn         sensor_rpt;     /**< Sensor report callback */
	zwrep_alrm_fn			alrm_rpt;		/**< Alarm report callback */
	zwrep_csc_fn			central_scene_rpt;		/**< Central Scene report callback */
	zwrep_barrier_fn		barrier_op_rpt;		/**< Barrier Operator report callback */
	zwrep_barrier_subsys_fn		barrier_op_subsys_rpt;		/**< Barrier Operator Subsystem report callback */
} hcaal_ctx_t, *hcaal_ctx_p;


int hcaal_init(hcaal_ctx_p hcaal_ctx, zwp_net_t net,
               zwnet_init_p init, zwnet_p *zwnet);
void hcaal_exit(hcaal_ctx_p hcaal_ctx, zwnet_p net);
void hcaal_scene_cb_set(hcaal_ctx_p hcaal_ctx, zwscn_init_response_p scene_cb);
int hcaal_rpt_setup(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd);
int hcaal_set(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *args, zwscn_fn_postset_poll_cb_t postset_poll_cb);
int hcaal_get(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, uint8_t cmd, void *args, zwscn_device_zwpoll_req_t *poll_req);
int hcaal_get_nameloc(void *net, uint8_t node_id, uint8_t ep_id, uint16_t cls_id, char *name_buf, size_t name_buf_size, char *loc_buf, size_t loc_buf_size);
int hcaal_poll_rm(void *net, uint16_t handle);
int hcaal_poll_rm_mul(void *net, uint32_t usr_token);
ZWSCN_BOOL_T hcaal_is_obj_valid(void *net, ZWSCN_TYPE_VALIDITY_CHECK_T flag_type, uint8_t node_id, uint8_t ep_id, uint16_t cls_id);

/** Interfaces functions*/
#define hcaal_basic_get         zwif_basic_get
#define hcaal_switch_get        zwif_switch_get
#define hcaal_level_get         zwif_level_get
#define hcaal_color_get         zwif_color_sw_get
#define hcaal_dlck_op_get       zwif_dlck_op_get
#define hcaal_thrmo_setp_get    zwif_thrmo_setp_get
#define hcaal_bsensor_get       zwif_bsensor_get
#define hcaal_sensor_get        zwif_sensor_get

int hcaal_basic_rpt_set(zwifd_p ifd, zwrep_ts_fn rpt_cb);
int hcaal_switch_rpt_set(zwifd_p ifd, zwrep_switch_fn rpt_cb);
int hcaal_level_rpt_set(zwifd_p ifd, zwrep_ts_fn rpt_cb);
int hcaal_color_rpt_set(zwifd_p ifd, zwrep_color_sw_get_fn rpt_cb);
int hcaal_dlck_op_rpt_set(zwifd_p ifd, zwrep_dlck_op_fn rpt_cb);
int hcaal_thrmo_setp_rpt_set(zwifd_p ifd, zwrep_thrmo_setp_fn rpt_cb);
int hcaal_bsensor_rpt_set(zwifd_p ifd, zwrep_bsensor_fn rpt_cb);
int hcaal_sensor_rpt_set(zwifd_p ifd, zwrep_sensor_fn rpt_cb);
int hcaal_alrm_rpt_set(zwifd_p ifd, zwrep_alrm_fn rpt_cb);
int hcaal_central_scene_rpt_set(zwifd_p ifd, zwrep_csc_fn rpt_cb);
int hcaal_barrier_op_rpt_set(zwifd_p ifd, zwrep_barrier_fn rpt_cb);
int hcaal_barrier_op_subsys_rpt_set(zwifd_p ifd, zwrep_barrier_subsys_fn rpt_cb);
/** }@ */

#endif /* ZWP_HCAPI_AAL_H_ */
