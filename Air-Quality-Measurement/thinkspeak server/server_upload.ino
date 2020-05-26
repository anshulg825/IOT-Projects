#include <SoftwareSerial.h>
#include <stdlib.h>
#include <DHT.h>
 
#define DHTPIN 2        // Setting the SDA pin
#define DHTTYPE DHT11   // DHT11 (AM2302) Sensor type setting
 
// Upload Notification LED Setting (Adunono On Board LED)
int ledPin = 13;
 
// Enter Write API key for your own thingspeak channel
String apiKey = "Thingspeak channel write API Key";
 
SoftwareSerial ser(10, 11); // RX / TX configuration, serial object creation
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  dht.begin();
  // Notification LED output setting
  pinMode(ledPin, OUTPUT);
  // Serial communication speed 9600 baud rate setting    
  Serial.begin(9600);
  // Start the software serial
  ser.begin(9600);
  // ESP8266 Reset
  ser.println("AT+RST");
}
 
void loop() {
  // blink LED on board
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);
 
  // Read DHT11 value
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
 
  // String Conversion
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);
  String strHumi = dtostrf(humi, 4, 1, buf);
  
  Serial.println(strTemp);
  Serial.println(strHumi);
  
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com Access IP
  cmd += "\",80";           // api.thingspeak.com access port, 80
  ser.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // Set String, Data to send by GET method
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr +="&field2=";
  getStr += String(strHumi);
  getStr += "\r\n\r\n";
 
  // Send Data
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);
 
  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
    
  // Thingspeak delay to meet 15 seconds minimum upload interval
  delay(16000);  
}