#include <Adafruit_NeoPixel.h>

#define BUTON_PIN 2   // Pinul GPIO pentru buton (ESP32-C3)
#define RELEU_PIN  3  // Pinul GPIO pentru releu
#define LED_PIN    4  // Pinul GPIO pentru LED-ul WS2812B
#define NUMAR_LEDS 1  // Numărul de LED-uri în strip
#define DEBOUNCE_MS 50  // Timp de debounce în milisecunde

volatile bool stareReleu = false;  // Variabilă globală pentru stare releu
volatile bool stareModificata = false;  // Flag pentru schimbare stare
volatile unsigned long ultimulTimpApasare = 0;  // Ultimul timp de apăsare

Adafruit_NeoPixel leduri(NUMAR_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Funcție ISR pentru întrerupere (schimbă starea releului și LED-ului)
void IRAM_ATTR schimbareStare() {
    unsigned long timpCurent = millis();
    if (timpCurent - ultimulTimpApasare > DEBOUNCE_MS) {  // Aplică debounce
        stareReleu = !stareReleu;  // Comută starea releului
        stareModificata = true;    // Semnalizează că trebuie să actualizăm LED-ul și releul
        ultimulTimpApasare = timpCurent;  // Actualizează ultimul timp de apăsare
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Pornire control Buton, Releu și LED WS2812B...");

    pinMode(RELEU_PIN, OUTPUT);
    digitalWrite(RELEU_PIN, LOW);  // Inițializare releu OPRIT

    leduri.begin();
    leduri.clear();
    leduri.setBrightness(60);
    leduri.show();

    // Atașare întrerupere la buton (detectează apăsarea - rising edge)
    pinMode(BUTON_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BUTON_PIN), schimbareStare, RISING);
}

void loop() {
    if (stareModificata) {  // Dacă butonul a fost apăsat și starea s-a schimbat
        stareModificata = false;  // Resetare flag

        if (stareReleu) {
            Serial.println("Releu PORNIT, LED Verde");
            pinMode(RELEU_PIN, OUTPUT);
            seteazaCuloareLED(0, 255, 0);  // Verde
        } else {
            Serial.println("Releu OPRIT, LED Roșu");
            pinMode(RELEU_PIN, INPUT);
            seteazaCuloareLED(255, 0, 0);  // Roșu
        }
    }

    // loop() rămâne liber și nu consumă CPU inutil
}

// Funcție pentru setarea culorii LED-ului
void seteazaCuloareLED(uint8_t rosu, uint8_t verde, uint8_t albastru) {
    leduri.setPixelColor(0, leduri.Color(rosu, verde, albastru));
    leduri.show();
}
