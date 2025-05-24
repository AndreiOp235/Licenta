#include <esp_now.h>
#include <WiFi.h>
#include <NeoPixelBus.h>
#include <WiFiClient.h>

#define pixelPin 4
#define relay 3
#define pornit OUTPUT
#define oprit INPUT

const char* ssid = "test";
const char* password = "faraparola";

bool flagEroare=1;
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> strip(1, pixelPin);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi connection failed");
  }
}

void OnDataRecv(const esp_now_recv_info_t *recvInfo, const uint8_t *data, int len) {
//  if(data[0]>40)
  {
    pinMode(relay, pornit);
    RgbColor color =RgbColor(0, 1, 0);
    strip.SetPixelColor(0, color);
    strip.Show(); // Initialize off
    //connect to socket 192.168.50.192:6000 and send the addres of the node that sent the message, then close the socket

    /**/
    // Send MAC address to server
    connectToWiFi();
    WiFiClient client;
    if (client.connect("192.168.50.192", 6000)) {
      char macStr[18];
      snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
              recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
              recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);

      client.println(macStr);  // Send MAC
      client.stop();           // Close connection
    } else {
      Serial.println("Connection to server failed");
    }
    /**/

    delay(1000);
    pinMode(relay, oprit);
    color =RgbColor(1, 0, 0);
    strip.SetPixelColor(0, color);
    strip.Show(); // Initialize off
  }

}

void setup() {
  pinMode(relay, oprit);
  strip.Begin();
  RgbColor color =RgbColor(0, 0, 1);
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
