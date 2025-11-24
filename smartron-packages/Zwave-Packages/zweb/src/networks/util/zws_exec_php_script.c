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
 * @file    zws_exec_php_script.c
 *
 * @brief   Manipulate php scripts in ZWare
 * @details Communicate with php scripts in ZWare to send request and get resposne
 *
 * @author  Angela
 *
 * @version 1.0 - 2016-07-13
 * - Initial version
 */
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "zws_exec_php_script.h"
#include "zws_memory.h"
#include "zws_file.h"
#include "zws_log.h"
#include "zws_error.h"
#include "zws_io.h"


/**
 * @defgroup zws_ifttt_response IFTTT_Response
 * @{
 * @ingroup util
 */

zws_status_t zws_php_request_transmitter(int pipefd[2],
										 char* chScriptPath,
										 int iArgCnt,
										 char *chArg1,
										 char *chArg2,
										 char *chArg3)
{
	char *root_dir_path;
	char *php_cli_file_path;
	char *php_script_file_path;
	zws_status_t res = ZWS_STATUS_ERROR;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Child: dup2() failed: %s", strerror(errno));
		goto l_err_dup2;
	}
	close(pipefd[1]);

	if (zws_file_get_root_dir_path(&root_dir_path) != ZWS_STATUS_OK)
	{
		goto l_err_root_dir_path;
	}

	if ((php_cli_file_path = (char *)ZWS_MALLOC(strlen(root_dir_path) + 1
		+ strlen(PHP_CLI_FILE) + 1)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_php_cli_file_path_alloc;
	}
	sprintf(php_cli_file_path, "%s/%s", root_dir_path, PHP_CLI_FILE);

	if ((php_script_file_path = (char *)ZWS_MALLOC(strlen(root_dir_path) + 1
		+ strlen(chScriptPath) + 1)) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, ZWS_MSG_MALLOC_FAILED);
		goto l_err_php_script_file_path_alloc;
	}
	sprintf(php_script_file_path, "%s/%s", root_dir_path, chScriptPath);

	if(iArgCnt == 0)
	{ 
		if (execl(php_cli_file_path, php_cli_file_path, php_script_file_path, NULL) == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to exec() php script: %s %s: %s",
				php_cli_file_path, php_script_file_path, strerror(errno));
			goto l_err_exec_script;
		}
	}
	else if(iArgCnt == 1)
	{
		if (execl(php_cli_file_path, php_cli_file_path, php_script_file_path, chArg1, NULL) == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to exec() php script: %s %s: %s",
				php_cli_file_path, php_script_file_path, strerror(errno));
			goto l_err_exec_script;
		}
	}
	else if (iArgCnt == 2)
	{
		if (execl(php_cli_file_path, php_cli_file_path, php_script_file_path, chArg1, chArg2, NULL) == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to exec() php script: %s %s: %s",
				php_cli_file_path, php_script_file_path, strerror(errno));
			goto l_err_exec_script;
		}
	}
	else if (iArgCnt == 3)
	{
		if (execl(php_cli_file_path, php_cli_file_path, php_script_file_path, chArg1, chArg2, chArg3, NULL) == -1)
		{
			ZWS_LOG(ZWS_LOG_ERR, "Failed to exec() php script: %s %s: %s",
				php_cli_file_path, php_script_file_path, strerror(errno));
			goto l_err_exec_script;
		}
	}

	// Unreachable portion. Anyway...
	ZWS_FREE(php_script_file_path);
	ZWS_FREE(php_cli_file_path);
	zws_file_free_root_dir_path(root_dir_path);

	return ZWS_STATUS_OK;

l_err_exec_script:
	ZWS_FREE(php_script_file_path);
l_err_php_script_file_path_alloc:
	ZWS_FREE(php_cli_file_path);
l_err_php_cli_file_path_alloc:
	zws_file_free_root_dir_path(root_dir_path);
l_err_root_dir_path:
l_err_dup2:
	return res;
}

zws_status_t zws_php_request_receiver(int pipefd[2], char **ppchResponse, int *piResLen)
{
	FILE *receive_file;

	close(pipefd[1]);
	if ((receive_file = fdopen(pipefd[0], "r")) == NULL)
	{
		ZWS_LOG(ZWS_LOG_ERR, "Parent: fdopen() failed: %s", strerror(errno));
		goto l_err_fdopen;
	}

	if ((zws_read_from_FILE_with_dest_alloc(receive_file, ppchResponse, piResLen, 0)) != 0)
	{
		goto l_err_php_response_read;
	}

	fclose(receive_file);

	return ZWS_STATUS_OK;

	ZWS_FREE(*ppchResponse);
	*ppchResponse = NULL;
l_err_php_response_read:
	fclose(receive_file);
l_err_fdopen:
	return ZWS_STATUS_ERROR;
}

zws_status_t zws_exec_php_request(char* chScriptPath,
								  int iArgCnt,
								  char *chArg1,
								  char *chArg2,
								  char *chArg3,
								  char **ppchResponse,
								  int *piResLen,
								  int *piExitCode)
{
	int pipefd[2];
	pid_t pid;
	int receiver_status;
	int child_status;
	zws_status_t return_status;

	return_status = ZWS_STATUS_ERROR;

	if (pipe(pipefd) == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR , "pipe() failed: %s" , strerror(errno));
		goto l_err_pipe;
	}

	pid = vfork();

	if (pid == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR , "fork() failed: %s" , strerror(errno));
		goto l_err_fork;
	}

	// Child
	if (pid == 0)
	{
		if (zws_php_request_transmitter(pipefd, chScriptPath, iArgCnt, chArg1, chArg2, chArg3) != ZWS_STATUS_OK)
		{
			// No flush
			_exit(-1);
		}
		// Unreachable code
		_exit(-1);
	}

	// Parent
	receiver_status = zws_php_request_receiver(pipefd , ppchResponse, piResLen);

	if(ppchResponse)
		ZWS_LOG(ZWS_LOG_DEBUG , "Parent: Response: '%s'" , (*ppchResponse ? *ppchResponse : "null"));

	// Wait for child to terminate even if 'receiver_status' is != 0
	if (waitpid(pid , &child_status , 0) == -1)
	{
		ZWS_LOG(ZWS_LOG_ERR , "Parent: waitpid() failed: %s" , strerror(errno));
		goto l_err_child_wait;
	}

	if (ppchResponse)
		ZWS_LOG(ZWS_LOG_DEBUG , "Parent: Response: '%s' [After wait]" , (*ppchResponse ? *ppchResponse : "null"));

	if (receiver_status != 0)
	{
		goto l_err_user_name_receiver;
	}

	if (!WIFEXITED(child_status))
	{
		ZWS_LOG(ZWS_LOG_ERR , "Parent: child did not teminate normally");
		goto l_err_child_exit_abnormal;
	}

	*piExitCode = WEXITSTATUS(child_status);

	if (*piExitCode != 0)
	{
		ZWS_LOG(ZWS_LOG_ERR , "Parent: child exited with failure. status: %d" , *piExitCode);
	}

	return ZWS_STATUS_OK;

l_err_child_exit_abnormal:
l_err_user_name_receiver:
l_err_child_wait:
	if (receiver_status == 0)
	{
		ZWS_FREE(*ppchResponse);
	}
l_err_fork:
l_err_pipe:
	return return_status;


}

/**
 * @}
 */

