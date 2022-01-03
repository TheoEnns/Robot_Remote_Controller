
enum{
  PUB_TYPE_NULL=0,
  PUB_TYPE_STRING,
  PUB_TYPE_BYTE_ARRAY,
  PUB_TYPE_FLOAT,
  PUB_TYPE_LONG,
  PUB_TYPE_INT,
  PUB_TYPE_BOOL,
  NUM_PUB_TYPES
}pub_type_t;

enum{
  PUB_CAT_PROPERTY=0,
  PUB_CAT_CONFIGURATION,
  PUB_CAT_STATUS,
  PUB_CAT_INFORMATION,
  PUB_CAT_LOGS,
  PUB_CAT_SETTINGS,
  NUM_CAT_TYPES
}pub_cat_t;

typedef uint8_t ID_t;
typedef uint8_t sizeT_t;
typedef uint8_t binary_t;

class Topic{
  private:
    pub_cat_t   _category;
    ID_t        _id;
    pub_type_t  _type;
    sizeT_t     _name_length; 
    binary_t*   _name; 
    sizeT_t     _size; 
    binary_t*   _data; 
    bool        _isFresh;
    void        freeData();
  public:
    Topic(pub_cat_t category, ID_t id, String name);
    ~Topic();

    void setName(String name);
    void setName(binary_t data[], sizeT_t name_length);
    void setString(String data);
    void setByteArray(binary_t* data, sizeT_t length); //does not copy!
    void setFloat(float data);
    void setLong(long data);
    void setInt(int data);
    void setBool(bool data);
    void setFresh();

    String getName();
    binary_t* getNameData(); //not a copy
    sizeT_t getNameLength();
    String getString();
    binary_t* getByteArray(sizeT_t* length); //not a copy
    float getFloat();
    long getLong();
    int getInt();
    bool getBool();
    bool getFresh();

    String toDisplay();
}topic_t;


class Display_Topic : public Topic{ 
  private:
    String displayText;
    String displayName;
    char color_r;		         
    char color_g;		         
    char color_b;		         
}dispTopic_t;
