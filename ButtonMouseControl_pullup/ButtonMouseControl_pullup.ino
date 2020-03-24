
#include "<Mouse.h>"
#include <Encoder.h>

// set pin numbers for the buttons:
const int upButton = 2;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;
const int mouseButton = 6;

// set encoder
Encoder knob(12, 11);

int lastKnobState = -1;
int timestamp = 0;

// output range of X or Y movement
// affects movement speed
int range = 5;
int responseDelay = 10;


void setup() {
  Serial.begin(9600);

  // initialize the buttons' inputs:
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(mouseButton, INPUT_PULLUP);

  // initialize mouse control:
  Mouse.begin();
}

void loop() {
  // read the buttons:

  int upState = digitalRead(upButton);
  int downState = digitalRead(downButton);
  int rightState = digitalRead(rightButton);
  int leftState = digitalRead(leftButton);
  int clickState = digitalRead(mouseButton);
  int rangeState = analogRead(A7);
  int knobState = knob.read();

  //read the value from the pot and map it to range
  range = map(rangeState, 0, 1023, 1, 20);
  // Serial.println(range);


  // calculate the movement distance based on the button states:
  int  xDistance = (leftState - rightState) * range;
  int  yDistance = (upState - downState) * range;

  // if X or Y is non-zero, move:
  if ((xDistance != 0) || (yDistance != 0)) {
    Mouse.move(xDistance, yDistance, 0);
  }

  // if the mouse button is pressed:
  if (clickState == LOW) {
    // if the mouse is not pressed, press it:
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }
  }

  int knobChange = knobState - lastKnobState;
  
  if (abs(knobChange) >= 2) {
    // get the direction (-1 or 1):
    int knobDirection = (knobChange / abs(knobChange));
    
    int xState = knobDirection * range;

    if (knobDirection != 0) {
      Mouse.move(xState, yDistance, 0);
    }
    
    // save knobState for next time through loop:
    lastKnobState = knobState;
  }

  // a delay so the mouse doesn't move too fast:
  delay(responseDelay);
}
