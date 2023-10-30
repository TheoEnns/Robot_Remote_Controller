#include "RCP_Topic.hpp"
#include "RCP_Core_Topics.hpp"
#include "RCP_Communications.hpp"

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
void handle_RCP_Traffic(binary_t* data, rcp_size_t length);

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

// size_t serialize_RCP_Topic(binary_t* data, rcp_size_t length. ){

// }

/*
void handle_RCP_Traffic(binary_t* data, rcp_size_t length){
// typedef enum{
//   RCP_TYPE_NULL=0,
//   RCP_TYPE_STRING,
//   RCP_TYPE_BYTE_ARRAY,
//   RCP_TYPE_FLOAT,
//   RCP_TYPE_LONG,
//   RCP_TYPE_INT,
//   RCP_TYPE_BOOL,
//   RCP_TYPE_CHAR,
//   RCP_TYPE_DOUBLE,
//   RCP_TYPE_MENU,
//   NUM_RCP_TYPES
// }RCP_type_t;

// typedef enum{
//   RCP_CAT_OPERATIONS=0,
//   RCP_CAT_CONFIGURATION,
//   RCP_CAT_STATUS,
//   RCP_CAT_LOGS,
//   RCP_CAT_SETTINGS,
//   RCP_CAT_HIDDENS,
//   NUM_CAT_TYPES
// }RCP_cat_t;

// typedef enum{
//   RCP_MSG_DISC_REQUEST=1,
//   RCP_MSG_ACK,
//   RCP_MSG_ANNOUNCE_TOPIC,
//   RCP_MSG_UPDATE_TOPIC,
//   RCP_MSG_RECOLOR_TOPIC,
//   // RCP_MSG_REQUEST_UPDATE,
//   RCP_MSG_HEARTBEAT,
//   RCP_MSG_LOG,
//   RCP_MSG_TONE,
//   NUM_RCP_MSG_TYPES
// }RCP_msg_type_t;

  switch(data[length-4]){
    case RCP_MSG_DISC_REQUEST:
      //Announce All Topics
      break;
    case RCP_MSG_ACK:
      //Keep an ACK Count?
      break;
    case RCP_MSG_ANNOUNCE_TOPIC:
          // length = 12; //  1 for type, 1 for topic cat, 1 for topic ID, 3 color bytes, 1 for name length, 1 for data length, 1 for msg type, 2 for CRC, 1 for end bit
          // length += _name_length + _size;
          // // data = (binary_t*)malloc((length)*sizeof(binary_t));
          // memcpy(data, _name, _name_length);
          // memcpy(data+_name_length, _data, _size);
          // data[length-1] = length;
          // data[length-4] = RCP_MSG_ANNOUNCE_TOPIC;
          // data[length-5] = _size;
          // data[length-6] = _name_length;
          // data[length-7] = _color_b;
          // data[length-8] = _color_g;
          // data[length-9] = _color_r;
          // data[length-10] = _id;
          // data[length-11] = _category;
          // data[length-12] = _type;

          // rcp_crc = crc_calculate(data, length-3);
          // data[length-2] = lowByte(rcp_crc);
          // data[length-3] =highByte(rcp_crc);
      int size = data[length-5];
      int nameLength = data[length-6];
      char * name = (char*)malloc(sizeof(char)*nameLength);

      rcp_size_t * currentIDs;
      RCPTopic ** array;
      getRCPArrayFromCategory(category, &currentIDs, &array);

      if(*currentIDs >= MAX_TOPIC_ID){
        Serial.println("ERROR: Cannnot Register New ID due to array limit");
        Serial.print("Tried to create ID ");
        Serial.println(data[length-10]);
        return;
      }
      if(*currentIDs != data[length-10]){
        Serial.println("ERROR: ID Mismatch in topic announce");
        Serial.print("Tried to create ID ");
        Serial.print(data[length-10]);
        Serial.print(" in category ");
        Serial.print(data[length-11]);
        Serial.print(" but expected ");
        Serial.println(*currentIDs);
        return;
      }

      ID_t newID = (ID_t) *currentIDs;
      (*currentIDs)++;

      RCPTopic* newTopic = new RCPTopic(category, newID, name, true);
      array[newID] = newTopic;
      newTopic->setColor( data[length-9], data[length-8], data[length-7])
      newTopic->rawDataSet(data[length-8], data, (rcp_size_t)size); data+_name_length
      break;
    case RCP_MSG_UPDATE_TOPIC:
          // data[length-1] = length;
          // data[length-4] = RCP_MSG_UPDATE_TOPIC;
          // data[length-5] = _size;
          // data[length-6] = _id;
          // data[length-7] = _category;
          // data[length-8] = _type;
      int size = data[length-5];
      rcp_size_t * currentIDs;
      RCPTopic ** array = NULL;
      getRCPArrayFromCategory(data[length-7], &currentIDs, &array);
      if(data[length-6]>=(binary_t)(*currentIDs)){
        Serial.println("ERROR: ID in update outside range");
        Serial.print("ERROR: ID is ");
        Serial.println(data[length-6]);
        Serial.print("ERROR: Category is ");
        Serial.println(data[length-7]);
        return;
      }
      RCPTopic* topic = array[data[length-6]];
      if(topic==NULL){
        Serial.println("ERROR: ID not found in update operation");
        return;
      }
      topic->rawDataSet(data[length-8], data, (rcp_size_t)size);
      break;
    case RCP_MSG_RECOLOR_TOPIC:
      break;
    case RCP_MSG_HEARTBEAT:
      break;
    case RCP_MSG_LOG:
      break;
    case RCP_MSG_TONE:
      break;
    default:
      break;
  }
}
*/

#endif //RCP_CONTROLLER_TOPIC_HPP

