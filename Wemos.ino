#include <Wire.h>
#include <Adafruit_MLX90614.h> 
#include <Adafruit_BME280.h>                            // Подключаем библиотеку Adafruit_BME280
#include <Adafruit_Sensor.h>                            // Подключаем библиотеку Adafruit_Sensor
#include <ESP8266WiFi.h>    
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SEALEVELPRESSURE_HPA (1013.25)                  // Задаем высоту

Adafruit_BME280 bme;                                    // Установка связи по интерфейсу I2C

String ssid = "NTI&me";
String pass = "";
String url = "/data/";
const char* host = "http://172.31.63.30:8000";

void setup() {
 Serial.begin(115200); /* открываем серийный порт для дебаггинга */
 Adafruit_MLX90614().begin();
 Wire.begin(); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
 bool status;
  WiFi.begin("NTI&me", "");
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.println("Waiting for connection");
  }
  
                                                       
  if (!bme.begin(0x76)) {                               // Проверка инициализации датчика
    Serial.println("Could not find a valid BME280 sensor, check wiring!"); // Печать, об ошибки инициализации.
    while (1);                                          // Зацикливаем
  }       
}
 
void loop() {
 Wire.beginTransmission(8); /* Начинаем передачу на адресе 8 */
 Wire.endTransmission();    /* прекращаем передачу */
 
 Wire.requestFrom(8, 13); /* запрашиваем и считываем данные с 8 и 13 адреса slave устройства */
 while(Wire.available()){
    char c = Wire.read();
  Serial.print(c);
 }
            /*client.println(bme.readTemperature());
            client.println(" *C</span></td></tr>");    
            client.println("<tr><td>Давление</td><td><span class=\"sensor\">");
            client.println(bme.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Точечная температура</td><td><span class=\"sensor\">");
            client.println(Adafruit_MLX90614().readObjectTempC());
            client.println(" *C</span></td></tr>"); 
            client.println("<tr><td>Освещенность</td><td><span class=\"sensor\">");
            client.println(c);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Влажность</td><td><span class=\"sensor\">");
            client.println(bme.readHumidity());
            client.println(" %</span></td></tr>"); 
            client.println("</body></html>");*/

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
    
    JSONencoder["prectot"] = 0;
    JSONencoder["qv2m"] = bme.readHumidity();
    JSONencoder["ps"] = bme.readPressure() / 100.0F;
    JSONencoder["t2m"] = bme.readTemperature();
 
    char JSONmessageBuffer[300];
    JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://172.31.63.30:8000/data/");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
    Serial.println(bme.readHumidity());   //Print HTTP return code
    Serial.println(bme.readPressure() / 100.0F);    //Print request response payload
    Serial.println(bme.readTemperature());    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
  delay(300000);
}
