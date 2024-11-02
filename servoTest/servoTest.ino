#include <ESP32Servo.h> // Correct library name for the ESP32 Servo library

// Define the servo object
Servo myServo;

// Define the pin where the servo is connected
#define SERVO_PIN 4 // Change this to your servo's pin

// Task handle for the servo task
TaskHandle_t servoTaskHandle = NULL;

// Function prototype for the servo sweep task
void servoSweepTask(void *pvParameters);

void setup() {
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
  // Sweep from 0 to 120 degrees and back to 0
  while (true) {
    // Sweep to 120 degrees
    for (int pos = 0; pos <= 120; pos++) {
      myServo.write(pos);           // Set the servo position
      vTaskDelay(15 / portTICK_PERIOD_MS); // Wait for the servo to reach the position
    }

    // Sweep back to 0 degrees
    for (int pos = 120; pos >= 0; pos--) {
      myServo.write(pos);           // Set the servo position
      vTaskDelay(15 / portTICK_PERIOD_MS); // Wait for the servo to reach the position
    }
  }
}
