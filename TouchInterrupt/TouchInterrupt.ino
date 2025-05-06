#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>

// Definire DEBUG Mode (comentează linia pentru a dezactiva Serial)
#define DEBUG  

#define threshold 40
#define PIN_TOUCH  14
#define LED_PIN    13 // GPIO pentru LED WS2812B
#define NUMAR_LEDS 1  // Numărul de LED-uri

Adafruit_NeoPixel leduri(NUMAR_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
bool touch1detected = false;

// Adresa MAC a receptorului
uint8_t broadcastAddress[] = {0x34, 0xCD, 0xB0, 0xD3, 0x88, 0x8C};

// Struct pentru date
typedef struct struct_message {
  int value;
} struct_message;

struct_message myData;

// Funcție ISR pentru touch
void gotTouch1() {
  touch1detected = true;
}

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32 Touch + ESP-NOW");
  #endif

  // Inițializare senzor touch cu întrerupere
  touchAttachInterrupt(PIN_TOUCH, gotTouch1, threshold);
  touchInterruptSetThresholdDirection(false);

  // Inițializare LED
  leduri.begin();
  leduri.clear();
  leduri.setBrightness(60);
  leduri.show();
  seteazaCuloareLED(0, 255, 0);  // Verde

  // Setare ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    #ifdef DEBUG
      Serial.println("ESP-NOW init failed!");
    #endif
    return;
  }

  // Adăugare peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    #ifdef DEBUG
      Serial.println("Failed to add peer");
    #endif
    return;
  }
}

// Callback ESP-NOW la trimitere
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  #ifdef DEBUG
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  #endif
}

void loop() {
  if (touch1detected) {
    #ifdef DEBUG
      Serial.println("Touch detected, sending data...");
    #endif
    
    seteazaCuloareLED(255, 0, 0);  // Roșu
    
    // Pregătim și trimitem datele
    myData.value = 3;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    #ifdef DEBUG
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      } else {
        Serial.println("Send failed");
      }
    #endif

    delay(3000);
    seteazaCuloareLED(255, 165, 0);  // Portocaliu

    touch1detected = false;  // Resetare flag
  }
}

// Funcție pentru setarea culorii LED-ului
void seteazaCuloareLED(uint8_t rosu, uint8_t verde, uint8_t albastru) {
  leduri.setPixelColor(0, leduri.Color(rosu, verde, albastru));
  leduri.show();
}
