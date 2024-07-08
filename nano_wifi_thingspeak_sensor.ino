#include <WiFiNINA.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = "Enter_your_ssid";   // your network SSID (name) 
char pass[] = "enter_your_pwd";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = 1661894;
const char * myWriteAPIKey = "6IK733J0QJ4MFJM8";

// Initialize our values

#define PIR 2 

int pirState, counter = 0;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
void PiRSensor();

String myStatus = "";

void setup() {
  pinMode(PIR, INPUT);     // declare sensor as input
   
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

void loop() {
  delay(2000);
  PiRSensor();
  delay(2000);

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, pirState);
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
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
