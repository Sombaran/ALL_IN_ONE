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
@file		zwscn_persistence_security_scene.h - security scenes persistent storage
  
                To provide support for persistent storage for Security Scenes
  
@author		Arif Mohammand
 
@version	1.0 2015-04-07  Initial release
*/

#ifndef _ZWSCN_PERSISTENCE_SECURITY_SCENE_H_
#define	_ZWSCN_PERSISTENCE_SECURITY_SCENE_H_

/** @defgroup GroupPrivatePersistenceSecurityScene Security Scenes persistent storage
 *  Functions for saving, loading and deleting Security Scenes to/from persistent storage
 *  @ingroup GroupPrivateGeneral
 */
///@{

//#define ZWSCN_LOAD_SCENE_WITH_NO_ACTION	1	/* if this is commented then the scenes without actions will not be loaded */

#define ZWSCN_SECURITY_SCENE_FILENAME_EXT	".ssn"		/**< Security Scene persistent file extension */
#define ZWSCN_FILENAME_SCENEID_LEN	8	/**< Length of Security Scene id in filename */
#define ZWSCN_FILENAME_EXT_LEN		4	/**< Length of file extension in filename */
#define ZWSCN_FILENAME_EXACT_LEN	((ZWSCN_LEN_CONTEXT_PERSISTENT - 1) + ZWSCN_FILENAME_SCENEID_LEN + ZWSCN_FILENAME_EXT_LEN) /**< Length of filename without string terminator */
#define ZWSCN_FILENAME_LEN			(ZWSCN_FILENAME_EXACT_LEN + 1)					/**< Length of filename with string terminator */
#define ZWSCN_FILENAME_DIR_LEN		4096											/**< Directory path length @remark same as linux PATH_MAX value */
#define ZWSCN_FILENAME_PATH_LEN		(ZWSCN_FILENAME_LEN + ZWSCN_FILENAME_DIR_LEN)	/**< Directory path and filename length */

#define ZWSCN_TAG_SECURITY_SCENE	0x10	/**< Security Scene tag identifier */
#define ZWSCN_TAG_ARM_EVENT			0x11	/**< Security Scene ARM Event tag identifier */
#define ZWSCN_TAG_DISARM_EVENT		0x12	/**< Security Scene DISARM Event tag identifier */
#define ZWSCN_TAG_ALARM_EVENT		0x13	/**< Security Scene ALARM Event tag identifier */

#define ZWSCN_SUBTAG_SCENE_ID							0x01	/**< Security Scene id subtag identifier */
#define ZWSCN_SUBTAG_SCENE_NAME							0x02	/**< Security Scene name subtag identifier */
#define ZWSCN_SUBTAG_SCENE_ACTIVE						0x03	/**< Security Scene active status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_IS_ARMED						0x04	/**< Security Scene is_armed status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_IS_ALARMED					0x05	/**< Security Scene is_alarmed status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_ID_AT_ARM					0x06	/**< Security Scene scene_id_at_arm subtag identifier */
#define ZWSCN_SUBTAG_SCENE_ID_AT_DISARM					0x07	/**< Security Scene scene_id_at_disarm subtag identifier */
#define ZWSCN_SUBTAG_SCENE_ID_AT_ALARM					0x08	/**< Security Scene scene_id_at_alarm subtag identifier */
#define ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_ON			0x09	/**< Security Scene is_notification_on status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_SMS_ON	0x10	/**< Security Scene is_notification_by_sms_on status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_IS_NOTIFICATION_BY_EMAIL_ON	0x11	/**< Security Scene is_notification_by_email_on status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_NOTIFICATION_SMS_NUMBER		0x12	/**< Security Scene notification_sms_number status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_NOTIFICATION_EMAIL			0x13	/**< Security Scene notification_email status subtag identifier */
#define ZWSCN_SUBTAG_SCENE_LT_ARM_TYPE					0x14	/**< Security Scene Arm last trigger type */
#define ZWSCN_SUBTAG_SCENE_LT_ARM_TIME					0x15	/**< Security Scene Arm last trigger time */
#define ZWSCN_SUBTAG_SCENE_LT_DISARM_TYPE				0x16	/**< Security Scene Disarm last trigger type */
#define ZWSCN_SUBTAG_SCENE_LT_DISARM_TIME				0x17	/**< Security Scene Disarm last trigger time */
#define ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TYPE				0x18	/**< Security Scene Alarm On last trigger type */
#define ZWSCN_SUBTAG_SCENE_LT_ALARM_ON_TIME				0x19	/**< Security Scene Alarm On last trigger time */
#define ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TYPE			0x20	/**< Security Scene Alarm Off last trigger type */
#define ZWSCN_SUBTAG_SCENE_LT_ALARM_OFF_TIME			0x21	/**< Security Scene Alarm Off last trigger time */

#define ZWSCN_SUBTAG_EVENT_TYPE					0x01	/**< Event type subtag identifier */
#define ZWSCN_SUBTAG_EVENT_ID					0x02	/**< Event id subtag identifier */
#define ZWSCN_SUBTAG_EVENT_CMD					0x03	/**< Event cmd value subtag identifier */
#define ZWSCN_SUBTAG_EVENT_DEVICE_ARGS_SIZE		0x04	/**< Event Device Arguments size subtag identifier */
#define ZWSCN_SUBTAG_EVENT_DEVICE_ARGS			0x05	/**< Event Device Arguments subtag identifier */

/**
 * @brief	Save a Security Scene in persistent storage
 * @param[in] net		Network runtime context
 * @param[in] scene_ptr	Security Scene pointer
 * @return	  ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_persistence_save_security_scene(void *net, zwscn_security_scene_cont_p scene_ptr);
/**
 * @brief	Load all Security Scenes from persistent storage
 * @param[in]	net			Network runtime context
 * @param[out]	out_list	Security Scene list
 * @return	    ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_persistence_load_all_security_scenes(void *net, zwscn_list_t *out_list);
/**
 * @brief	Delete a Security Scene from persistent storage
 * @param[in] net		Network runtime context
 * @param[in] scene_id	Security Scene id
 * @return    ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_persistence_delete_security_scene(void *net, uint32_t scene_id);
/**
 * @brief	Delete all Security Scenes from persistent storage
 * @param[in] net			Network runtime context
 * @param[in] scene_list 	Security Scene list
 * @return    ZWSCN_STATUS_T
 */
ZWSCN_STATUS_T	zwscn_persistence_delete_all_security_scenes(void *net, zwscn_list_t scene_list);
///@}

#endif	/* _ZWSCN_PERSISTENCE_SECURITY_SCENE_H_ */

