<?php

$url = "http://localhost/smart_village/api/login.php";

$data = [
    "email" => "sanjanakhansseen@gmail.com",
    "password" => "1234"
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