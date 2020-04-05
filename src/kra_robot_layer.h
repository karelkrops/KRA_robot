#ifndef ROBOT_LAYER_H
#define ROBOT_LAYER_H

/**
   Autor: Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 1.0.5
   datum verze: 05.4.2020

   Tato třída zavádí statickou vrstvu, která konsoliduje data ze všech spuštěných instancí a rozhoduje o platnosti získaných dat.
   Další vlastností je, že jednotlivé instance dostávají spětnou vazu v případě, že se jejich data výrazně liší od těch momentálně považovaných za platná.

**********************  FreeRTOS ******Alpha verze - předpříprava ************************
*  FreeRTOS zatím není implemetováno a to hlavně z důvodu přenositelnosti na různé platformy
*
   třída je nadstavbou třídy FreeRtosSablona, která umožňuje každé instanci běžet v samostatném procesorovém vládně.

   Startovací procedura vlákna:
  virtual void start(){
  for (;;){
    vTaskDelay(1000 / portTICK_PERIOD_MS); // po dělení jde o čas v milisekundách.
  }
  };
  ********************  end FreeRTOS ***************************
*/

//#include <KRA_robot.h>
#include "kra_freertos_sablona.h"

class RobotLayer 
:public FreeRtosSablona
{
  public:
    enum nameDateRobot {r_speed};
    RobotLayer(const bool startTaskTmp);
    virtual ~RobotLayer();
    static bool calcAllLayer0();
    static bool calcLayer0(const short index);
    static long getLayer0_dateLong(const short index);
    static float getLayer0_date(const short index);
//    static long getLayer0_date1(const short index);
    static short getLayer0_weight(const short index);
    bool setLayer_date(const short index, const float value);
    bool setLayer_date(const short index, const float value, const short weight);
    bool setLayer_date(const short index, const long value);
    bool setLayer_date(const short index, const long value, const short weight);
    bool setLayer_date(const short index, const unsigned long value);
    bool setLayer_date(const short index, const unsigned long value, const short weight);
    bool setLayer_date(const short index, const short value);
    bool setLayer_date(const short index, const short value, const short weight);
    bool setLayer_date(const short index, const int value);
    bool setLayer_date(const short index, const int value, const short weight);
    float getLayer_date(const short index);
    long getLayer_dateLong(const short index);
    static int numberLayer;
    virtual void start();
  protected:
    static bool setLayer0_date(const short index, const long value, const short weight);
    static const int NUMBER_DATE = 10;
    static const int MAX_NUMBER_LAYER = 5;
    static float layer0_date[];
    static char layer0_weight[];
    long layer_date[NUMBER_DATE];
    char layer_weight[NUMBER_DATE];
    virtual bool sendDiference(const short index, const long value, const short weight, const short indexLayer); // zpětná vazba do vrstvy
    static RobotLayer* pointerLayer[];
    RobotLayer* firstLayer(); // zatím není využíváno
    RobotLayer* nextLayer(); // zatím není využíváno
    RobotLayer* isExistNextLayer(); // zatím není využíváno
    int aktuelLayer; // asi nemá využití
    static bool semaphoreTake();
    static bool semaphoreGive();
    static bool taskDelay(int milliSecond);
    bool taskDelete();
#ifdef RTOS_ON
    static SemaphoreHandle_t xSemaphoreSetterOrGetter;
#endif // RTOS_ON
  private:
};

#endif // ROBOT_LAYER_H
