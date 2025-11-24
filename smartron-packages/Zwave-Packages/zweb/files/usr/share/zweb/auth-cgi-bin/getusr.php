<?php
error_reporting(E_ERROR);
$finput = fopen('php://stdin', 'r');
$cid = fgets($finput);
fclose($finput);
$sesid = trim($cid);

$location = ini_get("session.save_path");

if(($location != FALSE) && ($location != null))
{
	$file = "$location/sess_" . "$sesid";
}
else
{
	$file = "/tmp/sess_" . "$sesid";
}

//$file = "ses_796c950badca724ff2d14bc52e53f2af";
$contents=file_get_contents($file);

function unserialize_php($session_data) {
        $return_data = array();
        $offset = 0;
        while ($offset < strlen($session_data)) {
            if (!strstr(substr($session_data, $offset), "|")) {
		//Data format is not correct..
                return $return_data;
            }
            $pos = strpos($session_data, "|", $offset);
            $num = $pos - $offset;
            $varname = substr($session_data, $offset, $num);
            $offset += $num + 1;
            $data = unserialize(substr($session_data, $offset));
            $return_data[$varname] = $data;
            $offset += strlen(serialize($data));
        }
        return $return_data;
}


$foutput = fopen('php://stdout', 'w');
$var1="";

if($contents === false)
{
	fwrite($foutput, $var1);
}
else
{
	$sesinfo=unserialize_php($contents);	
	if(isset($sesinfo['usrname']))
	{
        	fwrite($foutput, $sesinfo['usrname']);
	}	
	else
	{
        	fwrite($foutput, $var1);
	}
	
}

fclose($foutput);
?>
