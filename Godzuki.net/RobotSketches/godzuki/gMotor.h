#include <Adafruit_MotorShield.h>

#ifndef G_MOTOR_H
#define G_MOTOR_H

class gMotor {
public:
	// Create the motor shield object with the default I2C address
	static Adafruit_MotorShield AFMS;
	// Or, create it with a different I2C address (say for stacking)
	// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

	// Select which 'port' M1, M2, M3 or M4. In this case, M1
	// You can also make another motor on port M2
	//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);


	gMotor();
	virtual ~gMotor();

	void setup( int motorID );

	void setSpeed(int thisSpeed);
	void setDirection(int dir);
	void go();
	void stop();

	Adafruit_DCMotor *myMotor;
private:
	int potentialDir;
	int potentialSpeed;
};

#endif

