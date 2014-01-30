
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

void setup() { 
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
}
 
void loop() 
{
  adc_key_in = analogRead(0);    // read the value from the sensor  
  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey) {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) {         
      oldkey = key;
      if (key >=0){
         Serial.println(adc_key_in, DEC);
      }
    }
  }
  if( key == -1 )
    digitalWrite(13, LOW);
  else
    digitalWrite(13, HIGH);
}
// Convert ADC value to key number
int get_key(unsigned int input)
{   
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {  
      return k;  
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}