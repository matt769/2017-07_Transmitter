// INPUTS
const byte pinJoystickAx = A4;
const byte pinJoystickAy = A3;
const byte pinJoystickAb = A5;
const byte pinJoystickBx = A2;
const byte pinJoystickBy = A0;
const byte pinJoystickBb = A1;
const byte pinButtonC = 3;    // push button
const byte pinButtonD = 4;    // toggle switch


// CALIBRATION

int AX_CENTRE = 493;
int AX_MIN = 2;
int AX_MAX = 984;
int AY_CENTRE = 531;
int AY_MIN = 1022;
int AY_MAX = 40;
int BX_CENTRE = 501;
int BX_MIN = 5;
int BX_MAX = 997;
int BY_CENTRE = 532;
int BY_MIN = 1020;
int BY_MAX = 44;

const int OUTPUT_MIN = 0;
const int OUTPUT_MAX = 255;

const int STICK_DEADBAND = 10; // applied to the analog read



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
  delay(10);
  joystickAx = analogRead(pinJoystickAx);
  delay(10);
  joystickAy = analogRead(pinJoystickAy);
  delay(10);
  joystickAbTmp = digitalRead(pinJoystickAb);
  delay(10);
  joystickBx = analogRead(pinJoystickBx);
  delay(10);
  joystickBy = analogRead(pinJoystickBy);
  delay(10);
  joystickBbTmp = digitalRead(pinJoystickBb);
  delay(10);
  buttonCTmp = digitalRead(pinButtonC);
  buttonDTmp = digitalRead(pinButtonD);
}

unsigned long joystickAbLastDebounceTime = 0;
unsigned long joystickBbLastDebounceTime = 0;
unsigned long buttonCLastDebounceTime = 0;
unsigned long buttonDLastDebounceTime = 0;
byte joystickAbPrevious = 0;  // DEFAULT LOW
byte joystickBbPrevious = 0;  // DEFAULT LOW
byte buttonCPrevious = 0;  // DEFAULT LOW
byte buttonDPrevious = 0;  // DEFAULT LOW


void debounceInputs() {
  static unsigned long debounceDelay = 50;

  if (joystickAbTmp != joystickAbPrevious) {
    joystickAbLastDebounceTime = millis();
  }
  if (joystickBbTmp != joystickBbPrevious) {
    joystickBbLastDebounceTime = millis();
  }
  if (buttonCTmp != buttonCPrevious) {
    buttonCLastDebounceTime = millis();
  }
  if (buttonDTmp != buttonDPrevious) {
    buttonDLastDebounceTime = millis();
  }
  if (millis() - joystickAbLastDebounceTime > debounceDelay) {
    joystickAb = joystickAbTmp;
  }
  if (millis() - joystickBbLastDebounceTime > debounceDelay) {
    joystickBb = joystickBbTmp;
  }
  if (millis() - buttonCLastDebounceTime > debounceDelay) {
    buttonC = buttonCTmp;
  }
  if (millis() - buttonDLastDebounceTime > debounceDelay) {
    buttonD = buttonDTmp;
  }

  joystickAbPrevious = joystickAbTmp;
  joystickBbPrevious = joystickBbTmp;
  buttonCPrevious = buttonCTmp;
  buttonDPrevious = buttonDTmp;

}


//493 2 984
// WHERE TO LIMIT THE ACTUAL READINGS??
// WHERE TO ADD DEADBAND

void constrainInput(int *input, int *centre, int *minimum, int *maximum) {
  // account for high mins and low maxs
  int tmpInput = *input;
  
  if ((*centre - *minimum) > 1) {
    tmpInput = min(tmpInput, *maximum);
    tmpInput = max(tmpInput, *minimum);
  }
  else {
    tmpInput = max(tmpInput, *maximum);
    tmpInput = min(tmpInput, *minimum);
  }

  if (abs(*input - *centre) > STICK_DEADBAND) {
    *input = tmpInput;
  }
  else {
    *input = *centre;
  }
}

void constrainInputsAll(){
  constrainInput(&joystickAx, &AX_CENTRE, &AX_MIN, &AX_MAX);
  constrainInput(&joystickAy, &AY_CENTRE, &AY_MIN, &AY_MAX);
  constrainInput(&joystickBx, &BX_CENTRE, &BX_MIN, &BX_MAX);
  constrainInput(&joystickBy, &BY_CENTRE, &BY_MIN, &BY_MAX);  
}


void mapInputs() {
  joystickAx = map(joystickAx, AX_MIN, AX_MAX, OUTPUT_MIN, OUTPUT_MAX);
  joystickAy = map(joystickAy, AY_MIN, AY_MAX, OUTPUT_MIN, OUTPUT_MAX);
  joystickAb = !joystickAbPrevious;
  joystickBx = map(joystickBx, BX_MIN, BX_MAX, OUTPUT_MIN, OUTPUT_MAX);
  joystickBy = map(joystickBy, BY_MIN, BY_MAX, OUTPUT_MIN, OUTPUT_MAX);
  joystickBb = !joystickBbPrevious;
  buttonC = !buttonCPrevious;
  buttonD = !buttonDPrevious;
}


void printInputs() {
  Serial.print(joystickAx); Serial.print('\t');
  Serial.print(joystickAy); Serial.print('\t');
  Serial.print(joystickAb); Serial.print('\t');
  Serial.print(joystickBx); Serial.print('\t');
  Serial.print(joystickBy); Serial.print('\t');
  Serial.print(joystickBb); Serial.print('\t');
  Serial.print(buttonC); Serial.print('\t');
  Serial.print(buttonD); Serial.print('\n');
}



int findCentre(byte pin) {
  int centre;
  unsigned long readingSum = 0;
  byte repetitions = 200;
  for (byte i = 0; i < repetitions; i++) {
    readingSum += analogRead(pin);
  }
  centre = readingSum / repetitions;
  return centre;
}





int findInputMax(byte pin, int centreReading) {
  int dir;
  int maxDiff = 0;
  int maxReading;
  // wait until reading changes from centre (beyond some threshold)
  while (abs(analogRead(pin) - centreReading) < 100) {
    // wait until input difference exceeds initial value
    //Serial.print(analogRead(pin));Serial.print('\t');Serial.println(centreReading);
    //    Serial.println("1");
  }
  // check which way stick has been moved
  if (analogRead(pin) - centreReading > 0) {
    // reading larger values than initial
    dir = 1;
  }
  else {
    dir = -1;
  }
  Serial.print("dir: "); Serial.println(dir);
  // now look for the maximum value stick is at until reading is back close to initial value
  while (abs(analogRead(pin) - centreReading) > 50) {
    //    Serial.print(analogRead(pin));Serial.print('\t');Serial.print(centreReading);
    maxDiff = max(maxDiff, abs(analogRead(pin) - centreReading));
    //    Serial.print('\t');Serial.println(maxDiff);
    //    Serial.println("2");
  }

  maxReading = centreReading + (maxDiff * dir);
  return maxReading;

}


int sign(int a) {
  int i;
  if (a > 0) {
    i = 1;
  }
  else if (a < 0) {
    i = -1;
  }
  else {
    i = 0;
  }
  return i;
}

void modifyInputRange(int *centre, int *minimum, int *maximum) {
  // check if limits are equally distant from the centre
  int minRange = min(abs(*centre - *minimum), abs(*centre - *maximum));
  // constrain
  *minimum = *centre - (sign(*centre - *minimum) * minRange);
  *maximum = *centre - (sign(*centre - *maximum) * minRange);

}



void calibration() {

  Serial.println(F("StickA X axis"));
  AX_CENTRE = findCentre(pinJoystickAx);
//  Serial.println(AX_CENTRE);
  Serial.println(F("Move to minimum"));
  AX_MIN = findInputMax(pinJoystickAx, AX_CENTRE);
//  Serial.println(AX_MIN);
  Serial.println(F("Move to maximum"));
  AX_MAX = findInputMax(pinJoystickAx, AX_CENTRE);
//  Serial.println(AX_MAX);
  modifyInputRange(&AX_CENTRE, &AX_MIN, &AX_MAX);
  Serial.println(AX_CENTRE);
  Serial.println(AX_MIN);
  Serial.println(AX_MAX);

  Serial.println(F("StickA Y axis"));
  AY_CENTRE = findCentre(pinJoystickAy);
  Serial.println(F("Move to minimum"));
  AY_MIN = findInputMax(pinJoystickAy, AY_CENTRE);
  Serial.println(F("Move to maximum"));
  AY_MAX = findInputMax(pinJoystickAy, AY_CENTRE);
  modifyInputRange(&AY_CENTRE, &AY_MIN, &AY_MAX);
  Serial.println(AY_CENTRE);
  Serial.println(AY_MIN);
  Serial.println(AY_MAX);

  Serial.println(F("StickB X axis"));
  BX_CENTRE = findCentre(pinJoystickBx);
  Serial.println(F("Move to minimum"));
  BX_MIN = findInputMax(pinJoystickBx, BX_CENTRE);
  Serial.println(F("Move to maximum"));
  BX_MAX = findInputMax(pinJoystickBx, BX_CENTRE);
  modifyInputRange(&BX_CENTRE, &BX_MIN, &BX_MAX);
  Serial.println(BX_CENTRE);
  Serial.println(BX_MIN);
  Serial.println(BX_MAX);

  Serial.println(F("StickB Y axis"));
  BY_CENTRE = findCentre(pinJoystickBy);
  Serial.println(F("Move to minimum"));
  BY_MIN = findInputMax(pinJoystickBy, BY_CENTRE);
  Serial.println(F("Move to maximum"));
  BY_MAX = findInputMax(pinJoystickBy, BY_CENTRE);
  modifyInputRange(&BY_CENTRE, &BY_MIN, &BY_MAX);
  Serial.println(BY_CENTRE);
  Serial.println(BY_MIN);
  Serial.println(BY_MAX);

  Serial.println(F("Stick calibration complete"));
}


void processInputs() {
//  Serial.print("xxx");
  readInputs();
//  Serial.print(joystickAx);Serial.print('\t');
  debounceInputs();
//  Serial.print(joystickAx);Serial.print('\t');
  constrainInputsAll();
//  Serial.print(joystickAx);Serial.print('\t');
  mapInputs();
//  Serial.println(joystickAx);
//  Serial.print("yyy");
}



void writeIntegerToEeprom(int address, int data){
  EEPROM.write(address,highByte(data));
  address++;
  EEPROM.write(address,lowByte(data));
}

int readIntegerFromEeprom(int address){
  byte high = EEPROM.read(address);
  byte low = EEPROM.read(address);
  int result=word(high,low);
  return result;
}


//void writeCalibrationToEeprom() {
//
//int address = 0;
//  
//AX_CENTRE;
//AX_MIN;
//AX_MAX;
//AY_CENTRE;
//AY_MIN;
//AY_MAX;
//BX_CENTRE;
//BX_MIN;
//BX_MAX;
//BY_CENTRE;
//BY_MIN;
//BY_MAX;
//}
//
//void readCalibrationFromEeprom() {
//
//}

// have some way of telling if settings are obviously not there?









