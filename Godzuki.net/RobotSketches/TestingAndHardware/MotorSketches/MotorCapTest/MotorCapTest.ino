/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Servo.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotorA = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
Adafruit_DCMotor *myMotorB = AFMS.getMotor(2);
Adafruit_DCMotor *runMotor = 0;
bool testMotorA = true;

int MAX_SPEED_A = 200;
int MAX_SPEED_B = 200;

int SWEEP_TOP = 200;
int SWEEP_BOTTOM = 15;
int SWEEP_INCREMENT = 15;
int curServoPosition = SWEEP_BOTTOM;

Servo myBaseServo;

void setup() {
	myBaseServo.attach( 9 );


	Serial.begin(9600);           // set up Serial library at 9600 bps
	Serial.println("Adafruit Motorshield v2 - DC Motor test!");

	AFMS.begin();  // create with the default frequency 1.6KHz
	//AFMS.begin(1000);  // OR with a different frequency, say 1KHz

	// Set the speed to start, from 0 (off) to 255 (max speed)
	myMotorA->setSpeed(150);
	myMotorA->run(FORWARD);
	// turn on motor
	myMotorA->run(RELEASE);

	// Set the speed to start, from 0 (off) to 255 (max speed)
	myMotorB->setSpeed(150);
	myMotorB->run(FORWARD);
	// turn on motor
	myMotorB->run(RELEASE);
}

void loop() {

	if( testMotorA )
		runMotor = myMotorA;
	else
		runMotor = myMotorB;
	int MAX_SPEED = (testMotorA ? MAX_SPEED_A : MAX_SPEED_B);
	testMotorA = !testMotorA;

	uint8_t i;

	Serial.print("tick");

	runMotor->run(FORWARD);
	for (i=0; i<MAX_SPEED ; i++) {
		runMotor->setSpeed(i);  
		delay(10);
	}
	for (i=MAX_SPEED ; i!=0; i--) {
		runMotor->setSpeed(i);  
		delay(10);
	}

	curServoPosition += SWEEP_INCREMENT;
	curServoPosition %= SWEEP_TOP;
	myBaseServo.write(curServoPosition);
	Serial.println(curServoPosition);

	Serial.print("tock");

	runMotor->run(BACKWARD);
	for (i=0; i<MAX_SPEED ; i++) {
		runMotor->setSpeed(i);  
		delay(10);
	}
	for (i=MAX_SPEED ; i!=0; i--) {
		runMotor->setSpeed(i);  
		delay(10);
	}
	curServoPosition += SWEEP_INCREMENT;
	curServoPosition %= SWEEP_TOP;
	myBaseServo.write(curServoPosition);
	Serial.println(curServoPosition);

	Serial.print("tech");
	runMotor->run(RELEASE);
	delay(500);
}