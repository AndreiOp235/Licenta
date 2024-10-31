#include <esp_now.h>
#include <WiFi.h>

// Uncomment the following line to enable debug output
//#define DEBUG

// REPLACE WITH THE MAC Address of your master (sender) 
uint8_t masterMACAddress[] = {0x88, 0x13, 0xbf, 0x0d, 0xaf, 0xcc};  // MAC address of the master device

// Define a structure to hold sensor readings
typedef struct struct_sensor_message {
    int pixel;  // Represents the sensor reading
} struct_sensor_message;

// Create an instance to hold incoming sensor readings
struct_sensor_message incomingSensorReadings;

// Create an instance to hold sensor readings to send back
struct_sensor_message sensorReadingsToSend;

esp_now_peer_info_t peerInfo;

// Macro for Serial output
#ifdef DEBUG
    #define SERIAL_PRINT(x) Serial.print(x)
    #define SERIAL_PRINTLN(x) Serial.println(x)
#else
    #define SERIAL_PRINT(x)
    #define SERIAL_PRINTLN(x)
#endif

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    SERIAL_PRINT("\r\nLast Packet Send Status:\t");
    SERIAL_PRINTLN(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void onDataReceived(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    memcpy(&incomingSensorReadings, incomingData, sizeof(incomingSensorReadings));
    SERIAL_PRINT("Bytes received: ");
    SERIAL_PRINTLN(len);
    SERIAL_PRINT("Received value: ");
    SERIAL_PRINTLN(incomingSensorReadings.pixel);

    // Increment the received value by 1
    sensorReadingsToSend.pixel = incomingSensorReadings.pixel + 1;
    SERIAL_PRINT("Sending back value: ");
    SERIAL_PRINTLN(sensorReadingsToSend.pixel);
    
    // Send the incremented value back to the master
    esp_now_send(masterMACAddress, (uint8_t *)&sensorReadingsToSend, sizeof(sensorReadingsToSend));
}

void setup() {
    // Init Serial Monitor
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
 
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        SERIAL_PRINTLN("Error initializing ESP-NOW");
        return;
    }

    // Register for Send Callback to get the status of transmitted packets
    esp_now_register_send_cb(onDataSent);
  
    // Register peer (the master device)
    memcpy(peerInfo.peer_addr, masterMACAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
  
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        SERIAL_PRINTLN("Failed to add peer");
        return;
    }

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(onDataReceived);
}

void loop() {
    // The slave device does not need to do anything in the loop
    // It will respond to incoming messages when they are received
    //delay(100); // Small delay to avoid CPU overload
}
