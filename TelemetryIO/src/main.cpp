#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

u8g2_uint_t offset;			// current offset for the scrolling text
u8g2_uint_t width;			// pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "UPV ECO-MARATHON   ";	// scroll this text from right to left


void setup(void) {

  u8g2.begin();  
  
  u8g2.setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);		// calculate the pixel width of the text
  
  u8g2.setFontMode(0);		// enable transparent mode, which is faster
}

//test

void loop(void) {
  u8g2_uint_t x;
  
  u8g2.firstPage();
  do {
  
    // draw the scrolling text at current offset
    x = offset;
    u8g2.setFont(u8g2_font_inb30_mr);		// set the target font
    do {								// repeated drawing of the scrolling text...
      u8g2.drawUTF8(x, 30, text);			// draw the scolling text
      x += width;						// add the pixel width of the scrolling text
    } while( x < u8g2.getDisplayWidth() );		// draw again until the complete display is filled
    
    u8g2.setFont(u8g2_font_inb16_mr);		// draw the current pixel width
    u8g2.setCursor(0, 58);
    u8g2.print("123456789");					// this value must be lesser than 128 unless U8G2_16BIT is set
    
  } while ( u8g2.nextPage() );
  
  offset-=1;							// scroll by one pixel
  if ( (u8g2_uint_t)offset < (u8g2_uint_t)-width )	
    offset = 0;							// start over again
    
  delay(10);
}