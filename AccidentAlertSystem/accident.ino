#include<SoftwareSerial.h>
#include<TinyGPS.h>
int pos = 0;
TinyGPS gps;
SoftwareSerial gpsSerial(10, 11);
static void print_float(float val, float invalid, int len, int prec);
static void smartdelay(unsigned long ms);
SoftwareSerial gsmSerial(2, 3);

#define x A1
#define y A2
#define z A3

int xsample=0,a=9;
int ysample=0;
int zsample=0;

#define samples 10
#define minVal -50
#define MaxVal 50


void setup()
{
  gsmSerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);
  gpsSerial.begin(9600);

  accelero();
 // Setting the baud rate of Serial Monitor (Arduino)
  delay(1900);
}

static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
    Serial.println(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;   
}

void accelero() {
  for(int i=0;i<samples;i++)
  {
    xsample+=analogRead(x);
    ysample+=analogRead(y);
    zsample+=analogRead(z);
  }

  xsample/=samples;
  ysample/=samples;
  zsample/=samples;

  Serial.println("Initial position of Car:");
  Serial.println(xsample);
  Serial.println(ysample);
  Serial.println(zsample);
  Serial.println("\n");
}


void loop()
{
  int value1=analogRead(x);
  int value2=analogRead(y);
  int value3=analogRead(z);

  int xValue=xsample-value1;
  int yValue=ysample-value2;
  int zValue=zsample-value3;
  float flat, flon;
  unsigned long date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  
  gps.f_get_position(&flat, &flon);

  if(yValue < minVal || yValue > MaxVal) {
  gps.stats(&chars, &sentences, &failed);
  
  Serial.println("ALERT! ACCIDENT LOCATION: ");
  Serial.print("Latitude: ");
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  
  Serial.print("Longitude: ");
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  smartdelay(1000);
  Serial.println("\n");
  
  int a=0;
  if(Serial.available()>0) {   
   switch(Serial.read()) {
    case 's':
      gsmSerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      gsmSerial.println("AT+CMGS=\"+917013025763\"\r"); // Replace x with mobile number
      delay(2500);

      gsmSerial.print("EMERGENCY:\n");
      gsmSerial.print("Latitude: ");gsmSerial.print(gsmSerial.print(flat));gsmSerial.print(" and ");
      gsmSerial.print("Longitude: ");gsmSerial.println(gsmSerial.println(flon));
      
      gsmSerial.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
      gsmSerial.print(flat,6);
      gsmSerial.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
      gsmSerial.print(flon,6);
      delay(3000);
      gsmSerial.println((char)26);// ASCII code of CTRL+Z
      delay(3000);
   
   case 'r':
     ;
     
  }

 if(gsmSerial.available()>0) {
   Serial.write(gsmSerial.read());
   }
 }
}
}

