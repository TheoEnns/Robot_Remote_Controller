#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"

#ifndef RCP_Tests_HPP             		
#define RCP_Tests_HPP

void testTopicCounts(){

  // RCPTopic* RCP_Test_Topic1 = CreateTopic(RCP_CAT_OPERATIONS, String("Empty_Test_Topic1"), true);
  // RCPTopic* RCP_Test_Topic2 = CreateTopic(RCP_CAT_STATUS, String("Empty_Test_Topic2"), true);
  // RCPTopic* RCP_Test_Topic3 = CreateTopic(RCP_CAT_LOGS, String("Empty_Test_Topic3"), true);
  // RCPTopic* RCP_Test_Topic4 = CreateTopic(RCP_CAT_SETTINGS, String("Empty_Test_Topic4"), true);
  // RCPTopic* RCP_Test_Topic5 = CreateTopic(RCP_CAT_HIDDENS, String("Empty_Test_Topic5"), true);
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
  RCPTopic* RCP_Test_Topic1 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 1"), true);
  RCP_Test_Topic1->setColor( 50,  0,  0);
  RCPTopic* RCP_Test_Topic2 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 2"), true);
  RCP_Test_Topic2->setColor( 150,  0,  0);
  RCPTopic* RCP_Test_Topic3 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 3"), true);
  RCP_Test_Topic3->setColor( 200,  0,  0);
  RCPTopic* RCP_Test_Topic4 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 4"), true);
  RCP_Test_Topic4->setColor( 0,  50,  0);
  RCPTopic* RCP_Test_Topic5 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 5"), true);
  RCP_Test_Topic5->setColor( 0,  150,  0);
  RCPTopic* RCP_Test_Topic6 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 6"), true);
  RCP_Test_Topic6->setColor( 0,  200,  0);
  RCPTopic* RCP_Test_Topic7 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 7"), true);
  RCP_Test_Topic7->setColor( 0,  0,  50);
  RCPTopic* RCP_Test_Topic8 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 8"), true);
  RCP_Test_Topic8->setColor( 0,  0,  150);
  RCPTopic* RCP_Test_Topic9 = CreateTopic(RCP_CAT_OPERATIONS, String("RCP Test Topic 9"), true);
  RCP_Test_Topic9->setColor( 0,  0,  200);
  
  for(int index=0;index<1;index++){ // Use this for memory leak testing
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());

    Serial.print("Testing RCP Get and Set on ");
    Serial.println(RCP_Test_Topic1->getName());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic1->valueToDisplay());

    RCP_Test_Topic1->setBool(true);
    Serial.print("Current value should read 1 :");
    Serial.println(RCP_Test_Topic1->getBool());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic1->valueToDisplay());

    RCP_Test_Topic2->setInt(1);
    Serial.print("Current value should read 0 : ");
    Serial.println(RCP_Test_Topic2->getBool());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic2->valueToDisplay());

    RCP_Test_Topic3->setString("True");
    Serial.print("Current value should read True : ");
    Serial.println(RCP_Test_Topic3->getString());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic3->valueToDisplay());

    RCP_Test_Topic4->setString("0123456789012345678901234567890");
    Serial.print("Current value should read 0123456789012345678901234567890 :");
    Serial.println(RCP_Test_Topic4->getString());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic4->valueToDisplay());
    
    RCP_Test_Topic5->setFloat(123.12);
    Serial.print("Current value should read 123.12 : ");
    Serial.println(RCP_Test_Topic5->getFloat());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic5->valueToDisplay());
    
    RCP_Test_Topic6->setLong(12312388);
    Serial.print("Current value should read 12312388 : ");
    Serial.println(RCP_Test_Topic6->getLong());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic6->valueToDisplay());
    
    RCP_Test_Topic7->setChar('A');
    Serial.print("Current value should read A : ");
    Serial.println(RCP_Test_Topic7->getChar());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic7->valueToDisplay());
    
    RCP_Test_Topic8->setDouble(12312388.99);
    Serial.print("Current value should read 12312388.99 : ");
    Serial.println(RCP_Test_Topic8->getDouble());
    Serial.print("Test Display : ");
    Serial.println(RCP_Test_Topic8->valueToDisplay());

    
    String testMenu = String("#ACTIVE\nIDLE\nSTOP\nESTOP\n12345");
    RCP_Test_Topic9->setMenu((binary_t*)testMenu.c_str(),testMenu.length());
    Serial.print("Current value should read '' : ");
    Serial.println(RCP_Test_Topic9->valueToDisplay());
    Serial.println(RCP_Test_Topic9->getMenuSelection());
    RCP_Test_Topic9->setMenuSelection(0);
    Serial.print("Changing to ACTIVE : ");
    Serial.println(RCP_Test_Topic9->valueToDisplay());
    RCP_Test_Topic9->setMenuSelection(3);
    Serial.print("Changing to ESTOP : ");
    Serial.println(RCP_Test_Topic9->valueToDisplay());
    RCP_Test_Topic9->setMenuSelection(4);
    Serial.print("Changing to '' : ");
    Serial.println(RCP_Test_Topic9->valueToDisplay());
    RCP_Test_Topic9->setMenuSelection(1);
    Serial.print("Changing to 'IDLE' : ");
    Serial.println(RCP_Test_Topic9->valueToDisplay());
  }
};


#endif //RCP_CONTROLLER_TOPIC_HPP