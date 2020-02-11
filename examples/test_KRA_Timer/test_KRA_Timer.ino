#include <KRA_robot.h>
KRA_Timer t1  = KRA_Timer(1000); // nastavení časovače na 1000ms
void setup() {
  // put your setup code here, to run once:
  t1.setTimer(2000); // nové nastavení času na 2000ms
  t1.addTimer(500); // přidání, zvětšení času o 500ms, tedy na 2500ms
  t1.startTimer(); // spuštění časovače
}

void loop() {
  // put your main code here, to run repeatedly:
  if (t1.isFinish()) {/* tento příkaz provede po uplinutí nastaveného času*/}
  if (t1.isFinishAndRestart()) {/* tento příkaz provede po uplinutí nastaveného času
    a opětovně spustí odpočítávání nového času*/}
    ti.stopTimer(); // zastavení, přerušení časovače
}
