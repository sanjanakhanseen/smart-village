<?php

require_once "db.php";

if ($_SERVER["REQUEST_METHOD"] !== "POST") {
    echo "Only POST request allowed";
    exit;
}

$moisture = $_POST["moisture"] ?? null;
$pump = $_POST["pump"] ?? null;

if ($moisture === null || $pump === null) {
    echo "Missing data";
    exit;
}

$sql = "INSERT INTO sensor_data (moisture, pump_status)
        VALUES (?, ?)";

$stmt = $conn->prepare($sql);
$stmt->bind_param("ds", $moisture, $pump);

if ($stmt->execute()) {
    echo "Data saved successfully";
} else {
    echo "Failed to save data";
}

$stmt->close();
$conn->close();

?>