// radio.ino
#define RF_POS rf_backlight_index
#define RF_NONE RF_POS > (byte)3 && RF_POS < (byte)-1
#define RF_ON RF_POS <= (byte)3
#define RF_OFF RF_POS == (byte)-1

void isSettingsPressed()
{
  static boolean pressed = false;
  static boolean last = false;
  int interval;

  pressed = digitalRead(RF_SETTINGS_PIN);

  if (pressed != last)
  {
    if(RF_ON) { 
      if (!timer.isEnabled(rf_incdec_timer))
        timer.enable(rf_incdec_timer); 
    }
    else { 
      if(timer.isEnabled(rf_incdec_timer))
        timer.disable(rf_incdec_timer); 
    }

    if (pressed)
    {
      rf_backlight_index++;
      if (RF_NONE)
      {
        rf_backlight_index = (byte)-1;
        PulseLcdBacklight(MAX_ALL, MIN_ALL, MIN_ALL, 250);
        PulseLcdBacklight(MIN_ALL, MAX_ALL, MIN_ALL, 250);
        PulseLcdBacklight(MIN_ALL, MIN_ALL, MAX_ALL, 250);

        if(settingsStatusOn(pulseIn(RF_SETTINGS_PIN, HIGH, 4000000)))
          SaveSettings();

        settingsStatusOff();
      }
      else
      {

#if SERIAL_DEBUG
        Serial.print("rf_backlight_index = ");
        Serial.println(rf_backlight_index);
#endif
        PulseLcdBacklight(MIN_ALL, MIN_ALL, MIN_ALL, 250);
        switch(rf_backlight_index)
        {
          case 0:
            PulseLcdBacklight(MAX_ALL, MAX_ALL, MAX_ALL, 250);
            break;
          case 1:
            PulseLcdBacklight(MAX_ALL, MIN_ALL, MIN_ALL, 250);
            break;
          case 2:
            PulseLcdBacklight(MIN_ALL, MAX_ALL, MIN_ALL, 250);
            break;
          case 3:
            PulseLcdBacklight(MIN_ALL, MIN_ALL, MAX_ALL, 250);
            break;
          default:
            break;
        }
        PulseLcdBacklight(MIN_ALL, MIN_ALL, MIN_ALL, 250);
      }
      SetLcdBacklight();
    }

    last = pressed;
  }
}

void isIncOrDecPressed()
{
  if (RF_ON)
  {
    byte data;
    byte * data_p;

    switch(rf_backlight_index)
    {
      case 0:
        data_p = &clockduino_t.backlight.brightness;
        break;
      case 1:
        data_p = &clockduino_t.backlight.red;
        break;
      case 2:
        data_p = &clockduino_t.backlight.green;
        break;
      case 3:
        data_p = &clockduino_t.backlight.blue;
        break;
      default:
        break;
    }

    data = *data_p;

    if (digitalRead(RF_INCREMENT_PIN))
    {
      if (data < 255)
        data++;
      else
      {
        PulseLcdBacklight(MAX_ALL - clockduino_t.backlight.red, MAX_ALL - clockduino_t.backlight.green, MAX_ALL - clockduino_t.backlight.blue, 0);
        return;
      }
    }
    else if (digitalRead(RF_DECREMENT_PIN))
    {
      if (data > 0)
        data--;
      else
      {
        PulseLcdBacklight(MAX_ALL + clockduino_t.backlight.red, MAX_ALL + clockduino_t.backlight.green, MAX_ALL + clockduino_t.backlight.blue, 0);
        return;
      }
    }

    *data_p = data;
  }

  MapLcdSettings();
  SetLcdBacklight();
}
