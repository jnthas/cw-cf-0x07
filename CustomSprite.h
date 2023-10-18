#pragma once

#include <Arduino.h>
#include <Sprite.h>

class CustomSprite: public Sprite {
  public:
    CustomSprite(int8_t x, int8_t y);
    uint8_t _totalFrames = 0;
    uint8_t _currentFrame = 0;
    uint8_t _spriteReference = 0;
    uint8_t _currentFrameCount = 0;
    unsigned long _lastMillisSpriteFrames = 0;
    unsigned long _lastResetTime = 0;

    void setDimensions(uint8_t width, uint8_t height);
    int8_t getX();
    int8_t getY();
    uint8_t getWidth();
    uint8_t getHeight();

    const char* name();
    void incFrame();    
};
