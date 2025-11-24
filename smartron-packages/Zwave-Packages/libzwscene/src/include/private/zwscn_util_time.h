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
@file		zwscn_util_time.h  
  
                To provide support for timezone related functionality
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/
#ifndef _ZWSCN_UTIL_TIMEZONE_H_
#define _ZWSCN_UTIL_TIMEZONE_H_

/** @defgroup GroupPrivateUtilTime Time utils
 *  Utility functions for Time and Timezone related operations
 *  @ingroup GroupPrivateUtils
 */
///@{

typedef int64_t		zwscn_timespan_t;		/**< Declaration of the unit for a span of time */

/**
 * @brief	Get current UTC time
 * @return Current UTC time in Unix format
 */
zwscn_time_t 		zwscn_time_now();
/**
 * @brief	Get current UTC time in microseconds
 * @return Current UTC time in microseconds
 */
zwscn_time_t 		zwscn_time_now_microseconds();
/**
 * @brief	Check if a given timezone is valid
 * @param[in] timezone_str	Timezone string
 * @return Boolean value
 */
int 				zwscn_timezone_is_valid(char *timezone_str);
/**
 * @brief	Get current Day of the Week in a given Timezone
 * @param[in] timezone_str Timezone string
 * @return Day of the Week
 */
int 				zwscn_timezone_get_day_of_week_now(char *timezone_str);
/**
 * @brief	Get current Hour in a given Timezone
 * @param[in] timezone_str Timezone string
 * @return Current Hour
 */
int 				zwscn_timezone_get_hour_now(char *timezone_str);
/**
 * @brief	Get current Minute in a given Timezone
 * @param[in] timezone_str Timezone string
 * @return Current Minute
 */
int 				zwscn_timezone_get_minute_now(char *timezone_str);
/**
 * @brief	Get number of microseconds remaining until input Schedule today
 * @param[in] tz_name				Timezone string
 * @param[in] schedule_day_bitmask	Schedule bitmask (for days on which this Schedule is valid)
 * @param[in] schedule_hour			Schedule hour
 * @param[in] schedule_minute		Schedule minute
 * @return Number of microseconds remaining until input Schedule
 */
zwscn_timespan_t 	zwscn_timezone_get_microseconds_to_schedule_today(const char *tz_name, int schedule_day_bitmask, int schedule_hour, int schedule_minute);
/**
 * Get number of microseconds remaining until next Schedule
 * @param[in] tz_name				Timezone string
 * @param[in] schedule_day_bitmask	Schedule bitmask (for days on which this Schedule is valid)
 * @param[in] schedule_hour			Schedule hour
 * @param[in] schedule_minute		Schedule minute
 * @return Number of microseconds remaining until next Schedule
 */
zwscn_timespan_t 	zwscn_timezone_get_microseconds_to_next_schedule(const char *tz_name, int schedule_day_bitmask, int schedule_hour, int schedule_minute);
/**
 * @brief	Check if Schedule is enable today
 * @param[in] tz_name				Timezone string
 * @param[in] schedule_day_bitmask	Schedule bitmask (for days on which this Schedule is valid)
 * @return Boolean value
 */
ZWSCN_BOOL_T 		zwscn_timezone_is_schedule_enabled_today(const char *tz_name, int schedule_day_bitmask);
/**
 * @brief	Get the next valid day for the this Schedule
 * @param[in] schedule_day_bitmask	Schedule bitmask (for days on which this Schedule is valid)
 * @param[in] curr_day				Current day
 * @return  Next valid day for the this Schedule
 */
int 				zwscn_timezone_get_next_schedule_day(int schedule_day_bitmask, int curr_day);
/**
 * @brief	Get the number of days until next valid Schedule
 * @param[in] schedule_day_bitmask	Schedule bitmask (for days on which this Schedule is valid)
 * @param[in] start_day				Start counting from this day
 * @return Number of days until next valid Schedule
 */
int 				zwscn_timezone_get_days_until_next_schedule(int schedule_day_bitmask, int start_day);
/**
 * @brief	Get current time for a Network in printable format
 * @param[in] net		Network runtime context
 * @return Time string
 * @note Free the returned string using zwscn_timezone_free_network_time_string() function
 */
char *				zwscn_timezone_get_network_time(void *net);
/**
 * @brief	Free the time string allocated using zwscn_timezone_get_network_time()
 * @param[in] time_string Time string
 */
void				zwscn_timezone_free_network_time_string(char *time_string);
/**
 * @brief	Get network time from Unix timestamp
 * @param[in] net		Network runtime context
 * @param[in] timestamp	Unix time
 * @return Time string
 */
char *zwscn_timezone_get_network_time_from_timestamp(void *net, uint64_t timestamp);
///@}

#endif // _ZWSCN_UTIL_TIMEZONE_H_
