#include <SPI.h>
#include "RF24.h"
#include <openGLCD.h>


RF24 myRadio (7, 8);

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

byte addresses[][6] = {"0"};


typedef struct package Package;
Package data;

void setup() {
  GLCD.Init();
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();

  Serial.begin(9600);
  
  GLCD.SelectFont(CalBlk36);
  GLCD.CursorTo(0, 0);
  GLCD.print("LETA");
  GLCD.SelectFont(Callibri11);
  GLCD.CursorTo(2, 4);
  GLCD.print("uBLOX 7M SERIES");

  GLCD.SelectFont(newbasic3x5);
  GLCD.CursorTo(24, 4);
  GLCD.print("E.LEVENT");
  delay(3000);
  GLCD.ClearScreen();
}

void loop() {
  if ( myRadio.available())
  {
    GLCD.ClearScreen();

    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }

    GLCD.SelectFont(Verdana12_bold);
    GLCD.CursorTo(0, 0); //SAT
    GLCD.print(data.sat);

    GLCD.SelectFont(fixednums15x31);  //TELEMETRY
    GLCD.CursorTo(0, 1);
    GLCD.print(data.distanceKmToLondon);

    GLCD.CursorTo(4, 1); //AIRSPEED
    GLCD.print(data.airspeed);

    GLCD.SelectFont(Verdana12_bold);  //altimeter
    GLCD.CursorTo(10, 0);
    GLCD.print(data.alti);

    GLCD.SelectFont(lcdnums12x16);  //battery
    GLCD.CursorTo(3, 0);
    GLCD.print(data.battery);



    GLCD.SelectFont(System5x7);
    GLCD.CursorTo(0, 3); //SAT
    GLCD.print("TELEMETRY -- AIRSPEED");


    GLCD.SelectFont(newbasic3x5);  //altimeter
    GLCD.CursorTo(23, 2);
    GLCD.print("ALTIMETER");

    //battery
    GLCD.CursorTo(0, 2);
    GLCD.print("SAT");

    GLCD.CursorTo(8, 0);
    GLCD.print("V");


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

    delay(100);


  }
  
}
