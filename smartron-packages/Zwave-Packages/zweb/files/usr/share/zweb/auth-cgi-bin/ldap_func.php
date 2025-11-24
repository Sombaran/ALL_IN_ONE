<?php

error_reporting(E_ERROR);

define('ERROR_LDAPSEARCH_SUCCESS', 0);
define('ERROR_LDAPSEARCH_FOUND_ZERO_RECORD', -2);
define('ERROR_LDAPSEARCH_FOUND_MULTIPLE_RECORDS', -3);
define('ERROR_LDAPSEARCH_LDAP_INTERNAL_FAILURE', -4);
define('ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER', -5);

define(LDAPSEARCH_DEBUG, 0);

require_once __DIR__.'/../htdocs/register/config.php';

/*
LDAP_SearchStringFormatter - Formatting a proper search string for ldap_search() function
@param[in]  $srch_key     LDAP search key
@param[in]  $srch_txt     LDAP search text
@param[out] $searchstr    Search string to be pass to ldap_search() function
return      true if o.k.; else returns false
*/
function LDAP_SearchStringFormatter($srch_key, $srch_txt, &$searchstr)
{
    $searchstr = '';

    switch ($srch_key) {
        case 'USR_ID':
            $searchstr = 'uid='.$srch_txt;
            break;

        case 'RAC':
            if (!preg_match('/^([0-9A-F][0-9A-F]-){7}[0-9A-F][0-9A-F]$/', $srch_txt)) {
                return false;
            }
            $searchstr = '(zwaveResource=zwaveRemoteAccessCode='.$srch_txt.'*)';
            break;

        case 'EMAIL':
            if (filter_var($srch_txt, FILTER_VALIDATE_EMAIL) == false) {
                return false;
            }
            $searchstr = 'mail='.$srch_txt;
            break;

        case 'USR_NAME':
            $user_name = explode(',', $srch_txt);

            if (count($user_name) == 1) {   //First name only
                $searchstr = 'cn='.$user_name[0];
            } elseif (count($user_name) == 2) {
                $first_name = trim($user_name[0]);
                $last_name = trim($user_name[1]);

                if ($first_name == '') {
                    if ($last_name == '') {
                        return false;
                    }
                    //Last name only
                    $searchstr = 'sn='.$last_name;
                } else {
                    if ($last_name == '') {
                        //First name only
                        $searchstr = 'cn='.$first_name;
                    } else {
                        //First and last names
                        $searchstr = '(&(cn='.$first_name.')(sn='.$last_name.'))';
                    }
                }
            } else {
                return false;
            }
            break;

        default:
            return false;
    }

    return true;
}

/*
LDAP_GetGWInfo - Get gateway information (Taken from admin_func.php)
@param[in]  $ldc        LDAP connection handler
@param[in]  $rac        Gateway remote access code
@param[out] $result     Array that the result will be stored
return      true if o.k.; else returns false
*/
function LDAP_GetGWInfo(&$ldc, $rac, &$result)
{
    $searchstr = 'zwaveRemoteAccessCode='.$rac;
    $required_attr = array('zwaveremoteaccesspin', 'zwavetimezone');

    $srch_result = ldap_search($ldc, LDAP_PHP_GW_BASE_DN, $searchstr, $required_attr);

    if (ldap_count_entries($ldc, $srch_result) > 0) {
        //Found
        $ldp_entries = ldap_get_entries($ldc, $srch_result);

        //Assume only one unique RAC
        $result['zwaveremoteaccesspin'] = $ldp_entries[0]['zwaveremoteaccesspin'][0];
        $result['zwavetimezone'] = $ldp_entries[0]['zwavetimezone'][0];
        ldap_free_result($srch_result);

        return true;
    }
    //Not found
    return false;
}

/*
LDAP_GetUserInfo_Func - Searching LDAP according to search key and search text
then return the required LDAP key information
@param[in]  $srch_key     LDAP search key
@param[in]  $srch_txt     LDAP search text
@param[out] $result       Get User info return in array
return      The above ERROR_LDAPSEARCH_XXXXX code, 0 mean success
*/
function LDAP_GetUserInfo_Func($srch_key, $srch_txt, &$result)
{
    $gw_arry = array();
    $searchstr = '';

    //Connect to LDAP server
    $ldc = ldap_connect(LDAP_PHP_HOST_NAME);

    if ($ldc) {
        ldap_set_option($ldc, LDAP_OPT_PROTOCOL_VERSION, 3);
        ldap_set_option($ldc, LDAP_OPT_REFERRALS, 0);

        $ldb = ldap_bind($ldc, LDAP_PHP_ADMIN_DN, LDAP_PHP_ADMIN_PASSWORD);
        if ($ldb) {
            if (!LDAP_SearchStringFormatter($srch_key, $srch_txt, $searchstr)) {
                //Search formatting error
                ldap_close($ldc);

                return ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER;
            }
            //DEBUG is used for development purpose only
            if (LDAPSEARCH_DEBUG == 1) {
                echo "LDAPSEARCH: $searchstr\n";
            }
            $required_attr = array('zwaveresource', 'cn', 'sn', 'uid', 'mail');
            $srch_result = ldap_search($ldc, LDAP_PHP_USER_BASE_DN, $searchstr, $required_attr);

            if (ldap_count_entries($ldc, $srch_result) > 0) {
                //Found
                $ldp_entries = ldap_get_entries($ldc, $srch_result);

                //Get number of items
                $count = $ldp_entries['count'];
                if ($count == 1) {
                    //Extracting Remote access code and store the result
                    $zw_resource = explode(',', $ldp_entries[0]['zwaveresource'][0]);
                    foreach ($zw_resource as $resource_str) {
                        $key_val = explode('=', $resource_str);
                        if ($key_val[0] == 'zwaveRemoteAccessCode') {
                            $result['zwaveremoteaccesscode'] = $key_val[1];
                            break;
                        }
                    }

                    //Extracting Remote access PIN and Time zone and store the result
                    if (LDAP_GetGWInfo($ldc, $result['zwaveremoteaccesscode'], $gw_arry) == true) {
                        $result['zwaveremoteaccesspin'] = $gw_arry['zwaveremoteaccesspin'];
                        $result['zwavetimezone'] = $gw_arry['zwavetimezone'];
                    }

                    //Store the result
                    $result['cn'] = $ldp_entries[0]['cn'][0];
                    $result['sn'] = $ldp_entries[0]['sn'][0];
                    $result['uid'] = $ldp_entries[0]['uid'][0];
                    $result['mail'] = $ldp_entries[0]['mail'][0];

                    if (LDAPSEARCH_DEBUG == 1) {
                        echo 'DEBUG First Name   : '.$result['cn']."\n";
                        echo 'DEBUG Last Name    : '.$result['sn']."\n";
                        echo 'DEBUG User ID      : '.$result['uid']."\n";
                        echo 'DEBUG Email        : '.$result['mail']."\n";
                        echo 'DEBUG Remote access code: '.$result['zwaveremoteaccesscode']."\n";
                        echo 'DEBUG Remote access PIN : '.$result['zwaveremoteaccesspin']."\n";
                        echo 'DEBUG Time zone : '.$result['zwavetimezone']."\n";
                    }
                }
            }
            ldap_free_result($srch_result);
            ldap_close($ldc);
            if ($count == 1) {
                return ERROR_LDAPSEARCH_SUCCESS;
            } elseif ($count == 0) {
                return ERROR_LDAPSEARCH_FOUND_ZERO_RECORD;
            } elseif ($count > 1) {
                return ERROR_LDAPSEARCH_FOUND_MULTIPLE_RECORDS;
            } else {
                return ERROR_LDAPSEARCH_LDAP_INTERNAL_FAILURE;
            }
        } else {
            //Bind error
            ldap_close($ldc);
        }
    }

    return ERROR_LDAPSEARCH_LDAP_INTERNAL_FAILURE;
}

/*
Main function to call LDAP_GetUserInfo_Func() based on required key
@param[in]  $srch_key     LDAP search key [USR_ID, USR_NAME, EMAIL, RAC]
@param[in]  $srch_txt     LDAP search text
@param[in]  $getinfo_key  Get User info base on key [FIRSTNAME, LASTNAME, FULLNAME, USR_ID, EMAIL, RAC, RAPIN, TIMEZONE]
@param[out] $result_str   Return the required LDAP information
return      The above ERROR_LDAPSEARCH_XXXXX code, 0 mean success
*/
function LDAP_GetUserInfo($srch_key, $srch_txt, $getinfo_key, &$result_str)
{
    $result_arry = array();

    $ldap_search_result = LDAP_GetUserInfo_Func($srch_key, $srch_txt, $result_arry);
    $result_str = '';
    if ($ldap_search_result == ERROR_LDAPSEARCH_SUCCESS) {
        switch ($getinfo_key) {
            case 'FIRSTNAME':
                $result_str = $result_arry['cn'];
                break;

            case 'LASTNAME':
                $result_str = $result_arry['sn'];
                break;

            case 'FULLNAME':
                $result_str = $result_arry['cn'].' '.$result_arry['sn'];
                break;

            case 'USR_ID':
                $result_str = $result_arry['uid'];
                break;

            case 'EMAIL':
                $result_str = $result_arry['mail'];
                break;

            case 'RAC':
                $result_str = $result_arry['zwaveremoteaccesscode'];
                break;

            case 'RAPIN':
                $result_str = $result_arry['zwaveremoteaccesspin'];
                break;

            case 'TIMEZONE':
                $result_str = $result_arry['zwavetimezone'];
                break;

            default:
                $result_str = '';

                return ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER;
        }
    }

    return $ldap_search_result;
}

/*
LDAP_SearchUserInfo - Searching LDAP according to search key and search text
The basic purpose is return TRUE or FALSE. SUCCESS mean TRUE
@param[in]  $srch_key     LDAP search key
@param[in]  $srch_txt     LDAP search text
return      The above ERROR_LDAPSEARCH_XXXXX code, 0 mean success
*/
function LDAP_SearchUserInfo($srch_key, $srch_txt)
{
    $searchstr = '';

    //Connect to LDAP server
    $ldc = ldap_connect(LDAP_PHP_HOST_NAME);

    if ($ldc) {
        ldap_set_option($ldc, LDAP_OPT_PROTOCOL_VERSION, 3);
        ldap_set_option($ldc, LDAP_OPT_REFERRALS, 0);

        $ldb = ldap_bind($ldc, LDAP_PHP_ADMIN_DN, LDAP_PHP_ADMIN_PASSWORD);
        if ($ldb) {
            if (!LDAP_SearchStringFormatter($srch_key, $srch_txt, $searchstr)) {
                //Search formatting error
                ldap_close($ldc);

                return ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER;
            }
            //DEBUG is used for development purpose only
            if (LDAPSEARCH_DEBUG == 1) {
                echo "LDAPSEARCH: $searchstr\n";
            }
            $required_attr = array('zwaveresource', 'cn', 'sn', 'uid', 'mail');
            $srch_result = ldap_search($ldc, LDAP_PHP_USER_BASE_DN, $searchstr, $required_attr);

            if (ldap_count_entries($ldc, $srch_result) > 0) {
                //Found
                $ldp_entries = ldap_get_entries($ldc, $srch_result);

                //Get number of items
                $count = $ldp_entries['count'];
            }
            ldap_free_result($srch_result);
            ldap_close($ldc);
            if ($count == 1) {
                return ERROR_LDAPSEARCH_SUCCESS;
            } elseif ($count == 0) {
                return ERROR_LDAPSEARCH_FOUND_ZERO_RECORD;
            } elseif ($count > 1) {
                return ERROR_LDAPSEARCH_FOUND_MULTIPLE_RECORDS;
            } else {
                return ERROR_LDAPSEARCH_LDAP_INTERNAL_FAILURE;
            }
        } else {
            //Bind error
            ldap_close($ldc);
        }
    }

    return ERROR_LDAPSEARCH_LDAP_INTERNAL_FAILURE;
}

/*
LDAP_CheckUser_SignIn_Credentials - Validating User Sign In UserID/eMail and Password
@param[in]  $userid_email   LDAP UserID or eMail address
@param[in]  $userpassword   LDAP UserPassword
return      The above ERR_LLOGIN_XXXXX code, LOGIN_SUCCESSFUL mean success
*/
function LDAP_CheckUser_SignIn_Credentials($userid_email, $userpassword)
{
    $error = ERR_LOGIN_FAILED_INTERNAL_ERROR;

    if ($userid_email === '') {
        $error = ERR_LOGIN_USERNAME_BLANK;
    } elseif ($userpassword === '') {
        $error = ERR_LOGIN_PASSWORD_BLANK;
    } else {
        $signin_userid = $userid_email;
        $signin_userpassword = $userpassword;
        if (filter_var($userid_email, FILTER_VALIDATE_EMAIL)) {
            // User entered email address, so search the corresponding the userid.
            $result_email_convert2userid = '';
            $ldap_search_result = LDAP_GetUserInfo('EMAIL', $userid_email, 'USR_ID', $result_email_convert2userid);
            if ($ldap_search_result === 0) {
                $signin_userid = $result_email_convert2userid;
            }
        }

        //Connect to LDAP server
        $ldc = ldap_connect(LDAP_PHP_HOST_NAME);
        if ($ldc) {
            ldap_set_option($ldc, LDAP_OPT_PROTOCOL_VERSION, 3);
            ldap_set_option($ldc, LDAP_OPT_REFERRALS, 0);

            $signin_username = 'uid='.$signin_userid.','.LDAP_PHP_USER_BASE_DN;
            if (($ldb = ldap_bind($ldc, $signin_username, $signin_userpassword)) == true) {
                //Destroy the session if another user already using current seesion.
                if (isset($_SESSION['usrname']) && !empty($_SESSION['usrname']) && ($_SESSION['usrname'] != $_POST['usrname'])) {
                    // Unset all of the session variables.
                    $_SESSION = array();

                    // Finally, destroy the session.
                    session_destroy();
                    session_write_close();
                    //start new session
                    session_start();
                }
                $_SESSION['usrname'] = $signin_userid;
                $_SESSION['LAST_ACTIVITY'] = time(); // update last activity time stamp
                //Login successful
                $error = LOGIN_SUCCESSFUL;
            } else {
                $error = ERR_LOGIN_FAILED_INVALID_USERNAME_OR_PASSWORD;
            }
            ldap_close($ldc);
        } else {
            $error = ERR_LOGIN_FAILED_INTERNAL_ERROR;
        }
    }

    return $error;
}
