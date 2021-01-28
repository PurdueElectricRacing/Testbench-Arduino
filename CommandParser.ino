#include <Wire.h>
#include <string.h>
#include "MCP4728.h"

MCP4728 mcp;
const int NUM_MCP_PINS = 4;

void setup() {
  
  Serial.begin(9600);

  /*
  // Try to initialize! : 
  // https://github.com/hideakitai/MCP4728/blob/master/MCP4728.h
   */
}
/* Message types: 
    READ 
    ADC | GPIO

    WRITE
    DAC | GPIO

    DAC_MAX, GPIO_MAX
*/

void loop() {

}

void serialEvent() {
  String message = Serial.readString();
  

  if(message.indexOf("READ") != -1) {
    //READ ###
    //Remove the 'READ ' part
    removeWord(&message);
    if(message.indexOf("GPIO") != -1) {
      //GPIO type 
      removeWord(&message);
      int pin = message.substring(0, message.indexOf(' ')).toInt();
      if(pin < NUM_DIGITAL_PINS) Serial.print(String(digitalRead(pin))+';');
      else commandError(3);
    }
    else if(message.indexOf("ADC") != -1) {
      //ADC type
    }
    else{
      commandError(1);
    }
    
  }else if(message.indexOf("WRITE") != -1) {
    //WRITE ###
    removeWord(&message);
    if(message.indexOf("GPIO") != -1) {
      //GPIO type
      removeWord(&message);
      int pin = message.substring(0, message.indexOf(' ')).toInt(); 
      int value = message.substring(message.indexOf(' ')).toInt();
      if(pin < NUM_DIGITAL_PINS) digitalWrite(pin, value);
      else commandError(3);
    }
    else if(message.indexOf("DAC") != -1) {
      //DAC type
    }
    else{
      commandError(2);
    }
  }
  else{
    commandError(0);
  }

}

void removeWord(String* m) {
  *m = m->substring(m->indexOf(' ') + 1);
}

void commandError(int e){
  Serial.print("Command error ");
  Serial.println(e);
}
