#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <OneButton.h>
#include <Adafruit_GPS.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>

// Button1
#define BUTTON1 12
OneButton ob1 = OneButton(BUTTON1, true, true);

// OLED initiation
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

// HW Serial port used
#define GPSSerial Serial2


// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

//DEBUG
uint32_t timer = millis();

//SD Card
String dataMessage;


void oneclick(){
  u8x8.clearDisplay();
  u8x8.drawString(0,1,"Click!");
  u8x8.drawString(0,2,"\x8d \xbb \xab");
}

void doubleclick(){
  Serial.println("double click!");
  u8x8.clearDisplay();
  u8x8.drawString(0,1,"Double click!");
}

void longclick(){
  u8x8.clearDisplay();
  u8x8.drawString(0,1,"Long click!");
}

void displayGPSinfo(String lat, String lon, String speed, String nSat){
  u8x8.clearDisplay();
  u8x8.setCursor(0, 1);
  u8x8.print(lat);
  u8x8.setCursor(0, 2);
  u8x8.print(lon);
  u8x8.setCursor(0, 3);
  u8x8.print(speed);
  u8x8.setCursor(0, 4);
  u8x8.print(nSat);
   
}


void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}


void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}


void setupSDcardReader(){
  if(!SD.begin()){
    Serial.println("Card Mount Fail");
    return;
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
}


void setup(void) {
  Serial.begin(115200);

  //Set I2C (SDA, SCL) Comunication speed to it's minimum (10khz) for the OLED screen.
  //Wire.setClock(10000);

  // Button1 calls
  ob1.attachClick(oneclick);
  ob1.attachDoubleClick(doubleclick);
  ob1.attachLongPressStart(longclick);

  // OLED screen parameters
  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);

  //GPS parameters
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  
  // 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  //Comentar pinaje de cada cosa
  //setupBtn1();
  //setupOLED();
  //setupGPS();
  setupSDcardReader();
  File file = SD.open("/test.txt");
  
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/test.txt", "Hello, how are you?? \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

  delay(1000);

}



void loop(void) {
  
  // Button1 'listener'
  ob1.tick();
  
  // GPS
  char c = GPS.read();

   if (GPS.newNMEAreceived()) {
    //Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');



    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);

    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    
    if (GPS.fix) {  
      Serial.print("Location: ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", ");
      Serial.println(GPS.longitudeDegrees, 4);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);      
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);

      displayGPSinfo(String(GPS.latitudeDegrees, 4), String(GPS.longitudeDegrees, 4), String(GPS.speed), String(GPS.satellites));

      dataMessage = String(GPS.latitudeDegrees, 4) + ", " + String(GPS.longitudeDegrees, 4) + ", " + String(GPS.speed) + ", " + String(GPS.satellites) + "\r\n";

      appendFile(SD, "/test.txt", dataMessage.c_str());

    }
  }



}