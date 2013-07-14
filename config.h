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

const byte RF_SETTINGS_PIN = A0;
const byte RF_INCREMENT_PIN = A1;
const byte RF_DECREMENT_PIN = A2;

const int POT_RANGE_HIGH = 100;
const int POT_RANGE_LOW = 1;

const byte MAX_ALL = 255;
const byte MIN_ALL = 0;
const byte MAX_RED = 150;
const byte MAX_GREEN = 100;

struct BacklightSettings {
  byte brightness;
  byte red;
  byte green;
  byte blue;
};

struct ClockSettings {
  int button_delay;
  BacklightSettings backlight;
};

const uint16_t EEPROM_OFFSET = 0x10;
const uint64_t EEPROM_HASH PROGMEM = 0x30AE885F78E5FCD8ULL;
const size_t EEPROM_HASH_SIZE = sizeof(uint64_t);
const size_t EEPROM_DATA_SIZE = sizeof(ClockSettings);
#define EEPROM_HASH_OFFSET EEPROM_OFFSET + sizeof(size_t)
#define EEPROM_DATA_OFFSET EEPROM_HASH_OFFSET + EEPROM_HASH_SIZE

byte map(byte x, byte in_min, byte in_max, byte out_min, byte out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define SERIAL_DEBUG false
#define STATUS_FALSE 7
#define STATUS_TRUE 8

#endif