#ifndef KRA_TIMER_H
#define KRA_TIMER_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 1.0.0
 * datum verze: 19.1.2020
 */

/**
   třída Timer slouží k řízení časování událostí aniž by došlo ke zdržení zpracování ostatních událostí

*/

class KRA_Timer {
  public:
    KRA_Timer();
    KRA_Timer(unsigned long newTimer);
    ~KRA_Timer();
    bool startTimer();
    bool stopTimer();
    bool isFinish();
    bool isFinishAndRestart();
    bool setTimer(unsigned long newTimer);
    bool addTimer(unsigned long addTimer);
  private:
    unsigned long timeStartTimer;
    unsigned long timer;
    unsigned long actualTimer;
};

#endif
