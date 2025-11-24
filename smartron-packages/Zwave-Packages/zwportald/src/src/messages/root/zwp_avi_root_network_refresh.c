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
 * @file    zwp_avi_root_network_refresh.c
 *
 * @brief   AVI message 'root_network_refresh'
 * @details Handles the AVI message 'root_network_refresh'.
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-01
 * - Initial version
 */

#include "zwp_status.h"
#include "zwp_avi_message.h"
#include "zwp_log.h"
#include "zwp_memory.h"
#include "zwp_error.h"
#include "zwp_portal.h"
#include "zwp_directory_service.h"
#include "zwp_network.h"

/**
 * @brief Get argument for Network Refresh command from Avro value object
 * @param[in]     arg_value         Avro value object
 * @param[out]    args_p            Arguments passed with Network Refresh command
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 *
 * @note
 * @par
 * The caller should call zwp_avi_root_network_refresh_argument_free() to free resources
 */
static zwp_status_t zwp_avi_root_network_refresh_argument(avro_value_t *arg_value, void **args_p)
{
	char *network_id;

	if (zw_avro_field_get_string(arg_value, "network_id", &network_id) != ZWP_STATUS_OK)
	{
		goto l_err_get_value_network_id;
	}

	*args_p = network_id;

	return ZWP_STATUS_OK;

l_err_get_value_network_id:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Free arguments for Network Refresh command
 * @param[in]     args              Arguments passed with Network Refresh command
 * @return        Nothing
 * @see zwp_avi_root_network_refresh_argument
 */
static void zwp_avi_root_network_refresh_argument_free(void *args)
{
	zw_avro_field_free_string(args);
}

/**
 * @brief Add a client profile to the portal internal list, overwriting old entry with the same gateway id.
 * @param[in]     portal            Portal object
 * @param[in]     profile           Portal client profile
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_root_network_add(zwp_portal_t portal, clnt_prof_t *profile)
{
	ZWP_LOG(ZWP_LOG_DEBUG, "Remote Access Code: %02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
			profile->clnt_id[0], profile->clnt_id[1], profile->clnt_id[2], profile->clnt_id[3],
			profile->clnt_id[4], profile->clnt_id[5], profile->clnt_id[6], profile->clnt_id[7]);

	if (zwportal_clnt_add(zwp_portal_get_portal_handle(portal), profile) == 0)
	{
		ZWP_LOG(ZWP_LOG_ERR, "zwportal_clnt_add() failed");
		goto l_err_clnt_add;
	}

	return ZWP_STATUS_OK;

l_err_clnt_add:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Remove a client profile from the portal internal list
 * @param[in]     portal            Portal object
 * @param[in]     network_id        Network ID
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_root_network_remove(zwp_portal_t portal, char *network_id)
{
	uint8_t clnt_id[8];
	unsigned int ra_code_buf[8];
	int i;
	clnt_prof_t profile;

	if (sscanf(network_id, "%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x",
			&ra_code_buf[0], &ra_code_buf[1], &ra_code_buf[2], &ra_code_buf[3],
			&ra_code_buf[4], &ra_code_buf[5], &ra_code_buf[6], &ra_code_buf[7]) == 8)
	{
		for (i = 0; i < 8; i++)
		{
			if (ra_code_buf[i] <= 0xFF)
			{
				clnt_id[i] = (uint8_t) ra_code_buf[i];
			}
			else
			{
				ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", network_id);
				goto l_err_ra_code_parse;
			}
		}
	}
	else
	{
		ZWP_LOG(ZWP_LOG_ERR, "Invalid value '%s' for Network ID", network_id);
		goto l_err_ra_code_parse;
	}

	if (zwportal_clnt_rm(zwp_portal_get_portal_handle(portal), clnt_id) == 0)
	{
		memcpy(profile.clnt_id, clnt_id, sizeof(profile.clnt_id));

		if (zwportal_clnt_find(zwp_portal_get_portal_handle(portal), &profile) != 0)
		{
			ZWP_LOG(ZWP_LOG_ERR, "zwportal_clnt_rm() failed but zwportal_clnt_find() succeeded. "
					"Client may still be registered with HCAPI");
			goto l_err_clnt_remove;
		}

		ZWP_LOG(ZWP_LOG_WARNING, "zwportal_clnt_rm() and zwportal_clnt_find() failed. "
				"Client already unregistered with HCAPI");
	}

	return ZWP_STATUS_OK;

l_err_clnt_remove:
l_err_ra_code_parse:
	return ZWP_STATUS_ERROR;
}

/**
 * @brief Refresh the list of networks mapped to a given user
 * @param[in]     portal            Portal object
 * @param[in]     user              User name
 * @param[in]     arg               Arguments (if any). Network ID.
 * @param[out]    result_p          Result object (if any)
 * @retval        ZWP_STATUS_OK     Success
 * @retval        ZWP_STATUS_ERROR  Failure
 */
static zwp_status_t zwp_avi_root_network_refresh_execute(zwp_portal_t portal, char *user, void *arg, void **result_p)
{
	char *network_id;
	clnt_prof_t *profile;
    zwp_net_t   net;

	network_id = (char *)arg;

	//Get the ZIP Gateway profile
	if (zwp_directory_service_zipgw_profile_get(zwp_portal_get_directory_service(portal), network_id, &profile) != ZWP_STATUS_OK)
	{
		goto l_err_directory_service_zipgw_profile_get;
	}

	if (profile)
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Registering portal client '%s' (overwriting settings if registered already)", network_id);

		if (zwp_avi_root_network_add(portal, profile) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to register/update settings for portal client '%s'", network_id);

			zwp_directory_service_zipgw_profile_free(profile);

			goto l_err_network_add;
		}

		zwp_directory_service_zipgw_profile_free(profile);
	}
	else
	{
		ZWP_LOG(ZWP_LOG_DEBUG, "Unregistering portal client '%s'", network_id);

		if (zwp_avi_root_network_remove(portal, network_id) != ZWP_STATUS_OK)
		{
			ZWP_LOG(ZWP_LOG_ERR, "Failed to unregister portal client '%s'", network_id);
			goto l_err_network_remove;
		}

		// Disconnect the network
		zwp_portal_client_kill_net_by_ra_code(portal, network_id);
	}

    //Check whether timezone has been changed
    if (zwp_portal_get_net(portal, network_id, &net) == ZWP_STATUS_OK)
    {
        char *timezone;

		ZWP_LOG(ZWP_LOG_DEBUG, "Network '%s' is active", network_id);

        if (zwp_directory_service_get_timezone(zwp_portal_get_directory_service(portal), network_id, &timezone) == ZWP_STATUS_OK)
        {
            //Compare network timezone and directory service returned timezone
            if (strncmp(timezone, zwp_net_get_timezone(net), ZWSCN_LEN_CLIENT_TIMEZONE - 1) == 0)
            {   //Same, don't do anything
                ZWP_LOG(ZWP_LOG_DEBUG, "Timezones are identical");
            }
            else
            {   ZWSCN_STATUS_T  scn_status;
                //Re-initialize scene module
                scn_status = zwscn_exit_network(net);

                if (scn_status == ZWSCN_ERR_NONE)
                {
                    scn_status = zwp_net_init_scn(net, timezone);
                    if (scn_status == ZWSCN_ERR_NONE)
                    {
                        ZWP_LOG(ZWP_LOG_DEBUG, "Re-initialize scene module done!");
                    }
                    else
                    {
                        ZWP_LOG(ZWP_LOG_ERR, "zwp_net_init_scn failed:%d", scn_status);
                    }

                }
                else
                {
                    ZWP_LOG(ZWP_LOG_ERR, "zwscn_exit_network failed:%d", scn_status);
                }
            }

            ZWP_FREE(timezone);
        }
        else
        {
            ZWP_LOG(ZWP_LOG_ERR, "zwp_directory_service_get_timezone() failed");
        }

        zwp_portal_free_net(net);
    }
    else
    {
		ZWP_LOG(ZWP_LOG_DEBUG, "Network '%s' is inactive", network_id);
    }

	return ZWP_STATUS_OK;

l_err_network_remove:
l_err_network_add:
l_err_directory_service_zipgw_profile_get:
	return ZWP_STATUS_ERROR;
}

/** Handlers for refreshing list of networks */
static zwp_avi_message_t root_network_refresh =
{
		"root_network_refresh",
		zwp_avi_root_network_refresh_argument, zwp_avi_root_network_refresh_argument_free,
		zwp_avi_root_network_refresh_execute,
		NULL, NULL
};

/** Register handlers */
zwp_status_t zwp_avi_root_network_refresh_init(zwu_array_list_t message_list)
{
	if (zwp_avi_message_list_add_message(message_list, &root_network_refresh) != ZWP_STATUS_OK)
	{
		goto l_err_network_list_get;
	}

	return ZWP_STATUS_OK;

l_err_network_list_get:
	return ZWP_STATUS_ERROR;
}



