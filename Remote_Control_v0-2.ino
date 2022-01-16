/*
Author: Theodore Enns
*/
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "RCP_Menu.hpp"
#include "RCP_Tests.hpp" 


void setup() {
  Serial.begin(115200);
  Serial.println("Startup...");
  initialize_Remote_Control_Topics();
  Serial.println("Init Complete");
  
  Serial.println("Runnning Tests...");
  testTopicGetAndSet();
  testTopicCounts();
  Serial.println("Tests Complete");

  initializeDisplay();
  // drawBaseDisplay();
  drawMainMenu();
  drawConnectivity();
  showMenu();
  showConnectivity();
}

void loop() {
  delay(1000);
  unsigned long elapse = micros();
  currentCategory = (RCP_cat_t)4;//(RCP_cat_t)(((int)currentCategory+1)%5);
  drawMainMenu();
  showMenu();
  elapse = micros() - elapse;
  Serial.println(elapse);
}