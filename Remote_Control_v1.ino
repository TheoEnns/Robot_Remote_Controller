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


unsigned long elapse;
char keypadV;
char dispClick;

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
    keypadV = 1;
    while(keypadV>0){
      keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
      keypadV = keypad.getButton(); // Clears the event buffer since this stacks up a lot
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
  rDisp.draw();
  rDisp.showDisplay();
}

void loop() {
  elapse = micros();
  if(button.getClickedInterrupt()) {
      dispClick = button.getClicked(); 
  }
  do{
    keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
    keypadV = keypad.getButton();

    

    rDisp.handleIO( keypadV, dispClick);
  }while(keypadV > 0);
  elapse = micros() - elapse;
  // Serial.print("IO Elapse: ");
  // Serial.println(elapse);

  RCP_Controller_HeartBeat_Rate->setFloat(RCP_Controller_HeartBeat_Rate->getFloat()+0.1);
  elapse = micros();
  rDisp.draw();
  rDisp.showDisplay();
  elapse = micros() - elapse;
  // Serial.print("Display Elapse: ");
  // Serial.println(elapse);
}