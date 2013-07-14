void cmd(){
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();
    switch(cmd)
    {
      case 's':
      case 'S':
        Serial.println("OK");
        Serial.println("Starting date/time entry mode");
        while(setTime())
          delay(100);
        Serial.println("OK");
        break;
      case 'd':
      case 'D':
        Serial.println("OK");
        rf_settings_delay = Serial.parseInt();
#if SERIAL_DEBUG        
      case 'b':
      case 'B':
        doPrintRGB = true;
        break;
#endif        
      default:
        emptySerialBuffer();
        break;
    }
  }
}

void emptySerialBuffer() 
{
  while(Serial.available())
    Serial.read();
}