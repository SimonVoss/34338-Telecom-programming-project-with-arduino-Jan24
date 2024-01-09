int R_old = 0;
int G_old = 0;
int B_old = 0;
static int RLED = 5;
static int GLED = 4;
static int BLED =2;
static int Relay =11; 
void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(RLED,OUTPUT);
  pinMode(GLED,OUTPUT);
  pinMode(BLED,OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  int x=lightSensor();
  setLight(0,x*255/100+1,0);

  
  
  
}
float lightSensor() {
  int light_digital = analogRead(A0);
  float light = map(light_digital, 0, 1023, 0, 100);
  return light;
}
int setLight(int R_new, int G_new, int B_new) {
  while ((R_old != R_new) || (G_old != G_new) || (B_old != B_new)) {
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
    analogWrite(RLED,R_old);
    analogWrite(GLED,G_old);
    analogWrite(BLED,B_old);
    delay(20); // A delay is added to make the transition slower
    
  }
  return 0;
}
