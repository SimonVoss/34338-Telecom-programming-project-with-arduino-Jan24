#include <Bonezegei_DHT11.h>  // Download: Bonezegei_DHT11 by - Bonezegei (JofelBatutay)
#include <SPI.h>
#include <MFRC522.h>  // Download: MFRC522 by - GithubCommunity
#include "Servo.h"    // Download: Servo by - Michael Margolis, Arduino
#include <SoftwareSerial.h>



//GPIO pins used
#define servoPin 0
#define RST_PIN 4  // Configurable, see typical pin layout above
#define SS_PIN 15  // Configurable, see typical pin layout above
#define TempHum_Pin 2


//Fan
int Fan_PWM = 0;       //Fan speed between 0 and 255
int Fan_Speed = 0;     //Fan speed percentage %
float Desired = 25.5;  //Temp variable
SoftwareSerial espSerial(3, 1);  // RX, TX


// Variables - Calls

//Temp humidity sensor
float Celcius, Humidity = 0;
Bonezegei_DHT11 dht(TempHum_Pin);
void setup() {
  //Initialice Serial comunication
  Serial.begin(115200);
 // Start the ESP8266 serial communication at 9600 baud rate
  espSerial.begin(115200);

  //Temp humidity sensor
  pinMode(TempHum_Pin, INPUT);
  dht.begin();  //Libary initialicer

  //RFID


  //Web Server

  //Light sensor


  //Door - actuator
}

void loop() {
  ClimateControl(Desired);
  //SerialPrint();  //Print to maintenance

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

  espSerial.println(Fan_Speed);  // Send the fan speed to Arduino
}

void ClimateControl(float Desired) {
  TempHumModule();  //This function should be called every loop

  if (Celcius - Desired > 5) {                                   //Temp diference in celcius
    Fan_Speed = 100;                                             //Input in percentage
  } else if (Celcius - Desired > 3) {                            //Temp diference in celcius
    Fan_Speed = 80;                                              //Input in percentage
  } else if (Celcius - Desired > 2) {                            //Temp diference in celcius
    Fan_Speed = 60;                                              //Input in percentage
  } else if (1 < Celcius - Desired && Celcius - Desired <= 2) {  //Temp diference in celcius
    Fan_Speed = 40;                                              //Input in percentage
  } else if (1 <= Celcius - Desired) {                           //Temp diference in celcius
    Fan_Speed = 0;                                               //Input in percentage
  }

  FanControl();  //Function that sends out a PWM signal to the MOSFET
}


void SerialPrint() {
  Serial.print("Fan strength is at: ");
  Serial.print(Fan_Speed);
  Serial.print(", Celcius: ");
  Serial.print(Celcius);
  Serial.print(" Humidity: ");
  Serial.println(Humidity);
}