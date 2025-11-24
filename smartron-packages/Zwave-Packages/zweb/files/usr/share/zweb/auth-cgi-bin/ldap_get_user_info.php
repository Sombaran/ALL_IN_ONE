<?php
// Note: This php required 3 parameters.
// Parameter 1: Either one of these [USR_ID, USR_NAME, EMAIL, RAC]
// Parameter 2: Your search string according to below usage example
// Parameter 3: Get user info field on [FIRSTNAME, LASTNAME, FULLNAME, USR_ID, EMAIL, RAC, RAPIN, TIMEZONE]
// Return console printout: Required detail based on Parameter 3
// Return: Return 0 if user existence. Return non-zero if user not existence
// Usage example for LDAP:
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    FULLNAME
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    FIRSTNAME
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    LASTNAME
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    USR_ID
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    EMAIL
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    RAC
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    RAPIN
// ../../php/bin/php ./ldap_get_user_info.php USR_ID   anlee                    TIMEZONE
// ../../php/bin/php ./ldap_get_user_info.php USR_NAME "anthony, lee"           EMAIL
// ../../php/bin/php ./ldap_get_user_info.php USR_NAME "anthony"                FULLNAME
// ../../php/bin/php ./ldap_get_user_info.php USR_NAME ",lee"                   FULLNAME
// ../../php/bin/php ./ldap_get_user_info.php EMAIL    anthonylee@example.com   RAC
// ../../php/bin/php ./ldap_get_user_info.php RAC      00-1E-32-FF-FF-12-34-56  RAPIN
// Usage example for oAuth2:
// ../../php/bin/php -c ../../php/lib/php.ini ./ldap_get_user_info.php  USR_ID        anlee     ACCESS_TOKEN
// ../../php/bin/php -c ../../php/lib/php.ini ./ldap_get_user_info.php  ACCESS_TOKEN  deadbeef  USR_ID

require_once __DIR__.'/../htdocs/register/config.php';
require_once __DIR__.'/ldap_func.php';
require_once __DIR__.'/oauth2_sqlite_func.php';

if ($argc != 4) {
    exit(ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER);
}

$result_getinfo = '';
$search_keyword = $argv[1];
$search_string = $argv[2];
$getinfo_keyword = $argv[3];
if (($search_keyword == 'ACCESS_TOKEN') || ($getinfo_keyword == 'ACCESS_TOKEN')) {
    $ldap_search_result = OAuth2_GetUserInfo($argv[1], $argv[2], $argv[3], $result_getinfo);
} else {
    $ldap_search_result = LDAP_GetUserInfo($argv[1], $argv[2], $argv[3], $result_getinfo);
}
if ($ldap_search_result === 0) {
    $output_handle = fopen('php://stdout', 'w');
    fwrite($output_handle, "$result_getinfo");
    fclose($output_handle);
}
exit($ldap_search_result);
