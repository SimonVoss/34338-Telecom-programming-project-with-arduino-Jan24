const int Fan_PIN = 5;  //Input at D1

int Fan_PWM = 0;    //Fan speed between 0 and 255
int Fan_Speed = 0;  //Fan speed percentage %

void setup() {

  Serial.begin(115200);

  pinMode(TempHum_Pin, OUTPUT);  //Pin Setup
}

void loop() {

  FanControl();  //Function that sends out a PWM signal to the MOSFET
}

void FanControl() {

  Fan_PWM = map(Fan_Speed, 0, 100, 0, 255); //Converting a percentage to 8bit for PWM signal to Fan

  analogWrite(Fan_PIN, Fan_PWM);

  Serial.print("Fan strength is at: ");
  Serial.print(Fan_Speed);

}
}
