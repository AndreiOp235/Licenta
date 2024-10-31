#include <esp_now.h>
#include <WiFi.h>

// MAC address of the receiver ESP32
uint8_t receiverAddress[] = {0x88, 0x13, 0xBF, 0x0B, 0x87, 0xF4};  // Replace with receiver's MAC

// Structure to hold the data
typedef struct struct_message {
    int data1;
    float data2;
} struct_message;

struct_message dataToSend;

// Variables for timing
unsigned long sendStartTime;
unsigned long sendTimes[10];  // Array to store elapsed times for 10 messages
int messageCount = 0;
bool testCompleted = false;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (messageCount < 10) {
        sendTimes[messageCount] = micros() - sendStartTime;  // Store elapsed time
        messageCount++;
    }

    if (messageCount == 10) {
        testCompleted = true;  // Mark test as completed after 10 messages
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 TX Speed Test Started");

    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the send callback
    esp_now_register_send_cb(onDataSent);

    // Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    // Disable Serial output for speed test
    Serial.end();
}

void loop() {
    if (!testCompleted) {
        // Send 10 messages and measure time for each
        for (int i = 0; i < 10; i++) {
            // Prepare data to send
            dataToSend.data1 = random(0, 100);  // Random integer data
            dataToSend.data2 = random(0, 100) / 10.0;  // Random float data

            // Record the start time in microseconds
            sendStartTime = micros();

            // Send message
            esp_now_send(receiverAddress, (uint8_t *) &dataToSend, sizeof(dataToSend));

            // Small delay to avoid overwhelming the ESP-NOW queue
            delay(10);
        }
    } else {
        // Enable Serial output after the test
        Serial.begin(115200);
        delay(100);  // Short delay to allow Serial to initialize

        // Print the results
        Serial.println("ESP32 TX Speed Test Results (in mili):");
        for (int i = 0; i < 10; i++) {
            Serial.print("Message ");
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(sendTimes[i]/1000.0);
            Serial.println(" ms");
        }

        // Delay for 3 seconds
        delay(3000);

        // Reset for another test if needed
        testCompleted = false;
        messageCount = 0;
        Serial.end();  // Turn off Serial again for next test
    }
}
