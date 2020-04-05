#ifndef TOFFI_LINE_SCANNER_H
#define TOFFI_LINE_SCANNER_H

/**
   autor Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 1.1.1
   datum verze: 14.2.2020

   třída zatím neumí sledovat negativní čáru. Sleduje pouze černou čáru na bílém podkladu
*/
#include <Arduino.h>
#include "toffi_layer.h"


#define DEBUG_LINE_SENSOR

class ToffiLineScanner : public ToffiLayer {
  public:
    ToffiLineScanner(const bool startTaskTmp);
    ToffiLineScanner(const bool startTaskTmp, const int numberLineSenzor, const int PINinfraOn, const int firsPIN_cidlo);
    const int PIN_cidlo1;
    const int PIN_cidlo2;
    const int PIN_cidlo3;
    const int PIN_cidlo4;
    const int PIN_cidlo5;
    const int PIN_cidlo6;
    const int PIN_cidlo7;
    const int PIN_cidlo8;
    const int PIN_infraOn;
    const int numberSenzor;
    //    int readAnalogSenzor(const int index);
    void execute();
    bool readDigitalSenzor(const int index); // načtení stavu konkrétního čidla
    int isLine(); // stav line. 0=bez čáry, 1= jednačára, 2= dvě čáry, -1= příčná čára
    bool isCrossLine();
    int getLine1();
    int getLine2();
    int getLastLine();
  protected:
    void cteniCidel();
    int cidlo[8];
//    bool bCidlo[8];
    int lineBorder;
    int lastLine;
    int lineON;
    int lineOFF;
    unsigned long timeOFF;
    void start();
    int line1;
    int line2;
    int lineWeight;
    int isLineNumber;
    unsigned long timeStart;
    unsigned long timeTask;  // čas trvání jednoho průběhu
    //     TickType_t xLastWakeTime;
    // const TickType_t xFrequency = 1;
#ifdef  RTOS_ON
    SemaphoreHandle_t xSemaphoreSensor;
#endif // RTOS_ON
};

#endif // TOFFI_LINE_SCANNER_H
