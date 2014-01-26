///////button control definitions
int adc_key_in;
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int key=-1;
int oldkey=-1;
///////servo control definitions
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 

const int SWEEP_TOP = 135;
const int SWEEP_CENTER = 90;
const int SWEEP_BOTTOM = 45;

int sweep_direction = +20;
unsigned long last_update_time = 0;
///////communications control definitions
const int MAX_SETUP_WAIT_TIME = 1000;
const long BAUD_RATE = 38400;
const int STATUS_LED_PIN = 13;
const unsigned long HEARTBEAT_FREQUENCY = 1500; //mills
const unsigned long HEARTBEAT_WIDTH = 100; //mills
unsigned long last_heartbeat_on = 0;
int ledState = 0;
int printHeartbeatStatus = 0;
int showKeyStateOnLED = 1;
////////////////////
void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  pinMode(STATUS_LED_PIN, OUTPUT);  //we'll use the debug LED to output a heartbeat
  
  Serial.begin(BAUD_RATE);
  int curSetupWaitTime = 0;
  while (!Serial && curSetupWaitTime < MAX_SETUP_WAIT_TIME ) {
    delay(20); // Wait untilSerial is ready - Leonardo
    curSetupWaitTime += 20;
  }
  Serial.println("Godzuki alive and well...");
} 
 
const int SWEEP_ONCE       = 0; // "up"    button
const int SWEEP_STOP       = 1; // "left"  button
const int GO_TO_CENTER     = 2; // "down"  button
const int SWEEP_CONTINUOUS = 3; // "right" button
const int READ_POSITION    = 4; // "enter"  button
const int SET_POSITION     = 5; // no button - kbd plus numeric value

int new_command_input = -1;
int current_command_input = -1;
int current_servo_command = -1;
int current_comms_command = -1;
int current_kbd_parameter = -1;

void loop() {
  init_status_led();
  new_command_input = get_debounced_key();
  if( new_command_input != current_command_input ) {
    switch( new_command_input ) {
      case GO_TO_CENTER: 
        Serial.println("Going to center");
        go_to_center();
        current_servo_command = new_command_input;
        break;
      case SWEEP_ONCE:
        Serial.println("Sweeping once");
        start_sweep();
        current_servo_command = new_command_input;
        break;
      case SWEEP_CONTINUOUS:
        Serial.println("Sweeping continuously");
        start_sweep();
        current_servo_command = new_command_input;
        break;
      case SWEEP_STOP:
        Serial.println("Stopping sweep");
        stop_sweep();
        current_servo_command = new_command_input;
        break;
      case READ_POSITION:
        current_comms_command = new_command_input;
        break;
      case SET_POSITION:
        Serial.print("Setting position to ");
        Serial.println(current_kbd_parameter);
        go_to_position( current_kbd_parameter );
        current_servo_command = new_command_input;
        break;
      default:
        break;
    }
  }
  // handle all the servo stuff
  if( current_servo_command == SWEEP_ONCE || current_servo_command == SWEEP_CONTINUOUS )
    continue_sweep();  
    
  // handle all the comms stuff
  if( current_comms_command == READ_POSITION ) {
      Serial.print("current motor position: ");    // do NOT update current_command...
      Serial.println(pos);                         // let it keep doing whatever it was doing...
      current_comms_command = -1;
  }
  current_command_input = new_command_input;

  // handle maintenance stuff
  check_heartbeat();
  write_final_led_status();
}

void init_status_led() {
  ledState = 2;
}
void light_status_led() {
  ledState = 1;
}
void clear_status_led() {
  if( ledState == 2 )
    ledState = 0;
}

void write_final_led_status() {
  if( ledState != 2 )
    digitalWrite(STATUS_LED_PIN, ledState ? HIGH : LOW);
}

void check_heartbeat() {
  if( millis() - last_heartbeat_on >= 0 && millis() - last_heartbeat_on <= HEARTBEAT_WIDTH ) {  // should still be on...
    light_status_led();
  }
  if( millis() - last_heartbeat_on > HEARTBEAT_WIDTH ) {
    if( printHeartbeatStatus ) {
      Serial.print("turning off at: ");
      Serial.println(millis());
    }
    clear_status_led();
  }
  if( millis() - last_heartbeat_on > HEARTBEAT_FREQUENCY ) {
    if( printHeartbeatStatus ) {
      Serial.print("turning on at: ");
      Serial.println(millis());
    }
    last_heartbeat_on = millis();
    light_status_led();
  }
}

void start_sweep() {
  go_to_position( SWEEP_BOTTOM );
  sweep_direction = +5;
  last_update_time = millis();
}
void continue_sweep() {
  int time_increment = 100;

  if( millis() - last_update_time < time_increment )
    return;
    
  last_update_time = millis();

  pos += sweep_direction;
  if( pos >= SWEEP_TOP ) {
    pos = SWEEP_TOP;
    sweep_direction = -sweep_direction;
  }
  if( pos <= SWEEP_BOTTOM ) {
    pos = SWEEP_BOTTOM;
    sweep_direction = -sweep_direction;
    if( current_servo_command == SWEEP_ONCE )
      stop_sweep();
  }
  go_to_position( pos );
}

void stop_sweep() {
  current_servo_command = SWEEP_STOP;
}

void go_to_position( int newPos ) {
   myservo.write(newPos);               // tell servo to go to position in variable 'pos' 
   pos = newPos;
   delay(50);                       // waits 15ms for the servo to reach the position }
}

void go_to_center() {
  go_to_position( SWEEP_CENTER );
}

int get_debounced_key() {
  if( Serial.available() ) {
      int kbdKey = Serial.read();
      switch( kbdKey ) {
      case 'c':
        return GO_TO_CENTER;
      case 'r':
        return READ_POSITION;
      case 's':
        return SWEEP_CONTINUOUS;
      case 'o':
        return SWEEP_ONCE;
      case 'x':
        return SWEEP_STOP;
      case 'g':
        current_kbd_parameter = 0;
        while( Serial.available() ) {
          kbdKey = Serial.read();
          current_kbd_parameter = 10*current_kbd_parameter + (kbdKey - '0');
        }
        if( current_kbd_parameter < SWEEP_BOTTOM )
          current_kbd_parameter = SWEEP_BOTTOM;
        if( current_kbd_parameter > SWEEP_TOP )
          current_kbd_parameter = SWEEP_TOP;
        return SET_POSITION;
    }
  }
  
  adc_key_in = analogRead(0);    // read the value from the sensor  
  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey) {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor  
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) {         
      oldkey = key;
//      if (key >=0){    // old code to monitor the actual adc values
//         Serial.println(adc_key_in, DEC);
//      }
    }
  }
  if( showKeyStateOnLED )
    if( key == -1 )
      clear_status_led();
    else
      light_status_led();
  return key;
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