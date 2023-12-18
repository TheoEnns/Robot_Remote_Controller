#include "RCP_Topic.hpp"

#ifndef RCP_COMMUNICATIONS_HPP             		
#define RCP_COMMUNICATIONS_HPP

#ifndef RCP_COMM
#define RCP_COMM Serial1 
#endif

/*
  typedef enum{
  RCP_MSG_DISC_REQUEST=1,
  RCP_MSG_ACK,
  RCP_MSG_ANNOUNCE_TOPIC,
  RCP_MSG_UPDATE_TOPIC,
  RCP_MSG_RECOLOR_TOPIC,
  // RCP_MSG_REQUEST_UPDATE,
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

#define RCP_sendBuffer_size 512
#define RCP_receiveBuffer_size 512
binary_t RCP_sendBuffer[RCP_sendBuffer_size];
binary_t RCP_receiveBuffer[RCP_receiveBuffer_size];
unsigned int  RCP_Rec_Index = 0;

extern void handle_RCP_Traffic(binary_t* data, rcp_size_t length);

bool readAck(binary_t lowCRC, binary_t highCRC);
crc_t readMessageBuffer();
bool send_MSG(int ackWait=0);
bool send_Discovery(binary_t lowCRC, binary_t highCRC, int ackWait=0);
bool send_Acknowledge(binary_t lowCRC, binary_t highCRC, int ackWait=0);
bool send_Announce(RCPTopic * topic, int ackWait=0);
bool send_Update(RCPTopic * topic, int ackWait=0);
bool send_Recolor(RCPTopic * topic, int ackWait=0);
bool send_Recolor(RCPTopic * topic, binary_t color_r, binary_t color_g, binary_t color_b, int ackWait=0);
bool send_HeartBeat(RCPTopic * heartBeat, int ackWait=0);
bool send_Log(String msg, RCPTopic *  logType, int ackWait=0);
bool send_Tone(int freq, int duration, int ackWait=0);

bool getMSG_Disc(binary_t* data, rcp_size_t &length);
bool getMSG_ACK(binary_t lowCRC, binary_t highCRC, binary_t* data, rcp_size_t &length);
bool getMSG_HeartBeat(RCPTopic * heartBeat, binary_t* data, rcp_size_t &length);
bool getMSG_LOG(String msg, RCPTopic* log, unsigned long time,  binary_t* data, rcp_size_t &length);
bool getMSG_TONE(int freq, int duration, binary_t* data, rcp_size_t &length);

bool readAck(binary_t lowCRC, binary_t highCRC){
  crc_t output = readMessageBuffer();
  if(lowByte(output) == lowCRC && highByte(output) == highCRC)
    return true;
  else
    return false;
}

crc_t readMessageBuffer(){
  while(RCP_COMM.available()){
    int byte = RCP_COMM.read();
    if(byte>0){
      RCP_receiveBuffer[RCP_Rec_Index] = byte;
      if(byte == RCP_END_BIT){
        int length = RCP_receiveBuffer[RCP_Rec_Index-1];
        int start = RCP_Rec_Index - length;
        if(RCP_MSG_ACK == RCP_receiveBuffer[RCP_Rec_Index-4]){
          rcp_crc = RCP_receiveBuffer[RCP_Rec_Index-3]<<8 + RCP_receiveBuffer[RCP_Rec_Index-2];
          RCP_Rec_Index = 0;
          return rcp_crc;
        }else{
          //crc check
          rcp_crc = crc_calculate(RCP_receiveBuffer+start, length-3);
          if(lowByte(rcp_crc) == RCP_receiveBuffer[RCP_Rec_Index-2] && highByte(rcp_crc) == RCP_receiveBuffer[RCP_Rec_Index-3]){
            handle_RCP_Traffic(RCP_receiveBuffer+start,length);
            RCP_Rec_Index = 0;
            return rcp_crc;
          }
        }
      }else{
        RCP_Rec_Index++;
        if(RCP_Rec_Index >= RCP_receiveBuffer_size){
          memcpy(RCP_receiveBuffer, RCP_receiveBuffer+256, 256);
          RCP_Rec_Index = 256;
        }
      }
    }
  }
  rcp_crc = 0;
  return rcp_crc;
}

bool send_MSG(rcp_size_t length, int ackWait){
  bool success = false;
  RCP_sendBuffer[length] = RCP_END_BIT;
  RCP_COMM.write(RCP_sendBuffer,length+1);
  unsigned long elapseAck = millis() + ackWait;
  do{
    success = readAck(RCP_sendBuffer[length-2], RCP_sendBuffer[length-3]);
  }while(success && elapseAck > millis());
  return success;
}

bool send_Discovery(int ackWait){
  rcp_size_t length;
  getMSG_Disc( RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Acknowledge(binary_t lowCRC, binary_t highCRC, int ackWait){
  rcp_size_t length;
  getMSG_ACK( lowCRC, highCRC, RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Announce(RCPTopic * topic, int ackWait){
  rcp_size_t length;
  topic->getMSG_announceTopic( RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Update(RCPTopic * topic, int ackWait){
  rcp_size_t length;
  topic->getMSG_publishTopic( RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Recolor(RCPTopic * topic, int ackWait){
  rcp_size_t length;
  topic->getMSG_changeColor( RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Recolor(RCPTopic * topic, binary_t color_r, binary_t color_g, binary_t color_b, int ackWait){
  rcp_size_t length;
  topic->getMSG_changeColor( color_r, color_g, color_b, RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_HeartBeat(RCPTopic * heartBeat, int ackWait){
  rcp_size_t length;
  getMSG_HeartBeat(heartBeat, RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Log(String msg, RCPTopic *  logType, int ackWait){
  rcp_size_t length;
  getMSG_LOG( msg, logType, millis(), RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}

bool send_Tone(int freq, int duration, int ackWait){
  rcp_size_t length;
  getMSG_TONE(freq, duration, RCP_sendBuffer, length);
  return send_MSG( length,  ackWait);
}


bool getMSG_Disc(binary_t* data, rcp_size_t &length){
  length = 4; //  1 for msg type, 2 for CRC, 1 for end bit
  data[length-1] = length;
  data[length-4] = RCP_MSG_DISC_REQUEST;
  rcp_crc = crc_calculate(data, length-3);
  data[length-2] = lowByte(rcp_crc);
  data[length-3] = highByte(rcp_crc);
  return true;
}

bool getMSG_ACK(binary_t lowCRC, binary_t highCRC, binary_t* data, rcp_size_t &length){
  length = 4; //  1 for msg type, 2 for CRC, 1 for end bit
  data[length-1] = length;
  data[length-2] = lowCRC;
  data[length-3] = highCRC;
  data[length-4] = RCP_MSG_ACK;
  return true;
}

bool getMSG_HeartBeat(RCPTopic * heartBeat, binary_t* data, rcp_size_t &length){
  heartBeat->getMSG_publishTopic(data, length);
  data[length-4] = RCP_MSG_HEARTBEAT;
  rcp_crc = crc_calculate(data, length-3);
  data[length-2] = lowByte(rcp_crc);
  data[length-3] = highByte(rcp_crc);
  return true;
}

bool getMSG_LOG(String msg, RCPTopic* log, unsigned long time,  binary_t* data, rcp_size_t &length){
  log->getMSG_publishTopic(data, length);

  data[length-4] = RCP_MSG_LOG;
  rcp_crc = crc_calculate(data, length-3);
  data[length-2] = lowByte(rcp_crc);
  data[length-3] = highByte(rcp_crc);
  return true;
}

bool getMSG_TONE(int freq, int duration, binary_t* data, rcp_size_t &length){
  length = 8; //  2 for freq, 2 for duration,  1 for msg type, 2 for CRC, 1 for end bit
  data[length-1] = length;
  data[length-4] = RCP_MSG_TONE;
  data[length-5] = lowByte(freq);
  data[length-6] = highByte(freq);
  data[length-7] = lowByte(duration);
  data[length-8] = highByte(duration);
  
  data[length-2] = lowByte(rcp_crc);
  data[length-3] = highByte(rcp_crc);
  return true;
}

#endif //RCP_COMMUNICATIONS_HPP