#if !defined(_WATCHDOG_T4_H_)
#define _WATCHDOG_T4_H_

#include "Arduino.h"

typedef void (*watchdog_class_ptr)();

class watchdog_class {
  public:
    watchdog_class() {;}
    void begin();
    void setTimeout(float val);
    void setTrigger(float val);
    bool setPin(uint8_t pin);
    void feed();
    void loop() { feed(); }
    void callback(watchdog_class_ptr handler);
    uint16_t lastReset() { return WDOG1_WRSR; }
    static watchdog_class_ptr watchdog_class_handler;

  private:
};

extern watchdog_class wdt;

#endif