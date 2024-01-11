#include <SoftwareSerial.h>

SoftwareSerial espSerial(3, 1);  // RX, TX
int fanSpeed = 100;              // Example fan speed value
void setup() {
  // Start the ESP8266 serial communication at 9600 baud rate
  espSerial.begin(115200);
}

void loop() {

  espSerial.println(fanSpeed);  // Send the fan speed to Arduino
  delay(2000);                  // Wait for a second


  fanSpeed = 50;
  espSerial.println(fanSpeed);
  delay(2000);  // Wait for a second

}
