// Weather.h
#ifndef Weather_h
#define Weather_h

#include "Arduino.h"

class Weather {
  public:
    // Weather icons as byte arrays for OLED
    static const unsigned char PROGMEM sunny[];
    static const unsigned char PROGMEM cloudy[];
    static const unsigned char PROGMEM rainy[];
    static const unsigned char PROGMEM thunderstorm[];
    
    // Animation frames
    static const int FRAME_COUNT = 4;  // Number of frames per animation
    static const unsigned char* const PROGMEM cloudyAnimation[FRAME_COUNT];
    static const unsigned char* const PROGMEM rainyAnimation[FRAME_COUNT];
};

// Define one example weather icon
const unsigned char Weather::sunny[] PROGMEM = {
    0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
    // Add more bytes for the complete icon
};

#endif
