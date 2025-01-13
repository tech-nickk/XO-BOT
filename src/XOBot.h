// XOBot.h
#ifndef XOBot_h
#define XOBot_h

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "XOBotExpressions.h"  // Your expressions.h renamed
#include "XOBotWeather.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class XOBot {
  public:
    // Constructor and basic functions
    XOBot(uint8_t buzzerPin, uint8_t vibratePin);
    void begin();
    void update();
    
    // Mode setters
    void setMode(uint8_t mode);
    void toggleMode();
    
    // Display functions
    void clearDisplay();
    void showBootSequence();
    void showExpression(uint8_t expression);
    void showWeather(uint8_t weatherType);
    
    // Time functions
    void setTime(uint8_t hours, uint8_t minutes);
    void setDate(uint8_t day, uint8_t month, uint16_t year);
    void displayTime();
    void displayDate();
    
    // Timer and stopwatch
    void startTimer(unsigned long duration);
    void stopTimer();
    void startStopwatch();
    void stopStopwatch();
    void resetStopwatch();
    
    // Notification functions
    void notify(const char* message, bool withHaptic = true, bool withSound = true);
    
    // System functions
    void playTone(unsigned int frequency, unsigned long duration);
    void vibrate(unsigned long duration);
    
    // Operating modes
    enum Mode {
      MODE_CLOCK,
      MODE_WEATHER,
      MODE_TIMER,
      MODE_STOPWATCH,
      MODE_NOTIFICATION
    };
    
  private:
    Adafruit_SSD1306* display;
    uint8_t _buzzerPin;
    uint8_t _vibratePin;
    uint8_t _currentMode;
    
    // Time tracking
    unsigned long _timerStart;
    unsigned long _timerDuration;
    unsigned long _stopwatchStart;
    bool _timerRunning;
    bool _stopwatchRunning;
    
    // Internal utility functions
    void drawCenteredText(const char* text, int16_t y);
    void animateTransition(uint8_t fromMode, uint8_t toMode);
};

// Animation sequences
const uint8_t BOOT_FRAMES = 5;
const char* const PROGMEM bootAnimation[BOOT_FRAMES] = {
    "X     ",
    "XO    ",
    "XO-   ",
    "XO-BOT",
    "XO-BOT!"
};

#endif
