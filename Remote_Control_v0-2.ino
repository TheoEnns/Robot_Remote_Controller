/*
Author: Theodore Enns
*/
 
#include "RCP_Topic.hpp" 
#include "ControllerTopics.hpp"
#include "Menu.hpp" 



void setup() {
  Serial.begin(115200);

  Serial.println("Initializing Display");
  myTFT.begin(DC_PIN, CS_PIN, PWM_PIN, SPI_PORT, SPI_SPEED);  // This is a non-hyperdisplay function, but it is required to make the display work
  myTFT.setInterfacePixelFormat(ILI9341_PXLFMT_16);
  
  // Fresh Background
  myTFT.clearDisplay();    
  myTFT.setCurrentWindowColorSequence((color_t)&color_lightGrey);
  myTFT.fillWindow();

  //Display Topics
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    myTFT.setWindowDefaults(&(topicNameWind[indx]));
    topicNameWind[indx].xMin = 0;
    topicNameWind[indx].yMin = TEXT_BOX_HEIGHT*(2*indx)+1;
    topicNameWind[indx].xMax = 239;
    topicNameWind[indx].yMax = TEXT_BOX_HEIGHT*(2*indx)+TEXT_BOX_HEIGHT;
    topicNameWind[indx].cursorX = 4+SELECTION_WIDTH;
    topicNameWind[indx].cursorY = 3;
    topicNameWind[indx].xReset = 0;
    topicNameWind[indx].yReset = 0;
    myTFT.setWindowMemory(&topicNameWind[indx], NULL, 
        (topicNameWind[indx].xMax-topicNameWind[indx].xMin+1)*
        (topicNameWind[indx].yMax-topicNameWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &topicNameWind[indx];                                   
    myTFT.buffer(); 
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("Topic Name #");
    myTFT.print(indx);

    myTFT.setWindowDefaults(&(topicValueWind[indx]));
    topicValueWind[indx].xMin = 0;
    topicValueWind[indx].yMin = TEXT_BOX_HEIGHT*(2*indx+1)+1;
    topicValueWind[indx].xMax = 239;
    topicValueWind[indx].yMax = TEXT_BOX_HEIGHT*(2*indx+1)+TEXT_BOX_HEIGHT-TEXT_BOX_SPACING;
    topicValueWind[indx].cursorX = 4+SELECTION_WIDTH;
    topicValueWind[indx].cursorY = 1;
    topicValueWind[indx].xReset = 0;
    topicValueWind[indx].yReset = 0;
    myTFT.setWindowMemory(&topicValueWind[indx], NULL, 
        (topicValueWind[indx].xMax-topicValueWind[indx].xMin+1)*
        (topicValueWind[indx].yMax-topicValueWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &topicValueWind[indx];                                   
    myTFT.buffer(); 
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("Topic Value #");
    myTFT.print(indx);
  }
  
  //Display Selection
    myTFT.pCurrentWindow = &topicNameWind[0]; 
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    myTFT.print("12345678");
    //Topic Name #012345678"
    //123456789012345678901 --> 21 Characters
    myTFT.pCurrentWindow = &topicValueWind[0];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);

  //Display All Topic Windows
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    myTFT.pCurrentWindow = &topicNameWind[indx];        
    myTFT.show();  
    myTFT.pCurrentWindow = &topicValueWind[indx];         
    myTFT.show();
  }

  //Display Categories
  for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
    myTFT.setWindowDefaults(&(categoryWind[indx]));
    int offset = TEXT_BOX_HEIGHT*(2*NUM_DISP_TOPICS)+1;
    categoryWind[indx].xMin = CAT_BOX_SPACING*(indx)+1;
    categoryWind[indx].xMax = CAT_BOX_SPACING*(indx)+CAT_BOX_WIDTH;
    categoryWind[indx].yMin = offset;// 319 - TEXT_BOX_HEIGHT ;
    categoryWind[indx].yMax = offset + TEXT_BOX_HEIGHT; // 319;
    categoryWind[indx].cursorX = 2;
    categoryWind[indx].cursorY = 4;
    categoryWind[indx].xReset = 0;
    categoryWind[indx].yReset = 0;
    myTFT.setWindowMemory(&categoryWind[indx], NULL, 
        (categoryWind[indx].xMax-categoryWind[indx].xMin+1)*
        (categoryWind[indx].yMax-categoryWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &categoryWind[indx];                                   
    myTFT.buffer(); 
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("CAT");
    myTFT.print(indx);     
    myTFT.show();
  }

  //Display Status
  myTFT.setWindowDefaults(&connectivityWind);
  int connectivityOffset = TEXT_BOX_HEIGHT*(2*NUM_DISP_TOPICS+1)+2;
  connectivityWind.xMin = 1;
  connectivityWind.xMax = 238;
  connectivityWind.yMin = connectivityOffset;
  connectivityWind.yMax = 319;
  connectivityWind.cursorX = 2;
  connectivityWind.cursorY = 4;
  connectivityWind.xReset = 0;
  connectivityWind.yReset = 0;
  myTFT.setWindowMemory(&connectivityWind, NULL, 
      (connectivityWind.xMax-connectivityWind.xMin+1)*
      (connectivityWind.yMax-connectivityWind.yMin+1),
      sizeof(ILI9341_color_16_t),true);
  myTFT.pCurrentWindow = &connectivityWind;                                   
  myTFT.buffer(); 
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print("ACTIVE 2/3hps 3err   ");
  //           123456789012345678901
  myTFT.show();
}

void loop() {

}