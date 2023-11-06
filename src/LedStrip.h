#ifndef LEDSTRIP_H
#define LEDSTRIP_H


#ifndef ARDUINO
  #include <cstdint>
#endif

struct Color {
  uint8_t hue;
  uint8_t saturation;
  uint8_t value;
};

class LedStrip {
public:
  LedStrip(unsigned int howManyLed = 10): howManyLed_(howManyLed) {}
  virtual void setLedAndShow(unsigned int i, Color& color) = 0;
  unsigned int size() const { return howManyLed_; }
protected:
  unsigned int howManyLed_;
};

class NullLedStrip: public LedStrip {
public:
  virtual void setLedAndShow(unsigned int i, Color& color) {}
};

#ifdef ARDUINO
  #include <StandardCplusplus.h>
#endif
#include <vector>

class StringLedStrip: public LedStrip {
  StringLedStrip(unsigned int howManyLed = 10):
    LedStrip(howManyLed),
    leds_(std::vector<Color>(howManyLed, {0,0,0}))
    {}
protected:
  std::vector<Color> leds_;
};


#ifdef ARDUINO
#include "FastLED.h"
class MicroLedStrip: public LedStrip {
public:
  MicroLedStrip(unsigned int howManyLed = 10, unsigned int ledPin = 6):
    LedStrip(howManyLed),
    ledPin_(ledPin),
    leds_(new CRGB[howManyLed])
    { FastLED.addLeds<NEOPIXEL, 6>(leds_, howManyLed_); }
  void setLedAndShow(unsigned int i, Color& color) { setLedAndShow(i, CHSV(color.hue, color.saturation, color.value)); };
protected:
  void setLedAndShow(unsigned int i, CHSV color) { auto& led = leds_[i]; led = color; FastLED.show(); }
protected:
  unsigned int ledPin_;
  CRGB* leds_;
};
#endif


#endif