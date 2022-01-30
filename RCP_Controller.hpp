#include "RCP_Topic.hpp"
#include "RCP_Core_Topics.hpp"

#ifndef RCP_CONTROLLER_HPP             		
#define RCP_CONTROLLER_HPP

// RCP_CAT_OPERATIONS  // For operational control values, Client inits and then Controller and client update

// RCP_CAT_STATUS // For status values, Client inits and updates its own, Controller displays only
  
// RCP_CAT_LOGS // For log transmition, universal init but client updates, gets saved to SD card

// RCP_CAT_SETTINGS, For controller settings, universal init and controller updates
  RCPTopic* RCP_MUTE;
  RCPTopic* RCP_DISABLE_DISP_CLICKS;

  RCPTopic* RCP_JS_RU_CenterX;
  RCPTopic* RCP_JS_LU_CenterX;
  RCPTopic* RCP_JS_RL_CenterX;
  RCPTopic* RCP_JS_LL_CenterX;
  RCPTopic* RCP_JS_RU_CenterY;
  RCPTopic* RCP_JS_LU_CenterY;
  RCPTopic* RCP_JS_RL_CenterY;
  RCPTopic* RCP_JS_LL_CenterY;

// RCP_CAT_HIDDENS // For controller publication, universal init and controller updates but not user visible

void initialize_Remote_Controller_Topics();

// RCP Initialization
void initialize_Remote_Controller_Topics(){
  // RCP_CAT_STATUS
    
  // RCP_CAT_LOGS
    
  // RCP_CAT_SETTINGS, For controller settings, universal init and controller updates
    
    RCP_MUTE = CreateTopic(RCP_CAT_SETTINGS,                      "Mute Sound", false);
    RCP_DISABLE_DISP_CLICKS = CreateTopic(RCP_CAT_SETTINGS,       "Block Display Buttons", false);
    RCP_JS_RU_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Right CenterX", false);
    RCP_JS_RU_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Right CenterY", false);  
    RCP_JS_LU_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Left  CenterX", false);
    RCP_JS_LU_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Left  CenterY", false);  
    RCP_JS_RL_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Right CenterX", false);
    RCP_JS_RL_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Right CenterY", false);  
    RCP_JS_LL_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Left  CenterX", false);
    RCP_JS_LL_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Left  CenterY", false);
                                                                  //123456789012345678901 
    
    RCP_MUTE->setBool(true);
    RCP_DISABLE_DISP_CLICKS->setBool(false);
    RCP_Controller_Msg_Rate->setInt(12000);
    RCP_Controller_HeartBeat_Rate->setInt(7);

    RCP_JS_RU_CenterX->setInt(512);
    RCP_JS_LU_CenterX->setInt(512);
    RCP_JS_RL_CenterX->setInt(512);
    RCP_JS_LL_CenterX->setInt(512);
    RCP_JS_RU_CenterY->setInt(512);
    RCP_JS_LU_CenterY->setInt(512);
    RCP_JS_RL_CenterY->setInt(512);
    RCP_JS_LL_CenterY->setInt(512);

  // RCP_CAT_HIDDENS
  }

#endif //RCP_CONTROLLER_TOPIC_HPP

