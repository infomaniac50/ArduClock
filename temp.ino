void printTemp()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(therm);
 
  lcd.setCursor(0, 0);
  lcd.print("Temperature");

  lcd.setCursor(0, 1);
  if (tempC < 0)
    lcd.print('-');
  else
    lcd.print('+');

  addLeadingZero(int(tempC));
  lcd.print(int(tempC));
  lcd.write(0);
  lcd.print('C');

  lcd.print(' ');
  
  float tempF = DallasTemperature::toFahrenheit(tempC);
  
  if (tempF < 0)
    lcd.print('-');
  else
    lcd.print('+');

  addLeadingZero(int(tempF));
  lcd.print(int(tempF));
  lcd.write(0);
  lcd.print('F');
}
