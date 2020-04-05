#include <Arduino.h>
#include "kra_robot.h"

//TestTask1* task1;

unsigned long oldTimeTask11 = 0;
int timeTask11 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  delay(100);
  Serial.println("START TASK");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Task11 time: ");
  Serial.println(timeTask11);
  delay(1000);
}
