#pragma once

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Locator.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>


// Commons
#include "IClockface.h"
#include "Icons.h"
#include "picopixel.h"
#include "fonts/atari.h"
#include "fonts/hour8pt7b.h"
#include "fonts/minute7pt7b.h"
#include "assets.h"


class Clockface: public IClockface {
  private:
    Adafruit_GFX* _display;
    CWDateTime* _dateTime;
    uint16_t frameDelay;
    uint8_t frameCount = 0;
    uint8_t frameSize = 0;

    void setFont(const char *fontName);
    void downloadDefinition(WiFiClient *client);
    void build();
    void renderFrame(uint8_t frame);
    void renderElements();
    void renderText(String text, JsonVariantConst value);
    void refreshDateTime();
    
  
  public:
    Clockface(Adafruit_GFX* display);
    void setup(CWDateTime *dateTime);
    void update();
};
