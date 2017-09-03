// INPUTS
const byte pinJoystickAx = A4;
const byte pinJoystickAy = A3;
const byte pinJoystickAb = A5;
const byte pinJoystickBx = A2;
const byte pinJoystickBy = A0;
const byte pinJoystickBb = A1;
const byte pinButtonC = 3;    // push button
const byte pinButtonD = 4;    // toggle switch


void setupInputs() {
  // set up user inputs (buttons, joysticks etc)
  pinMode(pinJoystickAx, INPUT); // not strictly required
  pinMode(pinJoystickAy, INPUT); // not strictly required
  pinMode(pinJoystickAb, INPUT_PULLUP);
  pinMode(pinJoystickBx, INPUT); // not strictly required
  pinMode(pinJoystickBy, INPUT); // not strictly required
  pinMode(pinJoystickBb, INPUT_PULLUP);
  pinMode(pinButtonC, INPUT_PULLUP);
  pinMode(pinButtonD, INPUT_PULLUP);
}



void readInputs() {
  joystickAx = analogRead(pinJoystickAx);
  joystickAy = analogRead(pinJoystickAy);
  joystickAb = digitalRead(pinJoystickAb);
  joystickBx = analogRead(pinJoystickBx);
//  Serial.println(joystickBx);
  joystickBy = analogRead(pinJoystickBy);
  joystickBb = digitalRead(pinJoystickBb);
  buttonC = digitalRead(pinButtonC);
  buttonD = digitalRead(pinButtonD);
}

// add calibration later
void mapInputs(){
  joystickAx = map(joystickAx,0,1023,0,255);
  joystickAy = map(joystickAy,1023,0,0,255);  // NOTE REVERSED
  joystickAb = !joystickAb;
  joystickBx = map(joystickBx,0,1023,0,255);
  joystickBy = map(joystickBy,1023,0,0,255);
  joystickBb = !joystickBb;
  buttonC = !buttonC;
  buttonD = !buttonD;
}


void printInputs(){
  Serial.print(joystickAx);Serial.print('\t');
  Serial.print(joystickAy);Serial.print('\t');
  Serial.print(joystickAb);Serial.print('\t');
  Serial.print(joystickBx);Serial.print('\t');
  Serial.print(joystickBy);Serial.print('\t');
  Serial.print(joystickBb);Serial.print('\t');
  Serial.print(buttonC);Serial.print('\t');
  Serial.print(buttonD);Serial.print('\n');
}

