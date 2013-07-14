// settings.ino


boolean settingsStatusOn(boolean value, int status_delay)
{
  if (value)
    digitalWrite(STATUS_TRUE, HIGH);
  else
    digitalWrite(STATUS_FALSE, HIGH);

  delay(status_delay);

  return value;
}

boolean settingsStatusOn(boolean value)
{
  return settingsStatusOn(value, 500);
}

void settingsStatusOff()
{
  digitalWrite(STATUS_TRUE, LOW);
  digitalWrite(STATUS_FALSE, LOW);
}

template <class T> int readBlock(int address, const T& value)
{   
  eeprom_read_block((void*)&value, (const void*)address, sizeof(value));
  return sizeof(value);
}

template <class T> int writeBlock(int address, const T& value)
{
  eeprom_write_block((void*)&value, (void*)address, sizeof(value));               
  return sizeof(value);
}

void LoadSettings()
{
  if(settingsStatusOn(isSettingsValid()))
  {
    readBlock(EEPROM_DATA_OFFSET, clockduino_t);
  }
  else
  {
    LoadDefaultSettings();
    uint64_t e;
    getPROGMEMHash(&e);
    setEEPROMHash(&e);

    SaveSettings();
  }

  settingsStatusOff();
}

void SaveSettings()
{
  writeBlock(EEPROM_DATA_OFFSET, clockduino_t);
}

void getEEPROMHash(uint64_t * hash)
{
  size_t data_size;
  readBlock(EEPROM_OFFSET, data_size);

  if (data_size != EEPROM_DATA_SIZE)
  {
    *hash = 0;
    return;
  }

  readBlock(EEPROM_HASH_OFFSET, *hash);
}

void setEEPROMHash(uint64_t * hash)
{
  writeBlock(EEPROM_OFFSET, EEPROM_DATA_SIZE);
  writeBlock(EEPROM_HASH_OFFSET, *hash);
}

void getPROGMEMHash(uint64_t * hash)
{
  memcpy_P(hash, &EEPROM_HASH, EEPROM_HASH_SIZE);
}

boolean isSettingsValid()
{
  uint64_t e;
  uint64_t p;
  getEEPROMHash(&e);
  getPROGMEMHash(&p);
  return e == p;
}

void LoadDefaultSettings()
{
  clockduino_t.backlight.red = 255;
  clockduino_t.backlight.green = 255;
  clockduino_t.backlight.blue = 255;
  clockduino_t.backlight.brightness = 255;
  clockduino_t.button_delay = 5;
}