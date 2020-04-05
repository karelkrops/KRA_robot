#include <KRA_robot.h>

#include <Arduino.h>


#include "toffi_line_scanner.h"
#include "toffi_motor.h"
//#include "toffi_mat.h"

//Button bStart=Button(10);
//Button bStop=Button(3);


ToffiLineScanner *scanner;
ToffiMotor *motor;
//ToffiMat *mat;

KRA_Timer timerVypisy = KRA_Timer(1000);
const bool startTask = false;
void vypisStavu();

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  //  delay(500);
  Serial.println("Program start.");
  delay(1000);
  //    mat = new ToffiMat(startTask);
  scanner = new ToffiLineScanner(startTask);
delay(1000);
  motor = new ToffiMotor(startTask);
//  motor->calcAllLayer0();
  unsigned short i1 = (unsigned int) scanner;
  Serial.println();
  Serial.print("adresa scanner:");
  Serial.println(i1, HEX);
  delay(100);
  // put your setup code here, to run once:

//  motor->setPower(0, 0);
//  xTaskCreate(taskToffiMonitor,"monitor",128,NULL,3,NULL);
}

void loop() {

  //  if(bStart.getClick()>0)Serial.println("Byl stisknut START");
  //  if(bStop.getClick()>0)Serial.print("Byl stisknut STOP");

  if(!startTask)scanner->execute();
  scanner->calcAllLayer0();
  if(!startTask)motor->execute();
  if (scanner->isLine() <= 0)motor->setPower(0, 0);
  if (scanner->isLine() > 0)motor->setPower(100 - scanner->getLine1(), 100 + scanner->getLine1());

  //    mat->execute();
  if (!startTask && timerVypisy.isFinishAndRestart()) vypisStavu();
}


void vypisStavu() {
  //    Serial.println("TaskLoop()");
  Serial.print("line1:");
  Serial.print(int(scanner->getLine1()));
  //    Serial.print("-");

      Serial.print(scanner->getLayer0_date(scanner->sL1));
  Serial.print(" line2:");
  Serial.print(int(scanner->getLine2()));
  Serial.print(" lines:");
  Serial.print(int(scanner->isLine()));
  Serial.print(". ");

  Serial.print("Motor: UL ");
  Serial.print(motor->getLayer0_date(motor->UL));
  Serial.print(", UP ");
  Serial.print(motor->getLayer0_date(motor->UP));
  Serial.println("");

  if (1) {
    for (int i = 0; i < 8; i++) {
      Serial.print(" (");
      Serial.print(i);
      Serial.print(")=");
      Serial.print(scanner->readDigitalSenzor(i));
    }
    Serial.println("");
  }

}
