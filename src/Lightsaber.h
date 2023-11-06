#ifndef LIGHTSABER_H
#define LIGHTSABER_H


#ifndef ARDUINO
    #include <cstdint>
    uint8_t cubicwave8(uint8_t in) { return 0; }
    void delay(unsigned long ms) {}
    class NullSerial {
    public:
        virtual void println(const char* s) {}
        void begin(long speed) {}
    };
    #include <iostream>
    class ConsoleSerial: public NullSerial {
    public:
        void println(const char* s) { std::cout << s << std::endl; }
    };
    NullSerial Serial;
#endif


#include "Button.h"
#include "LedStrip.h"
#ifdef ARDUINO
  #include <StandardCplusplus.h>
#endif
#include <algorithm>
#include <vector>


class Lightsaber {
public:
  Lightsaber(InertialMeasurementUnit& inertialMeasurementUnit, LedStrip& ledStrip, Button& button, int ignitionDuration = 200):
    brightness_(1),
    color_({ 0, 0, 0 }),
    colorOff_({ 0, 0, 0 }),
    button_(button),
    hue_(0),
    iu_(inertialMeasurementUnit),
    ledStrip_(ledStrip),
    on_(false),
    rollDelay_(ignitionDuration / ledStrip.size()),
    saturation_(255)
  { setup(); }
  void switchOn() { if (off()) { send("ON"); unrollLed(); coolOff(); on_ = true; } }
  void switchOnAgain() { if (on()) switchOff(); switchOn(); }
  void switchOff() { if (on()) { send("OFF"); color_ = colorOff_; rollLed(); coolOff(); on_ = false; } } 
  void toggle() { on() ? switchOff() : switchOn(); }
  void tick() { static uint8_t count = 0; glow(count++); updateLed(); button().tick(); }
  static void onClick() { that().toggle(); }
  static void onDoubleClick() { that().rotateBrightness(); }
  static void onLongClick() { that().rotateHue(); }
protected:
  void glow(uint8_t t) {
    double amplitude = 0.33;
    int frequency = 3;
    int min = 256 - 256 * amplitude; /* stick wave to max brightness */
    brightness_ = amplitude * cubicwave8(frequency * t) + min;
  }
  Button& button() { return button_; }
  int hue() { return hue_; }
  int brightness() const { return brightness_; }
  int saturation() const { return saturation_; }  
  bool on() const { return on_; }
  bool off() const { return !on(); }
  void rotateBrightnessPlain() {
    static int count = 3;
    int brightnesses[3] = { 1, 64, 128 };
    brightness_ = brightnesses[++count % 3];
  }
  void rotateBrightnessSTL() {
    std::vector<int> brightnesses { 1, 64, 128 };
    std::rotate(brightnesses.begin(), brightnesses.begin() + 1, brightnesses.end());
    brightness_ = brightnesses.front();
  }
  void rotateBrightness() {
    rotateBrightnessPlain();
    if (on()) updateLed();
  }
  void rotateHue() {
    static int count = 3;
    int hues[3] = { 0, 96, 160 };
    hue_ = hues[++count % 3];
    if (on()) switchOnAgain();
  }
  void coolOff() const { delay(250); }
  void setLedAndShow(unsigned int i, Color color) { ledStrip_.setLedAndShow(i, color); }  
  /* update and unroll are the same but the delay */
  void updateLed() { if (on()) color_ = { hue(), saturation(), brightness() }; for (unsigned int i = 0; i < ledStrip_.size(); i++) { auto led = i; setLedAndShow(led, color_); } }
  void unrollLed() { color_ = { hue(), saturation(), brightness() }; for (unsigned int i = 0; i < ledStrip_.size(); i++) { auto led = i; setLedAndShow(led, color_); delay(rollDelay_); } }
  void rollLed() { for (unsigned int i = 0; i < ledStrip_.size(); i++) { auto led = ledStrip_.size() - 1 - i; setLedAndShow(led, color_); delay(rollDelay_); } }
  void setup() {
    instance_ = this;
    button().attachClick(Lightsaber::onClick);
    button().attachDoubleClick(Lightsaber::onDoubleClick);
    button().attachLongPressStop(Lightsaber::onLongClick);
  }
  static void log(const char* s) { Serial.println(s); }
  static void send(const char* s) { log(s); }
  static Lightsaber& that() { return *instance_; }
protected:
  static Lightsaber* instance_;
  int brightness_;
  Color color_;
  Color colorOff_;
  Button& button_;
  int hue_;
  InertialMeasurementUnit& iu_;
  LedStrip& ledStrip_;
  bool on_;
  int rollDelay_;
  int saturation_;
};
Lightsaber* Lightsaber::instance_;



#endif