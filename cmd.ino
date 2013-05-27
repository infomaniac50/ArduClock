#if SERIAL_DEBUG
__inline void cmd(){
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
      case 'b':
      case 'B':
        doPrintRGB = true;
        break;
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
#endif
