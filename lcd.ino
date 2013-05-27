__inline void setBacklight(uint8_t r, uint8_t g, uint8_t b) {
  // normalize the red LED - its brighter than the rest!  
  r = map(r, 0, 255, 0, 100);
  g = map(g, 0, 255, 0, 150);
 
  r = map(r, 0, 255, 0, brightness);
  g = map(g, 0, 255, 0, brightness);
  b = map(b, 0, 255, 0, brightness);    
 
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

__inline void confBacklight()
{
  int R = analogRead(A0);
  int G = analogRead(A1);
  int B = analogRead(A2);
  int I = analogRead(A3);
  R = map(R, 0, 1023, 0, 255);
  G = map(G, 0, 1023, 0, 255);
  B = map(B, 0, 1023, 0, 255);
  brightness = map(I, 0, 1023, 0, 255);
  setBacklight(R,G,B);  
}
