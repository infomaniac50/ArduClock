#ifndef config_h
#define config_h

// Pin number for OneWire bus
const unsigned char ONE_WIRE_BUS = 5;

//PWM OUTPUT Pins for RGB LCD
const unsigned char LCD_RED_PIN = 3;
const unsigned char LCD_GREEN_PIN = 9;
const unsigned char LCD_BLUE_PIN = 11;

//LCD Config
const unsigned char LCD_ROWS = 2;
const unsigned char LCD_COLS = 16;
const unsigned char LCD_CHAR_COLS = 5;
const unsigned char LCD_CHAR_ROWS = 8;

#define SERIAL_DEBUG false

#endif