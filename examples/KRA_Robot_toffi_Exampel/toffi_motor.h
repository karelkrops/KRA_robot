#ifndef TOFFI_MOTOR_H
#define TOFFI_MOTOR_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 1.0.1
 * datum verze: 1.2.2020
 * 
 * Parametrem motorů je rychlost na levém a pravém motoru 
      bool setPower(int pL, int pP);
 * Dalším parametrem je dopředné zrychlení zrychlení, kterým má robot nastavených parametrů dosáhnout
      bool setForwardAcceleration(int forwardAcceleration);
 * Dalším parametrem je rotační zrychlení, kterým má robot nastavených parametrů dosáhnout
      bool setRotationAcceleration(int rotationAcceleration);
 *
 */

#include "toffi_layer.h"



class ToffiMotor : public ToffiLayer {
  public:
    ToffiMotor(const bool startTask);
    void execute();
    void start();
    bool setPower(int pL, int pP){
      engineUL=pL;
      engineUP=pP;
      return true;
    }
    bool setForwardAcceleration(int forwardAcceleration){
      paramFA=forwardAcceleration;
      return true;
    }
    bool setRotationAcceleration(int rotationAcceleration){
      paramRA=rotationAcceleration;
      return true;
    }
  protected:
    bool motory(int pL, int pP);
    static const int PINTLAC = 6;
    static const int PINDIRL = 7;
    static const int PINDIRR = 8;
    static const int PINPWML = 9;
    static const int PINPWMR = 10;
    static const int MAXML = 100; // maximální povolená akcelerace (z důvodu zamezení smyku
    static const int MAXMP = 100; // maximální povolená akcelerace (z důvodu zamezení smyku
    static const int rychlostMax = 255; // stará logika. Maximální hodnota do PWM motorů
    int paramFA; // parametr forward acceleration
    int paramRA; // parametr rotation acceleration
    int engineML;
    int engineMP;
    int engineUL; // 
    int engineUP;
    int powerNL;
    int powerNP;

};
#endif // TOFFI_MOTOR_H
