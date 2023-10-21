#pragma once

#include <Arduino.h>
#include <Sprite.h>

class CustomSprite: public Sprite {
  private:
    bool _moving = false;            // Indicates whether the sprite is currently moving
    unsigned long _moveStartTime;   // Time when the movement started
    unsigned long _moveDuration;    // Total duration of the movement in milliseconds
    int8_t _moveInitialX;  // Initial X position for the movement
    int8_t _moveInitialY;  // Initial Y position for the movement
    int8_t _moveTargetX;   // Target X position for the movement
    int8_t _moveTargetY;   // Target Y position for the movement
    bool _shouldReturnToOrigin;  // Flag to indicate if the sprite should return to its origin

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

  // Start moving the sprite to a new position
  void startMoving(int16_t targetX, int16_t targetY, unsigned long duration, bool shouldReturnToOrigin) {
    _moveStartTime = millis();
    _moveDuration = duration;
    _moveInitialX = getX();  // Current X position
    _moveInitialY = getY();  // Current Y position
    _moveTargetX = targetX;
    _moveTargetY = targetY;
    _shouldReturnToOrigin = shouldReturnToOrigin;
    _moving = true;
  }

  // Stop the sprite's movement
  void stopMoving() {
    _moving = false;
  }

  // Check if the sprite is currently moving
  bool isMoving() {
    return _moving;
  }

  // Get the time when the movement started
  unsigned long getMoveStartTime() {
    return _moveStartTime;
  }

  // Get the total duration of the movement
  unsigned long getMoveDuration() {
    return _moveDuration;
  }

  // Get the initial X position for the movement
  int8_t getMoveInitialX() {
    return _moveInitialX;
  }

  // Get the initial Y position for the movement
  int8_t getMoveInitialY() {
    return _moveInitialY;
  }

  // Get the target X position for the movement
  int8_t getMoveTargetX() {
    return _moveTargetX;
  }

  // Get the target Y position for the movement
  int8_t getMoveTargetY() {
    return _moveTargetY;
  }
  
  // Linear interpolation
  int8_t lerp(int8_t start, int8_t end, float t) {
    return start + static_cast<int8_t>(t * (end - start));
  }

  // Set the X position
  void setX(int8_t newX) {
    _x = newX;
  }  

  // Set the Y position
  void setY(int8_t newY) {
    _y = newY;
  }

  // Check if the sprite should return to its origin
  bool shouldReturnToOrigin() {
    return _shouldReturnToOrigin;
  }
};