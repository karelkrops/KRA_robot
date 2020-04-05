#include <KRA_robot.h>
KRA_Button t1 = KRA_Button(10);
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:

}

void loop() {
  if (t1.getStatus()) {
    // tlačítko je právě stisknuto
  }
  int i = t1.getClick();
  if (i == 1)
    Serial.println("tlocitko bylo stisknoto jednou. (click)");
  if (i == 2)
    Serial.println("tlacitku bylo stisknutu dvakrat. (double click)");
  if (i == 3)
    Serial.println ("tlacitko bylo stisknuto trikrat. (triple click)");
  // put your main code here, to run repeatedly:

}
