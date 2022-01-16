
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"


#ifndef RCP_Tests_HPP             		
#define RCP_Tests_HPP

void testTopicCounts(){

  RCPTopic* RCP_Test_Topic1 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP_Test_Topic1"), true);
  RCPTopic* RCP_Test_Topic2 = CreateTopic(RCP_CAT_STATUS, String("RCP_Test_Topic2"), true);
  RCPTopic* RCP_Test_Topic3 = CreateTopic(RCP_CAT_LOGS, String("RCP_Test_Topic3"), true);
  RCPTopic* RCP_Test_Topic4 = CreateTopic(RCP_CAT_SETTINGS, String("RCP_Test_Topic4"), true);
  RCPTopic* RCP_Test_Topic5 = CreateTopic(RCP_CAT_HIDDENS, String("RCP_Test_Topic5"), true);
  Serial.println("Topic Count Test");
  Serial.print("Operations_RCP_Count : ");
  Serial.println(Operations_RCP_Count);
  Serial.print("Status_RCP_Count : ");
  Serial.println(Status_RCP_Count);
  Serial.print("Logs_RCP_Count : ");
  Serial.println(Logs_RCP_Count);
  Serial.print("Settings_RCP_Count : ");
  Serial.println(Settings_RCP_Count);
  Serial.print("Hiddens_RCP_Count : ");
  Serial.println(Hiddens_RCP_Count);
};

void testTopicGetAndSet(){
  RCPTopic* RCP_Test_Topic = CreateTopic(RCP_CAT_HIDDENS, String("RCP_Test_Topic"), true);
  
  for(int index=0;index<1;index++){ // Use this for memory leak testing
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());

    Serial.print("Testing RCP Get and Set on ");
    Serial.println(RCP_Test_Topic->getName());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());

    RCP_Test_Topic->setBool(true);
    Serial.print("Current value should read 1 :");
    Serial.println(RCP_Test_Topic->getBool());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());

    Serial.print("Current value should read 0 : ");
    Serial.println(RCP_Test_Topic->getInt());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());

    RCP_Test_Topic->setString("True");
    Serial.print("Current value should read True : ");
    Serial.println(RCP_Test_Topic->getString());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());

    RCP_Test_Topic->setString("0123456789012345678901234567890");
    Serial.print("Current value should read 0123456789012345678901234567890 :");
    Serial.println(RCP_Test_Topic->getString());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    
    RCP_Test_Topic->setFloat(123.12);
    Serial.print("Current value should read 123.12 : ");
    Serial.println(RCP_Test_Topic->getFloat());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    
    RCP_Test_Topic->setLong(12312388);
    Serial.print("Current value should read 12312388 : ");
    Serial.println(RCP_Test_Topic->getLong());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    
    RCP_Test_Topic->setChar('A');
    Serial.print("Current value should read A : ");
    Serial.println(RCP_Test_Topic->getChar());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    
    RCP_Test_Topic->setDouble(12312388.99);
    Serial.print("Current value should read 12312388.99 : ");
    Serial.println(RCP_Test_Topic->getDouble());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());

    
    String testMenu = String("#ACTIVE\nIDLE\nSTOP\nESTOP\n12345");
    RCP_Test_Topic->setMenu((binary_t*)testMenu.c_str(),testMenu.length());
    Serial.print("Current value should read '' : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    Serial.println(RCP_Test_Topic->getMenuSelection());
    RCP_Test_Topic->setMenuSelection(0);
    Serial.print("Changing to ACTIVE : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    RCP_Test_Topic->setMenuSelection(3);
    Serial.print("Changing to ESTOP : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
    RCP_Test_Topic->setMenuSelection(4);
    Serial.print("Changing to '' : ");
    Serial.println(RCP_Test_Topic->valueToDisplay());
  }
};


#endif //RCP_CONTROLLER_TOPIC_HPP