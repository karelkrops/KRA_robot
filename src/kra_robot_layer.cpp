#include "kra_robot_layer.h"
#include <Arduino.h>

// inicializace statických proměnných
int RobotLayer::numberLayer = 0;
#ifdef RTOS_ON
SemaphoreHandle_t RobotLayer::xSemaphoreSetterOrGetter = SemaphoreHandle_t();
#endif // RTOS_ON
float RobotLayer::layer0_date[NUMBER_DATE] = {};
char RobotLayer::layer0_weight[NUMBER_DATE] = {};
RobotLayer* RobotLayer::pointerLayer[MAX_NUMBER_LAYER] = {};
//RobotLayer::xSemaphoreSetterOrGetter = RobotLayer::xSemaphoreCreateMutex();

RobotLayer::RobotLayer(const bool startTaskTmp): FreeRtosSablona(startTaskTmp) {
  bool debuger = false;
  if (debuger) {
    //    while (!Serial) {}
    if (debuger)  Serial.println("RobotLayer::RobotLayer()");
    if (debuger)  delay(100);
  }
  // vyčištění proměnných vrstvy
  for (int i = 0; i < NUMBER_DATE; i++) {
    layer_weight[i] = 0;
    layer_date[i] = 0;
    if (debuger)  Serial.print(i);
  }
  if (debuger)Serial.println("");
  // nalezení prázdného záznamu v tabulce instalovaných vrstev
  for (int i = 0; i < MAX_NUMBER_LAYER; i++) {
    if (debuger)  Serial.print(i);
    if (debuger)  Serial.print(",");
    if (pointerLayer[i] == 0) {
      // uložení prázdné pozice a její obsazení nově vytvořenou vrstvou
      pointerLayer[i] = this;
      numberLayer++; // zvýšení počtu instalovaných vrstev
      if (debuger) {
        Serial.print("pointer task ");
        Serial.print(i);
        Serial.print(" = ");
        Serial.print(pointerLayer[i]->layer_date[0]);
        Serial.print(" = ");
        Serial.println((unsigned int)pointerLayer[i]);
        delay(100);
      }
      break;
    }
  }
  // inicializace semaforu na ochranu zápisu a čtení dat ve vícevláknové aplikaci
#ifdef RTOS_ON
  if (xSemaphoreSetterOrGetter != NULL)
  { // Semafor byl úspěšně vytvořen a lze jej použít.
    //    Serial.println("vytvoren semafor xSemaphoreSetterOrGetter");
  } else {
    //    Serial.println("vytvarim semafor xSemaphoreSetterOrGetter");
    RobotLayer::xSemaphoreSetterOrGetter = xSemaphoreCreateMutex(); // podobný jako xSemaphoreCreateBinary(), ale s proměnou prioritou, aby vlákno mající semafor mělo přednost před vláknem čekajícím na přidělený semafor
    //    Serial.println("vytvoren semafor xSemaphoreSetterOrGetter");
  }
#endif // RTOS_ON

}
RobotLayer::~RobotLayer() {
  Serial.print("Task destruktor");
  for (int i = 0; i < MAX_NUMBER_LAYER; i++) { // vyhledání rušené vrstvy
    if (RobotLayer::pointerLayer[i] == this) {
      RobotLayer::pointerLayer[i] = 0; //deaktivace rušené vrstvy.
    }
    numberLayer--;
    break;
  }

}

bool RobotLayer::semaphoreTake() {
#ifdef RTOS_ON
  return xSemaphoreTake(xSemaphoreSetterOrGetter, (TickType_t)10) == pdTRUE;
#endif // RTOS_ON
  return true;
}
bool RobotLayer::semaphoreGive() {
#ifdef RTOS_ON
  return xSemaphoreGive(xSemaphoreSetterOrGetter); // odevzdání semaforu
#endif // RTOS_ON
  return true;
}
bool RobotLayer::taskDelay(int milliSecond) {
#ifdef RTOS_ON
  vTaskDelay(milliSecond/portTICK_PERIOD_MS);
  return true;
#endif // RTOS_ON
  delay(milliSecond);
return true;
}
bool RobotLayer::taskDelete() {
#ifdef RTOS_ON
  // pokud by bylo třeba vlákna okončit, je třeba ho takto destruhovat
  if ( this->taskHandle != NULL )
  vTaskDelete(NULL);
#endif // RTOS_ON
return true;
}


void RobotLayer::start() {
  taskDelay( 200 );
  Serial.println("RobotLayer::start()");
  for (;;) {
    //    break;  // vlakno nejde ukončit. Dochází pak k cyklickému spouštění setup()
    taskDelay(10);
  }
    taskDelete();
}
bool RobotLayer::setLayer0_date(const short index, const long value, const short weight) {
  if (semaphoreTake())
  { // zvednutí semaforu
    if (index < NUMBER_DATE && index >= 0) {
      layer0_date[index] = value;                           // změna hodnot při zaručeném jedinečném přístupu
      layer0_weight[index] = weight;
      semaphoreGive(); // odevzdání semaforu
      return true;
    } else return false;
  }
  return false;
}

bool RobotLayer::setLayer_date(const short index, const float value) {
  // **************************************************************** DEBUGER ****************
  if (0) {
    Serial.print("RobotLayer::setLayer_date(");
    Serial.print(index);
    Serial.print(",");
    Serial.print((int)value);
    Serial.println(");");
  }
  if (index < NUMBER_DATE && index >= 0) {
    layer_date[index] = value;
    return true;
  } else return false;
}
bool RobotLayer::setLayer_date(const short index, const float value, const short weight) {
  if (setLayer_date(index, value)) {
    layer_weight[index] = weight;
    return true;
  } else return false;
}
bool RobotLayer::setLayer_date(const short index, const long value) {
  return setLayer_date(index, float(value));
}
bool RobotLayer::setLayer_date(const short index, const unsigned long value) {
  return setLayer_date(index, float(value));
}
bool RobotLayer::setLayer_date(const short index, const short value) {
  return setLayer_date(index, float(value));
}
bool RobotLayer::setLayer_date(const short index, const int value) {
  return setLayer_date(index, float(value));
}
bool RobotLayer::setLayer_date(const short index, const long value, const short weight) {
  return setLayer_date(index, float(value), weight);
}
bool RobotLayer::setLayer_date(const short index, const unsigned long value, const short weight) {
  return setLayer_date(index, float(value), weight);
}
bool RobotLayer::setLayer_date(const short index, const short value, const short weight) {
  return setLayer_date(index, float(value), weight);
}
bool RobotLayer::setLayer_date(const short index, const int value, const short weight) {
  return setLayer_date(index, float(value), weight);
}
bool RobotLayer::calcAllLayer0() {
  for (int i = 0; i < NUMBER_DATE; i++) {
    calcLayer0(i);
  }
  return true;
}

bool RobotLayer::calcLayer0(const short index) {
  bool debuger = false;
  if (debuger) {
    Serial.print("RobotLayer::calcLayer0  hodnota: ");
    Serial.print((int)index);
    delay(100);
  }
  if (index >= 0 && index < NUMBER_DATE) {
    if (semaphoreTake())
    { // zvednutí semaforu
      // změna hodnot při zaručeném jedinečném přístupu
      long date = 0;
      char weight = 0;
      // načtení nejdůvěryhodnější hodnoty
      for (int i = 0; i < MAX_NUMBER_LAYER; i++) {
        if (pointerLayer[i] != 0) {
          if (debuger) {
            Serial.print(" i:");

            Serial.print(i);
            Serial.print("=");
            Serial.print((int)pointerLayer[i]->layer_date[index]);
            Serial.print(", ");
          }
        }
        if (pointerLayer[i] != 0 && pointerLayer[i]->layer_weight[index] >= weight) {
          weight = pointerLayer[i]->layer_weight[index];
          date = pointerLayer[i]->layer_date[index];
        }
      }
      layer0_date[index] = date;
      layer0_weight[index] = weight;
      if (debuger) {
        Serial.print(" == ");
        Serial.print((int)date);
      }
      // odeslání zpětné vazby o chybě od nejdůvěryhodnější hodnotě
      for (int i = 0; i < MAX_NUMBER_LAYER; i++) {
        if (pointerLayer[i] != 0 && pointerLayer[i]->layer_weight[index] != 0) {
          if (1) {
            // odeslání zpětné vazby do ostatních vrstev
            // **********************************  zpětná vazba *******************************
            pointerLayer[i]->sendDiference(index, date, weight, i);
          }
        }
      }
      semaphoreGive(); // odevzdání semaforu
      if (debuger)Serial.println("");
      return true;
    }
  }
  if (debuger)Serial.println("");
  return false;
}


float RobotLayer::getLayer_date(const short index) {
  float x = 0;
  if (index >= 0 && index < NUMBER_DATE) {
    x = layer_date[index];
  }
  return x;
}


short RobotLayer::getLayer0_weight(const short index) {
  short x = 0;
  if (index >= 0 && index < NUMBER_DATE) {
    if (semaphoreTake())
    { // zvednutí semaforu
      x = layer0_weight[index];
      semaphoreGive(); // odevzdání semaforu
    }
  }
  return layer0_date[index];
  return x;
}
long RobotLayer::getLayer0_dateLong(const short index) {
  long x = 0;
  if (index >= 0 && index < NUMBER_DATE) {
    if (semaphoreTake())
    { // zvednutí semaforu
      x = long(layer0_date[index]);
      semaphoreGive(); // odevzdání semaforu
    }
  }
  return x;
}
float RobotLayer::getLayer0_date(const short index) {
  float x = 0;
  if (index >= 0 && index < NUMBER_DATE) {
    if (semaphoreTake())
    { // zvednutí semaforu
      x = layer0_date[index];
      semaphoreGive(); // odevzdání semaforu
    }
  }
  return x;
}



bool RobotLayer::sendDiference(const short index, const long value, const short weight, const short indexLayer) {
  if (index || value || weight || indexLayer) {};
  return true;
}
