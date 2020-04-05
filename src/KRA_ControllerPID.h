#ifndef KRA_CONTROLLERPID_H
#define KRA_CONTROLLERPID_H

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 0.0.1
 * datum verze: 15.2.2020
 * 
 * Třída je určena k počítání průběžných průměrů
 */

/**
 * popis digitální PID regulace (CZ): https://www.dixell.cz/teorie-regulace/
 * zdroj vzorečků: https://en.wikipedia.org/wiki/PID_controller
 * (CZ) https://cs.wikipedia.org/wiki/PID_regul%C3%A1tor
 */

class KRA_ControllerPID{
  public:
  KRA_ControllerPID();
  protected:
  float konstantI;
  float konstantD;
};

#endif // KRA_CONTROLLERPID_H
