  // RCPTopic* rcpOperationsList[MAX_TOPIC_ID];      // For operational control values, Client inits and then Controller and client update
  // rcp_size_t Operations_RCP_Count = 0;
  // RCPTopic* rcpConfigurationsList[MAX_TOPIC_ID];  // For configuration values,  Client inits and then Controller updates
  // rcp_size_t Configurations_RCP_Count = 0;
  // RCPTopic* rcpStatusList[MAX_TOPIC_ID];          // For status values, Client inits and updates its own, Controller displays only
  // rcp_size_t Status_RCP_Count = 0;
  // RCPTopic* rcpLogsList[MAX_TOPIC_ID];            // For log transmition, universal init but client updates, gets saved to SD card
  // rcp_size_t Logs_RCP_Count = 0;
  // RCPTopic* rcpSettingsList[MAX_TOPIC_ID];        // For controller settings, universal init and controller updates
  // rcp_size_t Settings_RCP_Count = 0;
  // RCPTopic* rcpHiddensList[MAX_TOPIC_ID];         // For controller publication, universal init and controller updates but not user visible
  // rcp_size_t Hiddens_RCP_Count = 0;

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
  RCPTopic* RCP_JS_UR_Center;
  RCPTopic* RCP_JS_UL_Center;
  RCPTopic* RCP_JS_LR_Center;
  RCPTopic* RCP_JS_LL_Center;

// RCP_CAT_HIDDENS // For controller publication, universal init and controller updates but not user visible
  typedef enum {
    RCP_CONTROLLER_LEFT_TWIST_COLOR,
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
    RCP_Device_Mode = CreateTopic(RCP_CAT_STATUS, "Device State:", true);
    RCP_Device_Mode->setChar((char)RCP_MODE_IDLE);
    
  // RCP_CAT_LOGS
    LOGT_VERBOSE = CreateTopic(RCP_CAT_LOGS, "Verbose:", true);
    LOGT_INFO = CreateTopic(RCP_CAT_LOGS, "Info:", true);
    LOGT_WARN = CreateTopic(RCP_CAT_LOGS, "Warn:", true);
    LOGT_ERROR = CreateTopic(RCP_CAT_LOGS, "Error:", true);
    LOGT_DEBUG = CreateTopic(RCP_CAT_LOGS, "Debug:", true);
    
  // RCP_CAT_SETTINGS, For controller settings, universal init and controller updates
    RCP_Command_StartStop = CreateTopic(RCP_CAT_SETTINGS,              "Set Start/Stop:", true);
    RCP_Client_Msg_Rate = CreateTopic(RCP_CAT_SETTINGS,           "Client Limit Byte/s:", true);
    RCP_Controller_Msg_Rate = CreateTopic(RCP_CAT_SETTINGS,       "Control Limit Byte/s:", true);
    RCP_Controller_HeartBeat_Rate = CreateTopic(RCP_CAT_SETTINGS, "Control Message Rate", true);
    RCP_JS_UR_Center = CreateTopic(RCP_CAT_SETTINGS,              "Cal Upper Right Stick", false); 
    RCP_JS_UL_Center = CreateTopic(RCP_CAT_SETTINGS,              "Cal Upper Left Stick", false); 
    RCP_JS_LR_Center = CreateTopic(RCP_CAT_SETTINGS,              "Cal Lower Right Stick", false); 
    RCP_JS_LL_Center = CreateTopic(RCP_CAT_SETTINGS,              "Cal Lower Right Stick", false);
                                                                 //123456789012345678901 
    // = CreateTopic(RCP_CAT_SETTINGS, "", true);
    
    RCP_Command_StartStop->setBool(false);
    RCP_Client_Msg_Rate->setInt(12000); //Theoretical ~14k max but allow for some slop
    RCP_Controller_Msg_Rate->setInt(12000);
    RCP_Controller_HeartBeat_Rate->setInt(7);

    RCP_JS_UR_Center->setInt(512);
    RCP_JS_UL_Center->setInt(512);
    RCP_JS_LR_Center->setInt(512);
    RCP_JS_LL_Center->setInt(512);

  // RCP_CAT_HIDDENS
    RCP_LED_Colors = CreateTopic(RCP_CAT_HIDDENS, "LED Colors", true);
    RCP_Right_Twist_Limit = CreateTopic(RCP_CAT_HIDDENS, "Right Twist Limit", true);
    RCP_Left_Twist_Limit = CreateTopic(RCP_CAT_HIDDENS, "Left Twist Limit", true);

    RCP_ControlHeartBeat = CreateTopic(RCP_CAT_HIDDENS, "Control Heartbeat", true);

    rcp_size_t numLEDs = RCP_CONTROLLER_NUM_LEDS;
    binary_t initLEDState[numLEDs] = { 
      0xF,0xF,0xF,
      0xF,0xF,0xF,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0,
      0x0,0x0,0x0};
    RCP_LED_Colors->setByteArray(initLEDState,numLEDs);
    RCP_Right_Twist_Limit->setInt(0);
    RCP_Left_Twist_Limit->setInt(0);
  }

#endif //RCP_CORE_TOPICS_HPP