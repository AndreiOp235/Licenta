#include <esp_now.h>
#include <WiFi.h>
#include <NeoPixelBus.h>
#define pixelPin 4
#define relay 3
#define pornit OUTPUT
#define oprit INPUT

bool flagEroare=1;
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> strip(1, pixelPin);

void OnDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len) {
//  if(data[0]>40)
  {
    pinMode(relay, pornit);
    RgbColor color =RgbColor(0, 255, 0);
    strip.SetPixelColor(0, color);
    strip.Show(); // Initialize off
    Serial.println(data[0]);
    delay(1000);
    pinMode(relay, oprit);
    color =RgbColor(255, 0, 0);
    strip.SetPixelColor(0, color);
    strip.Show(); // Initialize off
  }

}

void setup() {
  pinMode(relay, oprit);
  strip.Begin();
  RgbColor color =RgbColor(0, 0, 255);
  strip.SetPixelColor(0, color);
  strip.Show(); // Initialize off
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);  // ✅ Works with updated signature
}

void loop() {
  // do nothing
  
}
