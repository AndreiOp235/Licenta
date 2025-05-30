#include <esp_now.h>
#include <WiFi.h>
#include <NeoPixelBus.h>

#define pixelPin 13
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> strip(1, pixelPin);
// Replace with the MAC address of your receiver
uint8_t broadcastAddress[] = {0x34, 0xcd, 0xb0, 0xd3, 0x88, 0x8c};
float medie=0;
uint8_t byteToSend,medieInt,pin15,pin32;
RgbColor color1,color2;


void setup() {
  Serial.begin(115200);
  color1 =RgbColor(0, 0, 255);
  color2 =RgbColor(0, 255, 0);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  for(int i=0;i<100;i++)
  {
    medie+=(touchRead(15)/100.0);
    delay(10);
  }
  medieInt=medie;
    
  Serial.print(medie);
  Serial.println(" este valoarea de prag");
  strip.Begin();
  strip.SetPixelColor(0, color1);
  strip.Show(); // Initialize off
}

void loop() {
  pin15= touchRead(15); // example byte
  pin32= touchRead(32); // example byte
  Serial.print("D15: ");
  Serial.println(pin15);
  Serial.print("D32: ");
  Serial.println(pin32);
  if(abs(pin15-medieInt)>10)
    byteToSend=15;
  else if(pin32<4)
    byteToSend=32;
  else
    byteToSend=0;

  if(byteToSend)
  {
    esp_now_send(broadcastAddress, &byteToSend, sizeof(byteToSend));
    strip.SetPixelColor(0, color2);
    strip.Show(); // Initialize off
    delay(1000);
    strip.SetPixelColor(0, color1);
    strip.Show(); // Initialize off
  }
    
  //delay(50);
}
