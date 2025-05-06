#include <NeoPixelBus.h>

#define pixelPin 4
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod> strip(1, pixelPin);

void setup() {
  strip.Begin();
  RgbColor color =RgbColor(0, 0, 1);
  strip.SetPixelColor(0, color);
  strip.Show(); // Initialize off
}

void loop() {
 
}
