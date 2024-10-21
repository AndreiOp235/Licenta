#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// Define the DEBUG macro
#define DEBUG 0  // Set to 0 to disable debugging

#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_BEGIN(x) Serial.begin(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_BEGIN(x)
#endif

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  DEBUG_PRINT("\r\nLast Packet Send Status:\t");
  DEBUG_PRINTLN(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Initialize Serial Monitor based on DEBUG flag
  DEBUG_BEGIN(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    DEBUG_PRINTLN("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully initialized, register for Send Callback to get the status of transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    DEBUG_PRINTLN("Failed to add peer");
    return;
  }
}

unsigned long lastDebounceTime = 0;   // the last time the sensor reading changed
unsigned long debounceDelay = 50;     // the debounce time; increase if necessary
int lastReading = HIGH;               // keep track of the previous state
bool flag = false;                    // toggle flag to prevent repeat sends

void loop() {
  int reading = touchRead(4);         // Read touch sensor value

  // Check if reading is stable and within threshold
  if (reading < 50 && (millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();      // Reset the debounce timer
    
    if (!flag) {                      // Ensure we only send data once until sensor is released
      // Set values to send
      strcpy(myData.a, "THIS IS A CHAR");
      myData.b = reading;
      myData.c = 1.2;
      myData.d = false;
    
      // Send message via ESP-NOW
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
      if (result == ESP_OK) {
        DEBUG_PRINTLN("Sent with success");
      } else {
        DEBUG_PRINTLN("Error sending the data");
      }
    
      delay(2000);       // Optional: Delay to avoid excessive repeated sends
      flag = true;       // Set flag to prevent repeated sending
    }
  }
  
  // Reset flag when the sensor is no longer touched
  if (reading >= 50) {
    flag = false;        // Reset flag when sensor is released
  }
}
