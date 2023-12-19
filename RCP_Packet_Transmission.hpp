#ifndef RCP_PACKET_TRANSIT_HPP             		
#define RCP_PACKET_TRANSIT_HPP

/* This class sends and receives packets of info accross the uart for over the air communication.
   This class does NOT serialize or deserialize objects or enforce format on the packets. Packets must be of at least 1 byte.
   Here we apply CR error correction and buffered traffic but no data resend.
   */

#include <CRC.h> // see https://github.com/RobTillaart/CRC

class RCPRadio{
  private:
    Stream * SerialX;

    int finishedPacketLength = 0;

    uint8_t * sBuffer; //buffer of data to be sent
    uint8_t * rBuffer; //buffer of data being received
    size_t bLength = 512; //Size of send and receive buffers
    size_t sLength = 0; //current size of data to be sent

    int lastRead = -1; //last received character, held for escape sequence parsing
    size_t readIdx = 0; //current length of received data

    const uint8_t terminalByte = 0xFF; // This byte value will clear the receive buffer 
                                      // and trigger a packet check
    const uint8_t escapeByte = 0xFE; // This byte value will trigger an escape on 0xFF inside the 
                                      // packet causing the terminal byte value following 0xFE to be ignored
    const uint8_t placeholderByte = 0x00;
                                      // This byte adds a dead zone space inbetween the CRC and the terminal 0xFF
                                      //  thus preventing a xFE CRC value from neighbouring the 0xFF byte

    CRC16 sCRC; //CRC calculation object for sending data
    CRC16 rCRC; //CRC calculation object for receiving data

    void sendBuffer();

  public:
    RCPRadio(Stream * _SerialX, size_t _bLength=512);
    ~RCPRadio();

    
    void sendPacketToSerial(uint8_t * packet, size_t pLength, bool flush=false);
    int readSerialIntoBuffer(); // returns the length of the packet found, 0 if no packet found yet

    uint8_t * getPacket();
    size_t getPacketLength();

    uint8_t * get_rBuffer();
    uint8_t * get_sBuffer();
    size_t get_buffer_length();
};

RCPRadio::RCPRadio(Stream * _SerialX, size_t _bLength){
  SerialX = _SerialX;
  bLength = _bLength;
  sBuffer = (uint8_t*)malloc(sizeof(uint8_t)*(bLength));
  rBuffer = (uint8_t*)malloc(sizeof(uint8_t)*(bLength));

  sCRC.reset();
  rCRC.reset();
}

RCPRadio::~RCPRadio(){
  free(sBuffer);
  free(rBuffer);
}

void RCPRadio::sendPacketToSerial(uint8_t * packet, size_t pLength, bool flush){
  int j = 0;
  sCRC.reset();
  for(int i = 0; i<pLength; i++){
    sCRC.add(packet[i]);
    if(j>=bLength)
      return;
    if(packet[i]==terminalByte){
      sBuffer[j] = escapeByte;
      j++;
      // SerialUSB.println("INFO: Generated Escape Char");
    }
    if(j>=bLength)
      return;
    sBuffer[j] = packet[i];
    j++;
  }
  sLength = j;

  sendBuffer();
  if(flush){
    SerialX->flush();
  }
}

void RCPRadio::sendBuffer(){
  SerialX->write(terminalByte);
  SerialX->write(sBuffer,sLength);
  uint bufferCRC = sCRC.calc();
  char bufferCRCHigh = bufferCRC>>8;
  char bufferCRCLow = bufferCRC & terminalByte;
  if(bufferCRCHigh==terminalByte){
    SerialX->write(escapeByte);
    // SerialUSB.println("INFO: Generated Escape Char for High CRC");
  }
  SerialX->write(bufferCRCHigh);
  if(bufferCRCLow==terminalByte){
    SerialX->write(escapeByte);
    // SerialUSB.println("INFO: Generated Escape Char for Low CRC");
  }
  SerialX->write(bufferCRCLow);
  SerialX->write(placeholderByte);
  SerialX->write(terminalByte);
  // Serial.print("INFO: Sent CRC of ");
  // Serial.print(bufferCRCHigh,DEC);
  // Serial.print(" ");
  // Serial.println(bufferCRCLow,DEC);
}

int RCPRadio::readSerialIntoBuffer(){ 
  finishedPacketLength = 0; // returning zero implies no packet is ready
  if(readIdx >= bLength){
    readIdx = 0;
  }
  while(SerialX->available()){
    int input = SerialX->read();
    if(input == terminalByte){
      if(lastRead == escapeByte){
        lastRead = terminalByte;
        // SerialUSB.println("INFO: Purged Escape Char");
      } else {
        if(readIdx > 3){
          rCRC.reset();
          char testCRCHigh = rBuffer[readIdx-2];
          char testCRCLow = rBuffer[readIdx-1];
          for(int i = 0; i < readIdx-2; i++){
            rCRC.add(rBuffer[i]);
          }
          uint bufferCRC = rCRC.calc();
          char bufferCRCHigh = bufferCRC>>8;
          char bufferCRCLow = bufferCRC & 0x00FF;
          // SerialUSB.print("INFO: Calc CRC of ");
          // SerialUSB.print(bufferCRCHigh,DEC);
          // SerialUSB.print(" ");
          // SerialUSB.println(bufferCRCLow,DEC);
          if((bufferCRCHigh == testCRCHigh) && (bufferCRCLow == testCRCLow))
            finishedPacketLength = readIdx-2;
          else{
            // SerialUSB.println("ERROR: CRC Mismatched");
            // SerialUSB.print("Expected CRC: ");
            // SerialUSB.print(testCRCHigh,DEC);
            // SerialUSB.print(" ");
            // SerialUSB.println(testCRCLow,DEC);
            // SerialUSB.print("Got CRC: ");
            // SerialUSB.print(bufferCRCHigh,DEC);
            // SerialUSB.print(" ");
            // SerialUSB.println(bufferCRCLow,DEC);
          }
        }
        readIdx = 0;
        lastRead = -1;
      }
    }else{
      if(lastRead != -1){
        rBuffer[readIdx] = (char)lastRead;
        readIdx++;
      }
      lastRead = input;
    }
  }
  return finishedPacketLength;
}

uint8_t * RCPRadio::getPacket(){
  return rBuffer;
}

uint8_t * RCPRadio::get_rBuffer(){
  return rBuffer;
}

uint8_t * RCPRadio::get_sBuffer(){
  return rBuffer;
}

size_t RCPRadio::get_buffer_length(){
  return bLength;
}

size_t RCPRadio::getPacketLength(){
  if(finishedPacketLength>0)
    return finishedPacketLength;
  else
    return 0;
}

#endif //RCP_PACKET_TRANSIT_HPP