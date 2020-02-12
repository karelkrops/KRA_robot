#ifndef TOFFI_MAT_H
#define TOFFI_MAT_H

/**
   Autor: Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 1.0.1
   datum verze: 1.2.2020
*   
*   
*   V konstruktoru je třeba nastavit přepočítavací parametry na moment síly a rychlost zrychlení
*   rychlosti a zrychlení jsou udávány v mm/s a mm/s2
*   U robotů bez enkodérů na motorech, nebo bez akcelerometrů a nebo jiných rychlostních čidel, je třeba parametry nalézt pokusným měřením
*   
*/

#include "toffi_layer.h"

class ToffiMat: public ToffiLayer {
  public:
    ToffiMat(const bool startTaskTmp);
    int paramM_U;
    int paramA_M;
    void start();
    void execute();
    unsigned long deltaTime;
  protected:
    bool calcAcceleration();
    bool calcMoment();
    bool calcSpeed();
    bool calcPosition();
};

#endif // TOFFI_MAT_H
