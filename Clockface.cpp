
#include "Clockface.h"

unsigned long lastMillis = 0;
unsigned long lastMillisFrames = 0;

// TODO document size
static DynamicJsonDocument doc(65536);

Clockface::Clockface(Adafruit_GFX* display)
{
  _display = display;
  Locator::provide(display);
}

void Clockface::setup(CWDateTime *dateTime) {
  this->_dateTime = dateTime;

  build();
  renderElements();
  refreshDateTime();

  // No animations
  if (frameDelay == 0)
    renderFrame(0);
}


void Clockface::update() 
{  
  // Render animation
  if (frameDelay > 0 && millis() - lastMillisFrames >= frameDelay) {
    if (frameCount >= frameSize)
      frameCount = 0;

    renderFrame(frameCount);

    frameCount++;
    lastMillisFrames = millis();
  }  

  // Update Date/Time
  if (millis() - lastMillis >= 1000) {
    refreshDateTime();
    lastMillis = millis();
  }  
}

void Clockface::setFont(const char *fontName) {

  if (strcmp(fontName, "picopixel") == 0) {
    Locator::getDisplay()->setFont(&Picopixel);
  } else if (strcmp(fontName, "atari") == 0) {
    Locator::getDisplay()->setFont(&atariFont);
  } else if (strcmp(fontName, "big") == 0) {
    Locator::getDisplay()->setFont(&hour8pt7b);
  } else if (strcmp(fontName, "medium") == 0) {
    Locator::getDisplay()->setFont(&minute7pt7b);
  } else {
    Locator::getDisplay()->setFont();
  }
}

void Clockface::renderText(String text, JsonVariantConst value) {
  int16_t  x1, y1;
  uint16_t w, h;

  setFont(value["font"].as<const char*>());

  Locator::getDisplay()->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  
  //BG Color
  Locator::getDisplay()->fillRect(
      value["x"].as<const uint16_t>() + x1,
      value["y"].as<const uint16_t>() + y1, 
      w, 
      h, 
      value["bgColor"].as<const uint16_t>());
  
  Locator::getDisplay()->setTextColor(value["fgColor"].as<const uint16_t>());
  Locator::getDisplay()->setCursor(value["x"].as<const uint16_t>(), value["y"].as<const uint16_t>());
  Locator::getDisplay()->print(text);

}

void Clockface::refreshDateTime() {

  JsonArrayConst elements = doc["elements"].as<JsonArrayConst>();
  for (JsonVariantConst value : elements) {
    const char* type = value["type"].as<const char*>();
      
    if (strcmp(type, "datetime") == 0) { 
      renderText(_dateTime->getFormattedTime(value["content"].as<const char*>()), value);
    }
  }
}

void Clockface::downloadDefinition(WiFiClient *client) {

  // const char* host = "raw.githubusercontent.com";
  // const char* path = "/jnthas/clock-club/main/first-demo.json";
  // const uint16_t port = 443;

  const char* host = "192.168.3.19";
  const char* path = "/star-wars.json";
  const uint16_t port = 9000;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected");
    return;
  }

  //client->setInsecure();
  client->setTimeout(10000);
  if (!client->connect(host, port)) {
    Serial.println(F("Connection failed"));
    return;
  }

  // Send HTTP request
  client->printf("GET %s HTTP/1.1\r\n", path);
  client->printf("Host: %s\r\n", host);
  client->println(F("Connection: close"));
  if (client->println() == 0) {
    Serial.println(F("Failed to send request"));
    client->stop();
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client->readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    client->stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client->find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client->stop();
    return;
  }

}

void Clockface::build() {

  WiFiClient client;
  
  downloadDefinition(&client);

  DeserializationError error = deserializeJson(doc, client);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    client.stop();
    return;
  }
  
  Serial.printf("[Canvas] Building clockface '%s' by %s, version %d\n", doc["name"].as<const char*>(), doc["author"].as<const char*>(), doc["version"].as<const uint16_t>());    
  Locator::getDisplay()->setFont(&Picopixel);

  // Clear screen
  Locator::getDisplay()->fillRect(0, 0, 64, 64, doc["bgColor"].as<const uint16_t>());

  frameDelay = doc["delay"].as<const uint16_t>();
  frameSize = doc["frames"].size();

  client.stop();
}

void Clockface::renderFrame(uint8_t frame) {
    
  const JsonArrayConst image = doc["frames"][frame]["array"].as<JsonArrayConst>();
  
  uint8_t x = doc["frames"][frame]["x"].as<const uint8_t>();
  uint8_t y = doc["frames"][frame]["y"].as<const uint8_t>();
  uint8_t w = doc["frames"][frame]["width"].as<const uint8_t>();
  uint8_t x_temp = x;

  for (JsonVariantConst pixel : image) {
    if (x_temp == w+x) {
      x_temp = x;
      y++;
    }
    Locator::getDisplay()->drawPixel(x_temp, y, pixel.as<const uint16_t>());
    x_temp++;
  }
}

void Clockface::renderElements() {
  JsonArrayConst elements = doc["elements"].as<JsonArrayConst>();
  for (JsonVariantConst value : elements) {
    const char* type = value["type"].as<const char*>();

    if (strcmp(type, "text") == 0) {
      renderText(value["content"].as<const char*>(), value);   

    } else if (strcmp(type, "fillrect") == 0) {
      
      Locator::getDisplay()->fillRect(
          value["x"].as<const uint16_t>(),
          value["y"].as<const uint16_t>(), 
          value["width"].as<const uint16_t>(), 
          value["height"].as<const uint16_t>(), 
          value["color"].as<const uint16_t>());

    } else if (strcmp(type, "rect") == 0) {
      
      Locator::getDisplay()->drawRect(
          value["x"].as<const uint16_t>(),
          value["y"].as<const uint16_t>(), 
          value["width"].as<const uint16_t>(), 
          value["height"].as<const uint16_t>(), 
          value["color"].as<const uint16_t>());

    } else if (strcmp(type, "line") == 0) {
      
      Locator::getDisplay()->drawLine(
          value["x"].as<const uint16_t>(),
          value["y"].as<const uint16_t>(), 
          value["x1"].as<const uint16_t>(), 
          value["y1"].as<const uint16_t>(), 
          value["color"].as<const uint16_t>());

    }
  }
}
