#ifndef BUTTON_H
#define BUTTON_H


extern "C" {
  typedef void (*callbackFunction)(void);
}

class Button {
public:
  virtual void attachClick(callbackFunction newFunction) = 0;
  virtual void attachDoubleClick(callbackFunction newFunction) = 0;
  virtual void attachLongPressStop(callbackFunction newFunction) = 0;
  virtual void tick() = 0;
};

class NullButton: public Button {
public:
  void attachClick(callbackFunction newFunction) {}
  void attachDoubleClick(callbackFunction newFunction) {}
  void attachLongPressStop(callbackFunction newFunction) {}
  void tick() {}
};

class ConsoleButton: public NullButton {
};

#ifdef ARDUINO
#include <OneButton.h>
class MicroButton: public Button {
public:
  MicroButton(unsigned int pin): Button() { button_ = new OneButton(pin, true); }
  void attachClick(callbackFunction newFunction) { button_->attachClick(newFunction); };
  void attachDoubleClick(callbackFunction newFunction) { button_->attachDoubleClick(newFunction); };
  void attachLongPressStop(callbackFunction newFunction) { button_->attachLongPressStop(newFunction); };
  void tick() { button_->tick(); }
protected:
  OneButton* button_;
};
#endif


#endif