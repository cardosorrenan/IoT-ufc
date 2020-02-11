<?php

header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

$response = array();
 
if (isset($_GET['temp']) && isset($_GET['umid']) && isset($_GET['prox'])) {
 
    $temp = $_GET['temp'];
    $umid = $_GET['umid'];
    $prox = $_GET['prox'];
 
    $filepath = realpath (dirname(__FILE__));
	require_once($filepath."/db_connect.php");

    $db = new DB_CONNECT();

    $result = mysql_query("INSERT INTO sensores(temp,umid,prox) VALUES('$temp','$umid','$prox')");
 
    if ($result) {
        $response["success"] = 1;
        $response["message"] = "As informacoes foram armazenadas com sucesso";
 
        echo json_encode($response);
    } else {
        $response["success"] = 0;
        $response["message"] = "Algo esta errado...";
 
        echo json_encode($response);
    }
} else {
    $response["success"] = 0;
    $response["message"] = "Por favor, revise a requisicao";
 
    echo json_encode($response);
}
?>
