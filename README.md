# i2c1602
A Raspberry Pi C library to write LCD 16x2 with I2C using WiringPi

This library is based mainly on [Lewis Loflin's code](https://www.bristolwatch.com/rpi/i2clcd.htm). It's use wiringPi I2C to interface with an I2C 16x2 LCD, which has a I2C converter module attached in the back.

# To clone and install this library:
```
$ wget https://github.com/vinhcatba/i2c1602/archive/refs/heads/main.zip
$ unzip main.zip
$ cd i2c1602-main
$ make
$ sudo make install
```
To use:
```
include <i2c1602.h>
I2C16x2 lcd;
```
compile your program with `-lwiringPi -li2c1602` flags.

for more details see `example.c` and source + header file. 
# Usage
## void lcd_init(int addr); 
initialize LCD at addr I2C address. Address can be found by typing `i2cdetect -y 1` to terminal.
this function will set data length to 4-bit, 2 line, small font size, display ON, cursor OFF, cursor blink OFF, direction Left to Right and clear the display screen + RAM.
## void lcd_setAddr(int fd); 
this function set/change LCD I2C address, without init.

address stores in `(int)addr` inside `struct I2C16x2`

## void lcd_byte(int bits, int mode);		  
send a byte of data, using 4-bit lenght (send 2 nibble). Mode can be LCD_CMD for command or LCD_CHR for character.

## void lcd_toggle_enable(int bits);		 
toggle Enable bit after sending a nibble (4 bits)

## void typeInt(int i);					  
print integer as a string to LCD at current cursor position

## void typeFloat(float myFloat);			  
print float with 2 digits after decimal point as a string to LCD at current cursor position

## void typeString(const char \*s);			  
print string of any length to lcd (no text wrapping)

## void typeChar(char val);				  
print a char to current cursor position.

## void lcdLoc(int line);					  
move cursor to position line. Position is LINE1/LINE2 + offset. Example: `lcdLoc(LINE1)` to move cursor to LINE 1 ROW 0 LINE 1; `lcdLoc(LINE2+7)` move to LINE 2 ROW 7 

## void ClrLcd(void);						  
clear LCD display + RAM and return cursor to home (LINE 1 ROW 0 or 0x80)
