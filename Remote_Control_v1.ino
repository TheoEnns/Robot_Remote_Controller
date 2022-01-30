/*
Author: Theodore Enns
*/

//Compile Flags for Debug
// #define DEBUG_CONTROL_IO 1
#define RUN_RCP_TESTS 1
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "RCP_Controller.hpp"
#include "RCP_Communications.hpp" 
#include "RCP_Menu.hpp"
#include "RCP_IO_Configuration.hpp"
#include "RCP_Tests.hpp" 
#include "ESP32_Sound.hpp" 

RCPMenu rDisp;

unsigned long elapse;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Startup...");
  initialize_Core_Topics();
  initialize_Remote_Controller_Topics(); // Must come after core topics
  initializeIO();
  rDisp.initializeDisplay();
  initEspSound();
  Serial.println("Init Complete");
  
#ifdef RUN_RCP_TESTS
  Serial.println("Runnning Tests...");
  testTopicGetAndSet();
  testTopicCounts();
  Serial.println("Tests Complete");
#endif

  rDisp.draw();
  rDisp.showDisplay();
  
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
}

void loop() {
  elapse = micros();
  readControllerIO();
  readDispIO();
  updateLEDStrip();
  updateSound();
  elapse = micros() - elapse;
  // Serial.print("IO Elapse: ");
  // Serial.println(elapse);

  // RCP_Controller_HeartBeat_Rate->setFloat(RCP_Controller_HeartBeat_Rate->getFloat()+0.1);
  elapse = micros();
  rDisp.handleIO();
  rDisp.draw();
  rDisp.showDisplay();
  elapse = micros() - elapse;
  // Serial.print("Display Elapse: ");
  // Serial.println(elapse);

  // Serial.println("----\n\n");
}