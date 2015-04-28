#include <PinChangeInt.h>

int pin = 13;
volatile int state = LOW;

volatile unsigned long leftTotalClicks=0;
volatile unsigned long rightTotalClicks=0;

long roughSpeedCalcWaitTime = 1000;
double leftInstantaneousSpeed;
double rightInstantaneousSpeed;
volatile long leftAggregateClicks=0;
volatile long rightAggregateClicks=0;
volatile long leftLastAggregateClicks=0;
volatile long rightLastAggregateClicks=0;
volatile long lastSpeedCalcTime=0;

int leftDir=0;
int rightDir=0;

int totalLoop=0;

void setup()
{
  Serial1.begin(9600);             //Set serial baud rate to 9600
  pinMode(pin, OUTPUT);
  PCintPort::attachInterrupt(10, updateLeft,CHANGE);
  PCintPort::attachInterrupt(11, updateRight,CHANGE);
  //attachInterrupt(0, updateLeft, CHANGE);	// int 0 is pin 3
  //attachInterrupt(1, updateRight, CHANGE);	// int 1 is pin 2
}

void loop()
{
	delay(roughSpeedCalcWaitTime);	// really a 1000 delay speed update command
	if( totalLoop++ % 10 == 0 )
		rightDir = 1-rightDir;

	calculateSpeeds();
	displaySpeeds();
}
void calculateSpeeds() {
	unsigned long now = millis();
	leftInstantaneousSpeed = (leftAggregateClicks-leftLastAggregateClicks) / ((now-lastSpeedCalcTime) / 1000.0);
	leftLastAggregateClicks = leftAggregateClicks;
	rightInstantaneousSpeed = (rightAggregateClicks-rightLastAggregateClicks) / ((now-lastSpeedCalcTime) / 1000.0);
	rightLastAggregateClicks = rightAggregateClicks;
	lastSpeedCalcTime = now;
}
void displaySpeeds() {
	Serial1.print( "Total l/r: " );
	Serial1.print( leftTotalClicks );
	Serial1.print( "/" );
	Serial1.print( rightTotalClicks );
	Serial1.print( "inst spd l/r: " );
	Serial1.print( leftInstantaneousSpeed );
	Serial1.print( "/" );
	Serial1.println( rightInstantaneousSpeed );
}



void updateLeft()
{
	leftTotalClicks++;
	leftAggregateClicks += (leftDir ? 1 : -1);
}

void updateRight()
{
	rightTotalClicks++;
	rightAggregateClicks += (rightDir ? 1 : -1);
}
