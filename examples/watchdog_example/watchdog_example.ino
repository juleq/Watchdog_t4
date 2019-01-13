#include "Watchdog_t4.h"

void setup() {
  Serial4.begin(115200);
  wdt.setPin(19); // pin that drives LOW when a wdt occurs.
  wdt.setTrigger(5); // callback 5 seconds before final timeout
  wdt.callback(myCallback);
  wdt.setTimeout(10); // reset timeout
  wdt.begin(); // enable watchdog
}

void loop() {
  //  wdt.loop(); // feed the dog, uncomment to test callback
}

void myCallback() {
  Serial4.println("GET READY TO RESET!");
}
