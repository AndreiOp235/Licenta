#include <WiFi.h>

const char* ssid = "test";
const char* password = "faraparola";
const char* serverIP = "192.168.50.155"; // Replace with server ESP32's IP
const int serverPort = 12340;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;
  if (client.connect(serverIP, serverPort)) {
    unsigned long start = millis();
    client.println("PING");
    while (client.available() == 0) {
      if (millis() - start > 2000) { // Timeout of 2 seconds
        Serial.println("No response, timeout");
        client.stop();
        return;
      }
    }
    String response = client.readStringUntil('\n');
    unsigned long end = millis();
    Serial.println("Response: " + response);
    Serial.print("RTT: ");
    Serial.print(end - start);
    Serial.println(" ms");
    client.stop();
  }
  delay(1000); // Wait 1 second before the next ping
}
