// Simple sketch for traffic lights with pedestrian cross by button press

// constants for cars led pins
const byte carsRedLedPin = 12;
const byte carsYellowLedPin = 13;
const byte carsGreenLedPin = 2;

// constants for pedestrian led pins
const byte pedestrianRedLedPin = 3;
const byte pedestrianGreenLedPin = 4;

const byte buttonPin = 11; // const for button pin
const byte variant = 11; // my variant number

bool buttonState = false; // variable to read button state
bool enablePedestrian;  // flag variable for pedestrian loop
unsigned int buttonTime; // variable to check how much time pass since waiting for pressing the button
int buttonWaitTime = 1000; // how much time we should wait for pressing the button and it's yellow led work duration time

void setup() {
  // setting leds pins pinMode to output
  pinMode(carsGreenLedPin, OUTPUT);
  pinMode(carsYellowLedPin, OUTPUT);
  pinMode(carsRedLedPin, OUTPUT);
  pinMode(pedestrianGreenLedPin, OUTPUT);
  pinMode(pedestrianRedLedPin, OUTPUT);
  pinMode(buttonPin, INPUT); // setting button pin pinMode to input

  digitalWrite(pedestrianRedLedPin, HIGH); // enabling pedestrian red led cause it should be enabled due to normal loop
}

void loop() {
  //enabling green cars led for (variant) seconds
  digitalWrite(carsGreenLedPin, HIGH);
  delay(variant * 1000);
  digitalWrite(carsGreenLedPin, LOW);

  digitalWrite(carsYellowLedPin, HIGH); // enabling yellow led 
  buttonTime = millis(); // remember arduino time before waiting button press loop
  while(millis() < buttonTime + buttonWaitTime) { 
    // the loop will be broken after (buttonWaitTime) since its began or manually after button press
    buttonState = digitalRead(buttonPin); // reading button state
    if(buttonState == HIGH) enablePedestrian = true;
  }
  digitalWrite(carsYellowLedPin, LOW); // ssetting yellow led off

  // checking if pedestrian lights must be used
  if(enablePedestrian) {
    // enabling red cars led, disabling pedestrian red, and anebling pedestrian green for (variant + 2) seconds
    digitalWrite(carsRedLedPin, HIGH);
    digitalWrite(pedestrianRedLedPin, LOW);
    digitalWrite(pedestrianGreenLedPin, HIGH);
    delay((variant + 2 ) * 1000);
    
    digitalWrite(pedestrianGreenLedPin, LOW); // disabling pedestrian green
    digitalWrite(pedestrianRedLedPin, HIGH); // enabling pedestrian red
    digitalWrite(carsRedLedPin, LOW); // disabling cars red
    enablePedestrian = false; // disabling pedestrian flag
  } else {
    // anabling cars red led for (variant) seconds
    digitalWrite(carsRedLedPin, HIGH);
    delay(variant * 1000);
    digitalWrite(carsRedLedPin, LOW); // disabling cars red led
  }

  // anabling cars yellow led for 1 seconds
  digitalWrite(carsYellowLedPin, HIGH);
  delay(1000);
  digitalWrite(carsYellowLedPin, LOW); // disabling cars yellow led
}
