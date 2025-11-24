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
@file		zwscn_util_email.c  
  
                Utility functions for email support
  
@author		Arif Mohammand
 
@version	1.0 2015-06-26  Initial release
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "zwscn_defines.h"
#include "zwscn_scene_api.h"
#include "zwscn_util.h"
#include "zwscn_util_config.h"
#include "zwscn_util_message_log.h"
//SMTP
//#include "ssl.h"
#include "auth-client.h"
#include "libesmtp.h"

#if !defined (__GNUC__) || __GNUC__ < 2
# define __attribute__(x)
#endif
#define unused      __attribute__((unused))

typedef struct zwscn_smtp_auth_info {
	char *username;
	char *password;
} zwscn_smtp_auth_info_t, *zwscn_smtp_auth_info_p;

ZWSCN_STATUS_T zwscn_util_email_send_via_sendmail(const char *to, const char *from, const char *subject, const char *message);
ZWSCN_STATUS_T zwscn_util_email_send_via_smtp(void *net, const char *to, const char *from, const char *subject, const char *message);
int authinteract (auth_client_request_t request, char **result, int fields, void *arg unused);
int tlsinteract (char *buf, int buflen, int rwflag unused, void *arg unused);
void event_cb (smtp_session_t session, int event_no, void *arg,...);
int handle_invalid_peer_certificate(long vfy_result);

ZWSCN_STATUS_T zwscn_util_email_send(void *net, const char *to, const char *from, const char *subject, const char *message) {
	ZWSCN_STATUS_T result;
	
	if(zwscn_config_smtp_is_enabled(net) == ZWSCN_TRUE) {
		result = zwscn_util_email_send_via_smtp(net, to, from, subject, message);
	} else {
		result = zwscn_util_email_send_via_sendmail(to, from, subject, message);
	}
	
	return result;
}

ZWSCN_STATUS_T zwscn_util_email_send_via_sendmail(const char *to, const char *from, const char *subject, const char *message) {
    FILE *mailpipe = NULL;
	char mta_string[ZWSCN_TEMP_STR_MAX] = {0};
	
	ZWSCN_MESSAGE_LOG_DEBUG("EMAIL sending email to %s from %s\n", to, from);
	snprintf(mta_string, sizeof(mta_string), "/usr/lib/sendmail -t -f %s", from);
//	mailpipe = popen("/usr/lib/sendmail -t", "w");
	mailpipe = popen(mta_string, "w");
    if (mailpipe != NULL) {
        fprintf(mailpipe, "To: %s\n", to);
        fprintf(mailpipe, "From: %s\n", from);
        fprintf(mailpipe, "Subject: %s\n\n", subject);
        fwrite(message, 1, strlen(message), mailpipe);
        fwrite(".\n", 1, 2, mailpipe);
        pclose(mailpipe);
     }
     else {
         zwscn_message_log_critical("Failed to invoke sendmail (/usr/lib/sendmail)");
		 goto l_err;
     }
	 
     return ZWSCN_ERR_NONE;
l_err:
	return ZWSCN_ERR_GENERIC;
}

ZWSCN_STATUS_T zwscn_util_email_send_via_smtp(void *net, const char *to, const char *from, const char *subject, const char *message) {
	smtp_session_t session;
	smtp_message_t message_smtp;
//	smtp_recipient_t recipient;
	auth_context_t authctx;
	const smtp_status_t *status;
	char temp_msg[ZWSCN_TEMP_STR_MAX] = "MIME-Version: 1.0\r\nContent-Type: text/plain;\r\n\r\n"; //NOTE: Header processing ends with \r\n\r\n. Rest of the message is treated as body text.
//	FILE *fp;
//	char *file="test-mail";
	char *smtp_server = NULL;
	ZWSCN_BOOL_T auth_enable = ZWSCN_TRUE;
	zwscn_smtp_auth_info_t smtp_auth_info;
	
	smtp_server = zwscn_config_smtp_get_server(net);
	if(smtp_server == NULL) {
		goto l_err_server;
	}
	auth_enable = zwscn_config_smtp_is_auth_enabled(net);
	
	auth_client_init ();
	session = smtp_create_session ();
	message_smtp = smtp_add_message (session);
	
	//Session and server
	smtp_starttls_enable (session, Starttls_REQUIRED);
	smtp_set_server (session, smtp_server);
	smtp_set_header_option (message_smtp, "Message-Id", Hdr_PROHIBIT, 1);
	
	//Auth
	authctx = auth_create_context ();
	auth_set_mechanism_flags (authctx, AUTH_PLUGIN_PLAIN, 0);
	smtp_auth_info.username = zwscn_config_smtp_get_username(net);
	smtp_auth_info.password = zwscn_config_smtp_get_password(net);
	auth_set_interact_cb (authctx, authinteract, &smtp_auth_info);
	if(auth_enable) {
		smtp_auth_set_context (session, authctx);
	}
	ZWSCN_MESSAGE_LOG_DEBUG("EMAIL sending email to %s from %s using SMTP\n", to, smtp_auth_info.username);
	
	//TLS/SSL
	smtp_starttls_set_password_cb (tlsinteract, NULL);
	smtp_set_eventcb(session, event_cb, NULL);
	
	//TO
	smtp_set_header (message_smtp, "To", NULL, to);
	
	//From
	smtp_set_reverse_path (message_smtp, from);
	smtp_set_header (message_smtp, "From", NULL, from);
	
	//Subject
	smtp_set_header (message_smtp, "Subject", subject);
	smtp_set_header_option (message_smtp, "Subject", Hdr_OVERRIDE, 1);
	
	//Text
	{
//		char temp_mesg[1024] = "MIME-Version: 1.0\r\nContent-Type: text/plain;\r\ncharset=iso-8859-1\r\nContent-Transfer-Encoding: 7bit\r\n\r\n";
		zwscn_util_strcat(temp_msg, message, ZWSCN_TEMP_STR_MAX);
		zwscn_util_strcat(temp_msg, "\r\n", ZWSCN_TEMP_STR_MAX);
		smtp_set_message_str(message_smtp, temp_msg);
		
//		if ((fp = fopen (file, "r")) == NULL) {
//		  zwscn_message_log_critical("can't open %s: %s\n", file, strerror (errno));
////		  goto l_err;
//		}
//		smtp_set_message_fp (message_smtp, fp);
	}
	
//	recipient = smtp_add_recipient (message_smtp, to);
	smtp_add_recipient (message_smtp, to);
	if (!smtp_start_session (session)) {
      char buf[ZWSCN_TEMP_STR_MAX];
      zwscn_message_log_critical("SMTP server problem %s\n", smtp_strerror(smtp_errno(), buf, sizeof buf));
    } else {
      /* Report on the success or otherwise of the mail transfer.
       */
      status = smtp_message_transfer_status (message_smtp);
      zwscn_message_log_debug("%d %s", status->code, (status->text != NULL) ? status->text : "\n");
//      smtp_enumerate_recipients (message_smtp, print_recipient_status, NULL);
    }
	
	auth_destroy_context (authctx);
	smtp_destroy_session (session);
//	fclose (fp);
	auth_client_exit();
	free(smtp_server);
	free(smtp_auth_info.username);
	free(smtp_auth_info.password);
	
	return ZWSCN_ERR_NONE;
//l_err:
//	return ZWSCN_ERR_GENERIC;
l_err_server:
	return ZWSCN_ERR_INVALID_DATA;
}

int authinteract (auth_client_request_t request, char **result, int fields, void *arg unused) {
	zwscn_smtp_auth_info_p smtp_auth_info_ptr = NULL;
	int i;
	char *username = NULL;
	char *password = NULL;
	
	smtp_auth_info_ptr = (zwscn_smtp_auth_info_p) arg; //pass as an argument while setting the cb
	username = smtp_auth_info_ptr->username;
	password = smtp_auth_info_ptr->password;
	if((username == NULL) || (password == NULL)) {
		goto l_err_invalid_data;
	}
	for (i = 0; i < fields; i++) {
		if(request[i].flags & AUTH_USER)
			result[i] = username;
		else if(request[i].flags & AUTH_PASS) {
			result[i] = password;
		}
	}

	return 1;
l_err_invalid_data:
	return 0; //zero means error in this function
}

int tlsinteract (char *buf, int buflen, int rwflag unused, void *arg unused) {
	char *pw;
	int len;
//  pw = getpass ("certificate password");
	pw = ""; //TODO - get correct value later
	len = strlen (pw);
	if (len + 1 > buflen)
		return 0;
	strcpy (buf, pw);
	return len;
}

void event_cb (smtp_session_t session, int event_no, void *arg,...) {
	va_list alist;
	int *ok;
	
	va_start(alist, arg);
	switch(event_no) {
		case SMTP_EV_CONNECT:
		case SMTP_EV_MAILSTATUS:
		case SMTP_EV_RCPTSTATUS:
		case SMTP_EV_MESSAGEDATA:
		case SMTP_EV_MESSAGESENT:
		case SMTP_EV_DISCONNECT:
			break;
		case SMTP_EV_WEAK_CIPHER: {
			int bits;
			bits = va_arg(alist, long);
			ok = va_arg(alist, int*);
			zwscn_message_log_info("SMTP_EV_WEAK_CIPHER, bits=%d - accepted.\n", bits);
			*ok = 1;
			break;
		}
		case SMTP_EV_STARTTLS_OK:
			zwscn_message_log_info("SMTP_EV_STARTTLS_OK - TLS started here.\n");
			break;
		case SMTP_EV_INVALID_PEER_CERTIFICATE: {
			long vfy_result;
			vfy_result = va_arg(alist, long);
			ok = va_arg(alist, int*);
			*ok = handle_invalid_peer_certificate(vfy_result);
			break;
		}
		case SMTP_EV_NO_PEER_CERTIFICATE: {
			ok = va_arg(alist, int*);
			zwscn_message_log_info("SMTP_EV_NO_PEER_CERTIFICATE - accepted.\n");
			*ok = 1;
			break;
		}
		case SMTP_EV_WRONG_PEER_CERTIFICATE: {
			ok = va_arg(alist, int*);
			zwscn_message_log_info("SMTP_EV_WRONG_PEER_CERTIFICATE - accepted.\n");
			*ok = 1;
			break;
		}
		case SMTP_EV_NO_CLIENT_CERTIFICATE: {
			ok = va_arg(alist, int*);
			zwscn_message_log_info("SMTP_EV_NO_CLIENT_CERTIFICATE - accepted.\n");
			*ok = 1;
			break;
		}
		default:
			zwscn_message_log_info("SMTP_EV_NO_CLIENT_CERTIFICATE Got event: %d - ignored.\n", event_no);
	}
	va_end(alist);
}

int handle_invalid_peer_certificate(long vfy_result) {
  const char *k ="rare error";
  
//  switch(vfy_result) {
//	  case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
//		k="X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT"; break;
//	  case X509_V_ERR_UNABLE_TO_GET_CRL:
//		k="X509_V_ERR_UNABLE_TO_GET_CRL"; break;
//	  case X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE:
//		k="X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE"; break;
//	  case X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE:
//		k="X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE"; break;
//	  case X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY:
//		k="X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY"; break;
//	  case X509_V_ERR_CERT_SIGNATURE_FAILURE:
//		k="X509_V_ERR_CERT_SIGNATURE_FAILURE"; break;
//	  case X509_V_ERR_CRL_SIGNATURE_FAILURE:
//		k="X509_V_ERR_CRL_SIGNATURE_FAILURE"; break;
//	  case X509_V_ERR_CERT_NOT_YET_VALID:
//		k="X509_V_ERR_CERT_NOT_YET_VALID"; break;
//	  case X509_V_ERR_CERT_HAS_EXPIRED:
//		k="X509_V_ERR_CERT_HAS_EXPIRED"; break;
//	  case X509_V_ERR_CRL_NOT_YET_VALID:
//		k="X509_V_ERR_CRL_NOT_YET_VALID"; break;
//	  case X509_V_ERR_CRL_HAS_EXPIRED:
//		k="X509_V_ERR_CRL_HAS_EXPIRED"; break;
//	  case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD:
//		k="X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD"; break;
//	  case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD:
//		k="X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD"; break;
//	  case X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD:
//		k="X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD"; break;
//	  case X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD:
//		k="X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD"; break;
//	  case X509_V_ERR_OUT_OF_MEM:
//		k="X509_V_ERR_OUT_OF_MEM"; break;
//	  case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
//		k="X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT"; break;
//	  case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
//		k="X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN"; break;
//	  case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
//		k="X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY"; break;
//	  case X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE:
//		k="X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE"; break;
//	  case X509_V_ERR_CERT_CHAIN_TOO_LONG:
//		k="X509_V_ERR_CERT_CHAIN_TOO_LONG"; break;
//	  case X509_V_ERR_CERT_REVOKED:
//		k="X509_V_ERR_CERT_REVOKED"; break;
//	  case X509_V_ERR_INVALID_CA:
//		k="X509_V_ERR_INVALID_CA"; break;
//	  case X509_V_ERR_PATH_LENGTH_EXCEEDED:
//		k="X509_V_ERR_PATH_LENGTH_EXCEEDED"; break;
//	  case X509_V_ERR_INVALID_PURPOSE:
//		k="X509_V_ERR_INVALID_PURPOSE"; break;
//	  case X509_V_ERR_CERT_UNTRUSTED:
//		k="X509_V_ERR_CERT_UNTRUSTED"; break;
//	  case X509_V_ERR_CERT_REJECTED:
//		k="X509_V_ERR_CERT_REJECTED"; break;
//  }
  zwscn_message_log_info("SMTP_EV_INVALID_PEER_CERTIFICATE: %ld: %s\n", vfy_result, k);
  return 1; /* Accept the problem */
}



