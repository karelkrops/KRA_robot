#include <KRA_robot.h>

#define TIMER1_RESOLUTION 65536UL  // Timer1 is 16 bit

int count; // tesovací pošítadlo

void TaskBlink();  // předdefinici funkce vlákna
void setup() {


  
  Serial.begin(115200);
  while(!Serial){} // čekání na inicializaci Serial
  delay(100); // pro některé desky je nutné použít delay

   
  xTaskCreate( // vytvoření vlákna
    TaskBlink // volaná funkce, která oběží v samosatném vlákně
    ,  (const portCHAR *)"LoopTes"   // A name just for humans (maximálně osm znaků)
    ,  127  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL // parametr předávaný vláknu
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL  // handler
    );

  // nastevení časovače TIMER1
  //  TCCR1A = 0;                 // clear control register A
  //  setPeriod(2000);
  //  TCCR1B = 0;
  //  TCNT1 = 0;    // TODO: does this cause an undesired interrupt?
  //  TCCR1B = _BV(WGM13) | clockSelectBits;
  TIMSK1 = _BV(TOIE1);
  // wdt_disable();
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("stav pocitadla:");
  Serial.println(count);
  
  delay(1000); // čekání 1000ms
}

void TaskBlink(void * pvParameters) // definice vlákna
{
  (void) pvParameters;
  // inicializace / setup


  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  count=0; // vynulování počítadla

  for (;;) { // nekonečný ciklus, obdoba loop
    // obsah vlákna

    count++; // navýšení testovacího počítadla
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / 2 ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / 2 ); // čekání 1000ms. Jeden krok v běžném FreeRTOS trvá 15ms. RTOS KRA_robot je upraveno na krok 2ms
//    vTaskDelayUntil(,100/2); // čekání na předem nastavený takt. Jeden takt jsou 2ms. První proměná je referenční čas

  }
  vTaskDelete(NULL); // okončení vlákna. Bez zavolání této funkce nesmí být vlákno / funkce vlákna ukončena. Paramatrem může být také handel vlákna
}
