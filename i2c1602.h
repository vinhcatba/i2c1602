#ifndef i2c16x2_h
#define i2c16x2_h
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Define some device constants
// Control bits: There are 4 control bits
// From right to left: Register Select (RS) bit, Read/Write (RW) bit, Enable (EN) bit, Backlight ON/OFF bit.
#define LCD_CHR         1			// RS = 1 = Character.
#define LCD_CMD         0			// RS = 0 = Command.
#define LCD_RW          0b0010		// RW = 0 = Write; RW 1 = Read. However we should NOT use Read mode.
#define LCD_EN          0b0100		// Enable bit. Must be pulse HIGH for at least 450ns after sending 4 bits.
#define LCD_BACKLIGHT   0b1000		// Backlight control. 0 = OFF, 1 = ON.

// NOTE: SHOULD NOT USE READ MODE. 
// LCD operate in 5V, but Raspberry Pi can only handle 3.3v. 
// Reading from LCD can create 5V input to Pi -> damage! (not tested) 
// To use Read mode (LCD_RW), use a bidirectional level shifter circuit.
// However, this library does not use LCD_RW, (write-only library), 
// so you should be fine if your code too doesn't use LCD_RW.

// Address of first character in 1st and 2nd line for 16x2 LCD.
#define LINE1 0x80		  // 1st line
#define LINE2 0xC0		  // 2nd line

// function propotypes
void lcd_setAddr(int fd);
void lcd_init(int addr);

void lcd_byte(int bits, int mode);		  // send a byte of data
void lcd_toggle_enable(int bits);		  // toggle Enable bit
void typeInt(int i);					  // print integer
void typeFloat(float myFloat);			  // print float
void typeString(const char *s);			  // print string
void typeChar(char val);				  // print a char
void lcdLoc(int line);					  // move cursor
void ClrLcd(void);						  // clr LCD return home

typedef struct I2C16x2 {
	int fd;
	int addr;
} I2C16x2;

I2C16x2 lcd;

#endif