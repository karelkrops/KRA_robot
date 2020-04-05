#include "kra_timer.h"
#include <Arduino.h>

KRA_Timer::KRA_Timer(): timeStartTimer(0), timer(0) {
  stopTimer();
}
KRA_Timer::KRA_Timer(unsigned long newTimer) {
  setTimer(newTimer);
  startTimer();
}

KRA_Timer::~KRA_Timer() {

}

bool KRA_Timer::startTimer() {
  isFinish();
  timeStartTimer = millis();
  if (!timeStartTimer) timeStartTimer = 1;
  isModeFinish=false;
  return true;
}

bool KRA_Timer::setTimer(unsigned long newTimer) {
  timer = newTimer;
  actualTimer = timer;
  return true;
}

bool KRA_Timer::isFinish() {
  if (timeStartTimer) {
    if ( (timeStartTimer + actualTimer) - millis() > 0xFFFFEFFFU) {
      stopTimer();
      return true;
    }
  }
  if(isModeFinish){
    stopTimer();
    return true;
  }
  return false;
}

bool KRA_Timer::isOnTime(){
  if (timeStartTimer) {
    if ( (timeStartTimer + actualTimer) - millis() <= actualTimer) {
      isModeFinish=false;
      return true;
    } else {
      isModeFinish=true;
      timeStartTimer = 0;
      return false;
    }
  }
  return false;
}
bool KRA_Timer::stopTimer() {
  timeStartTimer = 0;
  isModeFinish=false;
  actualTimer = timer; // nastavení aktualizování časovače
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
