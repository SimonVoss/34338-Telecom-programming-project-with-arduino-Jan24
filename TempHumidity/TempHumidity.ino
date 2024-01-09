#include <Bonezegei_DHT11.h>

const int TempHum_Pin = 2;

int previousMillis = 0;
float Celcius, Humidity = 0;

//param = DHT11 signal pin
Bonezegei_DHT11 dht(TempHum_Pin);


void setup() {
  Serial.begin(115200);

  pinMode(TempHum_Pin, INPUT);

  dht.begin();
}

void loop() {

  TempHumModule();
}

void TempHumModule() {
  unsigned long currentMillis = millis();

  // Check if the specified interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    if (dht.getData()) {               // get All data from DHT11
      Celcius = dht.getTemperature();  // return temperature in celsius
      Humidity = dht.getHumidity();    // return humidity

      //float tempFar = dht.getTemperature(true);  // return temperature in fahrenheit if true celsius if false
    }
    // Update the previousMillis for the next iteration
    previousMillis = currentMillis;

    Serial.print("Celcius: ");
    Serial.print(Celcius);
    Serial.print(" Humidity: ");
    Serial.println(Humidity);
  }
}
