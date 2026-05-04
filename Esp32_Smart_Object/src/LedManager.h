#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
enum LedState {
  GREEN,
  YELLOW,
  RED,
  DANGER,
  OFF
};

class LedManager {
  private:
    int pinGreen;
    int pinYellow;
    int pinRed;

    LedState currentState;

    unsigned long lastBlink;
    bool blinkState;
    int blinkInterval;

    void applyState();

  public:
    LedManager(int g, int y, int r);

    void init();
    void setState(LedState state);
    void update();
};

#endif