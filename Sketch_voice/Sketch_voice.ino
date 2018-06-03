/*
Rhok | Go To Sleep Easy 
This utilises Arduino Voice Recognition in conjuction with the Sheeld App.
User says "help" => LED turns on.
User says "off" => LED turns off.
*/

//Call the custom settings to optimize the flash memory usage
#define CUSTOM_SETTINGS
//Calling for Arduino Voice Recognize Shield
#define INCLUDE_VOICE_RECOGNIZER_SHIELD

//Including 1Sheeld library
#include <OneSheeld.h>

//Set commands to be recognized by the Arduino Voice Recognition Shield
const char on[] = "help";
const char off[] = "off";

//Set the lamp to pin 13
int lamp = 13;

void setup() {
  // Setup code here, to run once:
  //Initialize the communication between 1Sheeld's Arduino Voice Recognition Shield and Arduino
  OneSheeld.begin();
  //Set the lamp to be an Output
  pinMode(lamp,OUTPUT);
  Serial.println("Set up sucessful");
}

void loop() {
  // put your main code here, to run repeatedly:
  //check if 1Sheeld's Arduino Voice Recognition Shield received a new command
  if(VoiceRecognition.isNewCommandReceived())
  {
    //Compare the last command received by the Arduino Voice Recognition Shield with the command "on"
    if(!strcmp(on,VoiceRecognition.getLastCommand()))
    {
      //Then turn the light on
      digitalWrite(lamp,HIGH);
      Serial.println("Active");
      
    }
    //Compare the last command received by the Arduino Voice Recognition Shield with the command "off"
    else if(!strcmp(off,VoiceRecognition.getLastCommand()))
    {
      //Then turn the light off
      digitalWrite(lamp,LOW);
      Serial.println("Inactive");
    }
  }

}
