#ifndef HELPERS_H_
#define HELPERS_H_


#define VREF 5      // точное напряжение на пине 5V (в данном случае зависит от стабилизатора на плате Arduino)
#define DIV_R1 10000  // точное значение 10 кОм резистора
#define DIV_R2 666   // точное значение 4.7 кОм резистора

#define MAX_INPUT_DCV 31.4   // макс. входное напряжение ( напряжение ХХ сварочника )
#define MAX_ADC_RESOLUTION 404    // макс. разрешение АЦП ( 10 бит для Arduino = 1023 )
#define ADC_READING_CONST ( float ) MAX_INPUT_DCV / ( float )MAX_ADC_RESOLUTION   // константа для преобразования считаного АЦП значения, в напряжение


const int adcVoltagePin = PC0;
const int adcCurrentPin = PC1;
const int adcCurrentAltPin = PC2;

const int shimOffPin = PD4;
//const int ledPin = PD2;


const int RETRY_DURATION = 200;
const int RETRY_DELAY_MILLIS = 500;
const int VOLTAGE_VALUE_MAX = 14;

#include "TroykaCurrent.h"


ACS712 sensorCurrent(adcCurrentPin);

int retrys_counter = 0;


static FILE uartout = {0} ;

unsigned long previousMillis = 0;        // will store last time
unsigned long currentMillis = 0;
const long interval = 500;           // interval at which to delay


bool currentCondition = false;
bool prevCondition = false;

int adcvalue = 0;
float voltage_read = 0;
float voltagevalue = 0;
float currentvalue = 0;
float currentaltvalue = 0;
int adcvalue1 = 0;
int adcvar1 = 0;
int adcvar2 = 0;
bool ok = false;


// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

void setupUart(int baud){
  Serial.begin(baud);
  // fill in the UART file descriptor with pointer to writer.
   fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);

   // The uart is the standard output device STDOUT.
  stdout = &uartout ;
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

//void _debug(){
////  printf("%6ld:%4d - Status: %s[%s]: %2d", currentMillis,   adcvalue, ( currentCondition ? "Stuck!" : "Ok" ), ( prevCondition ? "Stuck!" : "Ok" ), retrys_counter );
////  printf("%4d | %f = %f | %s[%s]: %2d", adcvalue, currentvalue, voltagevalue, ( currentCondition ? "Stuck!" : "Ok" ), ( prevCondition ? "Stuck!" : "Ok" ), retrys_counter );
////printf("%4d : %f V, %f A ( %f A ) | %s[%s]: %2d", adcvalue, voltagevalue, currentvalue, currentaltvalue, ( currentCondition ? "Stuck!" : "Ok" ), ( prevCondition ? "Stuck!" : "Ok" ), retrys_counter );
//printf("%4d,%f,%f( %f A ) | %s[%s]: %2d", adcvalue, voltagevalue, currentvalue, currentaltvalue, ( currentCondition ? "Stuck!" : "Ok" ), ( prevCondition ? "Stuck!" : "Ok" ), retrys_counter );
//  Serial.println();
//}

#endif  // HELPERS_H_
