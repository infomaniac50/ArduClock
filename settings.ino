// settings.ino
void ReadEEPROM(int address, int length, byte * data)
{
  int start_address = address + EEPROM_OFFSET;
  int end_address = start_address + length;
  for(int i = start_address; i < end_address; i++)
    data[i] = EEPROM.read(i);
}

void WriteEEPROM(int address, int length, byte * data)
{
  int start_address = address + EEPROM_OFFSET;
  int end_address = start_address + length;
  for(int i = start_address; i < end_address; i++)
    EEPROM.write(i, data[i]);
}

void LoadSettings()
{
  if(!isSettingsValid())
  {
    LoadDefaultSettings();
    SaveSettings();

    return;
  }

  byte * p = (byte*)(&clockduino_t);

  ReadEEPROM(EEPROM_HASH_SIZE, sizeof(BacklightSettings), p);
}

void SaveSettings()
{
  byte * p = (byte*)(&clockduino_t);

  WriteEEPROM(EEPROM_HASH_SIZE, sizeof(BacklightSettings), p);
}

boolean isSettingsValid()
{
  boolean is_valid = true;
  byte * m;
  byte * n;

  m = new byte(EEPROM_HASH_SIZE);
  n = new byte(EEPROM_HASH_SIZE);
  ReadEEPROM(0, EEPROM_HASH_SIZE, m);
  memcpy_P(n, &EEPROM_HASH, EEPROM_HASH_SIZE);

  for(uint16_t i = 0; i < EEPROM_HASH_SIZE; i++)
  {
    if (*n != *m)
      is_valid = false;

    n++;
    m++;
  }

  delete(m);
  delete(n);

  return is_valid;
}

void LoadDefaultSettings()
{
  clockduino_t.backlight.red = 255;
  clockduino_t.backlight.green = 255;
  clockduino_t.backlight.blue = 255;
  clockduino_t.backlight.brightness = 255;
  clockduino_t.button_delay = 5;
}