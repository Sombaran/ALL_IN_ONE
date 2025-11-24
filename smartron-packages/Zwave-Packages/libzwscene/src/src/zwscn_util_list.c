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
@file		zwscn_util_list.c  
  
                To provide support for list data structure
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include "glib.h"
#include "zwscn_util_hash.h"
#include "zwscn_util_list.h"
#include "zwscn_util_mqueue.h"
#include "zwscn_util_thread.h"
#include "zwscn_util_mutex.h"
#include "zwscn_util_rwlock.h"
#include "zwscn_util_timer.h"
#include "zwscn_scene_api.h"
#include "zwscn_statelog.h"
#include "zwscn_scene.h"
#include "zwscn_util_mem.h"
#include "zwscn_security_scene_api.h"
#include "zwscn_security_scene.h"

//gint (*GCompareFunc) (gconstpointer a, gconstpointer b);
typedef int (*zwscn_list_find_fn)(void *data_first, void *data_second); 

static void *zwscn_list_find(zwscn_list_t list, void *input_data, zwscn_list_find_fn list_find_handler);
static int zwscn_list_find_handler_scene(void *list_item, void *id_ptr);
static int zwscn_list_find_handler_action(void *list_item, void *id_ptr);
static int zwscn_list_find_handler_schedule(void *list_item, void *id_ptr);
static int zwscn_list_find_handler_event(void *list_item, void *id_ptr);
static int zwscn_list_find_handler_security_scene(void *list_item, void *id_ptr);
static int zwscn_list_find_handler_security_event(void *list_item, void *id_ptr);

zwscn_list_t zwscn_list_add(zwscn_list_t list, void *data) {
	return g_list_append(list, data);
}

zwscn_list_t zwscn_list_insert(zwscn_list_t list, void *data, int position) {
	return g_list_insert(list, data, position);
}

zwscn_list_t zwscn_list_insert_sorted_strings(zwscn_list_t list, void *data) {
	return g_list_insert_sorted(list, data, (GCompareFunc)g_strcmp0);
}

zwscn_list_t zwscn_list_remove(zwscn_list_t list, void *data) {
	return g_list_remove(list, data);
}

zwscn_list_t zwscn_list_next(zwscn_list_t list){
    return g_list_next(list);
}

int zwscn_list_size(zwscn_list_t list) {
	return g_list_length(list);
}

int zwscn_list_get_index(zwscn_list_t list, void *data) {
	return g_list_index (list, data);
}

void zwscn_list_foreach(zwscn_list_t list, zwscn_list_foreach_fn_t list_foreach_handler_func, void *user_data) {
	g_list_foreach(list, list_foreach_handler_func, user_data);
}

void *zwscn_list_get_by_index(zwscn_list_t list, unsigned int index) {
	return g_list_nth_data(list, index);
}

void *zwscn_list_get_scene_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_scene);
}

void *zwscn_list_get_action_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_action);
}

void *zwscn_list_get_schedule_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_schedule);
}

void *zwscn_list_get_event_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_event);
}

void zwscn_list_free(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), free);
	(*list_ptr) = NULL;
}

void zwscn_list_free_scene_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_scene);
	(*list_ptr) = NULL;
}

void zwscn_list_free_action_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_action);
	(*list_ptr) = NULL;
}

void zwscn_list_free_schedule_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_schedule);
	(*list_ptr) = NULL;
}

void zwscn_list_free_event_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_event);
	(*list_ptr) = NULL;
}

static void *zwscn_list_find(zwscn_list_t list, void *input_data, zwscn_list_find_fn list_find_handler) {
	GList *list_item;
	list_item = g_list_find_custom(list, input_data, (GCompareFunc)list_find_handler);
	if(list_item) {
		return list_item->data;
	}
	
	return NULL;
}

static int zwscn_list_find_handler_scene(void *list_item, void *id_ptr) {
	zwscn_scene_cont_p scene_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	scene_ptr = (zwscn_scene_cont_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(scene_ptr->scene->id == id) {
		result = 0;
	}
	
	return result;
}

static int zwscn_list_find_handler_action(void *list_item, void *id_ptr) {
	zwscn_action_p action_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	action_ptr = (zwscn_action_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(action_ptr->id == id) {
		result = 0;
	}
	
	return result;
}

static int zwscn_list_find_handler_schedule(void *list_item, void *id_ptr) {
	zwscn_schedule_p schedule_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	schedule_ptr = (zwscn_schedule_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(schedule_ptr->id == id) {
		result = 0;
	}
	
	return result;
}

static int zwscn_list_find_handler_event(void *list_item, void *id_ptr) {
	zwscn_event_p event_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	event_ptr = (zwscn_event_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(event_ptr->id == id) {
		result = 0;
	}
	
	return result;
}

//Security Scene
void *zwscn_list_get_security_scene_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_security_scene);
}

void *zwscn_list_get_security_event_by_id(zwscn_list_t list, uint32_t id) {
	return zwscn_list_find(list, &id, zwscn_list_find_handler_security_event);
}

void zwscn_list_free_security_scene_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_security_scene);
	(*list_ptr) = NULL;
}

void zwscn_list_free_arm_security_event_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_event);
	(*list_ptr) = NULL;
}

void zwscn_list_free_disarm_security_event_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_event);
	(*list_ptr) = NULL;
}

void zwscn_list_free_alarm_security_event_list(zwscn_list_t *list_ptr) {
	g_list_free_full((*list_ptr), zwscn_free_event);
	(*list_ptr) = NULL;
}

static int zwscn_list_find_handler_security_scene(void *list_item, void *id_ptr) {
	zwscn_security_scene_cont_p scene_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	scene_ptr = (zwscn_security_scene_cont_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(scene_ptr->scene->id == id) {
		result = 0;
	}
	
	return result;
}

static int zwscn_list_find_handler_security_event(void *list_item, void *id_ptr) {
	zwscn_security_event_p event_ptr;
	uint32_t id;	
	int result = -1;
	
	if(!list_item || !id_ptr) {
		return result;
	}
	event_ptr = (zwscn_security_event_p)list_item;
	id = *((uint32_t *)id_ptr);
	
	if(event_ptr->id == id) {
		result = 0;
	}
	
	return result;
}
