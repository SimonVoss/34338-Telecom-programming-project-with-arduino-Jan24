void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  unsigned long time = millis();
  Serial.println(time);
  delay(1000);
  digitalWrite(5,HIGH);
}
