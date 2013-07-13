#ifndef config_h
#define config_h

// Pin number for OneWire bus
const byte ONE_WIRE_BUS = 5;

//PWM OUTPUT Pins for RGB LCD
const byte LCD_RED_PIN = 3;
const byte LCD_GREEN_PIN = 9;
const byte LCD_BLUE_PIN = 11;

//LCD Config
const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;
const byte LCD_CHAR_COLS = 5;
const byte LCD_CHAR_ROWS = 8;

#define SERIAL_DEBUG false

#endif