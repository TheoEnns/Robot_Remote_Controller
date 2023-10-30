#ifndef RCP_IO_CONFIG_HPP             		
#define RCP_IO_CONFIG_HPP

#include <Wire.h>
#include "SparkFun_Qwiic_Twist_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_Twist
#include<SparkFun_MicroMod_Button.h>
#include "SparkFun_Qwiic_Keypad_Arduino_Library.h" 
#include "SparkFun_Qwiic_Joystick_Arduino_Library.h"
#include <APA102.h>

#include "RCP_Core_Topics.hpp"
#include "RCP_Controller.hpp"



// LED Strip
const uint8_t dataPin = G1;
const uint8_t clockPin = G0;
#define LED_STRIP_COUNT 6
APA102<dataPin, clockPin> ledStrip;

//Control IO
int hijackTwist = false;
int quickActiveMode = false;
int quickActiveModeDebounce = false;
int twistR_Count;
int twistL_Count;
int twistR_Press;
int twistL_Press;
int joystickRU_X;
int joystickLU_X;
int joystickRL_X;
int joystickLL_X;
int joystickRU_Y;
int joystickLU_Y;
int joystickRL_Y;
int joystickLL_Y;
int joystickRU_Press;
int joystickLU_Press;
int joystickRL_Press;
int joystickLL_Press;
TWIST twistR;  
TWIST twistL;  
JOYSTICK joystickRU;
JOYSTICK joystickLU;
JOYSTICK joystickRL;
JOYSTICK joystickLL;
KEYPAD keypad; 
MicroModButton button;

//Display IO
String keypadV = "";
char dispClick = 0;
int forwardTwist = 0;
int backwardTwist = 0;
bool shiftClick = false;
bool selectClick;
bool escapeClick;

//Log Data
#define MAX_LOGS 128
char VERBOSE_LOGS[MAX_LOGS][MAX_TOPIC_DATA_LENGTH+1];
char INFO_LOGS[MAX_LOGS][MAX_TOPIC_DATA_LENGTH+1];
char WARN_LOGS[MAX_LOGS][MAX_TOPIC_DATA_LENGTH+1];
char ERROR_LOGS[MAX_LOGS][MAX_TOPIC_DATA_LENGTH+1];
char DEBUG_LOGS[MAX_LOGS][MAX_TOPIC_DATA_LENGTH+1];
char VERBOSE_LOGS_IDX = 0;
char INFO_LOGS_IDX = 0;
char WARN_LOGS_IDX = 0;
char ERROR_LOGS_IDX = 0;
char DEBUG_LOGS_IDX = 0;

// Keypad Cycling
#define NUM_KEYS_ON_PAD 12
String IO_Keypad_Group_1 = "1ABCabc";
String IO_Keypad_Group_2 = "2DEFdef";
String IO_Keypad_Group_3 = "3GHIghi";
String IO_Keypad_Group_4 = "4JKLjkl";
String IO_Keypad_Group_5 = "5MNOmno";
String IO_Keypad_Group_6 = "6PQRpqr";
String IO_Keypad_Group_7 = "7STUstu";
String IO_Keypad_Group_8 = "8VWXvwx";
String IO_Keypad_Group_9 = "9YZyz";
String IO_Keypad_Group_S = "* .?!";
String IO_Keypad_Group_0 = "0@$%:";
String IO_Keypad_Group_P = "#-/^";
String IO_Keypad_Groups[12] = {
  IO_Keypad_Group_1,
  IO_Keypad_Group_2,
  IO_Keypad_Group_3,
  IO_Keypad_Group_4,
  IO_Keypad_Group_5,
  IO_Keypad_Group_6,
  IO_Keypad_Group_7,
  IO_Keypad_Group_8,
  IO_Keypad_Group_9,
  IO_Keypad_Group_S,
  IO_Keypad_Group_0,
  IO_Keypad_Group_P
};

void initializeIO();
void readControllerIO();
void readDispIO();
int centerJS(float pos, float center);
void acquireHijackTwist();
void releaseHijackTwist();
char cycleChar(char input);
bool sameCharGroup(char inputA, char inputB);
void updateLEDStrip();

void initializeIO(){
  if(!button.begin())
    Serial.println("Buttons not found");
  else
    Serial.println("Buttons connected!");
  if (keypad.begin(Wire, 0x4B) == false)
    Serial.println("Keypad does not appear to be connected. Please check wiring.");
  else{
    char key = 1;
    while(key != 0 && key !=-1){
      keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
      key = keypad.getButton(); // Clears the event buffer since this stacks up a lot
    }
  }
  if(joystickRU.begin(Wire, 0x2C) == false)
    Serial.println("Joystick Right Upper does not appear to be connected. Please check wiring.");
  else{
    RCP_JS_RU_CenterX->setInt(joystickRU.getHorizontal());
    RCP_JS_RU_CenterY->setInt(joystickRU.getVertical());
  }
  if(joystickLU.begin(Wire, 0x2E) == false)
    Serial.println("Joystick Left Upper does not appear to be connected. Please check wiring.");
  else{
    RCP_JS_LU_CenterX->setInt(joystickLU.getHorizontal());
    RCP_JS_LU_CenterY->setInt(joystickLU.getVertical());
  }
  if(joystickRL.begin(Wire, 0x2B) == false)
    Serial.println("Joystick Right Lower does not appear to be connected. Please check wiring.");
  else{
    RCP_JS_RL_CenterX->setInt(joystickRL.getHorizontal());
    RCP_JS_RL_CenterY->setInt(joystickRL.getVertical());
  }
  if(joystickLL.begin(Wire, 0x2A) == false)
    Serial.println("Joystick Left Lower does not appear to be connected. Please check wiring.");
  else{
    RCP_JS_LL_CenterX->setInt(joystickLL.getHorizontal());
    RCP_JS_LL_CenterY->setInt(joystickLL.getVertical());
  }
  rcp_size_t size;
  if (twistL.begin(Wire, 0x45) == false)
    Serial.println("Twist Left does not appear to be connected. Please check wiring. ");
  else{
    binary_t* array = RCP_LED_Colors->getByteArray(&size);
    twistL.setColor( array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3],
                     array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3+1],
                     array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3+2]); 
    twistL.setCount(0);
    array = RCP_LED_Colors->getByteArray(&size);
    twistL.connectColor(array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3],
                        array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3+1],
                        array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3+2]); 
    twistL.setLimit(0);
  }  
  if (twistR.begin(Wire, 0x46) == false)
    Serial.println("Twist Right does not appear to be connected. Please check wiring. ");
  else{
    binary_t* array = RCP_LED_Colors->getByteArray(&size);
    twistR.setColor( array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3],
                     array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3+1],
                     array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3+2]); 
    twistR.setCount(0);
    array = RCP_LED_Colors->getByteArray(&size);
    twistR.connectColor(array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3],
                        array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3+1],
                        array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3+2]); 
    twistR.setLimit(0);
  }
}

int centerJS(float pos, float center){
  if(pos > center)
    return constrain((int)(512.0*(pos - center)/(1024-center)+512.0),512,1023);
  else
    return constrain((int)(512.0*(pos/center)),0,512);
}

void readControllerIO(){
  joystickRU_X = centerJS(joystickRU.getHorizontal(), RCP_JS_RU_CenterX->getInt());
  joystickRU_Y = centerJS(joystickRU.getVertical(), RCP_JS_RU_CenterY->getInt());
  joystickRU_Press = joystickRU.getButton()==0;
  joystickLU_X = centerJS(joystickLU.getHorizontal(), RCP_JS_LU_CenterX->getInt());
  joystickLU_Y = centerJS(joystickLU.getVertical(), RCP_JS_LU_CenterY->getInt());
  joystickLU_Press = joystickLU.getButton()==0;
  joystickRL_X = centerJS(joystickRL.getHorizontal(), RCP_JS_RL_CenterX->getInt());
  joystickRL_Y = centerJS(joystickRL.getVertical(), RCP_JS_RL_CenterY->getInt());
  joystickRL_Press = joystickRL.getButton()==0;
  joystickLL_X = centerJS(joystickLL.getHorizontal(), RCP_JS_LL_CenterX->getInt());
  joystickLL_Y = centerJS(joystickLL.getVertical(), RCP_JS_LL_CenterY->getInt());
  joystickLL_Press = joystickLL.getButton()==0;

  if(!hijackTwist){
      twistR_Press = twistR.isClicked();
      twistL_Press = twistL.isClicked();
      twistR_Count = twistR.getCount();
      twistL_Count = twistL.getCount();
  }else{
      twistR_Press = 0;
      twistL_Press = 0;
  }

  if(twistR.isPressed() && twistL.isPressed()){
    if(quickActiveModeDebounce){
     quickActiveMode = !quickActiveMode;// This needs a debounce
     quickActiveModeDebounce = false;
    }
  }else{
    quickActiveModeDebounce = true;
  }

  //Schedule Message Send
  // 10 bits per JS = 5 bytes, 2 bytes per Twist = 4, 1 byte booleans = 10
  binary_t array[15];
  array[0] =  
    (binary_t)(twistR_Press<<7) + (binary_t)(twistL_Press<<6)
    + (binary_t)(joystickRU_Press<<5) + (binary_t)(joystickLU_Press<<4)
    + (binary_t)(joystickRL_Press<<3) + (binary_t)(joystickLL_Press<<2)
    + (binary_t)quickActiveMode;
  array[1] = highByte(joystickRU_X)<<6
    + highByte(joystickLU_X)<<4
    + highByte(joystickRL_X)<<2
    + highByte(joystickLL_X);
  array[2] = highByte(joystickRU_Y)<<6
    + highByte(joystickLU_Y)<<4
    + highByte(joystickRL_Y)<<2
    + highByte(joystickLL_Y);
  array[3] = lowByte(joystickRU_X);
  array[4] = lowByte(joystickLU_X);
  array[5] = lowByte(joystickRL_X);
  array[6] = lowByte(joystickLL_X);
  array[7] = lowByte(joystickRU_Y);
  array[8] = lowByte(joystickLU_Y);
  array[9] = lowByte(joystickRL_Y);
  array[10] = lowByte(joystickLL_Y);
  array[11] = highByte(twistR_Count);
  array[12] = lowByte(twistR_Count);
  array[13] = highByte(twistL_Count);
  array[14] = lowByte(twistL_Count);
  RCP_ControlHeartBeat->setByteArray(array,10);

#ifdef DEBUG_CONTROL_IO
  Serial.print(joystickRU_X);Serial.print(" \t");
  Serial.print(joystickLU_X);Serial.print(" \t");
  Serial.print(joystickRL_X);Serial.print(" \t");
  Serial.print(joystickLL_X);Serial.print(" \t");
  Serial.println(twistR_Count);

  Serial.print(joystickRU_Y);Serial.print(" \t");
  Serial.print(joystickLU_Y);Serial.print(" \t");
  Serial.print(joystickRL_Y);Serial.print(" \t");
  Serial.print(joystickLL_Y);Serial.print(" \t");
  Serial.println(twistL_Count);

  Serial.println(array[0],DEC);
#endif
}

void readDispIO(){
  // String keypadV = "";
  // String dispClick = "";
  // int forwardTwist = 0;
  // int backwardTwist = 0;
  // bool shiftClick = false;
  // bool selectClick;
  // bool escapeClick;

  if(hijackTwist){
      escapeClick = twistR.isClicked();
      selectClick = twistL.isClicked();
      forwardTwist = twistR.getDiff()+twistL.getDiff();
      if(forwardTwist<0){
        backwardTwist = -forwardTwist;
        forwardTwist = 0;
      }else{
        backwardTwist = 0;
      }
  }else{
    escapeClick = false;
    selectClick = false;
  }

  if(button.getClickedInterrupt() || RCP_DISABLE_DISP_CLICKS->getBool()) {
    dispClick = button.getClicked(); 
  }else
    dispClick = 0;
  char key = 0;
  keypadV = "";
  do{
    keypad.updateFIFO();  // necessary for keypad to pull button from stack to readable register
    key = keypad.getButton();
    if(key != 0 && key !=-1)
      keypadV.concat(key);
  }while(key != 0 && key !=-1);
}

void acquireHijackTwist(){
  twistL.setColor( 255,255,255); 
  twistL.setCount(twistR_Count);
  twistL.connectColor(0,0,0); 
  twistL.setLimit(0);
    
  twistR.setColor( 255,255,255); 
  twistR.setCount(twistL_Count);
  twistR.connectColor(0,0,0); 
  twistR.setLimit(0);
  hijackTwist = true;
}

void releaseHijackTwist(){
  rcp_size_t size;
  binary_t* array = RCP_LED_Colors->getByteArray(&size);
  twistL.setColor(  array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3],
                    array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3+1],
                    array[RCP_CONTROLLER_LEFT_TWIST_COLOR*3+2]); 
  twistL.setCount(twistR_Count);
  array = RCP_LED_Colors->getByteArray(&size);
  twistL.connectColor(array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3],
                      array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3+1],
                      array[RCP_CONTROLLER_LEFT_TWIST_COLOR_DELTA*3+2]); 
  twistL.setLimit(RCP_Right_Twist_Limit->getInt());
    
  twistR.setColor( array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3],
                    array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3+1],
                    array[RCP_CONTROLLER_RIGHT_TWIST_COLOR*3+2]); 
  twistR.setCount(twistL_Count);
  array = RCP_LED_Colors->getByteArray(&size);
  twistR.connectColor(array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3],
                      array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3+1],
                      array[RCP_CONTROLLER_RIGHT_TWIST_COLOR_DELTA*3+2]); 
  twistR.setLimit(RCP_Left_Twist_Limit->getInt());
  hijackTwist = false;
}

bool sameCharGroup(char inputA, char inputB){
  int x = 0;
  for(;x<NUM_KEYS_ON_PAD;x++){
    int index = IO_Keypad_Groups[x].indexOf(inputA);
    if(index !=-1)
    {  
      if(IO_Keypad_Groups[x].indexOf(inputB) != -1)
        return true;
      else
        return false;
    }
  }
  return false;
}

char cycleChar(char input){
  int x = 0;
  for(;x<NUM_KEYS_ON_PAD;x++){
    int index = IO_Keypad_Groups[x].indexOf(input);
    if(index !=-1){
      index++;
      if(index >= IO_Keypad_Groups[x].length())
        index = 0;
      return IO_Keypad_Groups[x].charAt(index);
    }
  }
  return input;
}

void updateLEDStrip(){
  rcp_size_t length;
  binary_t * array = RCP_LED_Colors->getByteArray(&length);
  // rgb_color colors[LED_STRIP_COUNT] = {
  //     rgb_color(0x00,0x00,0x00),
  //     rgb_color(0x00,0x00,0x00),
  //     rgb_color(0x00,0x00,0x00),
  //     rgb_color(0x00,0x00,0x00),
  //     rgb_color(0x00,0x00,0x00),
  //     rgb_color(0x00,0x00,0x00)
  //   };
  // rgb_color colors[LED_STRIP_COUNT] = {
  //     rgb_color(array[RCP_CONTROLLER_LED1_COLOR*3],array[RCP_CONTROLLER_LED1_COLOR*3+1],array[RCP_CONTROLLER_LED1_COLOR*3+2]),
  //     rgb_color(array[RCP_CONTROLLER_LED2_COLOR*3],array[RCP_CONTROLLER_LED2_COLOR*3+1],array[RCP_CONTROLLER_LED2_COLOR*3+2]),
  //     rgb_color(array[RCP_CONTROLLER_LED3_COLOR*3],array[RCP_CONTROLLER_LED3_COLOR*3+1],array[RCP_CONTROLLER_LED3_COLOR*3+2]),
  //     rgb_color(array[RCP_CONTROLLER_LED4_COLOR*3],array[RCP_CONTROLLER_LED4_COLOR*3+1],array[RCP_CONTROLLER_LED4_COLOR*3+2]),
  //     rgb_color(array[RCP_CONTROLLER_LED5_COLOR*3],array[RCP_CONTROLLER_LED5_COLOR*3+1],array[RCP_CONTROLLER_LED5_COLOR*3+2]),
  //     rgb_color(array[RCP_CONTROLLER_LED6_COLOR*3],array[RCP_CONTROLLER_LED6_COLOR*3+1],array[RCP_CONTROLLER_LED6_COLOR*3+2])
  //   };
  rgb_color colors[LED_STRIP_COUNT] = {
      rgb_color(array[4*3],array[4*3+1],array[4*3+2]),
      rgb_color(array[5*3],array[5*3+1],array[5*3+2]),
      rgb_color(array[6*3],array[6*3+1],array[6*3+2]),
      rgb_color(array[7*3],array[7*3+1],array[7*3+2]),
      rgb_color(array[8*3],array[8*3+1],array[8*3+2]),
      rgb_color(array[9*3],array[9*3+1],array[9*3+2])
    };
  for(int i = 4*3; i < (9*3+2);i++){
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(array[i]);
  }
  ledStrip.write(colors, LED_STRIP_COUNT);
}


#endif //RCP_IO_CONFIG_HPP