#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <esp_now.h>

#define BUTON_PIN 2   // Pin GPIO pentru buton (ESP32-C3)
#define RELEU_PIN  3  // Pin GPIO pentru releu
#define LED_PIN    4  // Pin GPIO pentru LED-ul WS2812B
#define NUMAR_LEDS 1  // Numărul de LED-uri în strip
#define DEBOUNCE_MS 50  // Timp de debounce în milisecunde

volatile bool stareReleu = true;  // Inițial, releul este pornit
volatile bool stareModificata = false;  // Flag pentru schimbare stare
volatile unsigned long ultimulTimpApasare = 0;  // Ultimul timp de apăsare

Adafruit_NeoPixel leduri(NUMAR_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Funcție ISR pentru buton: resetează la verde și pornește releul
void IRAM_ATTR resetareStare() {
    unsigned long timpCurent = millis();
    if (timpCurent - ultimulTimpApasare > DEBOUNCE_MS) {  // Aplică debounce
        stareReleu = true;  // Releul PORNIT
        stareModificata = true;
        ultimulTimpApasare = timpCurent;
    }
}

// Callback ESP-NOW corect pentru ESP-IDF v5.0+
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    int valoarePrimita;
    memcpy(&valoarePrimita, incomingData, sizeof(valoarePrimita));

    if (valoarePrimita == 3) {  // Dacă primim un 3, oprim releul și punem LED-ul roșu
        stareReleu = false;  // Releul OPRIT
        stareModificata = true;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Pornire sistem...");

    pinMode(RELEU_PIN, OUTPUT);
    digitalWrite(RELEU_PIN, HIGH);  // Inițial, releul este PORNIT

    leduri.begin();
    leduri.clear();
    leduri.setBrightness(60);
    leduri.show();
    seteazaCuloareLED(0, 255, 0);  // Verde la start

    // Atașare întrerupere la buton (detectează apăsarea - rising edge)
    pinMode(BUTON_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTON_PIN), resetareStare, RISING);

    // Inițializare ESP-NOW
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Eroare la inițializarea ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);  // Înregistrează callback-ul
}

void loop() {
    if (stareModificata) {  // Dacă am primit un mesaj sau s-a apăsat butonul
        stareModificata = false;  // Resetare flag

        if (stareReleu) {
            Serial.println("R");
            pinMode(RELEU_PIN, OUTPUT);
            seteazaCuloareLED(0, 255, 0);  // Verde
        } else {
            Serial.println("S");
            pinMode(RELEU_PIN, INPUT);
            seteazaCuloareLED(255, 0, 0);  // Roșu
        }
    }
}

// Funcție pentru setarea culorii LED-ului
void seteazaCuloareLED(uint8_t rosu, uint8_t verde, uint8_t albastru) {
    leduri.setPixelColor(0, leduri.Color(rosu, verde, albastru));
    leduri.show();
}
