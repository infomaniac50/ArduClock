void setBacklight(byte r, byte g, byte b)
{
  settings.red = r;
  settings.green = g;
  settings.blue = b;

  setBacklight();
}

void setBacklight() {
  byte r = settings.red;
  byte g = settings.green;
  byte b = settings.blue;
  byte t = settings.brightness;

  // normalize the red LED - its brighter than the rest!
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, t);
  g = map(g, 0, 255, 0, t);
  b = map(b, 0, 255, 0, t);    
 
  // common anode so invert!
  r = map(r, 0, 255, 255, 0);
  g = map(g, 0, 255, 255, 0);
  b = map(b, 0, 255, 255, 0);

#if SERIAL_DEBUG
  if (doPrintRGB)
  {
    Serial.print("R = "); Serial.print(r, DEC);
    Serial.print(" G = "); Serial.print(g, DEC);
    Serial.print(" B = "); Serial.println(b, DEC);
  }

  doPrintRGB = false;
#endif

  analogWrite(REDLITE, r);
  analogWrite(GREENLITE, g);
  analogWrite(BLUELITE, b);
}

void confBacklight()
{
  int R = analogRead(A0);
  int G = analogRead(A1);
  int B = analogRead(A2);
  int I = analogRead(A3);

  settings.red = byte(map(R, 0, 1023, 0, 255));
  settings.green = byte(map(G, 0, 1023, 0, 255));
  settings.blue = byte(map(B, 0, 1023, 0, 255));
  settings.brightness = byte(map(I, 0, 1023, 0, 255));
  setBacklight();
}
