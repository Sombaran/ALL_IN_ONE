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
@file		zwscn_util_time.c  
  
                To provide support for timezone related functionality
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "glib.h"
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_mqueue_message.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_util_time.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"

zwscn_time_t zwscn_time_now() {
	GDateTime *time_utc; 
	zwscn_time_t time_utc_unix;
	
	time_utc = g_date_time_new_now_utc();
	time_utc_unix = g_date_time_to_unix(time_utc);
	g_date_time_unref(time_utc); //free time_utc
	
	return time_utc_unix;
}

zwscn_time_t zwscn_time_now_microseconds() {
	return g_get_real_time();
}

int zwscn_timezone_is_valid(char *timezone_str) {
	GTimeZone *timezone;
	char timezone_utc_str[]="UTC";
	char timezone_utc_str2[]="Etc/UTC";
	const char *timezone_abbr;
	GDateTime *datetime;
	int result = 0;
	int length = 0;
	int str_comp_result = 0;
	
	if(!timezone_str) {
		goto l_err;
	}
	//Check for know UTC timezones
	length = strlen(timezone_utc_str);
	str_comp_result = strncmp(timezone_utc_str, timezone_str, length);
	if(str_comp_result == 0) {
		result = 1;
		return result;
	}
	length = strlen(timezone_utc_str2);
	str_comp_result = strncmp(timezone_utc_str2, timezone_str, length);
	if(str_comp_result == 0) {
		result = 1;
		return result;
	}
	//Check for timezone validity
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_new_now(timezone);
	timezone_abbr = g_date_time_get_timezone_abbreviation(datetime);
	if(!timezone_abbr) {
		goto l_err;
	}
	if(!strcmp(timezone_utc_str, timezone_abbr)) {
		result = 0; // if the abbreviation of the input timezone comes out as UTC then input was not valid
	} else {
		result = 1; // if it is anything else it means it was a valid timezone string
	}
	
	g_date_time_unref(datetime);
	g_time_zone_unref(timezone);
	return result;
l_err:
	return 0;
}

int zwscn_timezone_get_day_of_week_now(char *timezone_str){
	GTimeZone *timezone;
	GDateTime *datetime;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_new_now(timezone);
	return g_date_time_get_day_of_week(datetime);
}

int zwscn_timezone_get_hour_now(char *timezone_str){
	GTimeZone *timezone;
	GDateTime *datetime;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_new_now(timezone);
	return g_date_time_get_hour(datetime);
}

int zwscn_timezone_get_minute_now(char *timezone_str){
	GTimeZone *timezone;
	GDateTime *datetime;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_new_now(timezone);
	return g_date_time_get_minute(datetime);
}

int zwscn_timezone_get_day_of_week(char *timezone_str, GDateTime *datetime){
	GTimeZone *timezone;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_to_timezone(datetime, timezone); //TODO check for NULL
	return g_date_time_get_day_of_week(datetime);
}

int zwscn_timezone_get_hour(char *timezone_str, GDateTime *datetime){
	GTimeZone *timezone;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_to_timezone(datetime, timezone); //TODO check for NULL
	return g_date_time_get_hour(datetime);
}

int zwscn_timezone_get_minute(char *timezone_str, GDateTime *datetime){
	GTimeZone *timezone;
	
	timezone = g_time_zone_new(timezone_str);
	datetime = g_date_time_to_timezone(datetime, timezone); //TODO check for NULL
	return g_date_time_get_minute(datetime);
}

//void scene_platform_make_schedule_range(uint32_t time_last_checked, uint32_t time_now, schedule_range_p s_range_ptr) {
//	time_t time;
//	GDateTime *datetime;
//	
//	if(time_last_checked != 0) {
//		time = time_last_checked;
//	} else {
//		time = time_now; //If it was never checked then assume the schedule range starts from this minute
//	}
//	datetime = g_date_time_new_from_unix_utc(time); 
//	s_range_ptr->min_day = zwscn_timezone_get_day_of_week(timezone_str, datetime); //TODO get timezone_str
//	s_range_ptr->min_hour = zwscn_timezone_get_hour(timezone_str, datetime); //TODO get timezone_str
//	s_range_ptr->min_minute = zwscn_timezone_get_minute(timezone_str, datetime); //TODO get timezone_str
//	
//	time = time_now;
//	datetime = g_date_time_new_from_unix_utc(time);
//	s_range_ptr->max_day = zwscn_timezone_get_day_of_week(timezone_str, datetime); //TODO get timezone_str
//	s_range_ptr->max_hour = zwscn_timezone_get_hour(timezone_str, datetime); //TODO get timezone_str
//	s_range_ptr->max_minute = zwscn_timezone_get_minute(timezone_str, datetime); //TODO get timezone_str
//}


// NOW time  g_date_time_new_now_utc() - save it as time stam - g_date_time_to_unix()
// last checked time  load - g_date_time_new_from_unix_utc()

// g_date_time_to_timezone ()

// g_date_time_get_day_of_week
// g_date_time_get_hour
// g_date_time_get_minute

zwscn_timespan_t zwscn_timezone_get_microseconds_to_schedule_today(const char *tz_name, int schedule_day_bitmask, int schedule_hour, int schedule_minute) {
	GTimeZone *timezone;
	GDateTime *datetime;
	GDateTime *datetime_schedule;
	GTimeSpan diff;
	int year, month, day, minute;
	
	timezone = g_time_zone_new(tz_name);
	datetime = g_date_time_new_now(timezone);
	year		= g_date_time_get_year(datetime);
	month		= g_date_time_get_month(datetime);
	day			= g_date_time_get_day_of_month(datetime);
	minute		= g_date_time_get_minute(datetime);
	
	datetime_schedule = g_date_time_new(timezone, year, month, day, schedule_hour, schedule_minute, 0);
	
	diff = g_date_time_difference(datetime_schedule, datetime);
	if((minute == schedule_minute) && (((diff < 0) && (diff > -(ZWSCN_TIME_SEC2MICRO(ZWSCN_TIME_SEC_MAX)))) || ((diff > 0) && (diff < (ZWSCN_TIME_SEC2MICRO(ZWSCN_TIME_SEC_MAX)))))) { //we are still in the same minute
		diff = 0;
	}
	
	g_time_zone_unref(timezone);
	g_date_time_unref(datetime);
	g_date_time_unref(datetime_schedule);
	
	return diff;
}

zwscn_timespan_t zwscn_timezone_get_microseconds_to_next_schedule(const char *tz_name, int schedule_day_bitmask, int schedule_hour, int schedule_minute) {
	GTimeZone *timezone;
	GDateTime *datetime;
	GDateTime *datetime_schedule;
	GDateTime *datetime_schedule_2;
	int start_day;
	int days_diff;
	GTimeSpan diff;
	int year, month, day; //, hour;
	
	timezone = g_time_zone_new(tz_name);
	datetime = g_date_time_new_now(timezone);
	year		= g_date_time_get_year(datetime);
	month		= g_date_time_get_month(datetime);
	day			= g_date_time_get_day_of_month(datetime);
//	hour		= g_date_time_get_hour(datetime);
	
	datetime_schedule = g_date_time_new(timezone, year, month, day, schedule_hour, schedule_minute, 0);
	
	start_day = g_date_time_get_day_of_week(datetime_schedule) + 1;
	days_diff = zwscn_timezone_get_days_until_next_schedule(schedule_day_bitmask, start_day);
	datetime_schedule_2 = g_date_time_add_days(datetime_schedule, (days_diff + 1));
	g_date_time_unref(datetime); //Going to reuse this variable in the next line, so deallocating memory from the earlier call
	datetime = g_date_time_new_now(timezone);
	diff = g_date_time_difference(datetime_schedule_2, datetime);
	
	g_time_zone_unref(timezone);
	g_date_time_unref(datetime);
	g_date_time_unref(datetime_schedule);
	g_date_time_unref(datetime_schedule_2);
	if(diff < 0) {
		goto l_err;
	}
	
	return diff;
l_err:
	return diff;
}

ZWSCN_BOOL_T zwscn_timezone_is_schedule_enabled_today(const char *tz_name, int schedule_day_bitmask) {
	GTimeZone *timezone;
	GDateTime *datetime;
	int day_of_week_today;
	int days;
	ZWSCN_BOOL_T result = ZWSCN_FALSE;
	
	timezone = g_time_zone_new(tz_name);
	datetime = g_date_time_new_now(timezone);
	day_of_week_today	= g_date_time_get_day_of_week(datetime);
	days = zwscn_timezone_get_days_until_next_schedule(schedule_day_bitmask, day_of_week_today);
	if(days == 0) {
		result = ZWSCN_TRUE;
	}
	
	g_time_zone_unref(timezone);
	g_date_time_unref(datetime);
	
	return result;
}

int zwscn_timezone_get_next_schedule_day(int schedule_day_bitmask, int curr_day) {
	int in_day;
	int out_day;

	if(curr_day >= ZWSCN_TIME_WEEK_DAYS_MAX) {
		in_day = curr_day - ZWSCN_TIME_WEEK_DAYS_MAX;
	} else {
		in_day = curr_day;
	}
	
	for(out_day=in_day; out_day < ZWSCN_TIME_WEEK_DAYS_MAX; ++out_day) {
		if((0x01 << out_day) & schedule_day_bitmask) {
			break;
		}
	}
	
	if(out_day == ZWSCN_TIME_WEEK_DAYS_MAX) {
		for(out_day=0; out_day < in_day; ++out_day) {
			if((0x01 << out_day) & schedule_day_bitmask) {
				break;
			}
		}
		
		if(out_day == in_day) {
			//Schedule bitmask does not have any bit set
			goto l_err; //return error
		}
	}
	
	if(out_day == 0) {
		out_day = ZWSCN_TIME_WEEK_DAYS_MAX; 
	}
	
	return out_day;
l_err:
	return -1;
}

int zwscn_timezone_get_days_until_next_schedule(int schedule_day_bitmask, int start_day) {
	int next_day;
	int diff;
	
	next_day = zwscn_timezone_get_next_schedule_day(schedule_day_bitmask, start_day);
	if(next_day < 0) {
		goto l_err; //received error, so return error
	}
	if(next_day >= start_day) {
		diff = next_day - start_day;
	} else {
		diff = (ZWSCN_TIME_WEEK_DAYS_MAX-start_day) + next_day;
	}
	
	return diff;
l_err:
	return -1;
}

char *zwscn_timezone_get_network_time(void *net) {
	GTimeZone *timezone;
	GDateTime *datetime;
	zwscn_init_p init_data;
	char *tz_name;
	char *result;
	char *format = "%T %a (%u) %Z"; 
	
	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err_args;
	}
	
	tz_name = init_data->init_input->client_timezone;
	timezone = g_time_zone_new(tz_name);
	datetime = g_date_time_new_now(timezone);
	result = g_date_time_format(datetime, format); //NOTE: this should be freed by using g_free()
	
	g_time_zone_unref(timezone);
	g_date_time_unref(datetime);
	
	return result; //NOTE: use zwscn_timezone_free_network_time_string to free the result
l_err_args:
	return NULL;
}

char *zwscn_timezone_get_network_time_from_timestamp(void *net, uint64_t timestamp) {
	GTimeZone *timezone;
	GDateTime *datetime;
	GDateTime *datetime2;
	zwscn_init_p init_data;
	char *tz_name;
	char *result;
	char *format = "%T %F %Z"; 
	
	if(net == NULL) {
		goto l_err_args;
	}
	
	init_data = zwscn_get_init_data(net);
	if(init_data == NULL) {
		goto l_err_args;
	}
	
	tz_name = init_data->init_input->client_timezone;
	timezone = g_time_zone_new(tz_name);
	datetime = g_date_time_new_from_unix_utc(timestamp);
	datetime2 = g_date_time_to_timezone(datetime, timezone);
	result = g_date_time_format(datetime2, format); //NOTE: this should be freed by using g_free()
	
	g_time_zone_unref(timezone);
	g_date_time_unref(datetime);
	g_date_time_unref(datetime2);
	
	return result; //NOTE: use zwscn_timezone_free_network_time_string to free the result
l_err_args:
	return NULL;
}


void zwscn_timezone_free_network_time_string(char *time_string) {
	g_free(time_string);
}
