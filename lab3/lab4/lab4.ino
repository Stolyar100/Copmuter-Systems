// Simple sketch for traffic lights

// constants for led pins
const byte greenLedPin = 11;
const byte yellowLedPin = 12;
const byte redLedPin = 13;

// constants for led work time
const byte greenTime = 11;
const byte yellowTime = 1;
const byte redTime = 11;

// method for switching rad and green light
void switchLight(byte ledToDisable, byte ledToEnable, byte ledToEnableTime) {
  // disabling ledToDisable
  digitalWrite(ledToDisable, LOW);

  // enabling yeloow led and setting delay with value from const yellowTime
  digitalWrite(yellowLedPin, HIGH);
  delay(yellowTime * 1000);

  // disabling yellow
  digitalWrite(yellowLedPin, LOW);

  // enabling ledToEnable and setting delay with value ledToEnableTime
  digitalWrite(ledToEnable, HIGH);
  delay(ledToEnableTime * 1000);
}

void setup() {
  // setting led's pins pinMode to output
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  // initial enabling green led with it delay time
  digitalWrite(greenLedPin, HIGH);
  delay(greenTime * 1000);
}

void loop() {
  // switching green light to red 
  switchLight(greenLedPin, redLedPin, redTime);
  // switching red light to green
  switchLight(redLedPin, greenLedPin, greenTime);
}
