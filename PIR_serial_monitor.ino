/*
 * PIR sensor tester for serial monitor
 */

#define PIR 2 

int pirState, counter = 0;  // we start, assuming no motion detected
int val = 0;  // variable for reading the pin status

void setup() {
  pinMode(PIR, INPUT);  // declare sensor as input
 
  Serial.begin(9600);
  Serial.println("PIR Sensor interface test!");
}

void loop(){
  delay(2000);
  PiRSensor();
}

void PiRSensor() {
  val = digitalRead(PIR);  // read input value
  if (val == HIGH) {
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = 1;
      Serial.println(pirState);
    }
  } else {
    if (pirState == HIGH) {
      // we have just turned off
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = 0;
      Serial.println(pirState);
    }
  }
}
