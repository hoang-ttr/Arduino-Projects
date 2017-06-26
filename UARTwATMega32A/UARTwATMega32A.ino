unsigned int data;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  data=Serial.parseInt();
  if(data)
  {
    Serial.write(data);
  }
}
