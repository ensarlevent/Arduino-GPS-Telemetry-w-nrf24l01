#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include "RF24.h"

TinyGPSPlus gps;

SoftwareSerial ss(4, 3);

//////////////////
RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};

struct package
{
  float latitude;
  float longtitude;
  int sat;
  int distanceKmToLondon;
  unsigned int airspeed;
  unsigned int alti;
  float battery;
};


typedef struct package Package;
Package data;

///////////////////

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  //////////////////
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openWritingPipe( addresses[0]);
  delay(100);
}

void loop() {
  smartDelay(1000);
  //////////////////////////////////////////////////////////////////
  static const double LONDON_LAT = 39.891640, LONDON_LON = 32.617352;
  //////////////////////////////////////////////////////////////////

  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON);

  myRadio.write(&data, sizeof(data));

  data.battery = analogRead(A0);
  data.battery = map(data.battery, 0, 1023, 0, 25);
  data.battery = data.battery;

  data.airspeed = gps.speed.kmph();
  data.distanceKmToLondon = distanceKmToLondon;
  data.sat = gps.satellites.value();
  data.latitude = gps.location.lat();
  data.longtitude = gps.location.lng();
  data.alti = gps.altitude.meters();

  Serial.print("Airspeed...");
  Serial.print(data.airspeed);
  Serial.print("....Distance....");
  Serial.print(data.distanceKmToLondon);
  Serial.print("....Sat....");
  Serial.print(data.sat);
  Serial.print("....Latitude....");
  Serial.print(data.latitude);
  Serial.print("....Longtitude....");
  Serial.print(data.longtitude);
  Serial.print("....Altimeter....");
  Serial.println(data.alti);
}



static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
