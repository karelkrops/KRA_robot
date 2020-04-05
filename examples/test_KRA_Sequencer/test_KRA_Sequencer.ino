#include <KRA_robot.h>

/**
   Autor: Karel Kropš
   pro Kroužek robotiky a automatizace Písek (KRA)
   verze: 2.0.0
   datum verze: 5.4.2020
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


// **************** start testovacich funkci sequencer ****************
void testSequecer(KRA_Command *pointer, String value) {
  pointer->answareContinue();
  value.trim();
  Serial.println("START: void testSequecer(KRA_Sequencer *pointer,long times);" + value);
}
void testSequecer2(KRA_Command *pointer,  String value) {
  pointer->answareContinue();
  value.trim();
  Serial.println("START: void testSequecer2(KRA_Sequencer *pointer,long times);" + value);
}
KRA_Sequencer se;
// **************** stop testovacich funkci sequencer ****************



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!Serial) {};
  delay(100);
  Serial.println("START");

  // **************** start testovani sequencer ****************
  String s = "f2(A5000,0,ah,oj),f1(100,0,\"0\\\")1\"),f2,f3(0,0),fe4(105,0)";
  se.addSequenc(s);
  se.onCommand("f1", testSequecer);
  se.onCommand("f2", testSequecer2);
  // **************** stop testovani sequencer ****************

}

void loop() {
  // put your main code here, to run repeatedly:
  KRA_Sequencer::execute();

}
