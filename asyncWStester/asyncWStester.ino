#include <FastLED.h>

// Define the number of LEDs
#define NUM_LEDS 1
// Define the data pin connected to the LED
#define DATA_PIN 5

// Create an array to hold the LED data
CRGB leds[NUM_LEDS];

// Task handle for the LED task
TaskHandle_t ledTaskHandle = NULL;

// Function prototype for the LED task
void ledTask(void *pvParameters);

void setup() {
  // Initialize the FastLED library
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // Create the LED task
  xTaskCreate(
    ledTask,          // Task function
    "LED Task",       // Name of the task
    2048,             // Stack size (bytes)
    NULL,             // Task input parameter
    1,                // Task priority
    &ledTaskHandle    // Task handle
  );
}

void loop() {
  // Main loop can be used for other tasks or can be empty
  vTaskDelay(1000); // Delay to prevent watchdog timer reset
}

// Asynchronous task to control the LED
void ledTask(void *pvParameters) {
  while (true) {
    // Cycle through different colors
    testLED(CRGB::Red);   // Show Red
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    testLED(CRGB::Green); // Show Green
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    testLED(CRGB::Blue);  // Show Blue
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    testLED(CRGB::White); // Show White
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    testLED(CRGB::Black); // Turn off
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Function to set the LED color and display
void testLED(CRGB color) {
  leds[0] = color; // Set the color of the first LED
  FastLED.show();  // Update the LED
}
