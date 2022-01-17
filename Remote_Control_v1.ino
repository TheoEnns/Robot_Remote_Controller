/*
Author: Theodore Enns
*/
#include<SparkFun_MicroMod_Button.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h" 
#include<Wire.h>
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "RCP_Menu.hpp"
#include "RCP_Tests.hpp" 

#define RUN_RCP_TESTS 1

RCPMenu rDisp;
KEYPAD keypad; 
MicroModButton button;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Startup...");
  initialize_Remote_Control_Topics();
  if(!button.begin())
    Serial.println("Buttons not found");
  else
    Serial.println("Buttons connected!");
  if (keypad.begin(Wire, 0x4B) == false)
    Serial.println("Keypad does not appear to be connected. Please check wiring. Freezing...");
  else{
    char key = 1;
    while(key>0){
      keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
      key = keypad.getButton(); // Clears the event buffer since this stacks up a lot
    }
  }
  Serial.println("Init Complete");
  
#ifdef RUN_RCP_TESTS
  Serial.println("Runnning Tests...");
  testTopicGetAndSet();
  testTopicCounts();
  Serial.println("Tests Complete");
#endif

  rDisp.initializeDisplay();
  rDisp.drawBaseDisplay();
  rDisp.drawMainMenu();
  rDisp.drawCategories();
  rDisp.drawConnectivity();
  rDisp.updateDisplay();
}

unsigned long elapse;
void loop() {
  elapse = micros();
  keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
  char key = keypad.getButton();
  while(key>0){
    switch(key){
      case '2':
        //Serial.println("Button UP released!");
        rDisp.lowerSelection();
        break;
      case '4':
        //Serial.println("Button LEFT released!");
        rDisp.lowerCategory();
        break;
      case '6':
        //Serial.println("Button RIGHT released!");
        rDisp.raiseCategory();
        break;
      case '8':
        //Serial.println("Button DOWN released!");
        rDisp.raiseSelection();
        break;
    }
    keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
    key = keypad.getButton();
  }

  if(button.getClickedInterrupt()) {
    uint8_t clicked;
    clicked = button.getClicked(); 
    if(clicked & 0x01)
    {//Serial.println("Button A released!");
      
    }
    if(clicked & 0x02)
    {//Serial.println("Button B released!");
      
    }
    if(clicked & 0x04)
    {//up
      rDisp.lowerSelection();
    }
    if(clicked & 0x08)
    {//down
      rDisp.raiseSelection();
    }
    if(clicked & 0x10)
    {//left
      rDisp.lowerCategory();
    }
    if(clicked & 0x20)
    {//right
      rDisp.raiseCategory();
    }
    if(clicked & 0x40)
    {// Serial.println("Button CENTER released!");
      
    }
  }
  elapse = micros() - elapse;
  // Serial.print("Display Elapse: ");
  // Serial.println(elapse);


  RCP_Controller_HeartBeat_Rate->setFloat(RCP_Controller_HeartBeat_Rate->getFloat()+0.1);
  elapse = micros();
  rDisp.drawValues();
  rDisp.drawConnectivity();
  rDisp.updateDisplay();
  elapse = micros() - elapse;
  Serial.print("Display Elapse: ");
  Serial.println(elapse);
}