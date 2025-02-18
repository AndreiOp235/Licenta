#include <FastLED.h>

#define BUTTON_PIN 2    // GPIO pin for the button
#define RELAY_PIN  3    // GPIO pin for the relay
#define LED_PIN    4    // GPIO pin for the WS2812B LED
#define NUM_LEDS   1    // Number of LEDs in the strip

CRGB leds[NUM_LEDS];

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("Button, Relay, and WS2812B LED Control Starting...");

    // Set up the button pin as input (external pull-down)
    pinMode(BUTTON_PIN, INPUT);

    // Set up the relay pin as output
    pinMode(RELAY_PIN, OUTPUT);

    // Ensure the relay starts in the OFF state
    digitalWrite(RELAY_PIN, LOW);

    // Initialize the WS2812B LED
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.clear();  // Turn off all LEDs initially
    FastLED.show();
}

void loop() {
    // Read the button state
    int buttonState = digitalRead(BUTTON_PIN);

    // If the button is pressed, activate the relay and set LED to green
    if (buttonState == HIGH) {
        Serial.println("Button Pressed - Relay ON, LED Green");
        pinMode(RELAY_PIN, OUTPUT);
        setLEDColor(0, 255, 0);         // Green: Button pressed, relay ON
    } else {
        Serial.println("Button Released - Relay OFF, LED Red");
        pinMode(RELAY_PIN, INPUT);   // Turn OFF the relay
        setLEDColor(255, 0, 0);         // Red: Button released, relay OFF
    }

    // Add a small delay to debounce the button
    delay(50);
}

// Function to set the LED color
void setLEDColor(uint8_t red, uint8_t green, uint8_t blue) {
    leds[0] = CRGB(red, green, blue);  // Set color for the first LED
    FastLED.show();  // Update the LED to show the new color
}
