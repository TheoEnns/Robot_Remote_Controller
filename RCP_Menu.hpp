#ifndef RCP_MENU_HPP             		
#define RCP_MENU_HPP
 
#include "RCP_Topic.hpp" 
#include "RCP_Core_Topics.hpp"

bool RIGHT_JUSTIFY_VALUES = true;

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

// Main Menu Display Dimensions
  #define NUM_DISP_TOPICS 7
  #define NUM_DISP_CATEGORY 5

  #define TEXT_BOX_HEIGHT 20
  #define TEXT_BOX_SPACING 2
  #define CAT_BOX_WIDTH 46
  #define CAT_BOX_SPACING 48
  #define SELECTION_WIDTH 3

//RCPMenu Class
class RCPMenu{
  private:

    // Main Menu Display Elements
    wind_info_t topicNameWind[NUM_DISP_TOPICS];
    bool topicNameWind_isStale[NUM_DISP_TOPICS];
    wind_info_t topicValueWind[NUM_DISP_TOPICS];
    bool topicValueWind_isStale[NUM_DISP_TOPICS];
    wind_info_t categoryWind[NUM_DISP_CATEGORY];
    bool categoryWind_isStale[NUM_DISP_CATEGORY];
    wind_info_t connectivityWind;
    bool connectivityWind_isStale;
    wind_info_t entryWind;
    bool entryWind_isStale;
    wind_info_t displayWind;

    bool doRedrawEntrySpace = false;
    bool doRedrawTopicSpace = true;
    bool doRedrawCategorySpace = true;
    bool doRedrawConnectivitySpace = true;

    //Indexes For Display
    RCP_cat_t currentCategory = RCP_CAT_OPERATIONS;
    rcp_size_t topDisplayID[NUM_DISP_CATEGORY];
    rcp_size_t menuSelection[NUM_DISP_CATEGORY];
    unsigned long errorCount = 0;
    unsigned int heartbeatsPerSec = 0;
    unsigned int ackPerSec = 0;

    //Entry Mode
    bool isEntryMode = false;
    RCP_type_t entryType;
    String entryValue;
    RCPTopic * entryTopic;
    void selectButton();
    void escapeButton();
    void backButton();
    void forwardButton();

  public:
    RCPMenu();
    ~RCPMenu();
    void initializeDisplay();

    //Process IO
    void handleIO(char keypad, char dispClick);

    //Main Draw Function
    void draw();

    //Draw Funstions
    void drawBaseDisplay();
    void drawMainMenu();
    void drawCategories();
    void drawCategory(int indx);
    void drawValues();
    void drawConnectivity();

    //Selected Entry Mode Functions
    void drawInitEntry();
    void clearEntry();
    void drawSelectionMenuEntry();
    void drawIntegarEntry();
    void drawDecimalEntry();
    void drawStringEntry();
    void drawBoolEntry();
    void drawLogs();

    //Show Functions
    void showDisplay();

    //State Changes
    void raiseSelection();
    void lowerSelection();
    void raiseCategory();
    void lowerCategory();
};

//Implementation
RCPMenu::RCPMenu(){
  // currentCategory = (RCP_cat_t)4;
  for(int indx=0;indx<menuSelection[NUM_DISP_CATEGORY];indx++){
    topDisplayID[indx] = 0;
    menuSelection[indx] = 0;
  }
}

RCPMenu::~RCPMenu(){
    //To Implement later if I ever need to since we aren't planning to free this class ever.
}

void RCPMenu::initializeDisplay(){
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
    topicNameWind[indx].cursorX = topicNameWind[indx].xReset = 4+SELECTION_WIDTH;
    topicNameWind[indx].cursorY = topicNameWind[indx].yReset = 3;
    // topicNameWind[indx].xReset = 0;
    // topicNameWind[indx].yReset = 0;
    myTFT.setWindowMemory(&topicNameWind[indx], NULL, 
        (topicNameWind[indx].xMax-topicNameWind[indx].xMin+1)*
        (topicNameWind[indx].yMax-topicNameWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &topicNameWind[indx];                                   
    myTFT.buffer(); 
    topicNameWind_isStale[indx] = true;
    
    myTFT.setWindowDefaults(&(topicValueWind[indx]));
    topicValueWind[indx].xMin = 0;
    topicValueWind[indx].yMin = TEXT_BOX_HEIGHT*(2*indx+1)+1;
    topicValueWind[indx].xMax = 239;
    topicValueWind[indx].yMax = TEXT_BOX_HEIGHT*(2*indx+1)+TEXT_BOX_HEIGHT-TEXT_BOX_SPACING;
    topicValueWind[indx].cursorX = topicValueWind[indx].xReset = 4+SELECTION_WIDTH;
    topicValueWind[indx].cursorY = topicValueWind[indx].yReset = 1;
    // topicValueWind[indx].xReset = 0;
    // topicValueWind[indx].yReset = 0;
    myTFT.setWindowMemory(&topicValueWind[indx], NULL, 
        (topicValueWind[indx].xMax-topicValueWind[indx].xMin+1)*
        (topicValueWind[indx].yMax-topicValueWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &topicValueWind[indx];                                   
    myTFT.buffer(); 
    topicValueWind_isStale[indx] = true;
  }
  
  //Display Categories
  for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
    myTFT.setWindowDefaults(&(categoryWind[indx]));
    int offset = TEXT_BOX_HEIGHT*(2*NUM_DISP_TOPICS)+1;
    categoryWind[indx].xMin = CAT_BOX_SPACING*(indx)+1;
    categoryWind[indx].xMax = CAT_BOX_SPACING*(indx)+CAT_BOX_WIDTH;
    categoryWind[indx].yMin = offset;// 319 - TEXT_BOX_HEIGHT ;
    categoryWind[indx].yMax = offset + TEXT_BOX_HEIGHT; // 319;
    categoryWind[indx].cursorX = categoryWind[indx].xReset = 2;
    categoryWind[indx].cursorY = categoryWind[indx].yReset = 4;
    // categoryWind[indx].xReset = 0;
    // categoryWind[indx].yReset = 0;
    myTFT.setWindowMemory(&categoryWind[indx], NULL, 
        (categoryWind[indx].xMax-categoryWind[indx].xMin+1)*
        (categoryWind[indx].yMax-categoryWind[indx].yMin+1),
        sizeof(ILI9341_color_16_t),true);
    myTFT.pCurrentWindow = &categoryWind[indx];                                   
    myTFT.buffer(); 
    categoryWind_isStale[indx] = true;
  }

  //Display Status
  myTFT.setWindowDefaults(&connectivityWind);
  int connectivityOffset = TEXT_BOX_HEIGHT*(2*NUM_DISP_TOPICS+1)+2;
  connectivityWind.xMin = 1;
  connectivityWind.xMax = 238;
  connectivityWind.yMin = connectivityOffset;
  connectivityWind.yMax = 319;
  connectivityWind.cursorX = connectivityWind.xReset = 2;
  connectivityWind.cursorY = connectivityWind.yReset = 4;
  // connectivityWind.xReset = 0;
  // connectivityWind.yReset = 0;
  myTFT.setWindowMemory(&connectivityWind, NULL, 
      (connectivityWind.xMax-connectivityWind.xMin+1)*
      (connectivityWind.yMax-connectivityWind.yMin+1),
      sizeof(ILI9341_color_16_t),true);
  myTFT.pCurrentWindow = &connectivityWind;                                   
  myTFT.buffer(); 
  connectivityWind_isStale = true;

  //Display Window
  //wind_info_t displayWind;
  myTFT.setWindowDefaults(&displayWind);
  displayWind.xMin = 0;
  displayWind.xMax = 239;
  displayWind.yMin = 0;
  displayWind.yMax = 319;
  displayWind.cursorX = displayWind.xReset = 2;
  displayWind.cursorY = displayWind.yReset = 4;
  // displayWind.xReset = 0;
  // displayWind.yReset = 0;
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
  entryWind.yMin = TEXT_BOX_HEIGHT*(4)+1;
  entryWind.yMax = TEXT_BOX_HEIGHT*(2*7)+1;//149 + TEXT_BOX_HEIGHT*(2)+1;
  entryWind.cursorX = entryWind.xReset = 2;
  entryWind.cursorY = entryWind.yReset = 4;
  // entryWind.xReset = 0;
  // entryWind.yReset = 0;
  myTFT.setWindowMemory(&entryWind, NULL, 
      (entryWind.xMax-entryWind.xMin+1)*
      (entryWind.yMax-entryWind.yMin+1),
      sizeof(ILI9341_color_16_t),true);
  myTFT.pCurrentWindow = &entryWind;                               
  myTFT.buffer();
  entryWind_isStale = false;   
  doRedrawEntrySpace = false;  
}

//Process IO
void RCPMenu::handleIO(char keypad, char dispClick){
  if(!isEntryMode){
    switch(keypad){
      case '2':
        //Serial.println("Button UP released!");
        lowerSelection();
        break;
      case '4':
        //Serial.println("Button LEFT released!");
        lowerCategory();
        break;
      case '6':
        //Serial.println("Button RIGHT released!");
        raiseCategory();
        break;
      case '8':
        //Serial.println("Button DOWN released!");
        raiseSelection();
        break;
    }
  }else{
      // Add entry specific 
      doRedrawEntrySpace = true;
  }
    
  if(dispClick & 0x01)
  {//Serial.println("Button A released!");
    selectButton();
  }
  if(dispClick & 0x02)
  {//Serial.println("Button B released!");
    if(!isEntryMode){
      selectButton();
      if(entryType == RCP_TYPE_BOOL)
        entryValue = entryValue.equals("1")?"0":"1";
      else if(entryType == RCP_TYPE_MENU)
        entryValue = String(0);
      else
        entryValue = "";
    }else
      escapeButton();
  }
  if(dispClick & 0x04)
  {//Serial.println("Button UP released!");
    if(!isEntryMode)
      lowerSelection();
    else
      backButton();
  }
  if(dispClick & 0x08)
  {//Serial.println("Button DOWN released!");
    if(!isEntryMode)
      raiseSelection();
    else
      forwardButton();
  }
  if(dispClick & 0x10)
  {// Serial.println("Button LEFT released!");
    if(!isEntryMode)
      lowerCategory();
    else
      backButton();
  }
  if(dispClick & 0x20)
  {// Serial.println("Button RIGHT released!");
    if(!isEntryMode)
      raiseCategory();
    else
      forwardButton();
  }
  if(dispClick & 0x40)
  {// Serial.println("Button CENTER released!");
    selectButton();
  }
}


void RCPMenu::selectButton(){
    if(!isEntryMode){
      isEntryMode = true;
      rcp_size_t * currentIDs;
      RCPTopic ** array = NULL;
      getArrayFromCategory(currentCategory, &currentIDs, &array);
      entryTopic = array[topDisplayID[currentCategory] + menuSelection[currentCategory]];
      entryType = entryTopic->getType();
      if(entryType == RCP_TYPE_BOOL)
        entryValue = String((int)entryTopic->getBool());
      else if(entryType == RCP_TYPE_MENU)
        entryValue = String((int)entryTopic->getMenuSelection());
      else
        entryValue = entryTopic->valueToDisplay();
      drawInitEntry();
      doRedrawEntrySpace = true;
    }else{
      isEntryMode = false;
      entryWind_isStale = false; 
      doRedrawEntrySpace = false;
      switch(entryType){
        case RCP_TYPE_NULL:
          break;
        case RCP_TYPE_STRING:
          entryTopic->setString(entryValue);
          break;
        case RCP_TYPE_BYTE_ARRAY:
          entryTopic->setByteArray((binary_t*)entryValue.c_str(),entryValue.length());
          break;
        case RCP_TYPE_FLOAT:
          entryTopic->setFloat(entryValue.toFloat());
          break;
        case RCP_TYPE_LONG:
          entryTopic->setLong((long)entryValue.toDouble());
          break;
        case RCP_TYPE_INT:
          entryTopic->setInt(entryValue.toInt());
          break;
        case RCP_TYPE_BOOL:
          entryTopic->setBool((bool)entryValue.toInt());
          break;
        case RCP_TYPE_CHAR:
          entryTopic->setChar((char)(entryValue.c_str()[0]));
          break;
        case RCP_TYPE_DOUBLE:
          entryTopic->setDouble(entryValue.toDouble());
          break;
        case RCP_TYPE_MENU:
          entryTopic->setMenuSelection(entryValue.toInt());
          break;
        default:
          break;
      }
      drawMainMenu();
    }
}

void RCPMenu::escapeButton(){
    isEntryMode = false; 
    entryWind_isStale = false; 
    doRedrawEntrySpace = false;
    drawMainMenu(); 
}

void RCPMenu::backButton(){
  int a;
  switch(entryType){
    case RCP_TYPE_NULL:
      break;
    case RCP_TYPE_STRING:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_BYTE_ARRAY:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_FLOAT:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_LONG:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_INT:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_BOOL:
      entryValue = entryValue.equals("1")?"0":"1";
      break;
    case RCP_TYPE_CHAR:
      a = (entryValue.c_str()[0])-1;
      entryValue.setCharAt(0,(char)(a<0?255:a));
      break;
    case RCP_TYPE_DOUBLE:
      entryValue.remove(entryValue.length()-1);
      break;
    case RCP_TYPE_MENU:
      a = entryValue.toInt()-1;
      entryValue = String(a<0?entryTopic->getMenuOptionMaxNum()-1:a);
      break;
    default:
      break;
  }
  doRedrawEntrySpace = true;
}

void RCPMenu::forwardButton(){
  int a;
  switch(entryType){
    case RCP_TYPE_NULL:
      break;
    case RCP_TYPE_STRING:
      break;
    case RCP_TYPE_BYTE_ARRAY:
      break;
    case RCP_TYPE_FLOAT:
      break;
    case RCP_TYPE_LONG:
      break;
    case RCP_TYPE_INT:
      break;
    case RCP_TYPE_BOOL:
      entryValue = entryValue.equals("1")?"0":"1";
      break;
    case RCP_TYPE_CHAR:
      a = (entryValue.c_str()[0])+1;
      entryValue.setCharAt(0,(char)(a>255?0:a));
      break;
    case RCP_TYPE_DOUBLE:
      break;
    case RCP_TYPE_MENU:
      a = entryValue.toInt()+1;
      entryValue = String(a>=entryTopic->getMenuOptionMaxNum()?0:a);
      break;
    default:
      break;
  }
  doRedrawEntrySpace = true;
}

//Main Draw Function
void RCPMenu::draw(){
  if(!isEntryMode){
    if(doRedrawTopicSpace){
      drawMainMenu();
    }else{
      drawValues();
    }
    if(doRedrawCategorySpace){
      drawCategories();
    }
  }else{
    if(doRedrawEntrySpace){
      switch(entryType){
        case RCP_TYPE_NULL:
          break;
        case RCP_TYPE_STRING:
          if(currentCategory == RCP_CAT_LOGS)
            drawLogs();
          else
            drawStringEntry();
          break;
        case RCP_TYPE_BYTE_ARRAY:
          drawStringEntry();
          break;
        case RCP_TYPE_FLOAT:
          drawDecimalEntry();
          break;
        case RCP_TYPE_LONG:
          drawIntegarEntry();
          break;
        case RCP_TYPE_INT:
          drawIntegarEntry();
          break;
        case RCP_TYPE_BOOL:
          drawBoolEntry();
          break;
        case RCP_TYPE_CHAR:
          drawStringEntry();
          break;
        case RCP_TYPE_DOUBLE:
          drawDecimalEntry();
          break;
        case RCP_TYPE_MENU:
          drawSelectionMenuEntry();
          break;
        default:
          break;
      }
    }
  }
  if(doRedrawConnectivitySpace){
    drawConnectivity();
  }
}

void RCPMenu::drawBaseDisplay(){
  // Fresh Background
  // myTFT.clearDisplay();   
  myTFT.setWindowDefaults(&displayWind); 
  myTFT.setCurrentWindowColorSequence((color_t)&color_lightGrey);
  myTFT.fillWindow();
}

void RCPMenu::drawMainMenu(){
  //Display Topics
  rcp_size_t * currentIDs;
  RCPTopic ** array = NULL;
  getArrayFromCategory(currentCategory, &currentIDs, &array);
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    if( (array != NULL) && (topDisplayID[currentCategory] + indx >= (*currentIDs)) ){
      myTFT.pCurrentWindow = (&(topicNameWind[indx]));
      myTFT.setCurrentWindowColorSequence((color_t)&color_black);
      myTFT.fillWindow();
      topicNameWind_isStale[indx] = true;

      myTFT.pCurrentWindow = (&(topicValueWind[indx]));
      myTFT.setCurrentWindowColorSequence((color_t)&color_black);
      myTFT.fillWindow();
      topicValueWind_isStale[indx] = true;
    }else{
      RCPTopic* newTopic = array[topDisplayID[currentCategory] + indx];
      ILI9341_color_16_t topic_color = myTFT.rgbTo16b( newTopic->getRed(), newTopic->getGreen(), newTopic->getBlue());
      myTFT.pCurrentWindow = (&(topicNameWind[indx]));
      myTFT.setCurrentWindowColorSequence((color_t)&topic_color);
      myTFT.fillWindow();
      myTFT.setCurrentWindowColorSequence((color_t)&color_white);
      myTFT.resetTextCursor();
      String textT = newTopic->getName().substring(0,21);
      myTFT.print(textT);
      // myTFT.print(newTopic->getNameRaw());
      topicNameWind_isStale[indx] = true;

      myTFT.pCurrentWindow = (&(topicValueWind[indx]));
      myTFT.setCurrentWindowColorSequence((color_t)&topic_color);
      myTFT.fillWindow();
      myTFT.setCurrentWindowColorSequence((color_t)&color_white);
      myTFT.resetTextCursor();
      String textV = newTopic->getString().substring(0,21);
      if(RIGHT_JUSTIFY_VALUES){
        for(int j = 21 - textV.length(); j > 0; j--)
          myTFT.print(' ');
      }
      myTFT.print(textV);
      // myTFT.print(newTopic->getString());
      topicValueWind_isStale[indx] = true;
    }
    //Display Selection
    if(indx == menuSelection[currentCategory]){
      myTFT.pCurrentWindow = &topicNameWind[menuSelection[currentCategory]];   
      myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
      myTFT.pCurrentWindow = &topicValueWind[menuSelection[currentCategory]];   
      myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    }
  }
  doRedrawTopicSpace = false;
}

void RCPMenu::drawCategories(){
  //Display Categories
  for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
    drawCategory(indx);
  }
  doRedrawCategorySpace = false;
}

void RCPMenu::drawCategory(int indx){
    myTFT.pCurrentWindow = (&(categoryWind[indx]));
    if(currentCategory == (RCP_cat_t)indx)
      myTFT.setCurrentWindowColorSequence((color_t)&color_blue);
    else
      myTFT.setCurrentWindowColorSequence((color_t)&color_black);
    myTFT.fillWindow();
    myTFT.setCurrentWindowColorSequence((color_t)&color_white);
    myTFT.resetTextCursor();
    if(indx==0)
      myTFT.print("OPS"); 
    else if(indx==1)
      myTFT.print("CONF"); 
    else if(indx==2)
      myTFT.print("STAT"); 
    else if(indx==3)
      myTFT.print("LOGS"); 
    else if(indx==4)
      myTFT.print("SETT"); 
    categoryWind_isStale[indx] = true;
}

void RCPMenu::drawValues(){
//Display Topics
  rcp_size_t * currentIDs;
  RCPTopic ** array = NULL;
  getArrayFromCategory(currentCategory, &currentIDs, &array);
  for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
    if( (array != NULL) && (topDisplayID[currentCategory] + indx >= (*currentIDs)) ){
      // Skip
    }else{
      RCPTopic* newTopic = array[topDisplayID[currentCategory] + indx];
      if(!newTopic->getFresh()){
        ILI9341_color_16_t topic_color = myTFT.rgbTo16b( newTopic->getRed(), newTopic->getGreen(), newTopic->getBlue());
        myTFT.pCurrentWindow = (&(topicValueWind[indx]));
        myTFT.setCurrentWindowColorSequence((color_t)&topic_color);
        myTFT.fillWindow();
        myTFT.setCurrentWindowColorSequence((color_t)&color_white);
        myTFT.resetTextCursor();
        String text = newTopic->getString().substring(0,21);
        if(RIGHT_JUSTIFY_VALUES){
          for(int j = 21 - text.length(); j > 0; j--)
            myTFT.print(' ');
        }
        myTFT.print(text);
        topicValueWind_isStale[indx] = true;
      }

      if(indx == menuSelection[currentCategory]){
        myTFT.pCurrentWindow = &topicValueWind[menuSelection[currentCategory]];   
        myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
      }
    }
  }
}

void RCPMenu::drawConnectivity(){
  //Display Status
  myTFT.pCurrentWindow = (&connectivityWind);
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.resetTextCursor();
  myTFT.print(RCP_Device_Mode->valueToDisplay());
  myTFT.print(" 2/3hps 3err   ");
  connectivityWind_isStale = true;
  doRedrawConnectivitySpace = false;
}

void RCPMenu::drawInitEntry(){
  myTFT.pCurrentWindow = (&(topicNameWind[0]));
  myTFT.setCurrentWindowColorSequence((color_t)&color_black);
  myTFT.fillWindow();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.resetTextCursor();
  String textT = entryTopic->getName().substring(0,21);
  myTFT.print(textT);
  myTFT.show();

  myTFT.pCurrentWindow = (&(topicValueWind[0]));
  myTFT.setCurrentWindowColorSequence((color_t)&color_black);
  myTFT.fillWindow();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.resetTextCursor();
  String textV = entryTopic->getString().substring(0,21);
  if(RIGHT_JUSTIFY_VALUES){
    for(int j = 21 - textV.length(); j > 0; j--)
      myTFT.print(' ');
  }
  myTFT.print(textV);
  myTFT.show();

  myTFT.pCurrentWindow = (&(topicNameWind[1]));
  myTFT.setCurrentWindowColorSequence((color_t)&color_black);
  myTFT.fillWindow(); 
  myTFT.show();

  myTFT.pCurrentWindow = (&(topicValueWind[1]));
  myTFT.setCurrentWindowColorSequence((color_t)&color_black);
  myTFT.fillWindow(); 
  myTFT.show();
}

void RCPMenu::clearEntry(){
  myTFT.pCurrentWindow = &entryWind; 
  myTFT.setCurrentWindowColorSequence((color_t)&color_lightGrey);
  myTFT.fillWindow();   
  entryWind_isStale = true;
  myTFT.show();
}

void RCPMenu::drawSelectionMenuEntry(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  int maxMenu = entryTopic->getMenuOptionMaxNum();
  for(int x=0;x < maxMenu; x++){
    String option = entryTopic->getMenuOption((binary_t)x);
    option = String(x) + String(": ") + option;
    myTFT.print(option);
    entryWind.cursorX = 2;
    entryWind.cursorY = 4 + 16*(x+1);
  }
  myTFT.print("Entry: ");
  myTFT.print(entryTopic->getMenuOption((binary_t)entryValue.toInt()));
  myTFT.print("_");
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::drawIntegarEntry(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print(entryValue);
  myTFT.print("_");
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::drawDecimalEntry(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print(entryValue);
  myTFT.print("_");
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::drawStringEntry(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print(entryValue);
  myTFT.print("_");
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::drawBoolEntry(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print(entryValue.equals("1")?"True":"False");
  myTFT.print("_");
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::drawLogs(){
  myTFT.pCurrentWindow = &entryWind;
  myTFT.setCurrentWindowColorSequence((color_t)&color_darkGrey);
  myTFT.fillWindow();   
  myTFT.resetTextCursor();
  myTFT.setCurrentWindowColorSequence((color_t)&color_white);
  myTFT.print(entryValue);
  entryWind_isStale = true;
  doRedrawEntrySpace = false;
}

void RCPMenu::showDisplay(){  
  if(isEntryMode){
    myTFT.pCurrentWindow = (&entryWind);
    myTFT.show();
    entryWind_isStale = false;
  }else{
    for(int indx = 0; indx < NUM_DISP_TOPICS; indx++){
      if(topicNameWind_isStale[indx]){
        myTFT.pCurrentWindow = (&(topicNameWind[indx]));
        myTFT.show();
        topicNameWind_isStale[indx] = false;
      }

      if(topicValueWind_isStale[indx]){
        myTFT.pCurrentWindow = (&(topicValueWind[indx]));
        myTFT.show();
        topicValueWind_isStale[indx] = false;
      }
    }
    
    for(int indx = 0; indx < NUM_DISP_CATEGORY; indx++){
      if(categoryWind_isStale[indx]){
        myTFT.pCurrentWindow = (&(categoryWind[indx]));  
        myTFT.show();
        categoryWind_isStale[indx] = false;
      }
    }
  }
  
  if(connectivityWind_isStale){
    myTFT.pCurrentWindow = (&connectivityWind);
    myTFT.show();
    connectivityWind_isStale = false;
  }
}

void RCPMenu::raiseSelection(){
  rcp_size_t new_selection = menuSelection[currentCategory] + 1;

  rcp_size_t * currentIDs;
  RCPTopic ** array = NULL;
  getArrayFromCategory(currentCategory, &currentIDs, &array);
  if(topDisplayID[currentCategory] + new_selection >= (*currentIDs))
    return;
  
  if(new_selection >= NUM_DISP_TOPICS){
    topDisplayID[currentCategory]++;
    drawMainMenu();
    return;
  }else{
    RCPTopic* oldTopic = array[topDisplayID[currentCategory] + menuSelection[currentCategory]];
    ILI9341_color_16_t topic_color = myTFT.rgbTo16b( oldTopic->getRed(), oldTopic->getGreen(), oldTopic->getBlue());

    myTFT.pCurrentWindow = &topicNameWind[menuSelection[currentCategory]];
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&topic_color);
    myTFT.pCurrentWindow = &topicValueWind[menuSelection[currentCategory]];
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&topic_color);
    topicNameWind_isStale[menuSelection[currentCategory]] = true;
    topicValueWind_isStale[menuSelection[currentCategory]] = true;

    myTFT.pCurrentWindow = &topicNameWind[new_selection];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    myTFT.pCurrentWindow = &topicValueWind[new_selection];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    menuSelection[currentCategory] = new_selection;
    topicNameWind_isStale[new_selection] = true;
    topicValueWind_isStale[new_selection] = true;
  }
}

void RCPMenu::lowerSelection(){
  rcp_size_t new_selection = menuSelection[currentCategory] -1;
  rcp_size_t * currentIDs;
  RCPTopic ** array = NULL;
  getArrayFromCategory(currentCategory, &currentIDs, &array);

  if(menuSelection[currentCategory] == 0){
    if(topDisplayID[currentCategory] == 0){
      return;
    } else {
      topDisplayID[currentCategory]--;
      new_selection = menuSelection[currentCategory] = 0;
      drawMainMenu();
      return;
    }
  }else{
    RCPTopic* oldTopic = array[topDisplayID[currentCategory] + menuSelection[currentCategory]];
    ILI9341_color_16_t topic_color = myTFT.rgbTo16b( oldTopic->getRed(), oldTopic->getGreen(), oldTopic->getBlue());

    myTFT.pCurrentWindow = &topicNameWind[menuSelection[currentCategory]];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&topic_color);
    myTFT.pCurrentWindow = &topicValueWind[menuSelection[currentCategory]];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&topic_color);
    topicNameWind_isStale[menuSelection[currentCategory]] = true;
    topicValueWind_isStale[menuSelection[currentCategory]] = true;

    myTFT.pCurrentWindow = &topicNameWind[new_selection];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    myTFT.pCurrentWindow = &topicValueWind[new_selection];   
    myTFT.rectangle(0, 0, SELECTION_WIDTH, myTFT.pCurrentWindow->yMax, true, (color_t)&color_white);
    menuSelection[currentCategory] = new_selection;
    topicNameWind_isStale[new_selection] = true;
    topicValueWind_isStale[new_selection] = true;
  }
}

void RCPMenu::raiseCategory(){
  RCP_cat_t newCategory = (RCP_cat_t)((int)currentCategory + 1);
  RCP_cat_t oldCategory = currentCategory;
  if(newCategory >= NUM_DISP_CATEGORY){
    newCategory = (RCP_cat_t)0;
  }
  currentCategory = newCategory;
  drawCategory(oldCategory);
  drawCategory(newCategory);
  drawMainMenu();
}

void RCPMenu::lowerCategory(){
  RCP_cat_t newCategory = (RCP_cat_t)((int)currentCategory - 1);
  RCP_cat_t oldCategory = currentCategory;
  if(newCategory < 0){
    newCategory = (RCP_cat_t)(NUM_DISP_CATEGORY - 1);
  }
  currentCategory = newCategory;
  drawCategory(oldCategory);
  drawCategory(newCategory);
  drawMainMenu();
}


#endif