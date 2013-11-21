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
        setTime(true);
        while(setTime(false))
          delay(100);
        Serial.println("OK");
        break;
      case 'd':
      case 'D':
        Serial.println("OK");
        clockduino_t.button_delay = Serial.parseInt();
        Serial.println(clockduino_t.button_delay);
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