#include "toffi_mat.h"
#include <Arduino.h>

ToffiMat::ToffiMat(const bool startTaskTmp):
  ToffiLayer(startTaskTmp)
{
  deltaTime=0; // inicializace času pro výpočet délty
  paramM_U=1; // poměr mezi momentu síly a déltou napětí
  paramA_M=1; // poměr mezi accelerací a momentem síli
  setLayer_date(pMU, 1,100); // konstanta přepočtu momentu síly na déltě napětí motoru
  setLayer_date(pAM, 2,100);; // startovní parametr přepočtu akcelerace v závyslosti na momentu síly
//  calcLayer0(pMU);
//  calcLayer0(pAM);
}
void ToffiMat::execute() {
  static unsigned long lastTime = millis();
  deltaTime = millis() - lastTime; // spočte déltu časového okna pro vrstvu mat
  setLayer_date(dT, deltaTime,100);
  calcAllLayer0();

  //calcLayer0(pMU);

  calcAcceleration();
  calcMoment();
  calcSpeed();
  calcPosition();
}
void ToffiMat::start() {
  for (;;) {
    execute();
    taskDelay(1);
  }
}

bool ToffiMat::calcAcceleration() {
  // spočte dopřednou a rotační akceleraci
  calcLayer0(ML);
  calcLayer0(MP);
  layer_date[nameDateRobotToffi::FA] = convertFA_ML_MP(getLayer0_date(ML), getLayer0_date(MP));
  layer_weight[nameDateRobotToffi::FA] = (getLayer0_weight(NL) + getLayer0_weight(NP)) / 2;
  layer_date[RA] = convertRA_ML_MP(getLayer0_date(ML), getLayer0_date(MP));
  layer_weight[RA] = layer_weight[nameDateRobotToffi::FA];

  // spočte nové otáčky pomocí zrychlení a časové délty
  calcLayer0(NL); calcLayer0(NP); calcLayer0(FA); calcLayer0(RA);
  layer_date[NL] = getLayer0_date(NL) + ((getLayer0_date(FA) * 1000) + (getLayer0_date(RA) / (TRACK / 2)/* rotační zrychlení */)) / deltaTime;
  layer_weight[NL] = (getLayer0_weight(NL) + getLayer0_weight(AL)) / 2;

  layer_date[NP] = getLayer0_date(NP) + ((getLayer0_date(FA) * 1000) - (getLayer0_date(RA) / (TRACK / 2)/* rotační zrychlení */)) / deltaTime;
  layer_weight[NP] = (getLayer0_weight(NP) + getLayer0_weight(AP)) / 2;
  return true;
}

bool ToffiMat::calcMoment() {
  // pomocí rotace o jeden byte je výpočet se nahrazuje nutnost použití float, které by bylo výrazně pomalejší
  // poměr krácení momentu síli daného charakteristikou motoru je s přesností 1/256
  //  layer_date[ML] = (getLayer0_date(UL) * getLayer0_date(kMUL) * (getLayer0_date(NL)>kdNmk?256:(getLayer0_date(NL)<<8)/kdNmk))>>8;
  //  layer_date[MP] = (getLayer0_date(UP) * getLayer0_date(kMUP) * (getLayer0_date(NP)>kdNmk?256:(getLayer0_date(NP)<<8)/kdNmk))>>8;
  calcLayer0(UL); calcLayer0(NL);
  layer_date[ML] = convertM_UN(getLayer0_date(UL) - getLayer0_date(NL), getLayer0_date(NL));
  layer_weight[ML] = (getLayer0_weight(UL) + getLayer0_weight(NL)) / 2;
  calcLayer0(UP); calcLayer0(NL);
  layer_date[MP] = convertM_UN(getLayer0_date(UP) - getLayer0_date(NP), getLayer0_date(NP));
  layer_weight[MP] = (getLayer0_weight(UP) + getLayer0_weight(NP)) / 2;
  return true;
}
bool ToffiMat::calcSpeed() {
  // výpočet délty dráhy kol
  layer_date[dSL] = convertS_N(getLayer0_date(NL)) * layer_date[dT];
  layer_date[dSP] = convertS_N(getLayer0_date(NP)) * layer_date[dT];
  calcLayer0(dSL); calcLayer0(dSP);
  // výpočet poloměru rotace
  layer_date[dr] = (getLayer0_date(dSL) * (TRACK / 2) - getLayer0_date(dSP) * (TRACK / 2)) / (getLayer0_date(dSL) - getLayer0_date(dSP)); // poloměr rotace v mm v daném časovém úseku.
  // výpočet délty drah robota
  layer_date[dFS] = convertS_N((getLayer0_date(NL) + getLayer0_date(NP)) / 2) * layer_date[dT];
  layer_date[dRS] = (getLayer0_date(dSL) - getLayer0_date(dSP)) * TRACK;
  // výpočet rychlostí
  layer_date[Fv] = convertS_N(getLayer0_date(NL)) + convertS_N(getLayer0_date(NP) / 2);
  layer_weight[Fv] = (getLayer0_weight(NL)) + convertS_N(getLayer0_weight(NP)) / 3; // o 1/3 snížena váha z důvodu nejistoty údajů při nárazech, nebo špatném doteku kol s podložkou
  layer_weight[Rv] = layer_weight[Fv]; // váha zjištěné rotace je stejná jako dopředné rychlosti
  return false;
}

bool ToffiMat::calcPosition()
{
  layer_date[pR] = getLayer0_date(pR) + getLayer0_date(dRS);
  layer_weight[pR] = (getLayer0_weight(pR) + getLayer0_weight(dRS)) / 2;
  calcLayer0(pR);
  layer_date[pX] = getLayer0_date(pX) * sin(getLayer0_date(pR) / 256.0);
  layer_weight[pX] = (getLayer0_weight(pX) + getLayer0_weight(pR)) / 2;
  layer_date[pY] = getLayer0_date(pX) * cos(getLayer0_date(pR) / 256.0);
  layer_weight[pY] = (getLayer0_weight(pY) + getLayer0_weight(pR)) / 2;
  calcLayer0(pX); calcLayer0(pY);
  return true;
}
