#include "Button.h"
#include <Arduino.h>

Button::Button(int pin): pinNumber(pin)
{
  timerLastPulse = 0;
  numberClick = 0;
  lastStatus = false;
  lastStatusTemp = false;
  lastTimeChange = 0;
  timeClick = 300; // doba mezi click
  timeMinimum = 10; // počet milisekund mezi změnou statusu tlačítka. Odstranění záchvěvů
  pinMode(pinNumber, INPUT_PULLUP);
}

Button::~Button()
{
}

bool Button::getStatus() {
  if ( (millis() - lastTimeChange) > timeMinimum) {
    lastStatusTemp = !digitalRead(pinNumber);
    lastTimeChange = millis();
  }
  return lastStatusTemp;
}
bool Button::getPulse() {
  if (getStatus() && !lastStatus) {
    lastStatus = true;
    return true;
  }
  if (!getStatus()) {
    lastStatus = false;
  }
  return false;
}
int Button::getClick() {
  if (timerLastPulse == 0) numberClick = 0;
  if (getPulse()) {
    numberClick++;
    timerLastPulse = millis();
    if (numberClick == 3) {
      timerLastPulse = 0;
      return numberClick;
    }
  } else {
    //    if (!timerLastPulse && millis()-timerLastPulse > timeClick){
    if (millis() - timerLastPulse > timeClick) {
      timerLastPulse = 0;
      return numberClick;
    }
  }
  return 0;
}
