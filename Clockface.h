#pragma once

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Locator.h>
#include <ArduinoJson.h>
#include <vector>
#include <CWPreferences.h>

// Commons
#include "IClockface.h"
#include "Icons.h"
#include "picopixel.h"
#include "fonts/atari.h"
#include "fonts/hour8pt7b.h"
#include "fonts/minute7pt7b.h"
#include "PNGRender.h"
#include "CustomSprite.h"
#include "CWHttpClient.h"

#define CLOCKFACE_NAME "cw-cf-0x07"

class Clockface : public IClockface
{
private:
  Adafruit_GFX *_display;
  CWDateTime *_dateTime;
  uint16_t frameDelay;

  void setFont(const char *fontName);
  void deserializeDefinition();
  void clockfaceSetup();
  void clockfaceLoop();
  void renderElements(JsonArrayConst elements);
  void renderText(String text, JsonVariantConst value);
  void createSprites();
  void refreshDateTime();

  std::vector<std::shared_ptr<CustomSprite>> sprites;

public:
  Clockface(Adafruit_GFX *display);
  void setup(CWDateTime *dateTime);
  void update();
};
