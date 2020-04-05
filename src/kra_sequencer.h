#ifndef KARA_SEQUENCER_H
#define KARA_SEQUENCER_H
#include <Arduino.h>

/**
   Autor: Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 2.0.0
   datum verze: 3.4.2020
*/


/**
    *********** format squnce / prikazu *************************
    * *****************************************************
   command:  xxxx(timeWaiting=0,timeActivate=0,value=""),xxnextx(100,1),xxnext2xxx,xxxnext3xx(0,0),atd..
   
   priklady prikazu:
   "rozsvit(0,1),cekej(0,10000),zhasni"
   "led(0,0,zluta),jed_a_hledej_caru(0,1000),led(0,0,\"cervena\"),zastav,"
   "led(2000,0,zelena),cteni_cidla(0,100),led(0,0,zluta)"  // zelena led je asinchroni, tedy nejprve se rozsviti zluta a teprve o 1900ms pozdeji se rozsviti zelena

   z prikladu je patrne ze:
   timeWaiting -> int cas v ms -> spusti na zacatku. Cas urcuje interval cekani pred spustenim. 0=bez cekani
                  Pokud je pred nim 'a' nebo 'A'-> pak jde cas assinchronek ke zybtku sequence.
   timeActivate -> int cas v ms -> spusti na zacatku. Cas urcuje interval cyklickeho volani funkce. 0 nebo 1 = spusti se pouze jednou
                  Pokud je pred nim 'a' nebo 'A'-> pak jde cas assinchronek ke zybtku sequence.
   value -> String -> muze, ale nemusi byt v uvozovkach. Pokud neni v uvozovkach, nesmi pouzivat znak ')'

    ************** void testSequencer(KRA_Command *pointer,String value); ***************
    * *******************************************************************************
   registrace funkce: void testSequencer(KRA_Command *pointer,String value);
   onCommand("xxxx",[](*pointeter,times){});  gde *pointer je ukazatel na sequncer a times je 0:start,-1:stop,jinak jecas od startu, max 32sekund
   registravana funkce muze squnceru sequneru odpovedet nasledujicim zpusobem
   
   pointer->answareContinue();  // odpovi, ze vse je OK a pokracuje se ve zpracovani
   pointer->answareNext();  // přejde na dalsi command stejne sequnce
   pointer->answareBreak(); // ukonci sequenci
   pointer->answareReset(); // pokracuje s novim casovim limitem
   pointer->runTime(); // vrati cas od startu kroku. Time je 0:start,-1:stop,jinak jecas od startu, max 32sekund
   pointer->pointerSequencer->addSequenc(String sequence); // pridani nove sequence
   pointer->pointerSequencer->addCommand(String sequence); // pridani jednoho prikazu k okamzitemu zpracovani


    *****************  odesilani sequnce ***************
    *  *****************************************
   KRA_Sequencer sequnce();
   sequnce.onCommand(char sequnceName[], void (&functionOn)(KRA_Command *pointer, String value); // registrace funkce
   sequnce.addSequenc(String sequence); // pridani nove sequence
   sequnce.addCommand(String sequence); // pridani jednoho prikazu k okamzitemu zpracovani

    **************DULEZITE ****************
    ***************************************
    ***********KRA_Sequencer::execute();****
   k zajisteni provozu je treba do smicky void loop(); umistit prikaz
   KRA_Sequencer::execute();
*/

class KRA_Sequencer;

class KRA_Command{
  public:
    KRA_Sequencer *pointerSequencer;
    String command; // jmeno aktualne zpracovavaneho command
    int commandIndex; // index aktualne zpracovavaneho command
    unsigned long commandTimes=0; // startovni cas command
    unsigned long timeActionCommand=0; // parametr maximalni cas command
    unsigned long timeWaitingCommand; // čas odložení akce command
    bool assinchronWaiting; // assinchroní čekání
    bool assinchronAction; // assinchronní akce
    bool assinchron;
    bool used;
    bool anNext; // priznak pro prechod na dalsi command
    bool anBreak; // priznak pro prechod na dalsi sequenc
  public:
    String commandValue; // parametr value pro command
    bool answareNext() {
      anNext = true;
      return true;
    }
    bool answareBreak() {
      anBreak = true;
      return true;
    }
    bool answareReset() {
      answareContinue();
          while ((commandTimes = millis()) == 0) // resetuje cas na zpracovani kroku
            {};
                  return true;
    }
    bool answareContinue() {
      return true; // oznamuje, ze nedoslok zatuhnuti. Proces pokracuje OK
    }
    bool resetTimer();
 
    void printDebug() {
      //      Serial.println("sequencActual:" + sequencActual);
      Serial.println("********DEB******** "+String(assinchronWaiting||assinchronAction?"assinchron":""));
      Serial.println("command:" + command + "(" + (assinchronWaiting ? "A" : "")+ String(timeWaitingCommand) + "," + (assinchronAction ? "A" : "") + String(timeActionCommand) + ",value=" + commandValue + ")times=" + String( millis() - commandTimes));
     // Serial.println("anNext:" + String(anNext ? "1" : "0") + " anBreak:" + String(anBreak ? "1" : "0"));
      Serial.println("*******************");
    }
};

class KRA_Sequencer {
  friend class KRA_Command;
  public:
    KRA_Sequencer();
    virtual ~KRA_Sequencer();
    bool onCommand(const char sequnceName[], void (&functionOn)(KRA_Command *pointer, String value));
    bool onCommand(String sequnceName, void (&functionOn)(KRA_Command *pointer, String value));
    bool addSequenc(String sequenc);
    bool addCommand(String command);
  protected:
    int findFreeCommand();
    int moveToAssinchron();
  public:
    bool breakSequnc();
//    bool resetTimer();
    static void execute();
    bool isRun() {
      return isRunIndikator == 0 ? false : true; // dotaz zda bezi ***  je treba dopsat. Zatim se muze stat, ze hodi true i kdyz ve fronte jeste ceka dalsi command, nebo cela dlsi squence
    }
    void debPrintCommand() { //
      Serial.println("void debPrintCommand():");
      Serial.println("sequencActual:" + sequencActual);
      Serial.println("sequencBuffer[0]:" + sequencBuffer[0]);
      Serial.println("sequencBuffer[1]:" + sequencBuffer[1]);
      Serial.println("sequencBuffer[2]:" + sequencBuffer[1]);
      Serial.println("sequencBuffer[3]:" + sequencBuffer[1]);
      Serial.println("sequencBuffer[4]:" + sequencBuffer[1]);
      Serial.println("sequencBuffer[5]:" + sequencBuffer[1]);
      //      Serial.println("typeFunction[0]:" + typeFunction[0]);
      //      Serial.println("typeFunction[1]:" + typeFunction[1]);
      //      Serial.println("typeFunction[2]:" + typeFunction[2]);
      // Serial.println("command:" + command + "(" + (callCommandStart ? "1" : "0") + "," + (callCommandInTime ? "1" : "0") + "," + (callCommandStop ? "1" : "0") + "," + String(timeActionCommand) + ",value=" + commandValue + ")times=" + String( millis() - ((unsigned long)commandTimes)));
      //Serial.println("commandTimes:" + String(commandTimes) + " callCommandStart:" + String(callCommandStart ? "1" : "0") + " callCommandInTime:" + String(callCommandInTime ? "1" : "0") + " callCommandStop:" + String(callCommandStop ? "1" : "0"));
      //      Serial.println("anNext:" + String(anNext ? "1" : "0") + " anBreak:" + String(anBreak ? "1" : "0"));
      Serial.println("*******************");
    }
    static int numberInstanc;
    bool executeItem();
  protected:
    static const int MAX_ASINCHRON_COMMAND = 20;
    KRA_Command commandItem[MAX_ASINCHRON_COMMAND];
    bool callCommandFunction(KRA_Command *command);
    bool nextCommand();
    String separeNextCommand();
    bool getNextSequence();
    KRA_Command getCommand(String *command);
    int findCommandIndex(String command);
    static const int MAX_SEQUNC_BUFFER = 10; // minimum 2
    static const int MAX_FUNCTION_BUFFER = 30;
    void (*functionSequenc[MAX_FUNCTION_BUFFER])(KRA_Command *pointer, String value); // pole funkci command
    String typeFunction[MAX_FUNCTION_BUFFER]; // pole s jmeny command
//    String command; // jmeno aktualne zpracovavaneho command
//    int commandIndex; // index aktualne zpracovavaneho command
//    long commandTimes; // startovni cas command
//    unsigned long timeActionCommand; // parametr maximalni cas command
//    bool anNext; // priznak pro prechod na dalsi command
//    bool anBreak; // priznak pro prechod na dalsi sequenc
//    bool callCommandStart; // parametr spoust pri startu casu command
//    bool callCommandInTime; // parametr cyklickeho spousteni v dobe casu command
//    bool callCommandStop; // parametr volani po uplinuti casu command
//    String commandValue; // parametr value pro command
    String sequencActual;
    int isRunIndikator;
    String sequencBuffer[MAX_SEQUNC_BUFFER];
    // *********** promene pro spravu instanci KRA_Sequencer ****************
  protected:
    static const int MAX_NUMBER_INSTANC = 10;
    static KRA_Sequencer* pointerInstanc[];
  private:
};

#endif // KARA_SEQUENCER_H
