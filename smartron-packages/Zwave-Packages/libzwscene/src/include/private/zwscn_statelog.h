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
@file		zwscn_statelog.h  
  
                To provide support for Scene Statelog functions
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_STATELOG_H_
#define _ZWSCN_STATELOG_H_

/** @defgroup GroupPrivateStatelog Statelog
 *  Functions for handling Statelog (data to indicate current and last state)
 *  @ingroup GroupPrivateGeneral
 */
///@{

/** Statelog data */
typedef struct {
	uint8_t			curr_op;			/**< Current Operation @li none @li update all @li update single scene @li scene add @li scene delete @li scene exec */
	uint32_t		curr_op_scened;		/**< Current Operation Scene id */
	uint8_t			curr_op_trigger; 	/**< Current Operation Trigger @remark Used for Scene execute e.g. @li Manual @li Schedule @li Device Event */
	
	uint8_t			last_op;			/**< Last Operation */
	uint32_t		last_op_scened;		/**< Last Operation Scene id */
	uint8_t			last_op_trigger;	/**< Last Operation Trigger @remark Used for Scene execute e.g. @li Manual @li Schedule @li Device Event */
	zwscn_time_t	last_op_time;		/**< Last Operation time */
} zwscn_statelog_t, *zwscn_statelog_p;	/**< Statelog data */

/** Security Scenes Statelog data */
typedef struct {
	uint32_t	counter;	/**< Security Scenes statelog change counter */
	uint32_t	scened;		/**< Security Scenes statelog last changed Security Scene id */
} zwscn_statelog_security_scene_t, *zwscn_statelog_security_scene_p;	/**< Security Scenes Statelog data */

/**
 * @brief	Statelog Set value
 * @param[in] net 			Network runtime context
 * @param[in] category		Statelog category (e.g. @ref ZWSCN_STATELOG_OP_SCENE_EXEC)
 * @param[in] subcategory	Statelog subcategory
 * @param[in] value			Value to set
 */
void zwscn_statelog_set(void *net, uint32_t category, uint32_t subcategory, uint64_t value);
/**
 * @brief	Statelog Get value
 * @param[in] net 			Network runtime context
 * @param[in] category		Statelog category
 * @param[in] subcategory	Statelog subcategory
 * @param[out] value		Value
 * @return     ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_statelog_get(void *net, uint32_t category, uint32_t subcategory, uint64_t *value);
/**
 * @brief	Mark the start of an operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 * @param[in] category	Statelog category
 * @param[in] op		Statelog operation
 */
void zwscn_set_statelog_start_op(void *net, uint32_t scene_id, uint8_t trigger, uint8_t category, uint8_t op);
/**
 * @brief	Mark the end of an operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 * @param[in] category	Statelog category
 * @param[in] op		Statelog operation
 */
void zwscn_set_statelog_end_op(void *net, uint32_t scene_id, uint8_t trigger, uint8_t category, uint8_t op);
/**
 * @brief	Mark the start of an Add operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_add(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of an Add operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_add(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of an Add Modify operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_add_modify(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of an Add Modify operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_add_modify(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of a Delete operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_del(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of a Delete operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_del(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of a Delete Modify operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_del_modify(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of a Delete Modify operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_del_modify(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of an Execute operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_exec(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of an Execute operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_exec(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of a Status Update operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_status_update(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of a Status Update operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_status_update(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of a Status Update On Report operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_start_op_status_update_on_report(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the end of a Status Update On Report operation in Statelog
 * @param[in] net 		Network runtime context
 * @param[in] scene_id	Scene id
 * @param[in] trigger	Trigger
 */
void zwscn_set_statelog_end_op_status_update_on_report(void *net, uint32_t scene_id, uint8_t trigger);
/**
 * @brief	Mark the start of a Status Update All operation in Statelog
 * @param[in] net 		Network runtime context
 */
void zwscn_set_statelog_start_op_status_update_all(void *net);
/**
 * @brief	Mark the end of a Status Update All operation in Statelog
 * @param[in] net 		Network runtime context
 */
void zwscn_set_statelog_end_op_status_update_all(void *net);
/**
 * @brief	Mark the start of a Status Update Pending operation in Statelog
 * @param[in] net 		Network runtime context
 */
void zwscn_set_statelog_start_op_status_update_pending(void *net);
/**
 * @brief	Mark the end of a Status Update Pending operation in Statelog
 * @param[in] net 		Network runtime context
 */
void zwscn_set_statelog_end_op_status_update_pending(void *net);
/**
 * @brief Set Security Scenes statelog scene id
 * @param[in] net 		Network runtime context
 * @param[in] category	Statelog category
 * @param[in] scened	Security Scene id
 */
void zwscn_statelog_security_scene_set(void *net, uint32_t category, uint32_t scened);
/**
 * @brief	Get Security Scenes statelog
 * @param[in] net 			Network runtime context
 * @param[in] category		Statelog category
 * @param[out] counter_ptr	Change counter
 * @param[out] scened_ptr	Scene id
 * @return ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T zwscn_statelog_security_scene_get(void *net, uint32_t category, uint32_t *counter_ptr, uint32_t *scened_ptr);
///@}

#endif // _ZWSCN_STATELOG_H_
