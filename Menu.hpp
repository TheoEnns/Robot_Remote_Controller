
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

//Display Values
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

void initializeDisplay(){

}
