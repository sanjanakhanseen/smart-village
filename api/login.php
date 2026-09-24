<?php

require_once "db.php";

if ($_SERVER["REQUEST_METHOD"] !== "POST") {
    echo "Only POST request allowed";
    exit;
}

$email = $_POST["email"] ?? null;
$password = $_POST["password"] ?? null;

if ($email === null || $password === null) {
    echo "Missing email or password";
    exit;
}

$sql = "SELECT id, email, password
        FROM users
        WHERE email = ?
        LIMIT 1";

$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $email);
$stmt->execute();

$result = $stmt->get_result();

if ($result->num_rows === 0) {
    echo "Invalid email or password";
    exit;
}

$user = $result->fetch_assoc();

if ($password === $user["password"]) {

    header("Content-Type: application/json");

    echo json_encode([
        "success" => true,
        "message" => "Login successful",
        "email" => $user["email"]
    ]);

} else {

    header("Content-Type: application/json");

    echo json_encode([
        "success" => false,
        "message" => "Invalid email or password"
    ]);
}

$stmt->close();
$conn->close();

?>