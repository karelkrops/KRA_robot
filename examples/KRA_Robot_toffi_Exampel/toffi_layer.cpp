#include "toffi_layer.h"
#include <Arduino.h>

ToffiLayer::ToffiLayer(const bool startTaskTmp): RobotLayer(startTaskTmp) {
  //  Serial.print("ToffiLayer::ToffiLayer()");
}

void ToffiLayer::start() {
  //  Serial.print("ToffiLayer::start()");
  for (;;) {
    Serial.print("ToffiLayer::start() - sem by se program nemel dostat.");
    taskDelay(10);
  }
}

int ToffiLayer::convertML_FA_RA(int FA, int RA) // konverze dopředného a rotačního zrychlení na moment síly levého motoru
{
  calcLayer0(pAM);
  Serial.println((int)getLayer0_date(pAM));
  delay(100);
  return FA * getLayer0_date(pAM) + (RA * getLayer0_date(pAM) * TRACK) / 2;
}
int ToffiLayer::convertMP_FA_RA(int FA, int RA) // konverze dopředného a rotačního zrychlení na moment síly pravého motoru
{
  return FA * getLayer0_date(pAM) - (RA * getLayer0_date(pAM) * TRACK) / 2;
}
