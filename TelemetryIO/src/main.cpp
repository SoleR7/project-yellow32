#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <OneButton.h>

// Button1
#define BUTTON1 12
OneButton ob1 = OneButton(BUTTON1, true, true);

// OLED initiation
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);


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

  ob1.attachClick(oneclick);
  ob1.attachDoubleClick(doubleclick);
  ob1.attachLongPressStart(longclick);

  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
}

void loop(void) {
  
  /*
  u8x8.drawString(0,1,"0-1");
  u8x8.drawString(0,2,"0-2");
  u8x8.drawString(0,3,"0-3");
  u8x8.drawString(0,4,"0-4");
  u8x8.drawString(0,5,"0-5");
  u8x8.drawString(0,6,"0-6");
  u8x8.drawString(0,7,"0-7");
  u8x8.drawString(3,1,"3-1");
  u8x8.drawString(6,1,"3-1");
  u8x8.drawString(9,1,"9-1");
  u8x8.drawString(12,1,"12-1");
  */
  

  ob1.tick();

}

//testtest