void printTime()
{
  if (RTC.isrunning())
  {
    lcd.setCursor(0,0);
    lcd.print("Date");
    lcd.setCursor(0,1);
    lcd.print("Time");
    
    DateTime now = RTC.now();
    
    lcd.setCursor(6, 0);  
    addLeadingZero(now.month());  
    lcd.print(now.month(), DEC);
    lcd.print("-");
    addLeadingZero(now.day());
    lcd.print(now.day(), DEC);
    lcd.print("-");
    lcd.print(now.year(), DEC);
  
    lcd.setCursor(6, 1);
        
    addLeadingZero(now.hour());
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    addLeadingZero(now.minute());
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    addLeadingZero(now.second());
    lcd.print(now.second(), DEC);
    if (now.isPM())
      lcd.print("PM");
    else
      lcd.print("AM");
  }
}

boolean setTime()
{
  static int month = 0;
  static int day = 0;
  static int year = 0;
  static int hour = 0;
  static int minute = 0;
  static int second = 0;
  static boolean pm = false;

  if (Serial.available() > 0)
  {
    char cmd = Serial.read();
    
    switch(cmd)
    {
      case 'M':
        month = Serial.parseInt();
        break;
      case 'D':
        day = Serial.parseInt();
        break;
      case 'Y':
        year = Serial.parseInt();
        break;
      case 'h':
        hour = Serial.parseInt();
        break;
      case 'm':
        minute = Serial.parseInt();
        break;
      case 's':
        second = Serial.parseInt();
        break;
      case 't':
        pm = Serial.parseInt() != 0;
        break;
      case '+':
        RTC.adjust(DateTime(year,month,day,hour,minute,second,pm));
        return false;
        break;
      default:
        emptySerialBuffer();
        break;
    }  
  }
  return true;
}