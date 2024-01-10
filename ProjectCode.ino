/*
This code is wirtten by:
s214405 - Simon Voss
s205937 - Sigurd Bernstein
s214412 - Matthias Winther
s214398 - Emil Pedersen
s214426 - Lukas Falk Nyholm
s214419 - Thomas Holst
For the cource 34338 Telecommunication programming projects with Arduino Jan 24
Date 09 Jan 2024
*/

//The code is spesific for the ESP8266 Node 0.9 MCU , pin numbers and other inputs functions might have to be changed for other microcontrollers

//Liabrys
#include <Bonezegei_DHT11.h>  // Download: Bonezegei_DHT11 by Bonezegei (JofelBatutay)

//GPIO pins used
const int TempHum_Pin = 2;
const int Fan_Pin = 5;


// Variables - Calls

//Temp humidity sensor
int previousMillis = 0;
float Celcius, Humidity = 0;
Bonezegei_DHT11 dht(TempHum_Pin);

//RFID

//Web Server

//Light sensor

//Fan
int Fan_PWM = 0;    //Fan speed between 0 and 255
int Fan_Speed = 0;  //Fan speed percentage %
float Desired = 24.5; //Temp variable 
//Door - actuator

//--------------------------------------------------------------------

void setup() {
  //Initialice Serial comunication
  Serial.begin(115200);


  //Temp humidity sensor
  pinMode(TempHum_Pin, INPUT);
  dht.begin();  //Libary initialicer

  //RFID

  //Web Server

  //Light sensor

  //Fan
  pinMode(Fan_Pin, OUTPUT);  //Pin Setup

  //Door - actuator
}

void loop() {
  ClimateControl(Desired);
  SerialPrint(); //Print to maintenance
}

void TempHumModule() {


  // Check if the specified interval has elapsed

  if (dht.getData()) {               // get All data from DHT11
    Celcius = dht.getTemperature();  // return temperature in celsius
    Humidity = dht.getHumidity();    // return humidity
    //float tempFar = dht.getTemperature(true);  // return temperature in fahrenheit if true celsius if false
  }
}

void FanControl() {

  Fan_PWM = map(Fan_Speed, 0, 100, 0, 255);

  analogWrite(Fan_Pin, Fan_PWM);
}

void ClimateControl(float Desired) {
  unsigned long currentMillis = millis();
  //if (currentMillis - previousMillis >= 1001) {
  TempHumModule();  //This function should be called every loop

  if (Celcius - Desired > 5) {                                   //Temp diference in celcius
    Fan_Speed = 100;                                             //Input in percentage
  } else if (Celcius - Desired > 3) {                            //Temp diference in celcius
    Fan_Speed = 80;                                              //Input in percentage
  } else if (Celcius - Desired > 2) {                            //Temp diference in celcius
    Fan_Speed = 60;                                              //Input in percentage
  } else if (1 < Celcius - Desired && Celcius - Desired <= 2) {  //Temp diference in celcius
    Fan_Speed = 50;                                              //Input in percentage
  } else if (1 <= Celcius - Desired) {                           //Temp diference in celcius
    Fan_Speed = 0;                                               //Input in percentage
  }

  FanControl();  //Function that sends out a PWM signal to the MOSFET
  //}
  // Update the previousMillis for the next iteration
  //previousMillis = currentMillis;
}


void SerialPrint() {
  Serial.print("Fan strength is at: ");
  Serial.print(Fan_Speed);
  Serial.print(", Celcius: ");
  Serial.print(Celcius);
  Serial.print(" Humidity: ");
  Serial.println(Humidity);
}
