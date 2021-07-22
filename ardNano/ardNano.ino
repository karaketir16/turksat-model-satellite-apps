#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

#include "Adafruit_BMP3XX.h"


#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

typedef struct __attribute__((packed)) nano_package {

    uint8_t startDelimiter = 0x7E;
  
    float gps_longtitude;
    float gps_altitude;
    float gps_latitude;

    uint8_t gps_fix;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t day;
    uint8_t month;
    uint16_t year;

    float pressure;
    float temp;

    float voltage;

    uint16_t checksum;
} nano_package;

// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// Connect the GPS TX (transmit) pin to Digital 8
// Connect the GPS RX (receive) pin to Digital 7


// you can change the pin numbers to match your wiring:
SoftwareSerial mySerialA(8, 7);
Adafruit_GPS GPS(&mySerialA);

auto &mySerial = Serial;

#define GPSECHO  false

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

// BME 280 connections
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
//..........................

#define SEALEVELPRESSURE_HPA (1004.63)

//Adafruit_BME280 bme(BME_CS); // hardware SPI/


Adafruit_BMP3XX bmp;


#define PMTK_SET_BAUD_38400 "$PMTK251,38400*27"

#include <stdint.h>


void calcChecksum(nano_package* np){
    np->checksum = 0;
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint16_t tot=0;
    for(int i =0; i < len - 2; i++){
        tot += data[i];
    }
    np->checksum = tot;
}

uint8_t ckeckChecksum(nano_package* np){
    int len = sizeof (nano_package);
    uint8_t * data = (uint8_t *) np;

    uint16_t tot=0;
    for(int i =0; i < len - 2; i++){
        tot += data[i];
    }
    return tot == np-> checksum;
}


void setup()
{

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  mySerial.begin(115200);
//  delay(5000);
//  mySerial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_BAUD_38400); 

  mySerialA.end();

  GPS.begin(38400);
  


  
  //GPS.begin(115200);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz




  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_NOANTENNA);

//  delay(1000);
  // Ask for firmware version
//  mySerial.println(PMTK_Q_RELEASE);
  

//  unsigned status = bme.begin(); /
    unsigned status = bmp.begin_SPI(BME_CS);


}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  
 /* Get a new sensor event */ 
// mySerial.println("ne?");
 
  char c = GPS.read();

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  
  if (millis() - timer > 100) {
        timer = millis(); // reset the timer
        uint8_t hour = GPS.hour+3;
      
        nano_package toSent;

        int sensorValue = analogRead(A0);
        float v1 = (sensorValue)*5.0/1023.0;
        float voltage = (3.025)*v1;

        bmp.performReading();


        toSent.gps_longtitude = GPS.longitude; 
        toSent.gps_latitude = GPS.latitude;
        toSent.gps_altitude  = GPS.altitude;
        toSent.gps_fix  = GPS.fix;
        toSent.hour = hour;
        toSent.minute = GPS.minute;
        toSent.second = GPS.seconds;
        toSent.day = GPS.day;
        toSent.month = GPS.month;
        toSent.year = GPS.year;
//        toSent.pressure = bme.readPressure() / 100.0F;
//        toSent.temp = bme.readTemperature();
//
       toSent.pressure = bmp.pressure / 100.0F;
       toSent.temp = bmp.temperature;
        
        toSent.voltage = voltage;
        
        calcChecksum(&toSent);
        
        mySerial.write((uint8_t*) &toSent, sizeof(nano_package));
      }
}   
