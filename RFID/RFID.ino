#include <SPI.h>
#include <MFRC522.h>
#include "Servo.h"

// Create a new servo object:
Servo myservo;
// Define the servo pin:
#define servoPin 0

#define RST_PIN         4          // Configurable, see typical pin layout above
#define SS_PIN          15         // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

String previousId = "";
int idMatch = 0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  myservo.attach(servoPin);
  myservo.write(0);

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}
 
void loop() {
  String idWhitelist[10] = {"3063ed22","3063ed22","3063ed22","3063ed22","3063ed22","3063ed22","3063ed22","3063ed22","3063ed22","3063ed22"};

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //Serial.println(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
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

  bool isTrue = checkId(id,idWhitelist);

  doorLock(isTrue,idMatch);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

//--------------------------------------------------------------------------------------

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
  } return userid;
}

bool checkId(String id, String *whitelist){
  bool isTrue = false;
  for (int i = 0; i < 10 ; i++){
    if (id.equals(whitelist[i])){
      isTrue = true;
      break;
    } else {
      isTrue = false;
    }
  }
  if (isTrue == true){
    Serial.println("Access Granted!");
  } else{
    Serial.println("Access Denied!");
  }
  return isTrue;
}

void doorLock(bool isTrue, int idMatch){
  Serial.println(idMatch);
  if(isTrue == true ){
    if(idMatch % 2 == 1){
      myservo.write(0);
    } else{
      myservo.write(180);
    }
  } else{
    myservo.write(0);
  }
}
