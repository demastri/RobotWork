void setup() 
{
  Serial.begin(9600);
  while (! Serial); // Wait untilSerial is ready - Leonardo
  Serial.println("Godzuki alive and well...");
}
 
void loop() 
{
  if (Serial.available())
  {
    char *outStr = " ";
    char ch = Serial.read();
    outStr[0] = ch;
    Serial.print("Got a character <");
    Serial.print(outStr);
    Serial.print(">");
    Serial.println();
  }
}
 
