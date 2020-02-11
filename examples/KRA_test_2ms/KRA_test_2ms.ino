#include "KRA_robot.h"
//#include "Arduino_FreeRTOS.h" // upravená knihovna pro časování 2ms

//#include <Arduino_FreeRTOS.h>  // tato knihovna má časování pouze 15ms


#define TIMER1_RESOLUTION 65536UL  // Timer1 is 16 bit
unsigned char clockSelectBits = 0;
unsigned short pwmPeriod = 0;

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial.println("Ahoj");

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  (const portCHAR *)"Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskAnalogRead
    ,  (const portCHAR *) "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  //  TCCR1A = 0;                 // clear control register A
  //  setPeriod(2000);
  //  TCCR1B = 0;
  //  TCNT1 = 0;    // TODO: does this cause an undesired interrupt?
  //  TCCR1B = _BV(WGM13) | clockSelectBits;
  TIMSK1 = _BV(TOIE1);
  // wdt_disable();
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}
unsigned long calcMicros;
unsigned long oldMicros = 0;
unsigned long actMicros = 0;
long c = 0;
void loop()
{
  // Empty. Things are done in Tasks.
  //  Serial.println(long(calcMicros));
  //  Serial.println(int(c));
  c++;

  delay(1000);
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;



  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 100 ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 100 ); // wait for one second
    long cc = c / 60;
    Serial.println(long(calcMicros));
    Serial.print("Cas ");
    Serial.print(long(c) / 3600);
    Serial.print(":");
    Serial.print((long(c) / 60) % 60);
    Serial.print(":");
    Serial.println((long(c)) % 60);

    //  Serial.println(int(cc));
    //    Serial.println("bla");
    //    c++;

  }
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  /*
    AnalogReadSerial
    Reads an analog input on pin 0, prints the result to the serial monitor.
    Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
    Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

    This example code is in the public domain.
  */
  for (;;)
  {
    actMicros = micros();
    calcMicros = actMicros - oldMicros;
    oldMicros = actMicros;
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}

void setPeriod(unsigned long microseconds)
{
  const unsigned long cycles = ((F_CPU / 100000 * microseconds) / 20);
  if (cycles < TIMER1_RESOLUTION) {
    clockSelectBits = _BV(CS10);
    pwmPeriod = cycles;
  } else if (cycles < TIMER1_RESOLUTION * 8) {
    clockSelectBits = _BV(CS11);
    pwmPeriod = cycles / 8;
  } else if (cycles < TIMER1_RESOLUTION * 64) {
    clockSelectBits = _BV(CS11) | _BV(CS10);
    pwmPeriod = cycles / 64;
  } else if (cycles < TIMER1_RESOLUTION * 256) {
    clockSelectBits = _BV(CS12);
    pwmPeriod = cycles / 256;
  } else if (cycles < TIMER1_RESOLUTION * 1024) {
    clockSelectBits = _BV(CS12) | _BV(CS10);
    pwmPeriod = cycles / 1024;
  } else {
    clockSelectBits = _BV(CS12) | _BV(CS10);
    pwmPeriod = TIMER1_RESOLUTION - 1;
  }
  ICR1 = pwmPeriod;
  TCCR1B = _BV(WGM13) | clockSelectBits;
}
