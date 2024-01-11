#include <SoftwareSerial.h>

SoftwareSerial espSerial(3, 1);  // RX, TX
int Fan_Speed = 100;              // Example fan speed value
void setup() {
  // Start the ESP8266 serial communication at 9600 baud rate
  espSerial.begin(115200);
}

void loop() {
  Fan_Speed = 100;
  espSerial.println(Fan_Speed);  // Send the fan speed to Arduino
  delay(2000);                  // Wait for a second


  Fan_Speed = 50;
  espSerial.println(Fan_Speed);
  delay(2000);  // Wait for a second

}
