#include "kra_freertos_sablona.h"
#include <Arduino.h>
#include <semphr.h> // add the FreeRTOS functions for Semaphores (or Flags).


FreeRtosSablona::FreeRtosSablona(const bool startTaskTmp):startTask(startTaskTmp)
{
  static char taskNumberCountInterni = 'a';
  char taskName[5] = "Ta_";
  taskName[3] = taskNumberCountInterni;
  if (1) {
    Serial.begin(115200);
    while (!Serial) {};
    Serial.print("FreeRtosSablona::FreeRtosSablona() ");
    Serial.print(taskName);
    Serial.println("");
  }
  startInterni(128 + taskNumberCountInterni, 3, taskName);
  taskNumberCountInterni++;
}

FreeRtosSablona::FreeRtosSablona(const bool startTaskTmp, unsigned portSHORT _port, UBaseType_t _priority, const char *_name):startTask(startTaskTmp)
{
  if (0) {
    //  Serial.begin(115200);
    while (!Serial) {};
    Serial.println("FreeRtosSablona::FreeRtosSablona(unsigned portSHORT _port, UBaseType_t _priority, const char *_name)");
  }
  startInterni(_port, _priority, _name);
}

void FreeRtosSablona::startInterni(unsigned portSHORT _port, UBaseType_t _priority, const char *_name)
{
  if (0) {
    //  Serial.begin(115200);
    while (!Serial) {};
    //  Serial.print("xTaskCreate(FreeRtosSablona::task,\"");
    //  Serial.print((const portCHAR *)_name);
    //  Serial.print("\",");
    //  Serial.print(_port);
    //  Serial.print(",");
    //  Serial.print((int)this);
    //  Serial.print(',');
    //  Serial.print(_priority);
    //  Serial.print(',');
    //  Serial.print((int)&this->taskHandle);
    //  Serial.print(")");
  }
  if (startTask) {
    xTaskCreate(
      FreeRtosSablona::task, (const portCHAR *)_name // name
      ,
      _port //
      ,
      this // poineter instance
      ,
      _priority // Priority
      ,
      &this->taskHandle);
    //    Serial.println("Task vytvořen");
  }
}

FreeRtosSablona::~FreeRtosSablona()
{
#if INCLUDE_vTaskDelete
  if (startTask) vTaskDelete(taskHandle);
#endif
  return;
}

void FreeRtosSablona::task(void *pvParameters)
{
  Serial.println("spoustim task.");
  delay(100);
  FreeRtosSablona *p = static_cast<FreeRtosSablona *>(pvParameters);
  Serial.println("spousten task.");
  delay(100);
  for (;;) {
    p->start();
  }
  vTaskDelete(NULL);
}

void FreeRtosSablona::start()
{
  /* code */
  int i = 0;
  for (;;)
  {
    /* code */
    Serial.print("FreeRtosSablona::start() ");
    Serial.println(i++);
    vTaskDelay(50);
  }
  vTaskDelete(NULL);
}

/*----------------- Tasks  ---------------*/

TestTask1::TestTask1() : FreeRtosSablona(true)
{
  Serial.begin(115200);
  while (!Serial) {};
  //  Serial.begin(115200);
  //  while(!Serial){};
  Serial.println("StartTask::TestTask1()");

  // nastavení semaforu

#ifdef FREERTOS_SABLONA_SEMAFOR
  xSemaphoreSetterOrGetter = xSemaphoreCreateMutex(); // podobný jako xSemaphoreCreateBinary(), ale s proměnou prioritou, aby vlákno mající semafor mělo přednost před vláknem čekajícím na přidělený semafor
  if (xSemaphoreSetterOrGetter != NULL)
  { /* Semafor byl úspěšně vytvořen a
      lze jej použít. */
  }
#endif

};
TestTask1::TestTask1(unsigned portSHORT _port, UBaseType_t _priority, const char *_name) : FreeRtosSablona(true, _port, _priority, _name)
{
  Serial.begin(115200);
  while (!Serial) {};
  //  Serial.begin(115200);
  //  while(!Serial){};
  Serial.println("StartTask::TestTask1()");

  // nastavení semaforu
#ifdef FREERTOS_SABLONA_SEMAFOR
  xSemaphoreSetterOrGetter = xSemaphoreCreateMutex(); // podobný jako xSemaphoreCreateBinary(), ale s proměnou prioritou, aby vlákno mající semafor mělo přednost před vláknem čekajícím na přidělený semafor
  if (xSemaphoreSetterOrGetter != NULL)
  { /* Semafor byl úspěšně vytvořen a
      lze jej použít. */
  }
#endif

};
void TestTask1::start()
{
  // code
  int i = 100;
  for (;;)
  {
    // code
    Serial.print("TestTask1::start() ");
    Serial.println(i++);

    Serial.print("Task Sensor time:");
    //    Serial.print((int)xx);

    Serial.println(".");

    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}
