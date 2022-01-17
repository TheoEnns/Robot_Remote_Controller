/*
Author: Theodore Enns
*/
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "RCP_Menu.hpp"
#include "RCP_Tests.hpp" 

RCPMenu rDisp;

void setup() {
  Serial.begin(115200);
  Serial.println("Startup...");
  initialize_Remote_Control_Topics();
  Serial.println("Init Complete");
  
  Serial.println("Runnning Tests...");
  testTopicGetAndSet();
  testTopicCounts();
  Serial.println("Tests Complete");

  rDisp.initializeDisplay();
  // drawBaserDisp();
  rDisp.drawMainMenu();
  rDisp.drawCategories();
  rDisp.drawConnectivity();
  rDisp.updateDisplay();
}

void loop() {
  delay(1000);
  unsigned long elapse = micros();
  rDisp.drawValues();
  rDisp.drawConnectivity();
  rDisp.updateDisplay();
  elapse = micros() - elapse;
  Serial.println(elapse);
}