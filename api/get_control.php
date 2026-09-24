<?php

require_once "db.php";

$sql = "SELECT mode, pump_command
        FROM device_control
        WHERE id = 1
        LIMIT 1";

$result = $conn->query($sql);

header("Content-Type: application/json");

if ($result && $result->num_rows > 0) {

    $data = $result->fetch_assoc();

    echo json_encode([
        "success" => true,
        "mode" => $data["mode"],
        "pump" => $data["pump_command"]
    ]);

} else {

    echo json_encode([
        "success" => false,
        "message" => "No control data found"
    ]);
}

$conn->close();

?>