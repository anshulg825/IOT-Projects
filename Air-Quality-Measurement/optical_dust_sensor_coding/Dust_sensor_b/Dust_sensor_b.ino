#include <dht.h>

dht DHT;

float Rl=20.0;
float approx=2373.315;
int GasSensorPin=A4; //ATmega328P - 12
float Rs,Vout,count,ratio,GasConc;
int ppm;

#define DHT11_PIN 2

int measurePin = A5;
int ledPower = 7;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
}

void loop(){

  count=analogRead(GasSensorPin);
  Vout=(count*4.88)/1000.0;
  Rs=((5.0*Rl)-(Rl*Vout))/Vout;
  ratio=Rs/approx;
  GasConc=146.15*(2.868-ratio)+10;
  ppm=GasConc;
  Serial.println("ppm=");
  Serial.println(ppm);
  
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  
  digitalWrite(ledPower,LOW);
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin);

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured*(5.0/1024);
  dustDensity = 0.17*calcVoltage-0.1;

  if ( dustDensity < 0)
  {
    dustDensity = 0.00;
  }

  Serial.println("Raw Signal Value (0-1023):");
  Serial.println(voMeasured);

  Serial.println("Voltage:");
  Serial.println(calcVoltage);

  Serial.println("Dust Density:");
  Serial.println(dustDensity);

  delay(1500);
}
