float tempC,lightPer;
unsigned int tempPin = 0,lightPin = 5;

void setup()
{
  analogReference(INTERNAL); //Vref=1.1
  Serial.begin(9600);
}

void loop()
{
  tempC = (analogRead(tempPin)*110)/1024;
  lightPer = map(analogRead(lightPin),0,1024,0,100);
  Serial.print("Temperature: ");Serial.print(tempC);Serial.print("oC        ");
  Serial.print("Light :");Serial.print(lightPer);Serial.println("%");
  delay(1000);
}
