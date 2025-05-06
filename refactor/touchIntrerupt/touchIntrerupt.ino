bool touchD=false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  // put your setup code here, to run once:
  touchInterruptSetThresholdDirection(true);
  touchAttachInterrupt(14, functie, 80);
}

void functie()
{
  touchD=true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(touchD==true)
    {
      Serial.println("S-a intamplat minunea");
      touchD=false;
      touchDetachInterrupt(12);
    }
}
