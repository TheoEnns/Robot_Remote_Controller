#ifndef RCP_MENU_HPP             		
#define RCP_MENU_HPP
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"

#include  "src/HyperDisplay\HyperDisplayLarge_4DLCD-320240_4WSPI.h" 

// MicroMod HyperDisplay Defines
#define PWM_PIN PWM0             
#define CS_PIN D0
#define DC_PIN D1
#define SPI_PORT SPI
#define SPI_SPEED 32000000    
#define SCREEN_HEIGHT   320
#define SCREEN_WIDTH    240
LCD320240_4WSPI myTFT;

// Color Defines
ILI9341_color_16_t color_white = myTFT.rgbTo16b( 255, 255, 255 );

ILI9341_color_16_t color_lightGrey = myTFT.rgbTo16b( 200, 200, 200 );
ILI9341_color_16_t color_lightRed = myTFT.rgbTo16b( 255, 100, 100 );
ILI9341_color_16_t color_lightGreen = myTFT.rgbTo16b( 100, 255, 100 );
ILI9341_color_16_t color_lightBlue = myTFT.rgbTo16b( 100, 100, 255 );

ILI9341_color_16_t color_red = myTFT.rgbTo16b( 255, 0, 0 );
ILI9341_color_16_t color_green = myTFT.rgbTo16b( 0, 255, 0 );
ILI9341_color_16_t color_blue = myTFT.rgbTo16b( 0, 0, 255 );
ILI9341_color_16_t color_grey = myTFT.rgbTo16b( 150, 150, 150 );

ILI9341_color_16_t color_darkRed = myTFT.rgbTo16b( 0xC0, 0, 0 );
ILI9341_color_16_t color_darkGreen = myTFT.rgbTo16b( 0, 0xC0, 0 );
ILI9341_color_16_t color_darkBlue = myTFT.rgbTo16b( 0, 0, 0xC0 );
ILI9341_color_16_t color_darkGrey = myTFT.rgbTo16b( 100, 100, 100 );

ILI9341_color_16_t color_black = myTFT.rgbTo16b( 0, 0, 0 );

// Main Menu Display Values
#define NUM_DISP_TOPICS 7
#define NUM_DISP_CATEGORY 5

#define TEXT_BOX_HEIGHT 20
#define TEXT_BOX_SPACING 2
#define CAT_BOX_WIDTH 46
#define CAT_BOX_SPACING 48
#define SELECTION_WIDTH 3

wind_info_t topicNameWind[NUM_DISP_TOPICS];
wind_info_t topicValueWind[NUM_DISP_TOPICS];
wind_info_t categoryWind[NUM_DISP_CATEGORY];
wind_info_t connectivityWind;
wind_info_t displayWind;
wind_info_t entryWind;

//Indexes For Display
RCP_cat_t currentCategory = RCP_CAT_OPERATIONS;
ID_t topDisplayID = 0;
rcp_size_t menuSelection = 0;
unsigned long errorCount = 0;
unsigned int heartbeatsPerSec = 0;
unsigned int ackPerSec = 0;

//Function Declares
void initializeDisplay();
// void initializeMainDisplay();
// void initializeEntryDisplay();
// void initializeLogDisplay();

void drawBaseDisplay();
void drawMainMenu();
void drawConnectivity();
void drawSelectionMenuEntry();
void drawIntegarEntry();
void drawDecimalEntry();
void drawStringEntry();
void drawBoolEntry();
void drawLogs();

void showMenu();
void showConnectivity();
void showEntry();

//Implementation
void initializeDisplay(){
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

  //Display Window
  //wind_info_t displayWind;
  myTFT.setWindowDefaults(&displayWind);
  displayWind.xMin = 0;
  displayWind.xMax = 239;
  displayWind.yMin = 0;
  displayWind.yMax = 319;
  displayWind.cursorX = 2;
  displayWind.cursorY = 4;
  displayWind.xReset = 0;
  displayWind.yReset = 0;
  myTFT.setWindowMemory(&displayWind, NULL, 
      (displayWind.xMax-displayWind.xMin+1)*
      (displayWind.yMax-displayWind.yMin+1),
      sizeof(ILI9341_color_16_t),false);
  myTFT.pCurrentWindow = &displayWind;     

  //Data Entry Window
  //wind_info_t entryWind;
  myTFT.setWindowDefaults(&entryWind);
  entryWind.xMin = 0;
  entryWind.xMax = 239;
  entryWind.yMin = TEXT_BOX_HEIGHT*(2)+1;
  entryWind.yMax = 149 + TEXT_BOX_HEIGHT*(2)+1;
  entryWind.cursorX = 2;
  entryWind.cursorY = 4;
  entryWind.xReset = 0;
  entryWind.yReset = 0;
  myTFT.setWindowMemory(&entryWind, NULL, 
      (entryWind.xMax-entryWind.xMin+1)*
      (entryWind.yMax-entryWind.yMin+1),
      sizeof(ILI9341_color_16_t),true);
  myTFT.pCurrentWindow = &entryWind;     
}

void drawBaseDisplay(){
  // Fresh Background
  myTFT.pCurrentWindow = NULL;
  myTFT.clearDisplay();    
  myTFT.setCurrentWindowColorSequence((color_t)&color_lightGrey);
  myTFT.fillWindow();
}

void drawMainMenu(){
  //Display Topics
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    myTFT.setWindowDefaults(&(topicNameWind[indx]));
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("Topic Name #");
    myTFT.print(indx);

    myTFT.setWindowDefaults(&(topicValueWind[indx]));
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("Topic Value #");
    myTFT.print(indx);
  }
  
  //Display Selection
  myTFT.pCurrentWindow = &topicValueWind[menuSelection];   
  myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);

  //Display Categories
  for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
    myTFT.setWindowDefaults(&(categoryWind[indx]));
    myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.print("CAT");
    myTFT.print(indx);   
  }
}

void drawConnectivity(){
  //Display Status
  myTFT.setWindowDefaults(&connectivityWind);
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print("ACTIVE 2/3hps 3err   ");
  //           123456789012345678901
}

void drawSelectionMenuEntry(){

}

void drawIntegarEntry(){

}

void drawDecimalEntry(){

}

void drawStringEntry(){

}

void drawBoolEntry(){

}

void drawLogs(){

}


void showMenu(){
  //Topics
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    myTFT.setWindowDefaults(&(topicNameWind[indx]));
    myTFT.show();

    myTFT.setWindowDefaults(&(topicValueWind[indx]));
    myTFT.show();
  }
  //Display Categories
  for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
    myTFT.setWindowDefaults(&(categoryWind[indx]));  
    myTFT.show();
  }
}

void showConnectivity(){
  myTFT.setWindowDefaults(&connectivityWind);
  myTFT.show();
}

void showEntry(){
  
}

/*
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
  */
#endif