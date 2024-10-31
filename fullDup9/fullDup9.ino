#include <esp_now.h>
#include <WiFi.h>

// Uncomment the following line to enable debug output
//#define DEBUG

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

// Variables to hold latencies
#define MAX_LATENCIES 100
unsigned long long latencies[MAX_LATENCIES];
int latencyIndex = 0;

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
    sendTimestamp = micros(); // Record the time when data is sent
    SERIAL_PRINT("\r\nLast Packet Send Status:\t");
    SERIAL_PRINTLN(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  
    if (status == ESP_NOW_SEND_SUCCESS) {
        transmissionStatus = "Delivery Success :)";
    } else {
        transmissionStatus = "Delivery Fail :(";
    }
}

// Function to calculate the average latency
double calculateAverageLatency(unsigned long long latencies[], int count) {
    if (count == 0) return 0;  // Avoid division by zero
    unsigned long long sum = 0;
    for (int i = 0; i < count; i++) {
        sum += latencies[i];
    }
    return static_cast<double>(sum) / count; // Return average
}

void onDataReceived(const esp_now_recv_info* info, const uint8_t *incomingData, int len) {
    receiveTimestamp = micros(); // Record the time when data is received
    memcpy(&incomingSensorReadings, incomingData, sizeof(incomingSensorReadings));
    
    // Calculate and print the time difference
    unsigned long long timeDifference = receiveTimestamp - sendTimestamp;
    latencies[latencyIndex++] = timeDifference;  // Store the latency

    sensorReadingsToSend.pixel = incomingSensorReadings.pixel + 1;
    SERIAL_PRINTLN(sensorReadingsToSend.pixel);

    if (sensorReadingsToSend.pixel > 300) {
        // Print all latencies
        Serial.println("All recorded latencies:");
        for (int i = 0; i < latencyIndex; i++) {
            Serial.print(latencies[i]);
            Serial.print(" µs ");
        }
        Serial.println();

        // Calculate and print average latency
        double averageLatency = calculateAverageLatency(latencies, latencyIndex);
        Serial.print("Average latency: ");
        Serial.println(averageLatency);

        // Reset the latencies
        latencyIndex = 0;
        
        delay(60 * 1000); // Wait for 1 minute
        sensorReadingsToSend.pixel = 100; // Reset the pixel value
    }

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(receiverMACAddress, (uint8_t *)&sensorReadingsToSend, sizeof(sensorReadingsToSend));
   
    if (result == ESP_OK) {
        SERIAL_PRINTLN("Sent with success");
    } else {
        SERIAL_PRINTLN("Error sending the data");
    }
}


void setup() {
    // Init Serial Monitor
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
    Serial.begin(115200);
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        SERIAL_PRINTLN("Error initializing ESP-NOW");
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
        SERIAL_PRINTLN("Failed to add peer");
        return;
    }

    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(onDataReceived);

    // Set values to send
    sensorReadingsToSend.pixel = sensorValueToSend;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(receiverMACAddress, (uint8_t *)&sensorReadingsToSend, sizeof(sensorReadingsToSend));
   
    if (result == ESP_OK) {
        SERIAL_PRINTLN("Sent with success");
    } else {
        SERIAL_PRINTLN("Error sending the data");
    }
}

void loop() {
    //delay(100); // Wait for 1 second before sending the next value
}
