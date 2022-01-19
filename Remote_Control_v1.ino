/*
Author: Theodore Enns
*/

//Compile Flags for Debug
// #define DEBUG_CONTROL_IO 1
#define RUN_RCP_TESTS 1
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "RCP_Menu.hpp"
#include "RCP_IO_Configuration.hpp"
#include "RCP_Tests.hpp" 

RCPMenu rDisp;

unsigned long elapse;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Startup...");
  initialize_Remote_Control_Topics();
  initializeIO();
  rDisp.initializeDisplay();
  Serial.println("Init Complete");
  
#ifdef RUN_RCP_TESTS
  Serial.println("Runnning Tests...");
  testTopicGetAndSet();
  testTopicCounts();
  Serial.println("Tests Complete");
#endif

  rDisp.draw();
  rDisp.showDisplay();
}

void loop() {
  elapse = micros();
  readControllerIO();
  readDispIO();
  elapse = micros() - elapse;
  // Serial.print("IO Elapse: ");
  // Serial.println(elapse);

  RCP_Controller_HeartBeat_Rate->setFloat(RCP_Controller_HeartBeat_Rate->getFloat()+0.1);
  elapse = micros();
  rDisp.handleIO();
  rDisp.draw();
  rDisp.showDisplay();
  elapse = micros() - elapse;
  // Serial.print("Display Elapse: ");
  // Serial.println(elapse);

  // Serial.println("----\n\n");
}