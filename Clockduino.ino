/*
 ArduClock - An Arduino based clock
 Derek Chafin
 
 Description: 
 This is a sketch for the Arduino that uses a DS1307 RTC to track and display the date and time.
 It connects to a LCD for display purposes. It also connects to a Dallas DS18B20 to display the temperature.
 This code is based on the Adafruit i2c/SPI LCD backpack sketch as shown below.

 Demonstration sketch for Adafruit i2c/SPI LCD backpack
 using MCP23008 I2C expander
 ( http://www.ladyada.net/products/i2cspilcdbackpack/index.html )

 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5
 * DAT to Analog #4
*/

// include the library code:
#include "config.h"
#include <Wire.h>
#include <RTClib.h>
#include <LiquidTWI2.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/pgmspace.h>
#include <SimpleTimer.h>
#include <avr/eeprom.h>
#include <new.h>

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
DeviceAddress therm;

LiquidTWI2 lcd(0x20, 1);
RTC_DS1307 RTC;

BacklightSettings mapped;
ClockSettings clockduino_t;

#if SERIAL_DEBUG
boolean doPrintRGB = false;
#endif

const char is_not_word[] PROGMEM = { " NOT" };
const char is_ready_word[] PROGMEM = { " READY" };
const char is_rtc_word[] PROGMEM = { "RTC" };
const char is_tmp_word[] PROGMEM = { "TMP" };

//Store our custom degree character in progmem
const byte degreeChar[] PROGMEM = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};

SimpleTimer timer;
int printTimeFunc, printTempFunc, rf_settings_timer, rf_incdec_timer;

volatile boolean checkISR = false;

byte rf_backlight_index = -1;

void setup() {
  boolean rtcok = true;
  boolean tmpok = true;

  pinMode(RF_SETTINGS_PIN, INPUT_PULLUP);
  pinMode(RF_INCREMENT_PIN, INPUT_PULLUP);
  pinMode(RF_DECREMENT_PIN, INPUT_PULLUP);

  pinMode(STATUS_TRUE, OUTPUT);
  pinMode(STATUS_FALSE, OUTPUT);
  
  Serial.begin(9600);

  // set up the LCD's number of rows and columns: 
  lcd.begin(LCD_COLS, LCD_ROWS);

  //Allocate some memory and fetch the character bits for the lcd object
  byte * degree = static_cast<byte*>(malloc(LCD_CHAR_ROWS));
  memcpy_P(degree, degreeChar, LCD_CHAR_ROWS);

  lcd.createChar(0, degree);

  //Don't need this memory anymore. Saves having to allocate a permanent spot on the heap for the character bits.
  free(degree);

  lcd.clear();
  //Wait for the LCD to clear
  delay(500);

  //Do a backlight POST for fun
  PulseLcdBacklight(MAX_ALL, MIN_ALL, MIN_ALL, 500);
  PulseLcdBacklight(MIN_ALL, MAX_ALL, MIN_ALL, 500);
  PulseLcdBacklight(MIN_ALL, MIN_ALL, MAX_ALL, 500);
  PulseLcdBacklight(MAX_ALL, MAX_ALL, MAX_ALL, 500);

  RTC.begin();
  // Print a message to the LCD.
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(2012,1,1,0,0,0));
    rtcok = false;
  }
  
  //Start Dallas OneWire
  sensors.begin();
  
  //Find the DS18S20 Temperature Sensor
  if(!sensors.getAddress(therm, 0)) 
    tmpok = false;
  else
    sensors.setResolution(therm, 9);  //Set it to 9-bit resolution
  
  //Confirm ready with GO/NOGO response
  lcd.setCursor(0, 0);

  char * sys_status1;
  char * sys_status2;
  char * sys_status3;

  sys_status1 = static_cast<char*>(malloc(strlen_P(is_rtc_word)));
  sys_status2 = static_cast<char*>(malloc(strlen_P(is_not_word)));
  sys_status3 = static_cast<char*>(malloc(strlen_P(is_ready_word)));

  strcpy_P(sys_status1, is_rtc_word);
  strcpy_P(sys_status2, is_not_word);
  strcpy_P(sys_status3, is_ready_word);

  lcd.print(sys_status1);
  if (!rtcok)
    lcd.print(sys_status2);
  lcd.print(sys_status3);

  lcd.setCursor(0, 1);
  strcpy_P(sys_status1, is_tmp_word);
  lcd.print(sys_status1);
  if(!tmpok)
    lcd.print(sys_status2);
  lcd.print(sys_status3);

  free(sys_status1);
  free(sys_status2);
  free(sys_status3);

  if (tmpok && rtcok)
    PulseLcdBacklight(0, 255, 0, 2000);
  else
    PulseLcdBacklight(255, 0, 0, 2000);

  LoadSettings();
  MapLcdSettings();
  SetLcdBacklight();

  timer.setInterval(1000, cmd);

  // This is not needed since it is done in loop() right?  
  // timer.setInterval(50, processISR);

  printTimeFunc = timer.setInterval(1000, printTime);
  printTempFunc = timer.setInterval(10000, printTemp);
  timer.disable(printTempFunc);

  lcd.clear();

  rf_settings_timer = timer.setInterval(250, isSettingsPressed);

  rf_incdec_timer = timer.setInterval(clockduino_t.button_delay, isIncOrDecPressed);
  timer.disable(rf_incdec_timer);

  enableISR();
}

void loop() {
  timer.run();

  if (checkISR)
  {
    disableISR();
    processISR();
    enableISR();
  }
}

// These two function enable and disable the pin interrupt.
// Do the disable/enable interrupt functions affect all ISR functions or just ours?
inline void enableISR()
{
  attachInterrupt(0, setCheckISR, RISING);
}

inline void disableISR()
{
  detachInterrupt(0);
}

// This is an external pin ISR function so we dont do much here.
void setCheckISR()
{
  checkISR = TRUE;
}

inline void processISR()
{
    if (checkISR)
    {
      //We are switching data sources here to clear the lcd.
      lcd.clear();
      
      //Toggle both time and temp update non-async timers.
      timer.toggle(printTimeFunc);
      timer.toggle(printTempFunc);

      //Run the active data function once after clearing the lcd.
      //Otherwise the lcd wont update until the timer expires.

      if(timer.isEnabled(printTimeFunc))
        printTime();

      if(timer.isEnabled(printTempFunc))
        printTemp();
    }

  // Set this to false so we dont get timer toggle runaways.
  checkISR = FALSE;
}

void addLeadingZero(int num)
{
  if (abs(num) < 10)
  {
    lcd.print('0');
  }
}
