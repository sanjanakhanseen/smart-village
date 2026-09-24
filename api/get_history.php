<?php

require_once "db.php";

$sql = "SELECT moisture, pump_status, created_at
        FROM sensor_data
        ORDER BY id DESC
        LIMIT 8";

$result = $conn->query($sql);

header("Content-Type: application/json");

$data = [];

if ($result) {
    while ($row = $result->fetch_assoc()) {
        $data[] = [
            "moisture" => (float)$row["moisture"],
            "pump" => $row["pump_status"],
            "created_at" => $row["created_at"]
        ];
    }
}

echo json_encode([
    "success" => true,
    "readings" => $data
]);

$conn->close();

?>