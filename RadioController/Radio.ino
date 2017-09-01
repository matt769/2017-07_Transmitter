// ADD
// setDataRate - 250K Bits per second gives longest range.
// setPALevel - increase from min - need to test power supply still ok
// setChannel
// setRetries

void setupRadio() {
  // RADIO
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);  // MIN, LOW, HIGH, MAX
  // Open a writing and reading pipe on each radio, MUST BE OPPOSITE addresses to the receiver
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
}


// Control byte
// bit 0:                                         Stick 1 button pressed
// bit 1:
// bit 2: 1 = Attitude mode, 0 = rate mode        Toggle switch
// bit 3: 1 = auto-level on, 0 = auto-level off   Stick 2 button pressed
// bit 4: 
// bit 5: 
// bit 6: 
// bit 7; 1 = TURN OFF MOTORS                     Push button


void buildPackage() {
  rcPackage.throttle = joystickAy;
  rcPackage.pitch = joystickBy;
  rcPackage.roll = joystickBx;
  rcPackage.yaw = joystickAx;

  byte tmp = 0;
  tmp |= joystickAb << 0;
  tmp |= buttonD << 3;  
  tmp |= joystickBb << 4;
  tmp |= buttonC << 7;
  rcPackage.control = tmp;

  rcPackage.checksum = 1; // placeholder
}

void sendPackage() {
  txSuccess = radio.write( &rcPackage, sizeof(rcPackage));
}


void printPackage(){
  Serial.print(rcPackage.throttle);Serial.print('\t');
  Serial.print(rcPackage.pitch);Serial.print('\t');
  Serial.print(rcPackage.roll);Serial.print('\t');
  Serial.print(rcPackage.yaw);Serial.print('\t');
  Serial.print(rcPackage.control);Serial.print('\t');
  Serial.print(rcPackage.checksum);Serial.print('\n');
}

