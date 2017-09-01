
// LEDs and LCD

void setupOutputs() {
  // set up outputs
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);
  digitalWrite(pinLedGreen, LOW);
  digitalWrite(pinLedRed, LOW);

  // LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("On");

}

void updateLcdHeader(){
  lcd.setCursor(0, 1);
  lcd.print("                ");  // blank line
  lcd.setCursor(0, 0);
}

void updateLcdFooter(){
  lcd.setCursor(0, 1);
  lcd.print("                "); // blank line
  lcd.setCursor(0, 1);
}


void updateOutputs() {
  if (controllerState == TRANSMITTING) {
    digitalWrite(pinLedGreen, HIGH); // Green LED will indicate there is a transmission
    digitalWrite(pinLedRed, !txSuccess);  // Turn on Red LED if transmission has failed
  }
  if (controllerState == STANDBY) {
    digitalWrite(pinLedGreen, HIGH);  // CHANGE THIS TO FLASH OR SOMETHING INSTEAD
    digitalWrite(pinLedRed, LOW);
  }

  // ADD LCD
  updateLcdHeader();  // Display current selection
  updateLcdFooter();  // Give feedback (if available)
  
}

void displayAllOutputs(){
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(joystickAx);
  lcd.setCursor(5, 0); lcd.print(joystickAy);
  lcd.setCursor(10, 0); lcd.print(joystickAb);
  lcd.setCursor(0, 1); lcd.print(joystickBx);
  lcd.setCursor(5, 1); lcd.print(joystickBy);
  lcd.setCursor(10, 1); lcd.print(joystickBb);
}



