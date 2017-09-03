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
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);

  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
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


byte calculateCheckSum(){
  byte sum = 0;
  sum += rcPackage.inputOne;
  sum += rcPackage.inputTwo;
  sum += rcPackage.inputThree;
  sum += rcPackage.inputFour;
  sum += rcPackage.control;
  sum = 1 - sum;
  return sum;
}




void buildPackage() {
  rcPackage.inputOne = joystickAy;
  rcPackage.inputTwo = joystickBx;
  rcPackage.inputThree = joystickBy;
  rcPackage.inputFour = joystickAx;

  byte tmp = 0;
  tmp |= joystickAb << 0;
  tmp |= buttonD << 3;  
  tmp |= joystickBb << 4;
  tmp |= buttonC << 7;
  rcPackage.control = tmp;

  rcPackage.checksum = calculateCheckSum(); // placeholder
}

void sendPackage() {
  radio.flush_rx();
  txSuccess = radio.write( &rcPackage, sizeof(rcPackage));
}


void printPackage(){
  Serial.print(rcPackage.inputOne);Serial.print('\t');
  Serial.print(rcPackage.inputTwo);Serial.print('\t');
  Serial.print(rcPackage.inputThree);Serial.print('\t');
  Serial.print(rcPackage.inputFour);Serial.print('\t');
  Serial.print(rcPackage.control);Serial.print('\t');
  Serial.print(rcPackage.checksum);Serial.print('\t');
}

