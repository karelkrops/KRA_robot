


#ifndef KRA_FREERTOS_SABLONA_H
#define KRA_FREERTOS_SABLONA_H

//#define RTOS_ON

#ifdef RTOS_ON

#ifdef __AVR__
#include <Arduino_FreeRTOS.h> // FreeRTOS for AVR
#include <semphr.h>           // add the FreeRTOS functions for Semaphores (or Flags).
#define AVR_RTOS
#endif // __AVR__

#ifdef __STM32__
#include <STM32FreeRTOS.h>
#endif // __STM32__

#endif // RTOS_ON

/**
   autor Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 1.0.2
   datum verze: 14.2.2020

*/

/**
   třída slouží jako vzor pro vytvoření třídy v samosatatném časovém vláknu

   cílem je členům kroužku usnadnit tvorbu vlastní třídy, která pojede ve svém vlastním vlákně
   Stačí tedy pouze vytvořit class dědící FreeRtosSablona a obsahující funkci start().

  class TestTask1:public FreeRtosSablona {
     public:
     TestTask1();
     ~TestTask1(){};
     virtual void start();
  };
*/

/**
   RTOSS popis: https://www.freertos.org/a00106.html

   zdroje:
   https://www.freertos.org/FreeRTOS_Support_Forum_Archive/March_2008/freertos_C_Class_for_FreeRTOS_1974955.html

   https://fjrg76.wordpress.com/2018/05/20/objectifying-task-creation-in-freertos/

   // RTOS timer příklad
   https://stackoverflow.com/questions/29007191/using-an-rtostimer-inside-a-class

   // RTOS Semafor
   https://www.hackster.io/feilipu/using-freertos-semaphores-in-arduino-ide-b3cd6c

   instrukce na umístění knihovny do knihovny Arduina
   https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ

   Podrobná specifikace knihoven pro Arduino
   https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification

*/
#ifdef RTOS_ON
#define FREERTOS_SABLONA_SEMAFOR
#endif //RTOS_ON

class FreeRtosSablona
{
  protected:
  private:
    void startInterni(unsigned int _port, int _priority, const char *_name);
    static void task(void *pvParameters);
    /* data */
  public:
    const bool startTask;
    FreeRtosSablona(const bool startTask);
    FreeRtosSablona(const bool startTask, unsigned int _port, int _priority, const char *_name);
    virtual ~FreeRtosSablona();
    virtual void start();
#ifdef RTOS_ON
    TaskHandle_t taskHandle;
#endif // RTOS_ON
};

/*----------------- Tasks  ---------------*/

class TestTask1 : public FreeRtosSablona
{
  public:
    TestTask1();
    TestTask1(unsigned int _port, int _priority, const char *_name);
    ~TestTask1() {};
    virtual void start();

    // ukázka použití semaforu
#ifdef FREERTOS_SABLONA_SEMAFOR
    SemaphoreHandle_t xSemaphoreSetterOrGetter;
    int value1;
    bool setValue1(int val)
    {
      if (xSemaphoreTake(xSemaphoreSetterOrGetter, (TickType_t)10) == pdTRUE)
      { // zvednutí semaforu
        value1 = val;                             // změna hodnot při zaručeném jedinečném přístupu
        xSemaphoreGive(xSemaphoreSetterOrGetter); // odevzdání semaforu
        return true;
      }
      return false;
    }
#endif // FREERTOS_SABLONA_SEMAFOR

};

#endif //KAREL_FREERTOS_SABLONA_H
