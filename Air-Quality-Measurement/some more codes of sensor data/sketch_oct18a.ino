float Rl=20.0;
float approx=2373.315;
int GasSensorPin=A4; //ATmega328P - 12
float Rs,Vout,count,ratio,GasConc;
int ppm;

void setup()
{
  
  Serial.begin(9600);

}



void loop()
{
  //ppm calibration
  count=analogRead(GasSensorPin);
  Vout=(count*4.88)/1000.0;
  Rs=((5.0*Rl)-(Rl*Vout))/Vout;
  ratio=Rs/approx;
  GasConc=146.15*(2.868-ratio)+10;
  ppm=GasConc;
  Serial.println(ppm);

}
