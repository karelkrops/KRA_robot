#ifndef KRA_BUTTON_H
#define KRA_BUTTON_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 1.0.0
 * datum verze: 19.1.2020
 */

/**
   třída pro zpracování reakce na tlačítko

   příklad inicializace:
 * ************** inicializace *********
  KRA_Button t1=KRA_Button(5);


   příklad načtení tlačítka:
 * **************  click  ***************
  switch(t1.getClick()){
    case 1:
      Serial.println("Jedno click");
      break;
    case 2:
      Serial.println("Double click");
      break;
    case 3:
      Serial.println("Tripl click");
      break;
    default:
      break;
  }
*************** click verze 2 **********
  int t1Status = t1.getClick();
  if (t1Status=1) ...
  if (t1Status=2) ...
  if (t1Status=3) ...
*************** status *****************
  if (t1.getStatus()) ...


odkaz na popis jak tvořit "sketch" (Library for Arduino) https://www.arduino.cc/en/Hacking/LibraryTutorial
https://www.arduino.cc/en/guide/libraries

*/

class KRA_Button
{
  private:
    const int pinNumber;
    unsigned long timeMinimum; // počet milisekund mezi změnou statusu tlačítka. Odstranění záchvěvů
    unsigned long timeClick; // doba mezi click
  public:
    KRA_Button(int pin);
    ~KRA_Button();
    bool getStatus();
    int getClick();
  protected:
    bool getPulse();
    unsigned long timerLastPulse;
    int numberClick;
    bool lastStatus;
    unsigned long lastTimeChange;
    bool lastStatusTemp;
};




#endif
