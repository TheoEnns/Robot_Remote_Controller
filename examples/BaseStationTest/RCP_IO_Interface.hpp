int16_t activeMode = false;
int16_t twistR_Count = 0;
int16_t twistL_Count = 0;
int16_t twistR_Press = 0;
int16_t twistL_Press = 0;
int16_t joystickRU_X = 512;
int16_t joystickLU_X = 512;
int16_t joystickRL_X = 512;
int16_t joystickLL_X = 512;
int16_t joystickRU_Y = 512;
int16_t joystickLU_Y = 512;
int16_t joystickRL_Y = 512;
int16_t joystickLL_Y = 512;
int16_t joystickRU_Press = 0;
int16_t joystickLU_Press = 0;
int16_t joystickRL_Press = 0;
int16_t joystickLL_Press = 0;

bool readFromPacket(uint8_t * packet, int length){
  if (length!=15){
    return false;
  }
  activeMode = (packet[0]&0b00000001);
  twistR_Count = ( packet[11]<<8) + packet[12];
  twistL_Count = ( packet[13]<<8) + packet[14];
  twistR_Press = (packet[0]&0b01000000)>>6;
  twistL_Press = (packet[0]&0b00100000)>>5;
  joystickRU_X = ((packet[1]&0b11000000)<<(8-6)) + packet[3];
  joystickLU_X = ((packet[1]&0b00110000)<<(8-4)) + packet[4];
  joystickRL_X = ((packet[1]&0b00001100)<<(8-2)) + packet[5];
  joystickLL_X = ((packet[1]&0b00000011)<<(8-0)) + packet[6];
  joystickRU_Y = ((packet[2]&0b11000000)<<(8-6)) + packet[7];
  joystickLU_Y = ((packet[2]&0b00110000)<<(8-4)) + packet[8];
  joystickRL_Y = ((packet[2]&0b00001100)<<(8-2)) + packet[9];
  joystickLL_Y = ((packet[2]&0b00000011)<<(8-0)) + packet[10];
  joystickRU_Press = (packet[0]&0b00010000)>>4;
  joystickLU_Press = (packet[0]&0b00001000)>>3;
  joystickRL_Press = (packet[0]&0b00000100)>>2;
  joystickLL_Press = (packet[0]&0b00000010)>>1;
}

void pretty_print_io_state(Stream * mSerial){
  mSerial->println(" "); 
  mSerial->print("activeMode ");
  mSerial->println(activeMode );
  mSerial->println(" "); 

  mSerial->print("twistR_Count ");
  mSerial->println(twistR_Count );
  mSerial->print("twistR_Press ");
  mSerial->println(twistR_Press );
  mSerial->println(" "); 

  mSerial->print("twistL_Count ");
  mSerial->println(twistL_Count );
  mSerial->print("twistL_Press ");
  mSerial->println(twistL_Press );
  mSerial->println(" "); 

  mSerial->print("joystickRU_X ");
  mSerial->println(joystickRU_X );
  mSerial->print("joystickRU_Y ");
  mSerial->println(joystickRU_Y );
  mSerial->print("joystickRU_Press ");
  mSerial->println(joystickRU_Press );
  mSerial->println(" "); 

  mSerial->print("joystickLU_X ");
  mSerial->println(joystickLU_X );
  mSerial->print("joystickLU_Y ");
  mSerial->println(joystickLU_Y );
  mSerial->print("joystickLU_Press ");
  mSerial->println(joystickLU_Press );
  mSerial->println(" "); 

  mSerial->print("joystickRL_X ");
  mSerial->println(joystickRL_X );
  mSerial->print("joystickRL_Y ");
  mSerial->println(joystickRL_Y );
  mSerial->print("joystickRL_Press ");
  mSerial->println(joystickRL_Press );
  mSerial->println(" "); 

  mSerial->print("joystickLL_X ");
  mSerial->println(joystickLL_X );
  mSerial->print("joystickLL_Y ");
  mSerial->println(joystickLL_Y );
  mSerial->print("joystickLL_Press ");
  mSerial->println(joystickLL_Press ); 
  mSerial->println(" "); 
  mSerial->println(" "); 
  mSerial->println(" ");  
}