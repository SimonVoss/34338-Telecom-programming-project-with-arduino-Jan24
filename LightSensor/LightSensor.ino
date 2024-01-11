int R_old = 0;
int G_old = 0;
int B_old = 0;
static int RLED = 10;
static int GLED = 3;
static int BLED = 16;
struct ID {
  int Temp;       //Temperature in celcius
  int Light[3];   //How the light is to be
  char id[8];     //ID of the card
  char Name[10];  // Name of the card holder
};
ID Thomas;
ID DEF;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  Thomas.Light[0] = 0;
  Thomas.Light[1] = 255;
  Thomas.Light[2] = 255;
  DEF.Light[0] = 0;
  DEF.Light[1] = 0;
  DEF.Light[2] = 0;
}


void loop() {
  // put your main code here, to run repeatedly:
  
}
void Lights(struct ID name) {
  if (lightSensor() < 4) {
    setLight(name.Light[0], name.Light[1], name.Light[2]);
  }
}

float lightSensor() {
  int light_digital = analogRead(A0);
  float light = map(light_digital, 0, 1023, 0, 100);
  return light;
}
void setLight(int R_new, int G_new, int B_new) {
  long previousMillis = 0;


  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > 2) {


    previousMillis = currentMillis;
    if ((R_old != R_new) || (G_old != G_new) || (B_old != B_new)) {
      // here the while loop is run untill the old values matches the current
      // if statements are run to check each value to make sure it matches if not then 1 is either added or subtracted
      if (R_old > R_new) {
        R_old -= 1;
      } else if (R_old < R_new) {
        R_old += 1;
      }
      if (G_old > G_new) {
        G_old -= 1;
      } else if (G_old < G_new) {
        G_old += 1;
      }
      if (B_old > B_new) {
        B_old -= 1;
      } else if (B_old < B_new) {
        B_old += 1;
      }
      // lastly we write to the colors
      analogWrite(RLED, R_old);
      analogWrite(GLED, G_old);
      analogWrite(BLED, B_old);
    }
  }
}
