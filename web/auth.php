<?php
include 'Server.php';

/// create a new instance
$server      = new Server();
if(!$server->initialize()) {
    die("forbidden");
}
/// check the GET param
$client_data = $server->parse_top_request();
if(!$client_data) {
    die("forbidden");
}

/// handle the request
$server->handle($client_data);
$server->close_connection();
