#include "RCP_Packet_Transmission.hpp"
#include "RCP_IO_Interface.hpp"

RCPRadio mRadio;

void setup() {
  SerialUSB.begin(115200);
  Serial1.begin(921600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int length = mRadio.readSerialIntoBuffer();
  if(length>0){
    uint8_t * packet = mRadio.getPacket();
    if(readFromPacket(packet, length)){
      pretty_print_io_state(&SerialUSB);
    }
  }

  delay(5);
  // SerialUSB.print(".");
}
