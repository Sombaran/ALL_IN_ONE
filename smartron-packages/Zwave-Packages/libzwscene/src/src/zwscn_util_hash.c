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
@file		zwscn_util_hash.c  
  
                To provide support for hash data structure
  
@author		Arif Mohammand
 
@version	1.0 2013-12-05  Initial release
*/

#include <stdlib.h>
#include <stdint.h>
#include "glib.h"
#include "zwscn_util_hash.h"

//typedef GHashTable zwscn_hash_t;
// void (*GDestroyNotify) (gpointer data);

zwscn_hash_t zwscn_hash_new() {
	return g_hash_table_new(NULL, NULL); // Use g_direct_hash() and g_direct_equal() by default
}

zwscn_hash_t zwscn_hash_new_extended(zwscn_hash_value_free_fn_t hash_value_free_handler) {
	return g_hash_table_new_full(NULL, NULL, NULL, hash_value_free_handler); // Use g_direct_hash() and g_direct_equal() by default
}

zwscn_hash_t zwscn_hash_new_str_hash() {
    return g_hash_table_new(g_str_hash, g_str_equal); // Uses g_str_hash() and g_str_hash()
}

void zwscn_hash_destroy(zwscn_hash_t hash){
	if(hash == NULL){
		goto l_err_args;
	}
	g_hash_table_destroy(hash); // Note: values are freed using the supplied mem free function zwscn_hash_value_free_fn

l_err_args:
	return;
}

int zwscn_hash_contains_key(zwscn_hash_t hash, void *key){
	if(hash == NULL){
		goto l_err_args;
	}
	return g_hash_table_contains(hash, key);

l_err_args:
	return 0;
}

int zwscn_hash_size(zwscn_hash_t hash){
	if(hash == NULL){
		goto l_err_args;
	}
	return g_hash_table_size(hash);

l_err_args:
	return 0;
}

void zwscn_hash_add(zwscn_hash_t hash, void *key){
	if(hash == NULL){
		goto l_err_args;
	}
	g_hash_table_add(hash, key);

l_err_args:
	return;
}

void zwscn_hash_insert(zwscn_hash_t hash, void *key, void *value){
	if(hash == NULL){
		goto l_err_args;
	}
	g_hash_table_insert(hash, key, value); // Note: This replaces the old value with new value if key exists which may cause mem leak

l_err_args:
	return;
}

int zwscn_hash_remove(zwscn_hash_t hash, void *key){
	if(hash == NULL){
		goto l_err_args;
	}
	return g_hash_table_remove(hash, key); // Note: value is freed using the supplied mem free function zwscn_hash_value_free_fn

l_err_args:
	return 0;
}

void zwscn_hash_remove_all(zwscn_hash_t hash){
	if(hash == NULL){
		goto l_err_args;
	}
	g_hash_table_remove_all(hash); // Note: values are freed using the supplied mem free function zwscn_hash_value_free_fn

l_err_args:
	return;
}

void *zwscn_hash_get_value(zwscn_hash_t hash, void *key){
	if(hash == NULL){
		goto l_err_args;
	}
	return g_hash_table_lookup(hash, key); // Returns null if not found

l_err_args:
	return NULL;
}

unsigned int zwscn_hash_foreach_remove(zwscn_hash_t hash, zwscn_hash_foreach_callback_fn_t hash_foreach_callback, void *user_data){
	if(hash == NULL){
		goto l_err_args;
	}
	return g_hash_table_foreach_remove(hash, hash_foreach_callback, user_data); //returns number of key-value pair removed

l_err_args:
	return 0;
} 
