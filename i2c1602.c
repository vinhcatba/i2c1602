/* 
// Author: Le Thanh Vinh
// Contact: thanhvinhkma@gmail.com
*/

#include "i2c16x2.h"

// set up i2c device
void lcd_setAddr(int addr) {
	lcd.addr = addr;
	lcd.fd = wiringPiI2CSetup(addr);
}

void lcd_byte(int bits, int mode) {
	// Send byte to I2C
	// bits = the data
	// mode = 1 for character, 0 for command

	// Data is 1 byte,
	// left-most 4 bits are actual data nibble,
	// right-most 4 bits are control bits.
	// So, to send 1 byte to LCD, we must write 6 times to I2C:
	// data's high bits (left-most 4 bits of data) + LCD_EN HIGHT + LCD_EN LOW
	// data's low bits (right-most 4 bits of data) + LCD_EN HIGHT + LCD_EN LOW

	int bits_high;
	int bits_low;
	// uses the two half byte writes to LCD
	bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
	bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

	// High bits
	wiringPiI2CWrite(lcd.fd, bits_high);
	lcd_toggle_enable(bits_high);

	// Low bits
	wiringPiI2CWrite(lcd.fd, bits_low);
	lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits) {
	// Toggle enable pin on LCD display
	wiringPiI2CWrite(lcd.fd, (bits | LCD_EN));		   // EN HIGH
	delayMicroseconds(500);							   // HIGH for 500us
	wiringPiI2CWrite(lcd.fd, (bits & ~LCD_EN));		   // EN LOW
	delayMicroseconds(500);							   // Data needs >37us to settle, use 500us to make sure
}

/*  The LCD always use 8-bit mode when start up but I2C converter module uses 4-bit mode only.
    So we must somehow initialize the LCD using 4-bit mode. According to HD44780 Datasheet p.46:
        1. Power ON, wait >15ms
        2. RS=0, RW=0, Send 0011, wait >4.1ms
        3. RS=0, RW=0, Send 0011, wait >100us
        4. RS=0, RW=0, Send 0011
        5. RS=0, RW=0, Send 0010
    Then we can send some initial commands, for example:
        6. Function Set
        7. Display ON/OFF control
        8. Entry Mode Set
        9. Display clear
*/
void lcd_init(int addr) {
	// since we use 4-bit mode,
	// we can combine (2), (3) as 0011 0011 = 0x33
	// and combine (4), (5) as 0011 0010 = 0x32
	lcd_setAddr(addr);				// set lcd
	delay(15);						// (1) wait >15ms for LCD to power On
	lcd_byte(0x33, LCD_CMD);		// (2) and (3)
	lcd_byte(0x32, LCD_CMD);		// (4) and (5)

	// initial commands
	lcd_byte(0x28, LCD_CMD);		// 4-bit mode, 2 line, small font size
	lcd_byte(0x0C, LCD_CMD);		// Display ON, Cursor OFF, Cursor blink OFF
	lcd_byte(0x06, LCD_CMD);		// Cursor move direction left to right, no display shift
	lcd_byte(0x01, LCD_CMD);		// Clear display
}

// clear lcd and move cursor to home location 0x80 (LINE 1 ROW 0)
void ClrLcd(void) {
	lcd_byte(0x01, LCD_CMD);
}

// move cursor to location on LCD
// example: to go to column 3 of row 1 calls: lcdLoc(LINE1+3). Column start from 0
void lcdLoc(int line) {
	lcd_byte(line, LCD_CMD);
}

// print char to LCD at current position
void typeChar(char val) {
	lcd_byte(val, LCD_CHR);
}

// print a string of any length to LCD (no text wrap)
void typeString(const char *s) {
	while (*s)
		lcd_byte(*(s++), LCD_CHR);
}

// print a float number, up to 4 digits before decimal point and 2 digits after decimal point
void typeFloat(float myFloat) {
	char buffer[20];
	sprintf(buffer, "%4.2f", myFloat);
	typeString(buffer);
}

// print integer as string
void typeInt(int i) {
	char array1[20];
	sprintf(array1, "%d", i);
	typeString(array1);
}
