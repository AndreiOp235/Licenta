void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(1, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(1));
  delay(1000);
}
