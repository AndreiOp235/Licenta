#include <esp_now.h>
#include <WiFi.h>

// Replace with the MAC address of your receiver
uint8_t broadcastAddress[] = {0x34, 0xcd, 0xb0, 0xd3, 0x88, 0x8c};
float medie=0;
uint8_t byteToSend,medieInt;


void setup() {
  Serial.begin(115200);

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
    medie+=(touchRead(T0)/100.0);
    delay(10);
  }
  medieInt=medie;
    
  //Serial.print(medie);
  //Serial.println(" este valoarea de prag");
}

void loop() {
  byteToSend = touchRead(T0); // example byte
  Serial.println(byteToSend);
  /*
  if(abs(byteToSend-medieInt)>10)
    esp_now_send(broadcastAddress, &byteToSend, sizeof(byteToSend));
  */
  if(byteToSend<4)
    esp_now_send(broadcastAddress, &byteToSend, sizeof(byteToSend));

  delay(50);
}
