// Simple sketch for traffic lights with pedestrian cross by button press
#include <Servo.h>  // include library for working with servo
Servo myservo;  // create servo object to control a servo

// constants for cars led pins
const int carsRedLedPin = 6;
const int carsYellowLedPin = 5;
const int carsGreenLedPin = 4;

// constants for pedestrian led pins
const int pedestrianRedLedPin = 2;
const int pedestrianGreenLedPin = 12;

const int buttonPin = 10; // const for button pin
const int zoomerPin = 3; // const for zoomer pin
const int servoPin = 11; // const for servo pin
const int distanceSensorTriggerPin = 7; // const for distance sensor trigger pin
const int distanceSensorEchoPin = 8; // const for distance sensor echo pin
const int photoresistorPin = A0;  // const for photoresistor pin
const int temperatureSensorPin = A5;  // const for tmp36 pin
const int variant = 60; // my variant number
const int redGreenDuration = 60; // work duration time for red and green led
const int dangerDistance = 60; // value from distant sensor considered to be danger distance

bool buttonState = false; // variable to read button state
bool enablePedestrian;  // flag variable for pedestrian loop
bool enableDangerAlert;  // flag variable for danger alert loop
unsigned int dangerCheckTime; // variable to check how much time pass since checking distance

float getTemperature() {
  float sensorInput = analogRead(temperatureSensorPin); //read the analog sensor and store it
  float temp = (float)sensorInput / 1024; // find percentage of input reading
  temp = temp * 5;  // multiply by 5V to get voltage
  temp = temp - 0.5;  // subtract the offset 
  temp = temp * 100;  // convert to degrees
  return temp;
}

int getDinstance() {
  int duration, distance;

  digitalWrite(distanceSensorTriggerPin, LOW);  // set trigger pin to low for more accurate results
  delayMicroseconds(2); 

  digitalWrite(distanceSensorTriggerPin, HIGH); // set trigget pin to high

  delayMicroseconds(10);  // wait 10 μs 
  digitalWrite(distanceSensorTriggerPin, LOW); 

  duration = pulseIn(distanceSensorEchoPin, HIGH);  // get high signal duration on the echo pin
  distance = duration / 58; // copmuting distance
  return distance;
}

void setZoomer() {
  int photoresistorValue = analogRead(photoresistorPin);  // get value from photoresistor
  int toneValue = map(photoresistorValue, 0, 1023, 50, 20000);  // map it to a frequency range

  noTone(zoomerPin);  // disable previous tone
  tone(zoomerPin, toneValue); // enable new tone
}

void setZoomerDanger() {
  enableDangerAlert = true; // set danger alert flag to true
  
  noTone(zoomerPin);  // disable previous tone
  tone(zoomerPin, (50 + variant * 200)); // enable new tone
}

void openBarrier(int workTime) {
  for (int pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree	
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(workTime / 90);                        // waits worktime / 90 ms for the servo to reach the position
  }
}

void closeBarrier(int workTime) {
  for (int pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    // in steps of 1 degree	
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(workTime / 90);                        // waits worktime / 90 ms for the servo to reach the position
  }
}

void setup() {
  Serial.begin(9600); //Start the Serial Port at 9600 baud
  // setting leds pins pinMode to output
  pinMode(carsGreenLedPin, OUTPUT);
  pinMode(carsYellowLedPin, OUTPUT);
  pinMode(carsRedLedPin, OUTPUT);
  pinMode(pedestrianGreenLedPin, OUTPUT);
  pinMode(pedestrianRedLedPin, OUTPUT);

  pinMode(distanceSensorTriggerPin, OUTPUT); // setting distace sensor trigger pinMode to output
  pinMode(buttonPin, INPUT); // setting button pin pinMode to input
  pinMode(temperatureSensorPin, INPUT); // setting tmp36 pin pinMode to input

  myservo.attach(servoPin);  // attaches the servo on servoPin to the servo object

  digitalWrite(pedestrianRedLedPin, HIGH); // enabling pedestrian red led cause it should be enabled due to normal loop
  setZoomer(); // enabling zoomer with red pedestriab led
  closeBarrier(1350);
}

void loop() {
  setZoomer(); // re enabling zoomer to update tone frequency

  digitalWrite(carsGreenLedPin, HIGH);  // enable green cars led

  dangerCheckTime = millis(); // save time when checking distant loop begins
  while(millis() < dangerCheckTime + redGreenDuration * 1000) { 
    // the loop will be broken after (redGreenDuration)
    if(getDinstance() <= dangerDistance) {
      enableDangerAlert = true;
      setZoomerDanger();
    }
  }

  digitalWrite(carsGreenLedPin, LOW); // disable green cars led

  digitalWrite(carsYellowLedPin, HIGH); // enable yellow led 
  delay(2000);

  buttonState = digitalRead(buttonPin); // read button state
  if(buttonState == HIGH) {
    enablePedestrian = true;  // if button pressed set enablePedestrian flag to true
    openBarrier(3000);
  } else {
    delay(3000);
  }

  digitalWrite(carsYellowLedPin, LOW); // disable yellow led

  digitalWrite(carsRedLedPin, HIGH);  // enable red cars led
  // check if danger alert is enabled
  if (enableDangerAlert) {
    noTone(zoomerPin);  // disable danger alert zoomer
    enableDangerAlert = false;  // set danger alert flag to false
    setZoomer();  // enable zoomer as it must work due to pedestrian red led
  }
  
  // checking if pedestrian lights must be used
  if(enablePedestrian) {
    // enabling red cars led, disabling pedestrian red, and anebling pedestrian green
    digitalWrite(pedestrianRedLedPin, LOW);
    digitalWrite(pedestrianGreenLedPin, HIGH);
    noTone(zoomerPin);  // disable pedestrian red light zoomer
    delay(redGreenDuration * 1000);
    
    digitalWrite(pedestrianGreenLedPin, LOW); // disabling pedestrian green
    digitalWrite(pedestrianRedLedPin, HIGH); // enabling pedestrian red
    setZoomer(); // enabling pedestrian red light zoomer
    digitalWrite(carsRedLedPin, LOW); // disabling cars red
    digitalWrite(carsYellowLedPin, HIGH); // anabling cars yellow led
    closeBarrier(5000); // close barier
    enablePedestrian = false; // disabling pedestrian flag
  } else {
    // enabling cars red led
    delay(redGreenDuration * 1000);
    digitalWrite(carsRedLedPin, LOW); // disabling cars red led
    digitalWrite(carsYellowLedPin, HIGH); // anabling cars yellow led
    delay(5000);  // yellow light daley
  }

  digitalWrite(carsYellowLedPin, LOW); // disabling cars yellow led

  Serial.println("Current temperature: ");
  Serial.println(getTemperature());

  Serial.println("Current light level: ");
  Serial.println(analogRead(photoresistorPin));

  Serial.println("Current distance: ");
  Serial.println(getDinstance());
}
