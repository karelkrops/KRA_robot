#include "toffi_line_scanner.h"


ToffiLineScanner::ToffiLineScanner(const bool startTaskTmp)
  :  ToffiLayer(startTaskTmp),
     PIN_infraOn(13),
     numberSenzor(8),
     PIN_cidlo1(A0),
     PIN_cidlo2(A1),
     PIN_cidlo3(A2),
     PIN_cidlo4(A3),
     PIN_cidlo5(A4),
     PIN_cidlo6(A5),
     PIN_cidlo7(A6),
     PIN_cidlo8(A7)
{
  lineON = 500;
  lineOFF = 100;
  lineBorder = 400;
  pinMode(PIN_infraOn, OUTPUT);
  digitalWrite(PIN_infraOn, 1); // zapnutí podsvícení snímacích čidel
  lastLine = 0; // nastavení posledního známého stavu čidla
  timeOFF = millis();
  line1 = 0;
  line2 = 0;
  lineWeight = 100;
  isLineNumber = 0;
#ifdef  RTOS_ON
  if (startTask) {
    xSemaphoreSensor = xSemaphoreCreateMutex();
    if (xSemaphoreSensor != NULL)
    { // Semafor byl úspěšně vytvořen a lze jej použít.
    }
  }
#endif // RTOS_ON

}

ToffiLineScanner::ToffiLineScanner(const bool startTaskTmp, const int numberLineSenzor, const int PINinfraOn, const int firsPIN_cidlo): ToffiLayer(startTaskTmp),
  PIN_infraOn(PINinfraOn),
  numberSenzor((numberLineSenzor < 9 && numberLineSenzor > 3) ? numberLineSenzor : 5),
  PIN_cidlo1(firsPIN_cidlo),
  PIN_cidlo2(firsPIN_cidlo + 1),
  PIN_cidlo3(firsPIN_cidlo + 2),
  PIN_cidlo4(firsPIN_cidlo + 3),
  PIN_cidlo5(firsPIN_cidlo + 4),
  PIN_cidlo6(firsPIN_cidlo + 5),
  PIN_cidlo7(firsPIN_cidlo + 6),
  PIN_cidlo8(firsPIN_cidlo + 7)
{
  lineON = 500;
  lineOFF = 100;
  lineBorder = 400;
  pinMode(PIN_infraOn, OUTPUT);
  digitalWrite(PIN_infraOn, 1); // zapnutí podsvícení snímacích čidel
  lastLine = 0; // nastavení posledního známého stavu čidla
  timeOFF = millis();
  line1 = 0;
  line2 = 0;
  lineWeight = 100;
  isLineNumber = 0;
#ifdef  RTOS_ON
  if (startTask) {
    xSemaphoreSensor = xSemaphoreCreateMutex();
    if (xSemaphoreSensor != NULL)
    { // Semafor byl úspěšně vytvořen a lze jej použít.
    }
  }
#endif // RTOS_ON
}

void ToffiLineScanner::cteniCidel()
{

  cidlo[0] = analogRead(PIN_cidlo1);
  cidlo[1] = analogRead(PIN_cidlo2);
  cidlo[2] = analogRead(PIN_cidlo3);
  cidlo[3] = analogRead(PIN_cidlo4);
  cidlo[4] = analogRead(PIN_cidlo5);
  cidlo[5] = analogRead(PIN_cidlo6);
  cidlo[6] = analogRead(PIN_cidlo7);
  cidlo[7] = analogRead(PIN_cidlo8);
#ifdef  RTOS_ON
  if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)10) == pdTRUE)
#endif // RTOS_ON
  { // zvednutí semaforu

    /*
      bCidlo[0] = cidlo[0] > lineBorder;
      bCidlo[1] = cidlo[1] > lineBorder;
      bCidlo[2] = cidlo[2] > lineBorder;
      bCidlo[3] = cidlo[3] > lineBorder;
      bCidlo[4] = cidlo[4] > lineBorder;
      bCidlo[5] = cidlo[5] > lineBorder;
      bCidlo[6] = cidlo[6] > lineBorder;
      bCidlo[7] = cidlo[7] > lineBorder;
    */

    int line = 0;
    line1 = 0;
    line2 = 0;
    int firstSensor = numberSenzor * 8 - numberSenzor * 16 + 8;
    long cidlo0 = 0;
    long cidlo1 = 0;
    long cidlo2 = 0;
    int cidloMin = 2047;
    int cidloMax = 0;
    int tmpNumberBorder = 0;
    bool firstLine = false;
    bool firstLineEnd = false;
    bool secndLine = false;
    bool isCrossLine = true;
    for (int i = 0 ; i < numberSenzor ; i++) {
      //   if (cidlo[i] > lineBorder) { // detekce čáry a načtení jejího okolí
      cidloMin = cidloMin > cidlo[i] ? cidlo[i] : cidloMin;
      cidloMax = cidloMax < cidlo[i] ? cidlo[i] : cidloMax;
      cidlo0 = i > 0 ? cidlo[i - 1] : lineOFF;
      cidlo1 = cidlo[i];
      cidlo2 = (i < (numberSenzor - 1)) ? cidlo[i + 1] : lineOFF;
      if (cidlo1 > lineBorder) { // detekce čáry
        if (!firstLineEnd)firstLine = true;
        if (firstLineEnd)secndLine = true;
        tmpNumberBorder++;
        if (tmpNumberBorder == 3) secndLine = true;
        line = ((cidlo1 - cidlo0) * 16) / ((cidlo1 - cidlo2) + (cidlo1 - cidlo0)) - 8 + firstSensor + i * 16;
        line = (line * 100) / ((numberSenzor - 1) * 8);
        if (secndLine) {
          line2 = line;
          if (line2 == 0)line2 = 1;
        } else {
          if (firstLine)firstLineEnd = true;
          line1 = line;
        }

      } // detekce čáry
      else {
        isCrossLine = false;
        if (tmpNumberBorder > 1) secndLine = true;
      }
    } // for
    if (tmpNumberBorder > 0 && line2 == 0) {
      lineON = ((long)lineON * 16 - lineON + cidloMax) / 16 ; // průměrná hodnota čidla
      lineOFF = ((long)lineOFF * 16 - lineOFF + cidloMin) / 16; // průměrná hodnota čidla
      lineBorder = (lineON * 3 + lineOFF)  / 4;
      lastLine = line1;
      lineWeight = 100;
      timeOFF = millis();
      isLineNumber = 1;
    } else {
      // postupné snižování věrohodnosti dat
      if (lineWeight > 0) {
        lineWeight = 100 - (millis() - timeOFF) / 100;
      }
      if (lineWeight < 0) lineWeight = 0;
      if (line2 != 0) isLineNumber = 2; else isLineNumber = 0;
      if (isCrossLine) isLineNumber = -1;
    }
    // měření času mezi jednotlivými čtení čidel
    timeTask = millis() - timeStart;
    timeStart = millis();
    //    xx= (timeTask>xx || xx<0 || xx >600)?timeTask:xx; // proměná vytvořena z důvodu znaššího měření intervalu mezi snímání čidel
    /*
          Serial.print(cidloMin);
          Serial.print("-");
          Serial.print(lineOFF);
          Serial.print("  ");
          Serial.print(cidloMax);
          Serial.print("-");
          Serial.print(lineON);
          Serial.println("  ");
    */
#ifdef  RTOS_ON
    if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
#endif // RTOS_ON
    // #ifdef DEBUG_LINE_SENSOR

    if (0) {
      Serial.print("LINE senzory: ");
      Serial.print(cidlo[0]);
      Serial.print(" ");
      Serial.print(cidlo[1]);
      Serial.print(" ");
      Serial.print(cidlo[2]);
      Serial.print(" ");
      Serial.print(cidlo[3]);
      Serial.print(" ");
      Serial.print(cidlo[4]);
      Serial.print(" ");
      Serial.print(cidlo[5]);
      Serial.print(" ");
      Serial.print(cidlo[6]);
      Serial.print(" ");
      Serial.print(cidlo[7]);

      Serial.print(", ON:");
      Serial.print((int)cidloMax);
      Serial.print("-");
      Serial.print((int)lineON);
      Serial.print(", OFF:");
      Serial.print((int)cidloMin);
      Serial.print("-");
      Serial.print(lineOFF);
      Serial.print(", BORRDER:");
      Serial.print(lineBorder);

      Serial.print(", LINE1:");
      Serial.print((int)line1);
      Serial.print(", LINE2:");
      Serial.print((int)line2);
      Serial.print(", LineLast:");
      Serial.print((int)lastLine);

      Serial.print(", isLine:");
      Serial.print(getLayer_date(isL), DEC);
      Serial.print(", váha:");
      Serial.print(layer_weight[sL1], HEX);

      Serial.println();
    }
    // #endif // DEBUG_LINE_SENSOR
  } // semafor
  setLayer_date(sL1, line1, lineWeight);
  setLayer_date(sL2, line2, lineWeight);
  setLayer_date(sLl, lastLine, lineWeight);
  setLayer_date(isL, isLineNumber, lineWeight);
  //  layer_date[sL1] = line1;
  //  layer_date[sL2] = line2;
  //  layer_date[sLl] = lastLine;


}

bool ToffiLineScanner::readDigitalSenzor(const int index) {
  bool x = false;
  if (index >= 0 && index < numberSenzor) {
#ifdef  RTOS_ON
    if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)10) == pdTRUE)
    { // zvednutí semaforu
#endif // RTOS_ON
      x = cidlo[index] > lineBorder;;
#ifdef  RTOS_ON
      if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
    }
#endif // RTOS_ON
  }
  return x;
}
int ToffiLineScanner::isLine() {
  int x = false;
#ifdef  RTOS_ON
  if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)10) == pdTRUE)
  { // zvednutí semaforu
#endif // RTOS_ON
    x = isLineNumber;
#ifdef  RTOS_ON
    if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
  }
#endif // RTOS_ON
  return x;
}
bool ToffiLineScanner::isCrossLine() {
  return isLine() == -1;
}
int ToffiLineScanner::getLine1() {
  int x = 0;
#ifdef  RTOS_ON
  if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)10) == pdTRUE)
  { // zvednutí semaforu
#endif // RTOS_ON
    /*
      Serial.print("LINE senzory: ");
      Serial.print(cidlo[0]);
      Serial.print(" ");
      Serial.print(cidlo[1]);
      Serial.print(" ");
      Serial.print(cidlo[2]);
      Serial.print(" ");
      Serial.print(cidlo[3]);
      Serial.print(" ");
      Serial.print(cidlo[4]);
      Serial.print(" ");
      Serial.print(cidlo[5]);
      Serial.print(" ");
      Serial.print(cidlo[6]);
      Serial.print(" ");
      Serial.print(cidlo[7]);
      Serial.print(", ON:");

      Serial.print(lineON);
      Serial.print(", OFF:");

      Serial.print(lineOFF);
      Serial.print(", BORRDER:");
      Serial.print(lineBorder);
      Serial.print(", LINE1:");
      Serial.print((int)line1);
      Serial.print(", LINE2:");
      Serial.print((int)line2);
      Serial.print(", LineLast:");
      Serial.print((int)lastLine);
      Serial.print(", isLine:");
      Serial.print(layer_weight[isL], DEC);
      Serial.print(", váha:");
      Serial.print(layer_weight[sL1], HEX);
      Serial.println();
    */
    x = line1;
#ifdef  RTOS_ON
    if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
  }
#endif // RTOS_ON
  return x;
}
int ToffiLineScanner::getLine2() {
  int x = 0;
#ifdef  RTOS_ON
  if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)10) == pdTRUE)
  { // zvednutí semaforu
#endif // RTOS_ON
    x = line2;
#ifdef  RTOS_ON
    if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
  }
#endif // RTOS_ON
  return x;
}
int ToffiLineScanner::getLastLine() {
  int x = 0;
#ifdef  RTOS_ON
  if (!startTask || xSemaphoreTake(xSemaphoreSensor, (TickType_t)1) == pdTRUE)
  { // zvednutí semaforu
#endif // RTOS_ON
    x = lastLine;
#ifdef  RTOS_ON
    if (startTask) xSemaphoreGive(xSemaphoreSensor); // odevzdání semaforu
  }
#endif // RTOS_ON
  return x;
}
void ToffiLineScanner::execute() {
  cteniCidel();
}

void ToffiLineScanner::start() {
  taskDelay(100);
  Serial.println("ToffiLineScanner::start()");
  taskDelay(100);
  // Initialise the xLastWakeTime variable with the current time.
  //    xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    cteniCidel();
#ifdef DEBUG_LINE_SENSOR
    taskDelay(4); // čekání 4ms
    // Wait for the next cycle.
    //         vTaskDelayUntil( &xLastWakeTime, xFrequency );
#else // DEBUG_LINE_SENSOR
    //    vTaskDelay(1);
#endif // DEBUG_LINE_SENSOR  
  }
}
