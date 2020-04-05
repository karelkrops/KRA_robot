#include <Arduino.h>
#include "toffi_motor.h"

ToffiMotor::ToffiMotor(const bool startTask)
  : ToffiLayer(startTask)
{
  if (0) {
    while (!Serial) {}
    Serial.println("ToffiMotor::ToffiMotor()");
  }
  paramFA = 5; // nastavení dopředné normálové akcelerace
  paramRA = 2; // nastavení rotační normálové akcelerace
  engineML = 0; // inicializece maximální momentální akcelerace motoru
  engineMP = 0; // inicializace maximální momentální akcelerace motoru
  setLayer_date(pAM, 1, 50);
  engineUL = 0;
  engineUP = 0;
  powerNL = 0;
  powerNP = 0;
}
void ToffiMotor::execute() {
  //     výpočet hodnot pro provoz motorů
  bool debuger = false;
  if (debuger) {
    Serial.print("ToffiMotor::execute()");
    Serial.print("UL: ");
    Serial.print(engineUL);
    Serial.print("=> powerML: ");
    Serial.print( powerNL );
    Serial.println("");
  }
  // vybere potřebnou výkon do motorů. pokud by překračoval maximální omezení, pak vypočtené hodnoty nahradí maximálními hodnotami
  int deltaUL = powerNL < engineUL  ? 1/*deltaTime*/ * 1/*zrychlení*/ : -1/*deltaTime*/ * 1/*zrychlení*/;
  int deltaUP = powerNP < engineUP  ? 1/*deltaTime*/ * 1/*zrychlení*/ : -1/*deltaTime*/ * 1/*zrychlení*/;
  powerNL = abs(engineUL - powerNL) < abs(deltaUL)  ? engineUL : powerNL + deltaUL;
  powerNP = abs(engineUL - powerNP) < abs(deltaUP)  ? engineUL : powerNP + deltaUP;;
  //  nastavení parametrů motorů
  if (debuger) {
    Serial.print("ToffiMotor::execute()");
    Serial.print("UL: ");
    Serial.print(engineUL);
    Serial.print("=> deltaUL: ");
    Serial.print( deltaUL );
    Serial.print("=> powerML: ");
    Serial.print( powerNL );
    Serial.println("");
  }
  motory(powerNL, powerNP);
}
void ToffiMotor::start() {
  taskDelay(100);
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("ToffiMotor::start()");
  for (;;) {
    execute();
    if (0) {
      Serial.println("loopMotor()");
      Serial.print("Task:");
      //        Serial.print((int)xx);

      Serial.println(".");
    }


    taskDelay(100); // čekání 100ms
  }
}


bool ToffiMotor::motory(int pL, int pP)
{
  if (pL > rychlostMax) pL = rychlostMax;
  if (pL < -rychlostMax) pL = -rychlostMax;
  if (pL < 0)
    digitalWrite(PINDIRL, HIGH);
  else
    digitalWrite(PINDIRL, LOW);
  if (pP > rychlostMax) pP = rychlostMax;
  if (pP < -rychlostMax) pP = -rychlostMax;

  if (pP < 0)
    digitalWrite(PINDIRR, LOW);
  else
    digitalWrite(PINDIRR, HIGH);
  setLayer_date(UL, pL, 100);
  setLayer_date(UP, pP, 100);
  analogWrite(PINPWML, abs(pL));
  analogWrite(PINPWMR, abs(pP));
  return true;
}
