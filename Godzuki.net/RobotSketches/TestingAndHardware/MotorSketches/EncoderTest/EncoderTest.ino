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
  pinMode(pin, OUTPUT);
  attachInterrupt(0, updateLeft, CHANGE);
  attachInterrupt(1, updateRight, CHANGE);
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
	Serial.print( "Total l/r: " );
	Serial.print( leftTotalClicks );
	Serial.print( "/" );
	Serial.println( rightTotalClicks );
	Serial.print( "Total l/r: " );
	Serial.print( leftAggregateClicks );
	Serial.print( "/" );
	Serial.println( rightAggregateClicks );
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
