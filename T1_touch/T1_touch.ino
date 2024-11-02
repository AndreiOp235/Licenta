#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>

#define TOUCH_PIN 27          // Define the GPIO pin for touch
#define THRESHOLD 100         // Define threshold for touch sensitivity
#define LED_PIN 13            // Define the GPIO pin for the WS2812 LED
#define NUM_LEDS 1            // Number of WS2812 LEDs

// Define the MAC addresses for the target devices
uint8_t target1[] = {0x88, 0x13, 0xBF, 0x0D, 0xAF, 0xCC};
uint8_t target2[] = {0x44, 0x17, 0x93, 0x7C, 0xB2, 0xD0};

// Structure to hold the data to send
typedef struct __attribute__((packed)) {
    char message[32]; // Message to send
} message_t;

message_t myData; // Create an instance of the message
bool messageSent = false; // Flag to check if the message has been sent

CRGB leds[NUM_LEDS]; // Declare an array for the LEDs

void setup() {
    Serial.begin(115200); // Start Serial communication
    WiFi.mode(WIFI_STA); // Set device as a Station
    Serial.println("ESP-NOW Sender Initialized");

    // Initialize FastLED
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    setColor(CRGB::Blue); // Set LED to blue before touch

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the peer devices
    esp_now_peer_info_t peer1;
    memcpy(peer1.peer_addr, target1, 6);
    peer1.channel = 0; // Use the default channel
    peer1.encrypt = false;

    if (esp_now_add_peer(&peer1) != ESP_OK) {
        Serial.println("Failed to add peer1");
        return;
    }

    esp_now_peer_info_t peer2;
    memcpy(peer2.peer_addr, target2, 6);
    peer2.channel = 0;
    peer2.encrypt = false;

    // Configure the touch pin
    pinMode(TOUCH_PIN, INPUT);
}

void loop() {
    // Check if the touch sensor is triggered and message has not been sent yet
    if (!messageSent && touchRead(TOUCH_PIN) > THRESHOLD) {
        // Prepare the message
        strcpy(myData.message, "Touched!");

        // Send message to both peers
        esp_now_send(target1, (uint8_t *)&myData, sizeof(myData));
        esp_now_send(target2, (uint8_t *)&myData, sizeof(myData));

        Serial.println("Message sent: Touched!");

        // Set the flag to true to prevent further sending
        messageSent = true; // Prevents subsequent sends until reset

        // Change LED color to red after touch
        setColor(CRGB::Red);
    }

    // You can reset the messageSent flag based on a different condition here if desired
}

// Function to set LED color
void setColor(const CRGB& color) {
    leds[0] = color;  // Set the first LED to the desired color
    FastLED.show();   // Update the LED display
}
