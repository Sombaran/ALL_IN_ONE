<?php

$dbfile = __DIR__.'/../oauth2-data/oauth.sqlite';

define('ERROR_SQLSEARCH_SUCCESS', 0);
define('ERROR_SQLSEARCH_FOUND_ZERO_RECORD', -2);
define('ERROR_SQLSEARCH_FOUND_MULTIPLE_RECORDS', -3);
define('ERROR_SQLSEARCH_INTERNAL_FAILURE', -4);
define('ERROR_SQLSEARCH_INVALID_ARGS_PARAMETER', -5);
define('ERROR_SQLSEARCH_EXPIRED_TOKEN', -6);

function OAuth2_GetUserInfo_Func($srch_key, $srch_txt, &$token)
{
    $errorcode = ERROR_SQLSEARCH_INTERNAL_FAILURE;
    if (!file_exists($GLOBALS['dbfile'])) {
        printf("ERROR: Database not exist. File: %s \n", $GLOBALS['dbfile']);

        return $errorcode;
    }

    $db = new PDO(sprintf('sqlite:%s', $GLOBALS['dbfile']));
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $query_table = 'oauth_access_tokens';
    $stmt = $db->prepare(sprintf('SELECT *, max(expires) AS "expires" FROM %s WHERE %s = "%s"', $query_table, $srch_key, $srch_txt));
    //$stmt = $db->prepare(sprintf('SELECT * FROM %s WHERE expires=(SELECT max(expires) FROM %s WHERE %s = "%s") AND %s = "%s"', $query_table, $query_table, $srch_key, $srch_txt, $srch_key, $srch_txt));

    $token = $stmt->execute(compact('access_token'));
    if (!$token = $stmt->fetch(\PDO::FETCH_ASSOC)) {
        $errorcode = ERROR_SQLSEARCH_FOUND_ZERO_RECORD;
    } elseif (!isset($token['expires']) || !isset($token['client_id'])) {
        $errorcode = ERROR_SQLSEARCH_FOUND_ZERO_RECORD;
    } elseif (time() > strtotime($token['expires'])) {
        $errorcode = ERROR_SQLSEARCH_EXPIRED_TOKEN;
    } else {
        $errorcode = ERROR_SQLSEARCH_SUCCESS;
    }

    //if ($errorcode == ERROR_SQLSEARCH_SUCCESS) {
    //    var_dump($token);
    //}

    return $errorcode;
}

function OAuth2_GetUserInfo($srch_key, $srch_txt, $getinfo_key, &$result_str)
{
    $result_arry = array();

    switch ($srch_key) {
        case 'ACCESS_TOKEN':
            $sql_srch_key = 'access_token';
            break;

        case 'USR_ID':
            $sql_srch_key = 'user_id';
            break;

        default:
            $result_str = '';

            return ERROR_SQLSEARCH_INVALID_ARGS_PARAMETER;
    }

    $sql_search_result = OAuth2_GetUserInfo_Func($sql_srch_key, $srch_txt, $result_arry);
    $result_str = '';
    if ($sql_search_result == ERROR_SQLSEARCH_SUCCESS) {
        switch ($getinfo_key) {
            case 'ACCESS_TOKEN':
                $result_str = $result_arry['access_token'];
                break;

            case 'CLIENT_ID':
                $result_str = $result_arry['client_id'];
                break;

            case 'USR_ID':
                $result_str = $result_arry['user_id'];
                break;

            case 'EXPIRES':
                $result_str = $result_arry['expires'];
                break;

            case 'SCOPE':
                $result_str = $result_arry['scope'];
                break;

            default:
                $result_str = '';

                return ERROR_SQLSEARCH_INVALID_ARGS_PARAMETER;
        }
    }

    return $sql_search_result;
}

/*
function OAuth2_GetUserInfo_TestCases()
{
    $result_getinfo = '';
    $my_search_key = 'USR_ID';
    $my_search_str = 'fteo';
    $my_getinfo_key = 'ACCESS_TOKEN';
    $oauth2_search_result = OAuth2_GetUserInfo($my_search_key, $my_search_str, $my_getinfo_key, $result_getinfo);
    echo $result_getinfo;

    $result_getinfo = '';
    $my_search_key = 'ACCESS_TOKEN';
    $my_search_str = '7a54f768196ca6245ff7ddfadc7d7bdd76f9ac6b';
    $my_getinfo_key = 'USR_ID';
    $oauth2_search_result = OAuth2_GetUserInfo($my_search_key, $my_search_str, $my_getinfo_key, $result_getinfo);
    echo $result_getinfo;
}
*/
