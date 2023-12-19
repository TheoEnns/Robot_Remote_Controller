#include "RCP_Topic.hpp"

#ifndef RCP_COMMUNICATIONS_HPP             		
#define RCP_COMMUNICATIONS_HPP

/*
  typedef enum{
  RCP_MSG_DISC_REQUEST=1,
  RCP_MSG_ACK,
  RCP_MSG_ANNOUNCE_TOPIC,
  RCP_MSG_UPDATE_TOPIC,
  RCP_MSG_RECOLOR_TOPIC,
  RCP_MSG_REQUEST_UPDATE,
  RCP_MSG_HEARTBEAT,
  RCP_MSG_LOG,
  RCP_MSG_TONE,
  NUM_RCP_MSG_TYPES
}RCP_msg_type_t;
}RCP_msg_type_t;
RCP_END_BIT
*/

/* Encoding scheme brainstorm:

1 control bit + 7 payload:
   128 payload into 7 bits: 144 bytes
   1 length byte of 7 bits: 1 to 128
   1 type byte
   1 Index byte: 0 to 127

xFF as a terminal byte with 0xFF as an escape
   150*1/255 = .6 bytes added per 150 byte packet on average
   1 to 256 payload

   1 length byte: 1 to 256
   1 Index byte: 0 to 255
   1 type byte: High + low
        high:
          0 - Name
          1 - Color
          2 - Color + Name
          3 - Color + Name + Init values
        Low:
          0 - joysticks
          1 - bool
          2 - char
          3 - int
          4 - long
          5 - data of 1 128 length
          6 - string of 1 to 128 length
          15 - ACK
    0 or 1 byte for name length
    0 or 1 byte for data length
    0 to 3 bytes for color
    0 to 64 bytes for name
    0 to 128 bytes for data
    2 bytes for check sum

    total max compressed length = 1+1+1+1+1+3+64+128+2 = 202
*/

/*
  XBEE Transmission notes:
         205 bytes at 1 hz is very stable
         250 bytes with 1/36 excaspes at 1hz is not stable (drops everything); possible 256 buffer limit on UART/XBee
         205 bytes at 50 ms (20Hz) has either a 1/10 failure rate or a 9/10 failure rate for the full loop; probable not enough delay for transmission?
         205 bytes at 100 ms is very stable (!1/100 failed packets)
         64 at 50ms works very well (no drops)
         64 at 25ms does not work (not enough transmit time for the full loop most likely)
         128 at  50ms works well
         256 at 100 ms fails
         230 at 100 ms mostly work (1/10) fail
         256 at 1000 ms fails
         230 at 1000 ms works well
         230 at 100 ms works (maybe 1/20 fails)
         150 at 100 ms works well
         20 at 50 ms works well

         Conclusion: 2560 bytes per second is about tolerable
         Heartbeat at 20ms + full topic sends at 100ms and acks for transmision equate to full bandwidth well
  Redboard Turbo Processing notes:
    20 bytes packet length
      Send time at 921600 baud is ~184 microsends (<1 millis!) for 20 byte packets
      Receive time is ~155 microseconds 
    205 bytes packet length
      Send time at 921600 baud is ~1720  microsends (<2 millis!) for 205 byte packets
      Receive time is ~1380  microseconds 
*/

#include "RCP_Packet_Transmission.hpp"
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"


class RCPComm{
  private:
    RCPRadio* myRadio;
    RCPTopic* myHeartBeat;

    unsigned long msg_elapse=25;
    unsigned long heartbeat_elapse=50;
    int cycle_category = 0;
    int cycle_id = 0;

    RCPTopic* Get_Next_Publishable_Topic();

  public:
    RCPComm(RCPRadio* _myRadio, RCPTopic* _myHeartBeat);
    ~RCPComm();

    void receive_RCP_Msgs();
    void send_RCP_Msgs();
    void announce_RCP_Msgs();
};

extern void handle_RCP_Traffic(binary_t* data, rcp_size_t length);

RCPComm::RCPComm(RCPRadio* _myRadio, RCPTopic* _myHeartBeat){
  myRadio = _myRadio;
  myHeartBeat = _myHeartBeat;
}

RCPComm::~RCPComm(){
}

void RCPComm::receive_RCP_Msgs(){
  // handle_RCP_Traffic(binary_t* data, rcp_size_t length);
}

void RCPComm::send_RCP_Msgs(){
  binary_t* data = myRadio->get_sBuffer();
  rcp_size_t length = 0;
  if(heartbeat_elapse < millis()){
    heartbeat_elapse = RCP_HeartBeat_Rate->getInt() + millis();

    myHeartBeat->getMSG_publishTopic(data, length);
    myRadio->sendPacketToSerial(data, length, true);
    return;
  } else if(msg_elapse < millis()){
    msg_elapse = RCP_Msg_Rate->getInt() + millis();

    RCPTopic* staleTopic = Get_Next_Publishable_Topic();
    staleTopic->getMSG_publishTopic(data, length);
    myRadio->sendPacketToSerial(data, length, true);
    return;
  }
}

void RCPComm::announce_RCP_Msgs(){
  binary_t* data = myRadio->get_sBuffer();
  rcp_size_t length = 0;

  for(int category = 0; category < NUM_CAT_TYPES; category++){
    for(ID_t id = 0; id < MAX_TOPIC_ID ; id++){
      RCPTopic* topic = GetRCPTopic((RCP_cat_t)category, id);
      if(topic != NULL){
        if(topic->getTransmissible() && topic->getTransmissionFresh()) {
          topic->getMSG_publishTopic(data, length);
          myRadio->sendPacketToSerial(data, length, true);
          delay(RCP_Msg_Rate->getInt());
          receive_RCP_Msgs();
        }
      }
    }
  }
  return;   
}

RCPTopic* RCPComm::Get_Next_Publishable_Topic(){
  for(int i = 0; i < NUM_CAT_TYPES;i++){
    RCP_cat_t category = (RCP_cat_t)((i + cycle_category)%NUM_CAT_TYPES);
    for(ID_t id=cycle_id; id < MAX_TOPIC_ID ; id++){
      RCPTopic* topic = GetRCPTopic(category, id);
      if(topic != NULL){
        if(topic->getTransmissible() && topic->getTransmissionFresh()) {
          cycle_id = id;
          cycle_category = category;
          return topic;
        }
      }
    }
  }
  return NULL; // No topics to update
}

#endif //RCP_COMMUNICATIONS_HPP