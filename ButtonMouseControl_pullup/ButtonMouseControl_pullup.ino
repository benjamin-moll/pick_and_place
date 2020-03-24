
#include "<Mouse.h>"
#include <Encoder.h>

// set pin numbers for the buttons:
const int upButton = 2;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;
const int stepUp = 6;
const int stepDown = 7;
const int mouseButton = 8;
const int homeButton = 9;
const int encoderButton = 10;

// set encoder
Encoder knob(12, 11);

int lastKnobState = -1;
int timestamp = 0;

int knobChange;

// output range of X or Y movement
// affects movement speed
int range = 5;
int responseDelay = 10;

int xDistance;
int yDistance;

// states
int upState;
int downState;
int rightState;
int leftState;
int stepUpState;
int stepDownState;
int clickState;
int homeState;
int encoderBtnState;
int encoderBtnXYState; //TODO: USE ME
int knobState;

void setup()
{
	Serial.begin(9600);

	// initialize the button inputs
	pinMode(upButton, INPUT_PULLUP);
	pinMode(downButton, INPUT_PULLUP);
	pinMode(leftButton, INPUT_PULLUP);
	pinMode(rightButton, INPUT_PULLUP);
	pinMode(stepUp, INPUT_PULLUP);
	pinMode(stepDown, INPUT_PULLUP);
	pinMode(mouseButton, INPUT_PULLUP);
	pinMode(homeButton, INPUT_PULLUP);
	pinMode(encoderButton, INPUT_PULLUP);

	// initialize mouse control
	Mouse.begin();
}

void loop()
{
	// read buttons
	upState = digitalRead(upButton);
	downState = digitalRead(downButton);
	rightState = digitalRead(rightButton);
	leftState = digitalRead(leftButton);

	stepUpState = digitalRead(stepUp);
	stepDownState = digitalRead(stepDown);

	clickState = digitalRead(mouseButton);
	homeState = digitalRead(homeButton);

	encoderBtnState = digitalRead(encoderButton);

	// read encoder
	knobState = knob.read();

	// check if mouse has moved
	moveMouse();

	// check if mouse has been pressed
	mousePress();

	// check if encoder has moved
	encoderMove();

	// check if homing button has been pressed
	home();

	// check if change step up or down has been pressed
	changeStep();

	// a classic delay
	delay(responseDelay);
}

void moveMouse()
{
	// calculate the movement distance based on the button states
	xDistance = (leftState - rightState) * range;
	yDistance = (upState - downState) * range;

	// if X or Y is non-zero, move
	if ((xDistance != 0) || (yDistance != 0))
	{
		Mouse.move(xDistance, yDistance, 0);
	}
}

void mousePress()
{
	// if the mouse button is pressed
	if (clickState == LOW)
	{
		// if the mouse is not pressed, press it
		if (!Mouse.isPressed(MOUSE_LEFT))
		{
			Mouse.press(MOUSE_LEFT);
		}
		else
		{
			Mouse.release(MOUSE_LEFT);
		}
	}
}

void encoderMove()
{
	knobChange = knobState - lastKnobState;

	if (abs(knobChange) >= 2)
	{
		// get the direction (-1 or 1)
		int knobDirection = (knobChange / abs(knobChange));

		int xState = knobDirection * range;

		if (knobDirection != 0)
		{
			Mouse.move(xState, yDistance, 0);
		}

		// save knobState for next time through loop
		lastKnobState = knobState;
	}
}

void encoderMoveX()
{
	
}

void encoderMoveY()
{

}

void home()
{
	if (homeState == LOW)
	{
		xDistance = 0;
		yDistance = 0;

		Mouse.move(xDistance, yDistance, 0);
	}
}

void changeStep()
{
	// read the value from the pot and map it to range
	// int rangeState = analogRead(A7); TODO: This is a button now
	// range = map(rangeState, 0, 1023, 1, 20); TODO: This is a button now
}

void encoderPress()
{
	stepUpState = digitalRead(stepUp);
	stepDownState = digitalRead(stepDown);
}
