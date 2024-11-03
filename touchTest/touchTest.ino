int threshold = 100;
bool touch1detected = false;

void gotTouch1() {
  touch1detected = true;
}


void setup() {
  Serial.begin(115200);
  delay(1000);  // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(27, gotTouch1, threshold);
}

void loop() {
  if (touch1detected) {
    touch1detected = false;
    Serial.println("Touch 1 detected");
  }
}