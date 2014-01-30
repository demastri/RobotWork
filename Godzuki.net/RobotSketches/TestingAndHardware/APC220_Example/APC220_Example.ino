void setup() 
{
  Serial1.begin(9600);             //Set serial baud rate to 9600
} 
 
void loop()
{
  Serial.println("Not here you big dummy....!");
  Serial1.println("Hello!");           //print out hello string 
  delay(1000);                  //1 second delay
}
