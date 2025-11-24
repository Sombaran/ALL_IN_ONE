<?php
error_reporting(E_ERROR);

define("ERROR_CODE_SUCCESS", 0);
define("ERROR_CODE_INVALID_SESSION_ID", -2);
define("ERROR_CODE_INVALID_USER_NAME", -3);

// Session ID
if (!isset($argv[1]) || empty($argv[1])){
	exit(ERROR_CODE_INVALID_SESSION_ID);
}

require_once("../htdocs/register/config.php");
require_once("../htdocs/register/session.php");

// Call this before session_start()
session_id($argv[1]);

session_start();

$session_timeout = ini_get("session.gc_maxlifetime");
if(!$session_timeout || $session_timeout < 0)
{
	$session_timeout = constant("PHP_SESSION_TIMEOUT");
}

if (isset($_SESSION['LAST_ACTIVITY']) && (time() - $_SESSION['LAST_ACTIVITY'] > $session_timeout )) {

    $_SESSION = array();   // unset $_SESSION variable for the run-time 
	session_destroy();   // destroy session data in storage
	session_write_close();
	exit(ERROR_CODE_INVALID_USER_NAME);
}

$_SESSION['LAST_ACTIVITY'] = time(); // update last activity time stamp

if (!isset($_SESSION['usrname']) || empty($_SESSION['usrname']))
{
	exit(ERROR_CODE_INVALID_USER_NAME);
}

$output_handle = fopen('php://stdout', 'w');

fwrite($output_handle, $_SESSION['usrname']);

fclose($output_handle);

session_write_close();

exit(ERROR_CODE_SUCCESS);


?>
