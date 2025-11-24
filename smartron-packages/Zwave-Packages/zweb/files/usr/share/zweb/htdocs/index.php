<?php
error_reporting(E_ERROR);

require_once("./register/config.php");
require_once("./register/session.php");
session_start();

header("Location:register/login.html");
die();

