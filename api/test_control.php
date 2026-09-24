<?php

$url = "http://localhost/smart_village/api/set_control.php";

$data = [
    "mode" => "MANUAL",
    "pump" => "ON"
];

$options = [
    "http" => [
        "method" => "POST",
        "header" => "Content-Type: application/x-www-form-urlencoded",
        "content" => http_build_query($data)
    ]
];

$context = stream_context_create($options);

$response = file_get_contents($url, false, $context);

echo $response;
?>