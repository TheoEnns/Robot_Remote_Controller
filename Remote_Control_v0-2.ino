/*
Author: Theodore Enns
*/
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"
#include "Menu.hpp"
#include "RCP_Tests.hpp" 


void setup() {
  Serial.begin(115200);

  testTopicGetAndSet();
  testTopicCounts();
}

void loop() {

}