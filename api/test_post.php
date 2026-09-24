<?php

$url = "http://localhost/smart_village/api/save_data.php";

$data = [
    "moisture" => 32,
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