<?php

require_once "db.php";

if ($_SERVER["REQUEST_METHOD"] !== "POST") {
    echo "Only POST request allowed";
    exit;
}

$mode = $_POST["mode"] ?? null;
$pump = $_POST["pump"] ?? null;

if ($mode === null || $pump === null) {
    echo "Missing data";
    exit;
}

$sql = "UPDATE device_control
        SET mode = ?, pump_command = ?
        WHERE id = 1";

$stmt = $conn->prepare($sql);
$stmt->bind_param("ss", $mode, $pump);

if ($stmt->execute()) {
    echo "Control updated successfully";
} else {
    echo "Failed to update control";
}

$stmt->close();
$conn->close();

?>