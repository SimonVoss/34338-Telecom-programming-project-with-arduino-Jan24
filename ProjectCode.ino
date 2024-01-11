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
#include <Bonezegei_DHT11.h>  // Download: Bonezegei_DHT11 by - Bonezegei (JofelBatutay)
#include <SPI.h>
#include <MFRC522.h>  // Download: MFRC522 by - GithubCommunity
#include "Servo.h"    // Download: Servo by - Michael Margolis, Arduino


//GPIO pins used
#define servoPin 0
#define RST_PIN 4  // Configurable, see typical pin layout above
#define SS_PIN 15  // Configurable, see typical pin layout above
#define TempHum_Pin 2
#define Fan_Pin 5




// Variables - Calls

//Temp humidity sensor
float Celcius, Humidity = 0;
Bonezegei_DHT11 dht(TempHum_Pin);


//RFID
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
Servo myservo;
byte nuidPICC[4]; // Init array that will store new NUID
String previousId = "";
int idMatch = 0;


//Web Server


//Light sensor

//Fan
int Fan_PWM = 0;       //Fan speed between 0 and 255
int Fan_Speed = 0;     //Fan speed percentage %
float Desired = 24.5;  //Temp variable
//Door - actuator

//--------------------------------------------------------------------

void setup() {
  //Initialice Serial comunication
  Serial.begin(115200);


  //Temp humidity sensor
  pinMode(TempHum_Pin, INPUT);
  dht.begin();  //Libary initialicer

  //RFID
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  myservo.attach(servoPin);
  myservo.write(0);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //Web Server

  //Light sensor

  //Fan
  pinMode(Fan_Pin, OUTPUT);  //Pin Setup

  //Door - actuator
}

void loop() {
  ClimateControl(Desired);
  SerialPrint();  //Print to maintenance
  RFID();
}

//-----------------------------------------RFID---------------------------------------------------------------------
void RFID() {
  String idWhitelist[10] = { "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22", "3063ed22" };

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial())
    return;

  //Serial.println(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  String id = hex2String(rfid.uid.uidByte, rfid.uid.size);

  if (id.equals(previousId)) {
    idMatch += 1;
  } else {
    previousId = id;
    idMatch = 0;
  }

  Serial.print(F("ID tag: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  bool isTrue = checkId(id, idWhitelist);

  doorLock(isTrue, idMatch);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

String hex2String(byte *buffer, byte bufferSize) {
  String userid;
  for (byte i = 0; i < rfid.uid.size; i++) {
    userid += String(rfid.uid.uidByte[i], HEX);
  }
  return userid;
}

bool checkId(String id, String *whitelist) {
  bool isTrue = false;
  for (int i = 0; i < 10; i++) {
    if (id.equals(whitelist[i])) {
      isTrue = true;
      break;
    } else {
      isTrue = false;
    }
  }
  if (isTrue == true) {
    Serial.println("Access Granted!");
  } else {
    Serial.println("Access Denied!");
  }
  return isTrue;
}

void doorLock(bool isTrue, int idMatch) {
  Serial.println(idMatch);
  if (isTrue == true) {
    if (idMatch % 2 == 1) {
      myservo.write(0);
    } else {
      myservo.write(180);
    }
  } else {
    myservo.write(0);
  }
}

//-----------------------------------------------------------------------------------------------------------------

void TempHumModule() {


  // Check if the specified interval has elapsed

  if (dht.getData()) {               // get All data from DHT11
    Celcius = dht.getTemperature();  // return temperature in celsius
    Humidity = dht.getHumidity();    // return humidity
    //float tempFar = dht.getTemperature(true);  // return temperature in fahrenheit if true celsius if false
  }
}

void FanControl() {

  Fan_PWM = map(Fan_Speed, 0, 100, 0, 255);  //From percentage % to 8 bit 0 - 255

  analogWrite(Fan_Pin, Fan_PWM);  //Writing out to the MOSFET gate
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
