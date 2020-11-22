#include <Wire.h>
#include <Adafruit_MLX90614.h> 

void setup() {
  Serial.begin(9600);
  Adafruit_MLX90614().begin();
}

void loop() {
  Serial.println("Температура:");
  /*Serial.print("Окружающая среда:      "); // Окружающая среда
  Serial.print(Adafruit_MLX90614().readAmbientTempC());
  Serial.println(" °C");*/
  Serial.print("Бесконтактное измерение температуры: "); // Бесконтактное измерение температуры объекта
  Serial.print(Adafruit_MLX90614().readObjectTempC());
  Serial.println(" °C");
  Serial.println();
  delay(1000);
}
