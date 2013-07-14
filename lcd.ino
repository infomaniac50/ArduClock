inline byte MapLcdRed(byte value, byte brightness) { return map(map(value, MIN_ALL, MAX_ALL, MIN_ALL, MAX_GREEN), MIN_ALL, MAX_ALL, MIN_ALL, brightness); }
inline byte MapLcdGreen(byte value, byte brightness) { return map(map(value, MIN_ALL, MAX_ALL, MIN_ALL, MAX_RED), MIN_ALL, MAX_ALL, MIN_ALL, brightness); }
inline byte MapLcdBlue(byte value, byte brightness) { return map(value, MIN_ALL, MAX_ALL, MIN_ALL, brightness); }

inline void SetLcdBacklight(byte r, byte g, byte b)
{
  // common anode so invert!
  analogWrite(LCD_RED_PIN, 255 - r);
  analogWrite(LCD_GREEN_PIN, 255 - g);
  analogWrite(LCD_BLUE_PIN, 255 - b);
}

inline void SetLcdBacklight()
{
  SetLcdBacklight(mapped.red, mapped.green, mapped.blue);
}

inline void MapLcdSettings(byte * red, byte * green, byte * blue, byte brightness)
{
  // normalize the red LED - its brighter than the rest!
  *red = MapLcdRed(*red, brightness);
  *green = MapLcdGreen(*green, brightness);
  *blue = MapLcdBlue(*blue, brightness);
}

inline BacklightSettings MapLcdSettings(BacklightSettings settings)
{
  MapLcdSettings(&settings.red, &settings.green, &settings.blue, settings.brightness);

  return settings;  
}

inline void MapLcdSettings()
{
  mapped = MapLcdSettings(settings);
}

inline void PulseLcdBacklight(byte r, byte g, byte b, int duration)
{
  SetLcdBacklight(MapLcdRed(r, MAX_ALL), MapLcdGreen(g, MAX_ALL), MapLcdBlue(b, MAX_ALL));
  delay(duration);
}

