// XOBot.cpp
#include "XOBot.h"

XOBot::XOBot(uint8_t buzzerPin, uint8_t vibratePin) {
    _buzzerPin = buzzerPin;
    _vibratePin = vibratePin;
    _currentMode = MODE_CLOCK;
    _timerRunning = false;
    _stopwatchRunning = false;
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

void XOBot::begin() {
    // Initialize pins
    pinMode(_buzzerPin, OUTPUT);
    pinMode(_vibratePin, OUTPUT);
    
    // Initialize display
    if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->cp437(true); // Use full 256 char 'Code Page 437' font
}

void XOBot::showBootSequence() {
    // Play startup tone
    playTone(880, 100);
    delay(100);
    playTone(1760, 100);
    
    // Display boot animation
    for(uint8_t i = 0; i < BOOT_FRAMES; i++) {
        display->clearDisplay();
        const char* frame = bootAnimation[i];
        int16_t textWidth = strlen(frame) * 6; // Assuming font width of 6 pixels
        int16_t x = (SCREEN_WIDTH - textWidth) / 2;
        display->setCursor(x, SCREEN_HEIGHT/2);
        display->print(frame);
        display->display();
        delay(200);
    }
    
    // Short vibration to indicate ready
    vibrate(100);
}

void XOBot::update() {
    switch(_currentMode) {
        case MODE_CLOCK:
            displayTime();
            break;
            
        case MODE_WEATHER:
            // Will be implemented when weather.h is ready
            break;
            
        case MODE_TIMER:
            updateTimer();
            break;
            
        case MODE_STOPWATCH:
            updateStopwatch();
            break;
            
        case MODE_NOTIFICATION:
            // Handle notifications
            break;
    }
}

void XOBot::displayTime() {
    display->clearDisplay();
    
    // Get current time
    // This would be replaced with actual RTC reading
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", hour(), minute());
    
    // Draw time
    display->setTextSize(2);
    drawCenteredText(timeStr, SCREEN_HEIGHT/2 - 8);
    
    // Draw date in smaller text below
    display->setTextSize(1);
    char dateStr[11];
    sprintf(dateStr, "%02d/%02d/%04d", day(), month(), year());
    drawCenteredText(dateStr, SCREEN_HEIGHT/2 + 8);
    
    display->display();
}

void XOBot::updateTimer() {
    if (!_timerRunning) return;
    
    unsigned long elapsed = millis() - _timerStart;
    if (elapsed >= _timerDuration) {
        _timerRunning = false;
        // Timer completed actions
        notify("Timer Done!", true, true);
        return;
    }
    
    // Display remaining time
    unsigned long remaining = (_timerDuration - elapsed) / 1000; // Convert to seconds
    display->clearDisplay();
    
    char timeStr[9];
    sprintf(timeStr, "%02lu:%02lu", remaining / 60, remaining % 60);
    
    display->setTextSize(2);
    drawCenteredText(timeStr, SCREEN_HEIGHT/2);
    display->display();
}

void XOBot::updateStopwatch() {
    if (!_stopwatchRunning) return;
    
    unsigned long elapsed = millis() - _stopwatchStart;
    unsigned long seconds = elapsed / 1000;
    
    display->clearDisplay();
    
    char timeStr[9];
    sprintf(timeStr, "%02lu:%02lu", seconds / 60, seconds % 60);
    
    display->setTextSize(2);
    drawCenteredText(timeStr, SCREEN_HEIGHT/2);
    display->display();
}

void XOBot::notify(const char* message, bool withHaptic, bool withSound) {
    if (withHaptic) vibrate(200);
    if (withSound) playTone(1000, 200);
    
    display->clearDisplay();
    display->setTextSize(1);
    drawCenteredText(message, SCREEN_HEIGHT/2);
    display->display();
    delay(2000); // Show notification for 2 seconds
}

void XOBot::drawCenteredText(const char* text, int16_t y) {
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    display->setCursor((SCREEN_WIDTH - w) / 2, y);
    display->print(text);
}

void XOBot::playTone(unsigned int frequency, unsigned long duration) {
    tone(_buzzerPin, frequency, duration);
}

void XOBot::vibrate(unsigned long duration) {
    digitalWrite(_vibratePin, HIGH);
    delay(duration);
    digitalWrite(_vibratePin, LOW);
}

void XOBot::toggleMode() {
    uint8_t nextMode = (_currentMode + 1) % 5; // 5 is the number of modes
    setMode(nextMode);
}

void XOBot::setMode(uint8_t mode) {
    if (mode >= 5) return; // Invalid mode
    
    uint8_t previousMode = _currentMode;
    _currentMode = mode;
    
    // Clear display and show brief mode indicator
    display->clearDisplay();
    display->setTextSize(1);
    
    const char* modeNames[] = {
        "Clock",
        "Weather",
        "Timer",
        "Stopwatch",
        "Notifications"
    };
    
    drawCenteredText(modeNames[mode], SCREEN_HEIGHT/2);
    display->display();
    delay(500);
}
