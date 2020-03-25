/*
   Pick And Place HID/USB
   
   A HID/USB Device for controlling the mouse. Allows
   for controlling x and y movement indepeendently, for 
   accuracy. User can use buttons, rotoraty pads, or a
   joystick to control each movement. The mouse press
   "latches" so items can be clicked, moved, then
   released. The mouse can be set to (x: 0, y: 0) by
   pressing the "home" button. The number of steps per
   input can be increased or decreased within the range
   of 1 to 20 steps.
   
   created 02/14/2020
   by Ben Moll & Noah Kernis
*/
#include "Mouse.h"
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

int lastKnobStateX = -1;
int lastKnobStateY = -1;
int timestamp = 0;

int knobChange;

// output range of X or Y movement
// affects movement speed
int range = 5;
int responseDelay = 20;
// TODO: Proper debouncing instead of delay

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
int encoderBtnXYState = 0;
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
  changeRange();

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
//   if (clickState == LOW)
//   {
//     // if the mouse is not pressed, press it
//     if (!Mouse.isPressed(MOUSE_LEFT))
//     {
//       Mouse.press(MOUSE_LEFT);
//     }
//     else
//     {
//       Mouse.release(MOUSE_LEFT);
//     }
//   }
	if (clickState == LOW)
	{
		Mouse.press(MOUSE_LEFT);
	}
	else
	{
		Mouse.release(MOUSE_LEFT);
	}
}

void encoderMove()
{
  whichDirection();

  if (encoderBtnXYState == 0)
  {
    encoderMoveX();
  }
  else
  {
    encoderMoveY();
  }
}

void whichDirection()
{
  if (encoderBtnState == LOW)
  {
    if (encoderBtnXYState == 0)
    {
      encoderBtnXYState = 1;
    }
    else
    {
      encoderBtnXYState = 0;
    }
  }
}

void encoderMoveX()
{
  knobChange = knobState - lastKnobStateX;

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
    lastKnobStateX = knobState;
  }
}

void encoderMoveY()
{
  knobChange = knobState - lastKnobStateY;

  if (abs(knobChange) >= 2)
  {
    // get the direction (-1 or 1)
    int knobDirection = (knobChange / abs(knobChange));

    int yState = knobDirection * range;

    if (knobDirection != 0)
    {
      Mouse.move(xDistance, yState, 0);
    }

    // save knobState for next time through loop
    lastKnobStateY = knobState;
  }
}

void home()
{
  if (homeState == LOW)
  {
    Mouse.move(-1000000, -1000000, 0);
  }
}

void changeRange()
{
  if (stepUpState == LOW)
  {
    if (range < 20)
    {
      range = range + 1;
    }
  }

  if (stepDownState == LOW)
  {
    if (range > 1)
    {
      range = range - 1;
    }
  }

  delay(responseDelay);
}

void encoderPress()
{
  stepUpState = digitalRead(stepUp);
  stepDownState = digitalRead(stepDown);
}
