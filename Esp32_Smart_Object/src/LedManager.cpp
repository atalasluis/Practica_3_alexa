#include "LedManager.h"

LedManager::LedManager(int g, int y, int r) {
  pinGreen = g;
  pinYellow = y;
  pinRed = r;

  currentState = OFF;
  lastBlink = 0;
  blinkState = false;
  blinkInterval = 300;
}

void LedManager::init() {
  pinMode(pinGreen, OUTPUT);
  pinMode(pinYellow, OUTPUT);
  pinMode(pinRed, OUTPUT);

  applyState();
}

void LedManager::setState(LedState state) {
  currentState = state;
}

void LedManager::update() {
  if (currentState == DANGER) {
    if (millis() - lastBlink > blinkInterval) {
      lastBlink = millis();
      blinkState = !blinkState;

      digitalWrite(pinGreen, blinkState);
      digitalWrite(pinYellow, blinkState);
      digitalWrite(pinRed, blinkState);
      
    }
  } else {
    applyState();
  }
}

void LedManager::applyState() {
  digitalWrite(pinGreen, LOW);
  digitalWrite(pinYellow, LOW);
  digitalWrite(pinRed, LOW);

  switch (currentState) {
    case GREEN:
      digitalWrite(pinGreen, HIGH);
      break;

    case YELLOW:
      digitalWrite(pinYellow, HIGH);
      break;

    case RED:
      digitalWrite(pinRed, HIGH);
      break;

    case OFF:
      // todos apagados
      digitalWrite(pinGreen, LOW);
      digitalWrite(pinYellow, LOW);
      digitalWrite(pinRed, LOW);
      break;

    default:
      break;
  }
}