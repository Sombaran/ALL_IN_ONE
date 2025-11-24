<?php
# Note: This php required 2 parameters.
# Parameter 1: Either one of these [USR_ID, USR_NAME, EMAIL, RAC]
# Parameter 2: Your search string according to below usage example
# Return: Return 0 if user existence. Return non-zero if user not existence
#
# Usage example:
# ../../php/bin/php ./ldap_check_user_existence.php USR_ID   anlee
# ../../php/bin/php ./ldap_check_user_existence.php USR_NAME "anthony, lee"
# ../../php/bin/php ./ldap_check_user_existence.php USR_NAME "anthony"
# ../../php/bin/php ./ldap_check_user_existence.php USR_NAME ",lee"
# ../../php/bin/php ./ldap_check_user_existence.php EMAIL    anthonylee@example.com
# ../../php/bin/php ./ldap_check_user_existence.php RAC      00-1E-32-FF-FF-12-34-56

require_once __DIR__.'/../htdocs/register/config.php';
require_once __DIR__.'/ldap_func.php';

/*
LDAP_SearchUserInfo_TestCases - 6 Test Cases for LDAP_SearchUserInfo()
*/
function LDAP_SearchUserInfo_TestCases()
{
    $total_testcases = 6;

    $ldap_search_result = LDAP_SearchUserInfo('USR_ID', 'fteo');
    echo "Result for TestCase 1 of $total_testcases: $ldap_search_result \n";
    if (ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    $ldap_search_result = LDAP_SearchUserInfo('USR_NAME', 'Frank, Teo');
    echo "Result for TestCase 2 of $total_testcases: $ldap_search_result \n";
    if ($ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    $ldap_search_result = LDAP_SearchUserInfo('USR_NAME', 'Fra*');
    echo "Result for TestCase 3 of $total_testcases: $ldap_search_result \n";
    if ($ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    $ldap_search_result = LDAP_SearchUserInfo('USR_NAME', ',Teo');
    echo "Result for TestCase 4 of $total_testcases: $ldap_search_result \n";
    if ($ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    $ldap_search_result = LDAP_SearchUserInfo('EMAIL', '*@sigmadesigns.com');
    echo "Result for TestCase 5 of $total_testcases: $ldap_search_result \n";
    if ($ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    $ldap_search_result = LDAP_SearchUserInfo('RAC', '00-1E-32-FF-FF-70-04-01');
    echo "Result for TestCase 6 of $total_testcases: $ldap_search_result \n";
    if ($ldap_search_result != ERROR_LDAPSEARCH_SUCCESS) {
        return $ldap_search_result;
    }

    return ERROR_LDAPSEARCH_SUCCESS;
}

if ($argc != 3) {
    exit(ERROR_LDAPSEARCH_INVALID_ARGS_PARAMETER);
}

//LDAP_SearchUserInfo_TestCases();
$ldap_search_result = LDAP_SearchUserInfo($argv[1], $argv[2]);

$output_handle = fopen('php://stdout', 'w');
fwrite($output_handle, "$ldap_search_result");
fclose($output_handle);
exit($ldap_search_result);
