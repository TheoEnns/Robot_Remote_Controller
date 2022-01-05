  // RCP_CAT_OPERATIONS=0,
  // RCP_CAT_CONFIGURATION,
  // RCP_CAT_STATUS,
  // RCP_CAT_LOGS,
  // RCP_CAT_SETTINGS,
  // NUM_CAT_TYPES
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

#include "RCP_Topics.hpp"

#ifndef RCP_CORE_TOPICS_HPP             		
#define RCP_CORE_TOPICS_HPP

  // RCP_CAT_STATUS
  RCPTopic* RCP_Device_Mode;
  void initStatus(){
    RCP_Device_Mode = CreateTopic(RCP_CAT_STATuS, "Device State");
    RCP_Device_Mode.setChar((char)RCP_MODE_IDLE);
  }

  // RCP_CAT_LOGS
  RCPTopic* LOGT_VERBOSE;
  RCPTopic* LOGT_INFO;
  RCPTopic* LOGT_WARN;
  RCPTopic* LOGT_ERROR;
  RCPTopic* LOGT_DEBUG;
  void initLogs(){
    LOGT_VERBOSE = CreateTopic(RCP_CAT_LOGS, "Verbose");
    LOGT_INFO = CreateTopic(RCP_CAT_LOGS, "Info");
    LOGT_WARN = CreateTopic(RCP_CAT_LOGS, "Warn");
    LOGT_ERROR = CreateTopic(RCP_CAT_LOGS, "Error");
    LOGT_DEBUG = CreateTopic(RCP_CAT_LOGS, "Debug");
  };

  // RCP_CAT_SETTINGS,
  RCPTopic* RCP_Device_Mode;
  RCPTopic* RCP_Client_Msg_Rate;
  RCPTopic* RCP_Controller_Msg_Rate;
  RCPTopic* RCP_;
  void initSettings(){
    = CreateTopic(RCP_CAT_SETTINGS, "");
    = CreateTopic(RCP_CAT_SETTINGS, "");
    = CreateTopic(RCP_CAT_SETTINGS, "");
    = CreateTopic(RCP_CAT_SETTINGS, "");
  };

  // RCP_CAT_HIDDENS,
  RCPTopic* ;
  void initHiddens(){
    = CreateTopic(RCP_CAT_HIDDENS, "");
  };

#endif //RCP_CORE_TOPICS_HPP