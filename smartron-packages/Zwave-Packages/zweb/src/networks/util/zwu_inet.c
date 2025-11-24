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
 * @file    zwu_inet.c
 *
 * @brief   Internet socket
 * @details Internet socket utility functions
 *
 * @author  Kumara
 *
 * @version 1.0 - 2013-10-07
 * - Initial version
 */

#include "zwu_inet.h"
#include "zws_memory.h"
#include "zws_error.h"
#include "zws_log.h"

zws_status_t zwu_inet_pton(const char *src, void *dst)
{
	uint8_t ipv4_addr[4];

	if (inet_pton(AF_INET6, src, dst) == 1)	// 1 => Success
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "IPv6 address: %s\n", src);
	}
	else if (inet_pton(AF_INET, src, dst) == 1)
	{
		//Save the portal IPv4 address
		memcpy(ipv4_addr, dst, 4);
		ZWS_LOG(ZWS_LOG_DEBUG, "IPv4 address: %d.%d.%d.%d\n", ipv4_addr[0], ipv4_addr[1], ipv4_addr[2], ipv4_addr[3]);

		//Convert the IPv4 address to IPv4-mapped IPv6 address
		memset(dst, 0, 10);
		((uint8_t *)dst)[10] = 0xFF;
		((uint8_t *)dst)[11] = 0xFF;
		memcpy(((uint8_t *)dst) + 12, ipv4_addr, 4);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid IP address: %s", src);
		goto l_err_invalid_ip;
	}

	return ZWS_STATUS_OK;

l_err_invalid_ip:
	return ZWS_STATUS_ERROR;
}

zws_status_t zwu_inet_ntop(const void *src, char *dst, size_t size)
{
	char ipv4_prefix[12];

	if (size < INET6_ADDRSTRLEN)
	{
		return ZWS_STATUS_ERROR;
	}

	// IPv4 mapped IPv6 address "::ffff:"
	memset(ipv4_prefix, 0, 10);
	ipv4_prefix[10] = ipv4_prefix[11] = 0xFF;

	memset(dst, 0, size);

	if ((memcmp(src, ipv4_prefix, sizeof(ipv4_prefix)) != 0)
			|| (inet_ntop(AF_INET, src + 12, dst, size) == NULL))
	{
		if (inet_ntop(AF_INET6, src, dst, size) == NULL)
		{
			goto l_err_ipv6_address_string;
		}
	}

	return ZWS_STATUS_OK;

l_err_ipv6_address_string:
	return ZWS_STATUS_ERROR;
}

zws_status_t zwu_inet_pton_iptype(const char *src, void *dst, uint8_t * iptype)
{
	if (inet_pton(AF_INET6, src, dst) == 1)	// 1 => Success
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "Received IPv6 address: %s\n", src);
		*iptype = TYPE_IPV6;
	}
	else if (inet_pton(AF_INET, src, dst) == 1)
	{
		*iptype = TYPE_IPV4;
		ZWS_LOG(ZWS_LOG_DEBUG, "Received IPv4 address: %s\n", src);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid IP address: %s", src);
		goto l_err_invalid_ip;
	}

	return ZWS_STATUS_OK;

l_err_invalid_ip:
	return ZWS_STATUS_ERROR;
}

zws_status_t zwu_inet_ntop_iptype(const void *src, char *dst, uint8_t iptype, size_t size)
{
	memset(dst, 0, size);

	if((iptype == TYPE_IPV6) && (inet_ntop(AF_INET6, src, dst, size) != NULL))
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "IPv6 address: %s\n", dst);
	}
	else if((iptype == TYPE_IPV4) && (inet_ntop(AF_INET, src, dst, size) != NULL))
	{
		ZWS_LOG(ZWS_LOG_DEBUG, "IPv4 address: %s\n", dst);
	}
	else
	{
		ZWS_LOG(ZWS_LOG_ERR, "Invalid IP addres");
		goto l_err_invalid_ip;
	}

	return ZWS_STATUS_OK;

l_err_invalid_ip:
	return ZWS_STATUS_ERROR;
}
