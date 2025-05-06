/*
This is an example how to use Touch Intrrerupts
The bigger the threshold, the more sensible is the touch
*/
int val;


void setup() {
  Serial.begin(115200);
  delay(1000);  // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Interrupt Test");

  
}

void loop() {
  val=touchRead(T0);
  if(val>40)
  {
    Serial.println(val);
  }
}
