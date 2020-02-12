#ifndef TOFFI_LAYER_H
#define TOFFI_LAYER_H
#include "kra_robot_layer.h"

/**
 * Autor: Karel Kropš
 * pro Kroužek robotiky a automatizace Písek (KRA)
 * verze: 1.0.1
 * datum verze: 1.2.2020
 * 
 * pro správný chod této nadstavby je třeba zadat rozvor kol a šířku stopy kol
 * TRACK a WHEELBASE. Oba parametry jsou v milimetrech;
 * 
 * 
 */

class ToffiLayer : public RobotLayer {
  public:
    enum nameDateRobotToffi : short {
                                     sL1, // směr naskenovaný line senzory v rozmezí (-127,127) 1 a -1 rovně. 0 bez liny
                                     sL2, // směr naskenovaný druhé line (křižovatka) senzory v rozmezí (-127,127) 1 a -1 rovně. 0 bez liny
                                     sLl, // poslední známí stav line senzoru před přerušenou line, nebo vypadnutím z otáčky
                                     isL, // stav line. 0=bez čáry, 1= jednačára, 2= dvě čáry, -1= příčná čára
                                     dT, // časový úsek [ms]
                                     UL, // PWM na levý motor -255 až 255
                                     UP, 
                                     ML = 2, // moment levého kola
                                     MP,// moment pravého kola
                                     NL, // otáčky levého kola. -255 až 255 odpovídá cílovému stavu UL
                                     NP, // otáčky pravého kola. -255 až 255 odpovídá cílovému stavu UP
                                     pMU, // konstanta přepočtu momentu síly v závyslosti na déltě napětí M=dU*pMU
                                     pAM, // parametr převodu momentu síly na akceleraci. A=M*mAM , parametr se může měnit v závyslosti na stavu a hmotnosti robota
                                     pSN, // konverze otáček N na rychlost v mm/s. parametr je závyslí na průměru kol.
                                     AL, // acceleration - zrychlení levého kola  @deprecate
                                     AP, // acceleration - zrychlení pravého kola  @deprecate
                                     FA, // forward acceleration - dopředné zrychlení
                                     RA, // rotational acceleration - rotační akcelerace
                                     Fv, // dopředná rychlost
                                     Rv, // rotační rychlost
                                     dSL, // délta dráhy ujetá levým kolem 
                                     dSP, // délta dráhy ujetá pravým kolem
                                     dFS, // délta dopředné dráhy
                                     dRS, // délta úhlové rotace
                                     pX, // pozice x v soustavě souřadnic
                                     pY, // pozice y v soustavě souřadnit
                                     pR, // natočení - azimut - směr v radiánech * 256 aby nebylo třeba používat čísla typu float
                                     dr, // poloměr rotace v mm v daném časovém úseku.
                                     dNL, // rozdíl (délta) otáček levého kola
                                     dNP, // 
                                     dRO, //
                                     O // orientace plošná. Rovinnový úhel od startovní pozice, nebo severu, pokud je k dispozici kompas.
                                    };
    static const long kdNmk = 5; // malá délta napětí, při které se láme charakteristika motoru
    static const int TRACK=15; // šířka stopy kol robota v mm (šířka mezi stopami kol);
    static const int WHEELBASE=0; // rozvor kol robota v mm (vzdálenost náprav - pouze u čtyřkolky)
    ToffiLayer(const bool startTaskTmp);
    void start();
    static int convertM_UN(int dU,int N){ // konverze délty napětí na motorech a otáček motoru na moment síly
      N=N;
      return dU*getLayer0_date(pMU);
    }
    static int convertU_NM(int N,int M){ // konverze napětí na motory na základě otáček motoru a požadovaného momentu síly
      return N+(M/getLayer0_date(pMU));
    }
    static int convertFA_ML_MP(int ML,int MP) // konverze momentú síly na jednotlivé motory na dopředného zrychlení
    {
      return ((ML+MP)/2)*getLayer0_date(pAM);
    }
    static int convertRA_ML_MP(int ML,int MP) // konverze momentú síly na jednotlivé motory na rotační zrychlení
    {
      return (ML-MP*TRACK)/getLayer0_date(pAM)/2;
    }
    static int convertML_FA_RA(int FA,int RA); // konverze dopředného a rotačního zrychlení na moment síly levého motoru
    static int convertMP_FA_RA(int FA,int RA); // konverze dopředného a rotačního zrychlení na moment síly pravého motoru 
    static int convertS_N(int N) // konverze otáček na rychlost v mm/s
    {
      return N*getLayer0_date(pSN);
    }
    static int conversN_S(int S) // konverze rychlosti na otáčky
    {
      return S/getLayer0_date(pSN);
    }
  protected:
};

#endif // TOFFI_LAYER_H
