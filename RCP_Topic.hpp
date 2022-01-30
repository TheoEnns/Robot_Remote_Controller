#ifndef RCP_TOPIC_HPP             		
#define RCP_TOPIC_HPP

#include <AceCRC.h> // https://github.com/bxparks/AceCRC

#ifndef ARDUINO_DUE
  #define float32_t float
  #define long64_t  long
  #define int32_t   int
  #define char8_t   char
  #define double64_t   double
#endif

typedef enum{
  RCP_MSG_DISC_REQUEST=1,
  RCP_MSG_ACK,
  RCP_MSG_ANNOUNCE_TOPIC,
  RCP_MSG_UPDATE_TOPIC,
  // RCP_MSG_REQUEST_UPDATE,
  RCP_MSG_HEARTBEAT,
  RCP_MSG_LOG,
  RCP_MSG_TONE,
  NUM_RCP_MSG_TYPES
}RCP_msg_type_t;

typedef enum{
  RCP_TYPE_NULL=0,
  RCP_TYPE_STRING,
  RCP_TYPE_BYTE_ARRAY,
  RCP_TYPE_FLOAT,
  RCP_TYPE_LONG,
  RCP_TYPE_INT,
  RCP_TYPE_BOOL,
  RCP_TYPE_CHAR,
  RCP_TYPE_DOUBLE,
  RCP_TYPE_MENU,
  NUM_RCP_TYPES
}RCP_type_t;

typedef enum{
  RCP_CAT_OPERATIONS=0,
  RCP_CAT_CONFIGURATION,
  RCP_CAT_STATUS,
  RCP_CAT_LOGS,
  RCP_CAT_SETTINGS,
  RCP_CAT_HIDDENS,
  NUM_CAT_TYPES
}RCP_cat_t;

typedef enum{
  RCP_MODE_LIVE=0,
  RCP_MODE_ESTOP,
  RCP_MODE_STOP,
  RCP_MODE_IDLE,
  RCP_MODE_WAIT,
  NUM_MODE_TYPES
}RCP_mode_t; // Note must fit in a char

typedef uint8_t ID_t;
typedef uint8_t rcp_size_t;
typedef uint8_t binary_t;

#define MAX_TOPIC_ID 256 //Maximum IDs that can be registered to a category
#define MAX_TOPIC_DATA_LENGTH 128
#define MAX_TOPIC_NAME_LENGTH 21
#define RCP_FP_PRES 7

class RCPTopic{
  private:
    RCP_cat_t   _category;
    ID_t        _id;
    RCP_type_t  _type;
    rcp_size_t     _name_length; 
    binary_t*   _name; 
    rcp_size_t     _size; 
    binary_t*   _data; 
    bool        _isDataFresh;
    bool        _isDisplayFresh;
    bool        _doesTransmit;
    // void        freeData();

    String _displayText;
    String _displayName;
    binary_t _color_r;		         
    binary_t _color_g;		         
    binary_t _color_b;

  public:
    RCPTopic(RCP_cat_t category, ID_t id, String name, bool doesTransmit);
    ~RCPTopic();

    // void setName(String name);
    // void setName(binary_t data[], rcp_size_t name_length);
    void setString(String data);
    void setByteArray(binary_t* data, rcp_size_t length); //does not copy!
    void setFloat(float32_t data);
    void setLong(long64_t data);
    void setInt(int32_t data);
    void setBool(bool data);
    void setChar(char8_t data);
    void setDouble(double64_t data);
    void setMenu(binary_t* data, rcp_size_t length);
    void setMenuSelection(binary_t selection);
    void setFresh();

    String getName();
    char* getNameRaw();
    rcp_size_t getNameLength();
    RCP_type_t getType();
    String getString();
    binary_t* getByteArray(rcp_size_t* length); //not a copy
    float32_t getFloat();
    long64_t getLong();
    int32_t getInt();
    bool getBool();
    char8_t getChar();
    double64_t getDouble();
    binary_t getMenuSelection();  
    binary_t getMenuOptionMaxNum();
    String getMenuOption(binary_t option);
    String getMenuOptions();

    bool getFresh();

    String valueToDisplay();
    void setColor(binary_t color_r, binary_t color_g, binary_t color_b);
    binary_t getRed();
    binary_t getGreen();
    binary_t getBlue();

    bool getMSG_announceTopic(binary_t* data, rcp_size_t* length);
    bool getMSG_publishTopic(binary_t* data, rcp_size_t* length);
    bool getMSG_changeColor(binary_t color_r, binary_t color_g, binary_t color_b, binary_t* data, rcp_size_t* length);
    bool getMSG_changeColor(binary_t* data, rcp_size_t* length);
};

RCPTopic* CreateTopic(RCP_cat_t category, String name, bool doesTransmit);
void getRCPArrayFromCategory(RCP_cat_t category, rcp_size_t** size, RCPTopic**topicList);


RCPTopic* rcpOperationsList[MAX_TOPIC_ID];      // For operational control values, Client inits and then Controller and client update
rcp_size_t Operations_RCP_Count = 0;
RCPTopic* rcpConfigurationsList[MAX_TOPIC_ID];  // For configuration values,  Client inits and then Controller updates
rcp_size_t Configurations_RCP_Count = 0;
RCPTopic* rcpStatusList[MAX_TOPIC_ID];          // For status values, Client inits and updates its own, Controller displays only
rcp_size_t Status_RCP_Count = 0;
RCPTopic* rcpLogsList[MAX_TOPIC_ID];            // For log transmition, universal init but client updates, gets saved to SD card
rcp_size_t Logs_RCP_Count = 0;
RCPTopic* rcpSettingsList[MAX_TOPIC_ID];        // For controller settings, universal init and controller updates
rcp_size_t Settings_RCP_Count = 0;
RCPTopic* rcpHiddensList[MAX_TOPIC_ID];         // For controller publication, universal init and controller updates but not user visible
rcp_size_t Hiddens_RCP_Count = 0;

//Implementation
void getRCPArrayFromCategory(RCP_cat_t category, rcp_size_t** size, RCPTopic*** topicList){
  switch(category){
    case(RCP_CAT_OPERATIONS):
      *size = &Operations_RCP_Count;
      *topicList = rcpOperationsList;
      break;
    case(RCP_CAT_CONFIGURATION):
      *size = &Configurations_RCP_Count;
      *topicList = rcpConfigurationsList;
      break;
    case(RCP_CAT_STATUS):
      *size = &Status_RCP_Count;
      *topicList = rcpStatusList;
      break;
    case(RCP_CAT_LOGS):
      *size = &Logs_RCP_Count;
      *topicList = rcpLogsList;
      break;
    case(RCP_CAT_SETTINGS):
      *size = &Settings_RCP_Count;
      *topicList = rcpSettingsList;
      break;
    default: //case(RCP_CAT_HIDDENS):
      *size = &Hiddens_RCP_Count;
      *topicList = rcpHiddensList;
  }
}

RCPTopic* CreateTopic(RCP_cat_t category, String name, bool doesTransmit){
  rcp_size_t * currentIDs;
  RCPTopic ** array;
  getRCPArrayFromCategory(category, &currentIDs, &array);

  if(*currentIDs >= MAX_TOPIC_ID){
    return NULL;
  }

  ID_t newID = (ID_t) *currentIDs;
  (*currentIDs)++;

  RCPTopic* newTopic = new RCPTopic(category, newID, name, doesTransmit);
  array[newID] = newTopic;
  return newTopic;
}

RCPTopic::RCPTopic(RCP_cat_t category, ID_t id, String name, bool doesTransmit){
  _category = category;
  _id = id;
  _displayName = name;
  _name_length = _displayName.length(); //note: no trailing null char
  if(_name_length>MAX_TOPIC_NAME_LENGTH){
    _name_length = MAX_TOPIC_NAME_LENGTH;
    _displayName = _displayName.substring(0,MAX_TOPIC_NAME_LENGTH);
  }
  _name = (binary_t*)malloc(sizeof(binary_t)*(_name_length+1));
  name.getBytes(_name,_name_length+1);
  _type = RCP_TYPE_NULL;
  _size = 0;
  _data = NULL;
  _isDataFresh = false;
  _isDisplayFresh = true;
  _doesTransmit = doesTransmit;

  _displayText = String("NULL");
  _color_r = 0;		         
  _color_g = 0;		         
  _color_b = 0;	
}

RCPTopic::~RCPTopic(){
  free(_name);
  if(_type != RCP_TYPE_NULL){
    free(_data);
  }
}
/*  // Disabled Name Change; these are fixed by design
void setName(String name){
  free(_name);
  _displayName = name;
  _name_length = _displayName.length(); //note: no trailing null char
  if(_name_length>MAX_TOPIC_NAME_LENGTH){
    _name_length = MAX_TOPIC_NAME_LENGTH;
    _displayName = _displayName.substring(0,MAX_TOPIC_NAME_LENGTH);
  }
  _name = (binary_t*)malloc(sizeof(binary_t)*(_name_length+1)); //last byte is null
  _name.getBytes(_name,_name_length);
  _isDataFresh = false;
}

void setName(binary_t data[], rcp_size_t name_length){
  free(_name);
  _displayName = String(data);
  _name_length = _displayName.length(); //note: no trailing null char
  if(_name_length>MAX_TOPIC_NAME_LENGTH){
    _name_length = MAX_TOPIC_NAME_LENGTH;
    _displayName = _displayName.substring(0,MAX_TOPIC_NAME_LENGTH);
  }
  _name = (binary_t*)malloc(sizeof(binary_t)*(_name_length+1));
  _name.getBytes(_name,_name_length);
  _isDataFresh = false;
}
*/    
void RCPTopic::setString(String data){
  if(_type != RCP_TYPE_NULL){
    free(_data);
  }

  _type = RCP_TYPE_STRING;
  _size = data.length(); 
  if(_size > MAX_TOPIC_DATA_LENGTH){
    _size = MAX_TOPIC_DATA_LENGTH;
    data = data.substring(0,MAX_TOPIC_DATA_LENGTH);
  }
  _size++;//note: trailing null char

  _data = (binary_t*)malloc(sizeof(binary_t)*_size);
  data.getBytes(_data, _size);
  _isDataFresh = false;
}

void RCPTopic::setByteArray(binary_t* data, rcp_size_t length){
  if(length > MAX_TOPIC_DATA_LENGTH){
    length = MAX_TOPIC_DATA_LENGTH;
  }
  if(_type != RCP_TYPE_NULL && (_type != RCP_TYPE_BYTE_ARRAY || _size != length)){
    free(_data);
  }
  if(_size != length || _type != RCP_TYPE_BYTE_ARRAY){
      _type = RCP_TYPE_BYTE_ARRAY;
      _size = length;
      _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }

  memcpy(_data, data, _size);
  _isDataFresh = false;
}

void RCPTopic::setFloat(float32_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_FLOAT){
    free(_data);
  }
  if(_type != RCP_TYPE_FLOAT){
    _size = sizeof(float32_t)*sizeof(binary_t);
    _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
    _type = RCP_TYPE_FLOAT;
  }
  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setLong(long64_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_LONG){
    free(_data);
  }
  if( _type != RCP_TYPE_LONG){
      _size = sizeof(binary_t)*sizeof(long64_t);
      _type = RCP_TYPE_LONG;
      _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }
  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setInt(int32_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_INT){
    free(_data);
  }
  if(_type != RCP_TYPE_INT){
    _size = sizeof(binary_t)*sizeof(int32_t);
    _type = RCP_TYPE_INT;
    _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }

  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setBool(bool data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_BOOL){
    free(_data);
  }
  if(_type != RCP_TYPE_BOOL){
    _size = sizeof(binary_t)*sizeof(bool);
    _type = RCP_TYPE_BOOL;
    _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }
  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setChar(char8_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_CHAR){
    free(_data);
  }
  if(_type != RCP_TYPE_CHAR){
    _size = sizeof(binary_t)*sizeof(char8_t);
    _type = RCP_TYPE_CHAR;
    _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }

  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setDouble(double64_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_DOUBLE){
    free(_data);
  }
  if(_type != RCP_TYPE_DOUBLE){
    _size = sizeof(binary_t)*sizeof(double64_t);
    _type = RCP_TYPE_DOUBLE;
    _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
  }

  memcpy(_data, &data, _size);
  _isDataFresh = false;
}

void RCPTopic::setMenu(binary_t* data, rcp_size_t length){
  if(length > MAX_TOPIC_DATA_LENGTH){
    length = MAX_TOPIC_DATA_LENGTH;
  }
  if(_type != RCP_TYPE_NULL && (_type != RCP_TYPE_MENU || _size != length)){
    free(_data);
  }
  if(_size != length || _type != RCP_TYPE_MENU){
      _type = RCP_TYPE_MENU;
      _size = length;
      _data = (binary_t*)malloc(_size); //For speed we do not want to reallocate though at the cost of space
      _data[0]=0;
      for(int indx = 1; indx < _size; indx++){
        if(data[indx] == '\n')
          data[indx] = 0;
      }
  }

  memcpy(_data, data, _size);
  _isDataFresh = false;
}

void RCPTopic::setMenuSelection(binary_t selection){
  if(_type != RCP_TYPE_MENU){
    return;
  }
  _data[0] = selection;
  _isDataFresh = false;
}

void RCPTopic::setFresh(){
  if(!_isDataFresh){
    valueToDisplay();
  }
}

void RCPTopic::setColor(binary_t color_r, binary_t color_g, binary_t color_b){
  _color_r = color_r;		         
  _color_g = color_g;		         
  _color_b = color_b;	
}

binary_t RCPTopic::getRed(){
  return _color_r;	
}

binary_t RCPTopic::getGreen(){
  return _color_g;	
}

binary_t RCPTopic::getBlue(){
  return _color_b;	
}

String RCPTopic::getName(){
  return _displayName;
}

char* RCPTopic::getNameRaw(){
  return (char*)_name;
}

rcp_size_t RCPTopic::getNameLength(){
  return _name_length;
}

RCP_type_t RCPTopic::getType(){
  return _type;
}

String RCPTopic::getString(){
  if(!_isDataFresh){
    setFresh();
  }
  return _displayText;
}

//Does not return a copy!
binary_t* RCPTopic::getByteArray(rcp_size_t* length){
  if(_type != RCP_TYPE_BYTE_ARRAY){
    *length = 0;
    return NULL;
  }else{
    *length = _size;
    return _data;
  }
}

float32_t RCPTopic::getFloat(){
  if(_type != RCP_TYPE_FLOAT){
    return 0;
  }else{
    float32_t myFloat;
    memcpy(&myFloat, _data, sizeof(float32_t));
    return myFloat;
  }
}

long64_t RCPTopic::getLong(){
  if(_type != RCP_TYPE_LONG){
    return 0;
  }else{
    long64_t myLong;
    memcpy(&myLong, _data, sizeof(long64_t));
    return myLong;
  }
}

int32_t RCPTopic::getInt(){
  if(_type != RCP_TYPE_INT){
    return 0;
  }else{
    int32_t myInt;
    memcpy(&myInt, _data, sizeof(int32_t));
    return myInt;
  }
}

bool RCPTopic::getBool(){
  if(_type != RCP_TYPE_BOOL){
    return 0;
  }else{
    bool myBool;
    memcpy(&myBool, _data, sizeof(bool));
    return myBool;
  }
}

char8_t RCPTopic::getChar(){
  if(_type != RCP_TYPE_CHAR){
    return 0;
  }else{
    char8_t myChar;
    memcpy(&myChar, _data, sizeof(char8_t));
    return myChar;
  }
}

double64_t RCPTopic::getDouble(){
  if(_type != RCP_TYPE_DOUBLE){
    return 0;
  }else{
    double64_t myDouble;
    memcpy(&myDouble, _data, sizeof(double64_t));
    return myDouble;
  }
}

binary_t RCPTopic::getMenuOptionMaxNum(){
  binary_t selectNum = 0;
  if(_type == RCP_TYPE_MENU){
    rcp_size_t index;
    for(index = 1; index < _size; index++){
      if(_data[index] == 0 || _data[index] == '\n'){
        selectNum++;
      }
    }
    return selectNum;
  }else{
    return 0;
  }
}

String RCPTopic::getMenuOption(binary_t option){
  binary_t selectNum = 0;
  rcp_size_t start = 1;
  rcp_size_t index;
  String selectString;
  if(_type != RCP_TYPE_MENU){
    return String("");
  }
  for(index = 1; index < _size; index++){
    if(_data[index] == 0 || _data[index] == '\n'){
      if(selectNum == option){
        break;
      }
      _data[index] = 0;
      start = index+1;
      selectNum++;
    }
  }
  if (index >= _size){
    selectString = String("");
  }else{
    selectString = String((char*)(_data+start));
  }
  return selectString;
}

String RCPTopic::getMenuOptions(){
  if(_type != RCP_TYPE_MENU){
    return "NULL SELECTION";
  }else{
    String myOptions = String((char*)(_data+1));
    return myOptions;
  }
}

binary_t RCPTopic::getMenuSelection(){
  if(_type != RCP_TYPE_MENU){
    return 0;
  }else{
    binary_t mySelection = *_data;
    return mySelection;
  }
}

bool RCPTopic::getFresh(){
  return _isDataFresh;
}

//Does not update the display text!
String RCPTopic::valueToDisplay(){
  if(!_isDataFresh){
    switch(_type){
      case RCP_TYPE_NULL:
        _displayText =  String("NULL");
        break;
      case RCP_TYPE_STRING:
        _displayText =  String((char*)_data);
        break;
      case RCP_TYPE_BYTE_ARRAY:
        _displayText =  String((char*)_data);//For now until I need a hex printout
        break;
      case RCP_TYPE_FLOAT:
        _displayText =  String(getFloat(), RCP_FP_PRES);
        break;
      case RCP_TYPE_LONG:
        _displayText =  String(getLong());
        break;
      case RCP_TYPE_INT:
        _displayText =  String(getInt());
        break;
      case RCP_TYPE_BOOL:
        _displayText =  getBool()?String("True"):String("False");
        break;
      case RCP_TYPE_CHAR:
        _displayText =  String(getChar());
        break;
      case RCP_TYPE_DOUBLE:
        _displayText =  String(getDouble(), RCP_FP_PRES);
        break;
      case RCP_TYPE_MENU:
        _displayText =  getMenuOption(getMenuSelection());
        break;
      default:
        _displayText = String("NULL");
        break;
    }
    _isDataFresh = true;
  }
  return _displayText;
}

bool RCPTopic::getMSG_announceTopic(binary_t* data, rcp_size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_publishTopic(binary_t* data, rcp_size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_changeColor(binary_t color_r, binary_t color_g, binary_t color_b, binary_t* data, rcp_size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_changeColor(binary_t* data, rcp_size_t* length){
  //TBA
  return true;
}

#endif // RCP_TOPIC_HPP  
