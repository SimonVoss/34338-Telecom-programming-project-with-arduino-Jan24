#include <SoftwareSerial.h>

SoftwareSerial arduinoSerial(10, 11); // RX, TX (choose pins that are available)
int Fan_Speed;

void setup() {
  // Start the Arduino serial communication at 9600 baud rate
  arduinoSerial.begin(9600);
}

void loop() {
  if (arduinoSerial.available()) { // Check if data is available to read
    Fan_Speed = arduinoSerial.parseInt(); // Read the incoming integer
    // Add your code here to do something with Fan_Speed
  }
  
  Fan_PWM = map(Fan_Speed, 0, 100, 0, 255);  //From percentage % to 8 bit 0 - 255

  analogWrite(Fan_Pin, Fan_PWM);  //Writing out to the MOSFET gate
}