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
#include <new.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidTWI2.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Flash.h>

//PWM OUTPUT Pins for RGB LCD
#define REDLITE 3
#define GREENLITE 9
#define BLUELITE 11

//LCD Config
#define LCD_ROWS 2
#define LCD_COLS 16

// Pin number for OneWire bus
#define ONE_WIRE_BUS 5

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
DeviceAddress therm;

// you can change the overall brightness by range 0 -> 255
int brightness = 255;

#define SERIAL_DEBUG 0

#if SERIAL_DEBUG
boolean doPrintRGB = false;
#endif

LiquidTWI2 lcd(0x20, 1);
RTC_DS1307 RTC;

FLASH_ARRAY(byte, degreeChar,
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000,
);

void setup() {
  pinMode(2, INPUT);
  
  boolean rtcok = true;
  boolean tmpok = true;

#if SERIAL_DEBUG
  Serial.begin(9600);
#endif

  // set up the LCD's number of rows and columns: 
  lcd.begin(LCD_COLS, LCD_ROWS);

  byte * degree = new byte[degreeChar.count()];

  for (int i = 0; i < 8; i++)
    degree[i] = degreeChar[i];

  lcd.createChar(0, degree);
  
  delete degree;

  lcd.clear();

  //Wait for the LCD to clear
  delay(500);
  //Do a backlight POST for fun
  setBacklight(255, 0, 0);
  delay(500);
  setBacklight(0, 255, 0);
  delay(500);
  setBacklight(0, 0, 255);
  delay(500);
  setBacklight(255, 255, 255);
  delay(500);  

  RTC.begin();
  // Print a message to the LCD.
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(2012,1,1,0,0,0));
    rtcok = false;
  }
  
  //Start Dallas OneWire
  sensors.begin();
  
  //Find the DS18S20 Temperature Sensor
  if(!sensors.getAddress(therm, 0)) tmpok = false;
  
  if (tmpok && rtcok)
    setBacklight(0, 255, 0);
  else
    setBacklight(255, 0, 0);
  

  //Confirm ready with GO/NOGO response
  lcd.setCursor(0, 0);
  if (rtcok)
    lcd.print("RTC IS GO");
  else
    lcd.print("RTC IS NOGO");

  lcd.setCursor(0, 1);
  if(tmpok)
    lcd.print("TEMP IS GO");
  else
    lcd.print("TEMP IS NOGO");

  delay(2000);

  //Set it to 9-bit resolution  
  sensors.setResolution(therm, 9);
}

void loop() {
  static boolean last_t = 0;

#if SERIAL_DEBUG
  cmd();
#endif

  confBacklight();

  boolean t = digitalRead(2) == LOW ? true : false;
  
  if (t != last_t)
    lcd.clear();
  
  if (t)
    printTime();
  else
    printTemp();
  
  last_t = t;
  
  delay(250);
}

void addLeadingZero(int num) __attribute__((noinline));

void addLeadingZero(int num)
{
  if (num < 10 && num > -10)
  {
    lcd.print("0");
  }
}
