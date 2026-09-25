# Smart Village: IoT-Based Agriculture and Water Management System

Smart Village is an IoT-based agriculture and water management system designed to monitor soil moisture and control a water pump automatically or manually.

## Features

- Real-time soil moisture monitoring
- Automatic water pump control
- Manual pump control
- 16x2 I2C LCD display
- ESP8266-based IoT communication
- PHP and MySQL backend
- Web-based monitoring and control
- Sensor data stored in MySQL database

## How It Works

The soil moisture sensor measures the moisture level of the soil and sends the data through ESP8266 to the PHP backend.

In AUTO mode, the system checks the soil moisture level and automatically controls the water pump based on a fixed moisture threshold.

In MANUAL mode, the user can control the water pump from the web interface.

The sensor readings and pump status are stored in the MySQL database and displayed on the web interface.

## Hardware

- ESP8266
- Soil Moisture Sensor
- 16x2 I2C LCD
- Relay Module
- Mini Water Pump
- 2xAAA Battery Holder
- Jumper Wires

## Software and Technologies

- HTML
- CSS
- JavaScript
- PHP
- MySQL
- Arduino IDE
- ESP8266
- XAMPP

## Project Structure

smart-village/
├── api/
├── database/
│   └── schema.sql
├── esp8266/
│   └── smart_village.ino
└── index.html

## Database

The database schema is available in:

database/schema.sql

The database contains tables for:

- Users
- Sensor data
- Device control

## ESP8266 Setup

Before uploading the Arduino code, update the Wi-Fi credentials and server URL in:

esp8266/smart_village.ino

Real Wi-Fi passwords should not be uploaded to the public repository.

## Backend Setup

This project uses XAMPP with Apache and MySQL.

Place the project folder inside the XAMPP htdocs directory.

Create the database by importing:

database/schema.sql

Then start Apache and MySQL from XAMPP.

## Project Purpose

The main goal of this project is to demonstrate how IoT can be used in agriculture for soil monitoring and smart water management.
