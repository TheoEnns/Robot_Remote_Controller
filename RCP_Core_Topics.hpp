#include "RCP_Topic.hpp"

#ifndef RCP_CORE_TOPICS_HPP             		
#define RCP_CORE_TOPICS_HPP

// RCP_CAT_OPERATIONS  // For operational control values, Client inits and then Controller and client update

// RCP_CAT_STATUS // For status values, Client inits and updates its own, Controller displays only
  RCPTopic* RCP_Device_Mode;
  
// RCP_CAT_LOGS // For log transmition, universal init but client updates, gets saved to SD card
  RCPTopic* LOGT_VERBOSE;
  RCPTopic* LOGT_INFO;
  RCPTopic* LOGT_WARN;
  RCPTopic* LOGT_ERROR;
  RCPTopic* LOGT_DEBUG;

// RCP_CAT_SETTINGS, For controller settings, universal init and controller updates
  RCPTopic* RCP_Command_StartStop;
  RCPTopic* RCP_Client_Msg_Rate;
  RCPTopic* RCP_Controller_Msg_Rate;
  RCPTopic* RCP_Controller_HeartBeat_Rate;
  RCPTopic* RCP_JS_RU_CenterX;
  RCPTopic* RCP_JS_LU_CenterX;
  RCPTopic* RCP_JS_RL_CenterX;
  RCPTopic* RCP_JS_LL_CenterX;
  RCPTopic* RCP_JS_RU_CenterY;
  RCPTopic* RCP_JS_LU_CenterY;
  RCPTopic* RCP_JS_RL_CenterY;
  RCPTopic* RCP_JS_LL_CenterY;

// RCP_CAT_HIDDENS // For controller publication, universal init and controller updates but not user visible
  typedef enum {
    RCP_CONTROLLER_LEFT_TWIST_COLOR=0,
    RCP_CONTROLLER_RIGHT_TWIST_COLOR,
    RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA,
    RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA,
    RCP_CONTROLLER_LED1_COLOR,
    RCP_CONTROLLER_LED2_COLOR,
    RCP_CONTROLLER_LED3_COLOR,
    RCP_CONTROLLER_LED4_COLOR,
    RCP_CONTROLLER_LED5_COLOR,
    RCP_CONTROLLER_LED6_COLOR,
    RCP_CONTROLLER_NUM_LEDS
  }rcp_controller_led_colors;

  RCPTopic* RCP_LED_Colors;
  RCPTopic* RCP_Right_Twist_Limit;
  RCPTopic* RCP_Left_Twist_Limit;
  RCPTopic* RCP_ControlHeartBeat;

void initialize_Remote_Control_Topics();

// RCP Initialization
void initialize_Remote_Control_Topics(){
  // RCP_CAT_STATUS
    String modeMenu = String("#LIVE\nSTOP\nIDLE\nESTOP\nWAIT\n");
    RCP_Device_Mode = CreateTopic(RCP_CAT_STATUS, "Device State", true);
    RCP_Device_Mode->setMenu((binary_t*)modeMenu.c_str(),modeMenu.length());
    RCP_Device_Mode->setMenuSelection(4);
    
  // RCP_CAT_LOGS
    LOGT_VERBOSE = CreateTopic(RCP_CAT_LOGS, "Verbose:", true);
    LOGT_INFO = CreateTopic(RCP_CAT_LOGS, "Info:", true);
    LOGT_WARN = CreateTopic(RCP_CAT_LOGS, "Warn:", true);
    LOGT_ERROR = CreateTopic(RCP_CAT_LOGS, "Error:", true);
    LOGT_DEBUG = CreateTopic(RCP_CAT_LOGS, "Debug:", true);
    
  // RCP_CAT_SETTINGS, For controller settings, universal init and controller updates
    RCP_Command_StartStop = CreateTopic(RCP_CAT_SETTINGS,         "Set Device Mode", true);
    RCP_Client_Msg_Rate = CreateTopic(RCP_CAT_SETTINGS,           "Client Limit Byte/s", true);
    RCP_Controller_Msg_Rate = CreateTopic(RCP_CAT_SETTINGS,       "Control Limit Byte/s", true);
    RCP_Controller_HeartBeat_Rate = CreateTopic(RCP_CAT_SETTINGS, "Control Message Rate", true);
    RCP_JS_RU_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Right CenterX", false);
    RCP_JS_RU_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Right CenterY", false);  
    RCP_JS_LU_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Left  CenterX", false);
    RCP_JS_LU_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Up  Left  CenterY", false);  
    RCP_JS_RL_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Right CenterX", false);
    RCP_JS_RL_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Right CenterY", false);  
    RCP_JS_LL_CenterX = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Left  CenterX", false);
    RCP_JS_LL_CenterY = CreateTopic(RCP_CAT_SETTINGS,              "JS Low Left  CenterY", false);
                                                                 //123456789012345678901 
    
    RCP_Command_StartStop->setMenu((binary_t*)modeMenu.c_str(),modeMenu.length());
    RCP_Command_StartStop->setMenuSelection(4);
    RCP_Client_Msg_Rate->setInt(12000); //Theoretical ~14k max but allow for some slop
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
    RCP_LED_Colors = CreateTopic(RCP_CAT_HIDDENS, "LED Colors", true);
    RCP_Right_Twist_Limit = CreateTopic(RCP_CAT_HIDDENS, "Right Twist Limit", true);
    RCP_Left_Twist_Limit = CreateTopic(RCP_CAT_HIDDENS, "Left Twist Limit", true);

    RCP_ControlHeartBeat = CreateTopic(RCP_CAT_HIDDENS, "Control Heartbeat", true);

    rcp_size_t numLEDs = RCP_CONTROLLER_NUM_LEDS;
    binary_t initLEDState[numLEDs*3] = { 
      0x0,0xFF,0x0, //RCP_CONTROLLER_LEFT_TWIST_COLOR,
      0x0,0xFF,0x0, //RCP_CONTROLLER_RIGHT_TWIST_COLOR,
      0x0,0x0,0x0, //RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA,
      0x0,0x0,0x0, //RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA,
      0x0,0x0,0x0, // RCP_CONTROLLER_LED1_COLOR,
      0x0,0x0,0x0, //RCP_CONTROLLER_LED2_COLOR,
      0x0,0x0,0x0, //RCP_CONTROLLER_LED3_COLOR,
      0x0,0x0,0x0, //RCP_CONTROLLER_LED4_COLOR,
      0x0,0x0,0x0, //RCP_CONTROLLER_LED5_COLOR,
      0x0,0x0,0x0}; //RCP_CONTROLLER_LED6_COLOR,
    RCP_LED_Colors->setByteArray(initLEDState,numLEDs);
    RCP_Right_Twist_Limit->setInt(0);
    RCP_Left_Twist_Limit->setInt(0);
  }

#endif //RCP_CORE_TOPICS_HPP