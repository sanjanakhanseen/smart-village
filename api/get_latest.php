<?php

require_once "db.php";

$sql = "SELECT moisture, pump_status, created_at
        FROM sensor_data
        ORDER BY id DESC
        LIMIT 1";

$result = $conn->query($sql);

if ($result && $result->num_rows > 0) {

    $data = $result->fetch_assoc();

    header("Content-Type: application/json");

    echo json_encode([
        "success" => true,
        "moisture" => (float)$data["moisture"],
        "pump" => $data["pump_status"],
        "updated_at" => $data["created_at"]
    ]);

} else {

    header("Content-Type: application/json");

    echo json_encode([
        "success" => false,
        "message" => "No sensor data found"
    ]);
}

$conn->close();

?>