#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t receiverMACAddress[] = {0x88, 0x13, 0xbf, 0x0b, 0x87, 0xf4};

// Define variables to store sensor readings to be sent
int sensorValueToSend = 100;

// Define variable to store incoming readings
int receivedSensorValue = 100;

// Variable to store if sending data was successful
String transmissionStatus;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_sensor_message {
    int pixel;  // Represents the sensor reading
} struct_sensor_message;

// Create an instance of struct_sensor_message to hold sensor readings
struct_sensor_message sensorReadingsToSend;

// Create an instance to hold incoming sensor readings
struct_sensor_message incomingSensorReadings;

esp_now_peer_info_t peerInfo;

// Variables to hold the time of sending and receiving messages
unsigned long long sendTimestamp = 0;
unsigned long long receiveTimestamp = 0;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    sendTimestamp = micros(); // Record the time when data is sent
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  
    if (status == ESP_NOW_SEND_SUCCESS) {
        transmissionStatus = "Delivery Success :)";
    } else {
        transmissionStatus = "Delivery Fail :(";
    }
}

// Callback when data is received
void onDataReceived(const esp_now_recv_info* info, const uint8_t *incomingData, int len) {
    receiveTimestamp = micros(); // Record the time when data is received
    memcpy(&incomingSensorReadings, incomingData, sizeof(incomingSensorReadings));
    Serial.print("Bytes received: ");
    Serial.println(len);
    
    // Calculate and print the time difference
    unsigned long long timeDifference = receiveTimestamp - sendTimestamp;
    Serial.print("Time between send and receive: ");
    Serial.print(timeDifference);
    Serial.println(" µs"); // Change to µs for microseconds

    sensorReadingsToSend.pixel = incomingSensorReadings.pixel + 1;
    Serial.println(sensorReadingsToSend.pixel);

    if (sensorReadingsToSend.pixel > 300) {
        delay(60 * 1000);
        sensorReadingsToSend.pixel = 100;
    }

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(receiverMACAddress, (uint8_t *)&sensorReadingsToSend, sizeof(sensorReadingsToSend));
   
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}

void setup() {
    // Init Serial Monitor
    Serial.begin(115200);
 
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register for Send Callback to get the status of transmitted packets
    esp_now_register_send_cb(onDataSent);
  
    // Register peer
    memcpy(peerInfo.peer_addr, receiverMACAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
  
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(onDataReceived);

    // Set values to send
    sensorReadingsToSend.pixel = sensorValueToSend;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(receiverMACAddress, (uint8_t *)&sensorReadingsToSend, sizeof(sensorReadingsToSend));
   
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}

void loop() {
    //delay(100); // Wait for 1 second before sending the next value
}
