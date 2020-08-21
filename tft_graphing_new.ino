//#include "avr/interrupt.h" 
#include <EnableInterrupt.h>
//#include <TroykaCurrent.h>

#include "globals.h"
#include "widget.h"

#include "orGraph.h"
#include "helpers.h"

orGraph graph = orGraph();



uint8_t voltage = 0;

uint8_t current = 0;


bool rnd_mode = true;
int incomingByte;

int inputPin = A5;

//#define MAX_INPUT_DCV 100   // макс. входное напряжение ( напряжение ХХ сварочника )
//#define MAX_ADC_RESOLUTION 927   // макс. разрешение АЦП ( 10 бит для Arduino = 1023 )
//#define ADC_READING_CONST ( float ) MAX_INPUT_DCV / ( float )MAX_ADC_RESOLUTION   // константа для преобразования считаного АЦП значения, в напряжение

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value


char voltage_str_temp[6];
char current_str_temp[6];

char currentCondition_str = '+';

//float voltagevalue = 0;
//float currentvalue = 0;

float __read(){
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  return total / numReadings;
}

float calcVoltage ( float adc_read ){
  return adc_read * (float) ADC_READING_CONST;
}

//bool currentCondition = false;
//bool prevCondition = false;
//
//const int VOLTAGE_VALUE_MAX = 14;

void __update(){
average = __read();
  voltagevalue = calcVoltage ( average );
  currentvalue  =  sensorCurrent.readCurrentDC()*4.0f;//calcCurrent ( adcvalue );  
/* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
dtostrf(voltagevalue, 4, 2, voltage_str_temp);
dtostrf(currentvalue, 4, 2, current_str_temp);

  prevCondition = currentCondition;
  currentCondition = ( VOLTAGE_VALUE_MAX > voltagevalue ? true : false );
  currentCondition_str = (currentCondition ? '!' : '+');

//  printf("%4d : %s V, %s A %c", average, voltage_str_temp, current_str_temp, currentCondition_str );
  printf("%4d,%s,%s", average, voltage_str_temp, current_str_temp );
//printf("%s,%s", voltage_str_temp, current_str_temp );
//printf("%s", voltage_str_temp);
  Serial.println();  
}




//ACS712 sensorCurrent(adcCurrentPin);

void setup(void) {
  randomSeed(analogRead(0));
  
  //Serial.begin(57600);
  setupUart(19200);
//  while(!Serial) {};
  printf("ADC_READING_CONST = %f\n\n", ADC_READING_CONST );  

  tft.begin();
// initialize the digital pin as an output for LED Backlibht
  initPins();

  tft.setRotation(rotation);
  tft.setTextSize(textSize);
  tft.setCursor(50, 50);
  tft.print("Hello ODROID-SHOW!");
  tft.setCursor(250, 200);

  tft.fillScreen(backgroundColor);
  tft.setCursor(0, 0);

  
//  WIDGET w = {0,0,320, 59,CYAN};
//  draw_status(w);
  
  WIDGET w1 = { 0, 0, 320, 240, RED };    
  graph.init(&tft, w1);

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // calibrate during the first five seconds
  while (millis() < 5000) {
    sensorValue = analogRead(inputPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
}

//void loop(void) {}

void loop(void) {
  __update();  
//  voltage = random(0, 100);
//  current = random(0, 150);
//  Serial.print(voltagevalue);Serial.print(":");Serial.println(current);
  PACKET pkt = {voltagevalue, currentvalue };
  graph.update(pkt);
  delay(17);
}
