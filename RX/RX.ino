#include <esp_now.h>
#include <WiFi.h>

// Uncomment the following line to enable debug mode
//#define DEBUG

// Structure to hold received data
typedef struct struct_message {
    int data1;
    float data2;
} struct_message;

struct_message receivedData;

// Callback when data is received
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    
    #ifdef DEBUG
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Data1: ");
    Serial.println(receivedData.data1);
    Serial.print("Data2: ");
    Serial.println(receivedData.data2);
    #endif
}

void setup() {
    #ifdef DEBUG
    Serial.begin(115200);
    #endif

    // Initialize WiFi in STA mode
    WiFi.mode(WIFI_STA);
    #ifdef DEBUG
    Serial.println("ESP32 RX Started");
    #endif

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        #ifdef DEBUG
        Serial.println("Error initializing ESP-NOW");
        #endif
        return;
    }

    // Register the receive callback with the updated function signature
    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    // Nothing to do here, data reception is handled in the callback
}
