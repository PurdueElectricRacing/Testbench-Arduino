#include <Wire.h>
#include <string.h>
#include "MCP4728.h"

MCP4728 mcp;
const int NUM_MCP_PINS = 4;

enum COMMAND_ERRORS {READ_WRITE, TYPE, PIN};

void setup() {
  
  Serial.begin(115200);

  /*
  // Try to initialize! : 
  // https://github.com/hideakitai/MCP4728/blob/master/MCP4728.h
   */
  Wire.begin();
  mcp.attach(Wire, 14); // 14 is the enable pin
  mcp.readRegisters();
  mcp.analogWrite(0,0);
  mcp.analogWrite(1,0);
  mcp.analogWrite(2,0);
  mcp.analogWrite(3,0);
  mcp.enable(true);
  mcp.readRegisters();
  delay(100);
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
      else commandError(COMMAND_ERRORS::PIN);
    }
    else if(message.indexOf("ADC") != -1) {
      //ADC type
    }
    else{
      commandError(COMMAND_ERRORS::TYPE);
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
      else commandError(COMMAND_ERRORS::PIN);
    }
    else if(message.indexOf("DAC") != -1) {
      //DAC type
      removeWord(&message);
      int pin = message.substring(0, message.indexOf(' ')).toInt(); 
      int value = message.substring(message.indexOf(' ')).toInt();
      if(pin < NUM_MCP_PINS) mcp.analogWrite(pin, value);
      else commandError(COMMAND_ERRORS::PIN);
      delay(10);
      mcp.readRegisters();

    }
    else{
      commandError(COMMAND_ERRORS::TYPE);
    }
  }
  else{
    commandError(COMMAND_ERRORS::READ_WRITE);
  }

}

void removeWord(String* m) {
  *m = m->substring(m->indexOf(' ') + 1);
}

void commandError(int e){
  Serial.print("\n Command error: ");
  switch(e) {
    case COMMAND_ERRORS::READ_WRITE:
      Serial.println("read write");
    break;
    case COMMAND_ERRORS::TYPE:
      Serial.println("type");
    break;
    case COMMAND_ERRORS::PIN:
      Serial.println("pin");
    break;
    default:
      Serial.println("unknown");
  }
}
