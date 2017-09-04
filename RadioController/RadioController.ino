// FIX LOOSE CONNECTION ON STICK B GND

// Read inputs (freq?)
// provide package for the radio (freq?)
// update LCD

// how long to the LCD updates take?

// ADD LATER
// setup/calibration
// store in EEPROM
// menus via LCD
// need to be able to go into menus while transmitting?
// have transmitting turned on by a switch or menu?
// battery monitor - need to check separately that is does not interfere with analog reads
// option to display input values on LCD
// get some status information in acknowledgement from receiver

// add option to show all input values


#include <ShiftedLCD.h> //allows using the LCD as if there were no shift register between in and ATMEGA
#include <SPI.h>
#include <RF24.h>
#include <EEPROM.h>



// INPUT VARIABLES
int joystickAx;
int joystickAy;
byte joystickAbTmp;
byte joystickAb;
int joystickBx;
int joystickBy;
byte joystickBbTmp;
byte joystickBb;
byte buttonCTmp;
byte buttonC;
byte buttonDTmp;
byte buttonD;

// OUTPUTS
const byte pinLedGreen = 5;
const byte pinLedRed = 6;

// LCD
byte pinLcdSlaveSelect = 10;
LiquidCrystal lcd(pinLcdSlaveSelect);

// RADIO
byte addresses[][6] = {"1Node", "2Node"};
bool radioNumber = 0; // receiver should be 1 (or anythin not 0)
RF24 radio(8, 7); // CE, CSN (SPI SS)

struct dataStruct {
  int inputOne; // number 0 to 1000
  int inputTwo;     // number 0 to 1000
  int inputThree;    // number 0 to 1000
  int inputFour;     // number 0 to 1000
  byte control; // for some control bits
  byte checksum;
} rcPackage;

int txFreq = 50; // in milliseconds i.e. txFreq = 100 ==> 1000/100 = 10Hz
// actual frequency will be slightly lower
unsigned long lastTransmission = 0;
unsigned long lastAcknowledgement = 0;
bool txSuccess = false;
byte ack = 0;

// STATES
enum states {STANDBY, TRANSMITTING, TX_ERROR};
enum states controllerState;



void setup() {

  Serial.begin(115200);

  setupInputs();
  setupOutputs();
  setupRadio();

  controllerState = TRANSMITTING;  // for testing

} // SETUP


void loop() {
  if (controllerState == TRANSMITTING) {
    readInputs();
    debounceInputs();
    mapInputs();
    buildPackage();
    
    if (millis() - lastTransmission > txFreq) {
      printInputs();
//      printPackage();
      sendPackage();
      lastTransmission = millis();
      //      printInputs();

      //      Serial.println(txSuccess);

//      Serial.print(F("Ack: "));
      if (radio.isAckPayloadAvailable()) {
        radio.read(&ack, sizeof(ack));
//        Serial.println(ack);
      }
      else {
//        Serial.println(F("None"));
      }

    }



  }
  updateOutputs();

} // LOOP









