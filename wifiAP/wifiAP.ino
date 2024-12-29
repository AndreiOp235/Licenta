#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "ESP32_AP"; // AP SSID
const char *password = "12345678"; // AP Password

WiFiUDP udp;
const int localUdpPort = 12345; // UDP port
char incomingPacket[255]; // Buffer for incoming data

void setup() {
  Serial.begin(115200);

  // Configure as AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);

  // Start UDP server
  udp.begin(localUdpPort);
  Serial.printf("UDP server started at port %d\n", localUdpPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // Read incoming packet
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0; // Null-terminate string
    }

    Serial.printf("Received packet: %s\n", incomingPacket);

    // Send response back to sender
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((const uint8_t *)"pong", strlen("pong"));

    udp.endPacket();
  }
}
