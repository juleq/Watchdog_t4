#include <Watchdog_t4.h>
#include "Arduino.h"

watchdog_class wdt;
watchdog_class_ptr watchdog_class::watchdog_class_handler = nullptr;

void watchdog_isr() {
  WDOG1_WICR |= (1UL << 14);
  if ( wdt.watchdog_class_handler ) wdt.watchdog_class_handler();
}

void watchdog_class::callback(watchdog_class_ptr handler) {
  watchdog_class_handler = handler;
  NVIC_ENABLE_IRQ(IRQ_WDOG1);
  WDOG1_WICR |= (WDOG1_WICR & 0xFF) | (3UL << 14);
}

void watchdog_class::begin() {
  _VectorsRam[16 + IRQ_WDOG1] = watchdog_isr;
  WDOG1_WCR |= WDOG_WCR_WDE | WDOG_WCR_WDT | WDOG_WCR_SRE;
  WDOG1_WMCR = 0;
  WDOG1_WICR |= (3UL << 14);
  feed();
}

void watchdog_class::reset() {
  WDOG1_WCR &= ~(1UL << 4);
}

void watchdog_class::setTrigger(float val) {
  if ( val < 0 ) val = 0;
  else if ( val > 127.5 ) val = 127.5;
  if ( val >= ((((WDOG1_WCR & 0xFF00 ) >> 8 ) + 1) *0.5f) ) val = 2;
  WDOG1_WICR |= (WDOG1_WICR & ~0xFF) | (3UL << 14) | (uint16_t)(val / 0.5f);
}

void watchdog_class::setTimeout(float val) {
  if ( val < 0.5f ) val = 0.5f;
  else if ( val > 128 ) val = 128;
  uint8_t value = (float)((  val / 0.5f) - 1);
  WDOG1_WCR |= (WDOG1_WCR & ~WDOG_WCR_WT(1)) | value << 8;
}

bool watchdog_class::setPin(uint8_t pin) {
  if ( pin == 19 ) {
    CORE_PIN19_CONFIG = 0x14;
    return 1;
  }
  if ( pin == 20 ) {
    CORE_PIN20_CONFIG = 0x11;
    return 1;
  }
  return 0;
}

void watchdog_class::feed() {
  WDOG1_WSR = 0x5555;
  WDOG1_WSR = 0xAAAA;
}
