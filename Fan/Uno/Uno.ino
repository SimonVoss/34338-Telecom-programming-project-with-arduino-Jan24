#include <SoftwareSerial.h>

#define Fan_Pin 3

int Fan_Speed, Fan_PWM = 0;

SoftwareSerial arduinoSerial(0, 1); // RX, TX (choose pins that are available)

void setup() {
  // Start the Arduino serial communication at 9600 baud rate
  arduinoSerial.begin(115200);
}

void loop() {
  if (arduinoSerial.available()) { // Check if data is available to read
    Fan_Speed = arduinoSerial.parseInt(); // Read the incoming integer
    // Add your code here to do something with Fan_Speed
  }
  
  Fan_PWM = map(Fan_Speed, 0, 100, 0, 255);  //From percentage % to 8 bit 0 - 255

  analogWrite(Fan_Pin, Fan_PWM);  //Writing out to the MOSFET gate
}
