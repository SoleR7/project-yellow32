#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <OneButton.h>
#include <Adafruit_GPS.h>

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


void setup(void) {
  Serial.begin(115200);

  //Set I2C (SDA, SCL) Comunication speed to it's minimum (10khz) for the OLED screen.
  Wire.setClock(10000);

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

  delay(1000);

}

void loop(void) {
  
  // Button1 'listener'
  ob1.tick();

  /*
  u8x8.drawString(0,1,"0-1");
  u8x8.drawString(9,1,"9-1");
  u8x8.drawString(12,1,"12-1");
  */
  
  // GPS
  char c = GPS.read();

   if (GPS.newNMEAreceived()) {
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
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
    Serial.print(" Quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
  }



}
