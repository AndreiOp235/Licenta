void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  Serial.print("D4: ");
  Serial.println(touchRead(4));
  */

  Serial.print("D15: ");
  Serial.println(touchRead(15));
  Serial.print("D32: ");
  Serial.println(touchRead(32));
  delay(10);
}
