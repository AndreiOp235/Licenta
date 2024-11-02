#include <ESP32Servo.h> // Correct library name for the ESP32 Servo library
#include <WiFi.h>
#include <esp_now.h>

// Define the servo object
Servo myServo;

// Define the pin where the servo is connected
#define SERVO_PIN 4 // Change this to your servo's pin

// Task handle for the servo task
TaskHandle_t servoTaskHandle = NULL;

// Flag to control servo movement
volatile bool stopServo = false;

// Function prototype for the servo sweep task
void servoSweepTask(void *pvParameters);

// Updated callback function to handle received ESP-NOW messages
void onDataReceive(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
    // Stop the servo when a message is received
    stopServo = true; // Set the flag to stop the servo

    // Notify the servo task to stop
    if (servoTaskHandle != NULL) {
        vTaskDelete(servoTaskHandle); // Delete the servo task directly
    }
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Set up WiFi for ESP-NOW
    WiFi.mode(WIFI_STA);
    esp_now_init();

    // Register the callback function for receiving messages
    esp_now_register_recv_cb(onDataReceive);

    // Attach the servo to the specified pin
    myServo.attach(SERVO_PIN);

    // Create the servo sweep task
    xTaskCreate(
        servoSweepTask,   // Task function
        "Servo Sweep Task",// Name of the task
        2048,             // Stack size (bytes)
        NULL,             // Task input parameter
        1,                // Task priority
        &servoTaskHandle  // Task handle
    );
}

void loop() {
    // Main loop can be empty or used for other tasks
    vTaskDelay(1000); // Delay to prevent watchdog timer reset
}

// Asynchronous task to sweep the servo
void servoSweepTask(void *pvParameters) {
    // Initial position of the servo
    int pos = 0;

    // Sweep from 0 to 120 degrees and back to 0
    while (true) {
        // If the stopServo flag is set, exit the loop
        if (stopServo) {
            // The servo will hold its position
            myServo.write(pos); // Keep the servo at its last position
            vTaskDelete(NULL); // Terminate this task
        }

        // Sweep to 120 degrees
        for (pos = 0; pos <= 120; pos++) {
            if (stopServo) {
                // If stop flag is set, exit the for loop
                break;
            }
            myServo.write(pos);           // Set the servo position
            vTaskDelay(15 / portTICK_PERIOD_MS); // Wait for the servo to reach the position
        }

        // If stopped, exit the sweeping process
        if (stopServo) {
            continue; // Skip back sweeping
        }

        // Sweep back to 0 degrees
        for (pos = 120; pos >= 0; pos--) {
            if (stopServo) {
                // If stop flag is set, exit the for loop
                break;
            }
            myServo.write(pos);           // Set the servo position
            vTaskDelay(15 / portTICK_PERIOD_MS); // Wait for the servo to reach the position
        }
    }
}
