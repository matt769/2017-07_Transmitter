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



void buildPackage() {
  rcPackage.throttle = joystickAy;
  rcPackage.pitch = joystickBy;
  rcPackage.roll = joystickBx;
  rcPackage.yaw = joystickAx;

  byte tmp = joystickBb << 1; // VALIDATE THIS
  tmp = tmp || joystickAb;    // VALIDATE THIS
  rcPackage.control = tmp;

  rcPackage.checksum = 1; // placeholder
}


void sendPackage() {
  txSuccess = radio.write( &rcPackage, sizeof(rcPackage));
}
