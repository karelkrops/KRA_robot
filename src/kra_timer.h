#ifndef KRA_TIMER_H
#define KRA_TIMER_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 1.1.0
 * datum verze: 23.2.2020
 */

/**
   třída Timer slouží k řízení časování událostí aniž by došlo ke zdržení zpracování ostatních událostí

   ve verzi 1.1.0 byla přidána funkce bool isOnTime()

*/

class KRA_Timer {
  public:
    KRA_Timer(); // deklarace nenastaveného a zastaveného časovače
    KRA_Timer(unsigned long newTimer); // nastavení a nastartování časovače
    ~KRA_Timer();
    bool startTimer(); // start časovače
    bool stopTimer(); // zastavení / přerušení časovače
    bool isOnTime(); // časovač běží a je v měřeném čase. po jeho uplinutí časovač přejde do stavu Finish
    bool isFinish(); // kontrola, zda čas proběhl koncovým časem časovače. Lze úspěšně načíst pouze jednorázově v daném časovém ciklu
    bool isFinishAndRestart(); // stejné jako isFinish(), pouze s opětovném nastartováním časovače
    bool setTimer(unsigned long newTimer); // přenastavení hlavního času časovače
    bool addTimer(unsigned long addTimer); // prodloží čas aktuálně běžícího časovače. Příští start se vrátí do normálu
  private:
    unsigned long timeStartTimer; // čas startu
    unsigned long timer; // základní nastavení časovače
    unsigned long actualTimer; // dočasné jednorázové nastavení času
    bool isModeFinish; // časovač je sice již zastaven funkcí isOnTime(), ale ještě nebyl načten funkcí isFinish(). 
};

#endif
