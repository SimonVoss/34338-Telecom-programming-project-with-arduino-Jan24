/*
This code is wirtten by:
s214405 - Simon Voss
s205937 Sigurd Bernstein
s
s
s
s
For the cource 34338 Telecommunication programming projects with Arduino Jan 24
Date 09 Jan 2024
*/

//The code is spesific for the ESP8266 Node 0.9 MCU , pin numbers and other inputs functions might have to be changed for other microcontrollers

//Liabrys
#include <Bonezegei_DHT11.h> // Download: Bonezegei_DHT11 by Bonezegei (JofelBatutay)

//GPIO pins used
const int TempHum_Pin = 2;


// Variables - Calls

//Temp humidity sensor
int previousMillis = 0;
float Celcius, Humidity = 0;
Bonezegei_DHT11 dht(TempHum_Pin);

//RFID

//Light sensor

//Fan 

//Door - actuator

//--------------------------------------------------------------------

void setup() {
  //Initialice Serial comunication
  Serial.begin(115200);


//Temp humidity sensor
pinMode(TempHum_Pin, INPUT);
dht.begin();

//RFID

//Light sensor

//Fan 

//Door - actuator
  
  
}

void loop() {

  TempHumModule(); //This function should be called every loop
  
}

void TempHumModule() {
  unsigned long currentMillis = millis();

  // Check if the specified interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    if (dht.getData()) {               // get All data from DHT11
      Celcius = dht.getTemperature();  // return temperature in celsius
      Humidity = dht.getHumidity();    // return humidity

    }
    // Update the previousMillis for the next iteration
    previousMillis = currentMillis;
  }
}
