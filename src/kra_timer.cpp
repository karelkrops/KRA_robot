#include "kra_timer.h"
#include <Arduino.h>

KRA_Timer::KRA_Timer(): timeStartTimer(0), timer(0) {

}
KRA_Timer::KRA_Timer(unsigned long newTimer) {
  setTimer(newTimer);
  startTimer();
}

KRA_Timer::~KRA_Timer() {

}

bool KRA_Timer::startTimer() {
  timeStartTimer = millis();
  if (!timeStartTimer) timeStartTimer = 1;
  actualTimer = timer;
  return true;
}

bool KRA_Timer::setTimer(unsigned long newTimer) {
  timer = newTimer;
  actualTimer = timer;
  return true;
}

bool KRA_Timer::isFinish() {
  if (timer) {
    if ( (timeStartTimer + actualTimer) - millis() > 0xFFFFEFFFU) {
      timeStartTimer = 0;
      return true;
    }
  }
  return false;
}


bool KRA_Timer::stopTimer() {
  timeStartTimer = 0;
  return true;
}

bool KRA_Timer::isFinishAndRestart() {
  if (isFinish()) {
    startTimer();
    return true;
  }
  return false;
}

bool KRA_Timer::addTimer(unsigned long addTimer) {
  actualTimer += addTimer;
  return true;
}
