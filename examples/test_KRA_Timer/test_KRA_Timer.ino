#include <KRA_robot.h>
KRA_Timer t1  = KRA_Timer(1000); // nastavení časovače na 1000ms
KRA_Timer t2; // nastavení prázdného a zastaveneho časovače
KRA_Timer t3  = KRA_Timer(100); // nastavení časovače na 100ms ,  časovač použitý k četnosti výpisu stavu sledovaného časovače t1
void setup() {
  Serial.begin(115200); // nastaveni vystupu Serial
  while (!Serial) {}; // cekani na inicializaci
  Serial.println ("Start testu casovacu t1 a t2");
  // put your setup code here, to run once:
  t1.setTimer(6000); // nové nastavení času na 6000ms
  t1.startTimer(); // spuštění časovače

  t2.setTimer(500); // nastavení časovače na 500ms
  t2.addTimer(1500); // prvniprubeh nastaven na 2000ms
  t2.startTimer(); // spusteni casovace, ktery byl pri deklarovan jako prazdny
}

void loop() {
  // put your main code here, to run repeatedly:
  if (t1.isFinish()) {
    /* tento příkaz provede po uplinutí nastaveného času*/
    Serial.println("Casovac t1 byl zastaven");
    t2.stopTimer();
    Serial.println("U casovace t2 byl zastaven Autorestart");
  }
  if (t2.isFinishAndRestart()) {
    /* tento příkaz provede po uplinutí nastaveného času
      a opětovně spustí odpočítávání nového času*/
    Serial.println("Casovac t2 probehl a byl restartovan.");
  }
  if (t3.isFinishAndRestart()) {
    Serial.print("Casovac t1:");
    Serial.println(t1.isOnTime() ? "bezi" : "byl ukoncen");
    if (!t1.isOnTime()) {
      t3.stopTimer(); // ukonceni vypisovani stavove hlasky
      Serial.println("Casovac t3(vypis casovace t1) byl zastaven");
    }
  }
}
