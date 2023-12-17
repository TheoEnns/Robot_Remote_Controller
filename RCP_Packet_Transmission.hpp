#ifndef RCP_PACKET_TRANSIT_HPP             		
#define RCP_PACKET_TRANSIT_HPP

/* This class sends and receives packets of info accross the uart for over the air communication.
   This class does NOT serialize or deserialize objects or enforce format on the packets. Packets must be of at least 1 byte.
   Here we apply CR error correction and buffered traffic but no data resend.
   */

   #include <CRC.h> // see https://github.com/RobTillaart/CRC

class RCPRadio{
  private:
    Stream * SerailX;

    size_t bLength; //Size of send and receive buffers
    int finishedPacketLength = 0;

    uint8_t * sBuffer; //buffer of data to be sent
    size_t sLength = 0; //current size of data to be sent

    uint8_t * rBuffer; //buffer of data being received
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
    RCPRadio(size_t _bLength=512);
    ~RCPRadio();

    
    void sendPacketToSerial(uint8_t * packet, size_t pLength, bool flush=false);
    int readSerialIntoBuffer(); // returns the length of the packet found, 0 if no packet found yet

    uint8_t * getPacket();
    size_t getPacketLength();
};

RCPRadio::RCPRadio(size_t _bLength){
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
    Serial1.flush();
  }
}

void RCPRadio::sendBuffer(){
  Serial1.write(terminalByte);
  Serial1.write(sBuffer,sLength);
  uint bufferCRC = sCRC.calc();
  char bufferCRCHigh = bufferCRC>>8;
  char bufferCRCLow = bufferCRC & terminalByte;
  if(bufferCRCHigh==terminalByte){
    Serial1.write(escapeByte);
    // SerialUSB.println("INFO: Generated Escape Char for High CRC");
  }
  Serial1.write(bufferCRCHigh);
  if(bufferCRCLow==terminalByte){
    Serial1.write(escapeByte);
    // SerialUSB.println("INFO: Generated Escape Char for Low CRC");
  }
  Serial1.write(bufferCRCLow);
  Serial1.write(placeholderByte);
  Serial1.write(terminalByte);
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
    while(Serial1.available()){
      int input = Serial1.read();
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

  size_t RCPRadio::getPacketLength(){
    if(finishedPacketLength>0)
      return finishedPacketLength;
    else
      return 0;
  }

#endif //RCP_PACKET_TRANSIT_HPP