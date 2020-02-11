<?php

header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array();
 
$filepath = realpath (dirname(__FILE__));
require_once($filepath."/db_connect.php");

$db = new DB_CONNECT();	
 
$result = mysql_query("SELECT * FROM sensores ORDER BY id DESC") or die(mysql_error());
 
if (mysql_num_rows($result) > 0) {
    
    $response["sensores"] = array();
 
    while ($row = mysql_fetch_array($result)) {
 
        $sensores = array();
        $sensores["id"] = $row["id"];
        $sensores["temp"] = $row["temp"];
		$sensores["umid"] = $row["umid"];
		$sensores["prox"] = $row["prox"];
		$sensores["date"] = $row["date"];

        array_push($response["sensores"], $sensores);
    }
 
    $response["success"] = 1;
 
    echo json_encode($response);
}	
else 
{
 
	$response["success"] = 0;
    $response["message"] = "Por favor, revise a requisição";
 

    echo json_encode($response);
}
?>
