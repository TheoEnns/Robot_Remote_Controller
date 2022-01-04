#ifndef RCP_TOPIC_HPP             		
#define RCP_TOPIC_HPP

#ifndef ARDUINO_DUE
  #define float32_t float
  #define long64_t  long
  #define int32_t   int
  #define char8_t   char
  #define double64_t   double
#endif

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
  NUM_RCP_TYPES
}RCP_type_t;

typedef enum{
  RCP_CAT_PROPERTY=0,
  RCP_CAT_CONFIGURATION,
  RCP_CAT_STATUS,
  RCP_CAT_INFORMATION,
  RCP_CAT_LOGS,
  RCP_CAT_SETTINGS,
  NUM_CAT_TYPES
}RCP_cat_t;

typedef enum{
  RCP_MODE_ACTIVE=0,
  RCP_MODE_ESTOP,
  RCP_MODE_STOP,
  RCP_MODE_IDLE,
  NUM_MODE_TYPES
}RCP_mode_t;

typedef uint8_t ID_t;
#define MAX_TOPIC_ID 256 //Maximum IDs that can be registered to a category
typedef uint8_t sizeT_t;
typedef uint8_t binary_t;

#define MAX_TOPIC_DATA_LENGTH 128
#define MAX_TOPIC_NAME_LENGTH 21

class RCPTopic{
  private:
    RCP_cat_t   _category;
    ID_t        _id;
    RCP_type_t  _type;
    sizeT_t     _name_length; 
    binary_t*   _name; 
    sizeT_t     _size; 
    binary_t*   _data; 
    bool        _isDataFresh;
    // void        freeData();

    String _displayText;
    String _displayName;
    binary_t _color_r;		         
    binary_t _color_g;		         
    binary_t _color_b;

  public:
    RCPTopic(RCP_cat_t category, ID_t id, String name);
    ~RCPTopic();

    // void setName(String name);
    // void setName(binary_t data[], sizeT_t name_length);
    void setString(String data);
    void setByteArray(binary_t* data, sizeT_t length); //does not copy!
    void setFloat(float32_t data);
    void setLong(long64_t data);
    void setInt(int32_t data);
    void setBool(bool data);
    void setChar(char8_t data);
    void setDouble(double64_t data);
    void setFresh();

    String getName();
    sizeT_t getNameLength();
    String getString();
    binary_t* getByteArray(sizeT_t* length); //not a copy
    float32_t getFloat();
    long64_t getLong();
    int32_t getInt();
    bool getBool();
    char8_t getChar();
    double64_t getDouble();
    bool getFresh();

    String valueToDisplay();
    void setColor(binary_t color_r, binary_t color_g, binary_t color_b);

    bool getMSG_announceTopic(binary_t* data, size_t* length);
    bool getMSG_publishTopic(binary_t* data, size_t* length);
    bool getMSG_changeColor(binary_t color_r, binary_t color_g, binary_t color_b, binary_t* data, size_t* length);
    bool getMSG_changeColor(binary_t* data, size_t* length);
};

RCPTopic::RCPTopic(RCP_cat_t category, ID_t id, String name){
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
  _isDataFresh = true;

  _displayText = "NULL";
  _color_r = 255;		         
  _color_g = 255;		         
  _color_b = 255;	
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

void setName(binary_t data[], sizeT_t name_length){
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
  // memcpy(_data, data, (_size)*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setByteArray(binary_t* data, sizeT_t length){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_BYTE_ARRAY){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*MAX_TOPIC_DATA_LENGTH); //For speed we do not want to reallocate though at the cost of space
  }

  _type = RCP_TYPE_BYTE_ARRAY;
  _size = length;
  if(_size > MAX_TOPIC_DATA_LENGTH){
    _size = MAX_TOPIC_DATA_LENGTH;
  }

  memcpy(_data, data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setFloat(float32_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_FLOAT){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(float32_t)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(float32_t);
  }
  _type = RCP_TYPE_FLOAT;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setLong(long64_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_LONG){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(long64_t)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(long64_t);
  }
  _type = RCP_TYPE_LONG;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setInt(int32_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_INT){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(int32_t)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(int32_t);
  }
  _type = RCP_TYPE_INT;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setBool(bool data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_BOOL){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(bool)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(bool);
  }
  _type = RCP_TYPE_BOOL;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setChar(char8_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_CHAR){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(char8_t)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(char8_t);
  }
  _type = RCP_TYPE_CHAR;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setDouble(double64_t data){
  if(_type != RCP_TYPE_NULL && _type != RCP_TYPE_DOUBLE){
    free(_data);
      _data = (binary_t*)malloc(sizeof(binary_t)*sizeof(double64_t)); //For speed we do not want to reallocate though at the cost of space
    _size = sizeof(double64_t);
  }
  _type = RCP_TYPE_DOUBLE;

  memcpy(_data, &data, _size*sizeof(binary_t));
  _isDataFresh = false;
}

void RCPTopic::setFresh(){
  _displayText = valueToDisplay();
  _isDataFresh = true;
}

void RCPTopic::setColor(binary_t color_r, binary_t color_g, binary_t color_b){
  _color_r = color_r;		         
  _color_g = color_g;		         
  _color_b = color_b;	
}

String RCPTopic::getName(){
  return _displayName;
}

sizeT_t RCPTopic::getNameLength(){
  return _name_length;
}

String RCPTopic::getString(){
  if(!_isDataFresh){
    setFresh();
  }
  return _displayText;
}

//Does not return a copy!
binary_t* RCPTopic::getByteArray(sizeT_t* length){
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
    char8_t myBool;
    memcpy(&myBool, _data, sizeof(char8_t));
    return myBool;
  }
}

double64_t RCPTopic::getDouble(){
  if(_type != RCP_TYPE_DOUBLE){
    return 0;
  }else{
    double64_t myBool;
    memcpy(&myBool, _data, sizeof(double64_t));
    return myBool;
  }
}

bool RCPTopic::getFresh(){
  return _isDataFresh;
}

//Does not update the display text!
String RCPTopic::valueToDisplay(){
  switch(_type){
    case RCP_TYPE_NULL:
      return String("NULL");
    case RCP_TYPE_STRING:
      return String((char*)_data);
    case RCP_TYPE_BYTE_ARRAY:
      return String((char*)_data);//For now until I need a hex printout
    case RCP_TYPE_FLOAT:
      return String(getFloat());
    case RCP_TYPE_LONG:
      return String(getLong());
    case RCP_TYPE_INT:
      return String(getInt());
    case RCP_TYPE_BOOL:
      return String(getBool());
    case RCP_TYPE_CHAR:
      return String(getChar());
    case RCP_TYPE_DOUBLE:
      return String(getDouble());
    default:
      return "NULL";
  }
}

bool RCPTopic::getMSG_announceTopic(binary_t* data, size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_publishTopic(binary_t* data, size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_changeColor(binary_t color_r, binary_t color_g, binary_t color_b, binary_t* data, size_t* length){
  //TBA
  return true;
}

bool RCPTopic::getMSG_changeColor(binary_t* data, size_t* length){
  //TBA
  return true;
}

#endif // RCP_TOPIC_HPP  
