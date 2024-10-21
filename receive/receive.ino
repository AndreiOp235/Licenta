#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
bool flag = false;

uint8_t newMACAddress[] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

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

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  DEBUG_PRINT("Bytes received: ");
  DEBUG_PRINTLN(len);
  DEBUG_PRINT("Char: ");
  DEBUG_PRINTLN(myData.a);
  DEBUG_PRINT("Int: ");
  DEBUG_PRINTLN(myData.b);
  DEBUG_PRINT("Float: ");
  DEBUG_PRINTLN(myData.c);
  DEBUG_PRINT("Bool: ");
  DEBUG_PRINTLN(myData.d);
  DEBUG_PRINTLN();
  flag = !flag;
  digitalWrite(4, flag);
}

void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, flag);
  
  // Initialize Serial Monitor conditionally based on DEBUG macro
  DEBUG_BEGIN(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  if (err == ESP_OK) {
    DEBUG_PRINTLN("Success changing Mac Address");
  }

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    DEBUG_PRINTLN("Error initializing ESP-NOW");
    return;
  }

  // Register callback function to receive ESP-NOW data
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Main loop
}
