#include <Arduino.h>
#include <Wire.h>
#include "MCP4728.h"

// #define DEBUG

#ifdef DEBUG
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif

MCP4728 mcp;


const int DIGITAL_PIN_COUNT = 14;
const int MCP_PIN_COUNT = 4;
const int ANALOG_PIN_COUNT = 6;


int count = 0;
char data[3];



struct Command
{
  Command(uint8_t command=0, uint8_t pin=0, uint16_t value=0)
  {
    data[0] = (command << 4) & 0xF0;
    data[0] |= (pin & 0x0F);
    data[1] = value >> 8;
    data[2] = value & 0x00FF;
  };

  void reinit(char data[])
  {
    this->data[0] = data[0];
    this->data[1] = data[1];
    this->data[2] = data[2];
  };

  uint8_t command() { return (data[0] & 0xF0) >> 4; };
  uint8_t pin() { return data[0] & 0x0F; };
  uint16_t value() { 
    uint16_t ret = (uint16_t) data[1] << 8;
    uint8_t rhs = data[2];
    ret |= rhs;
    return ret; 
  };
  int size() { return 3 * sizeof(char); };
  char data[3];
};




enum GpioCommands
{
  READ_ADC   = 0, 
  READ_GPIO  = 1, 
  WRITE_DAC  = 2, 
  WRITE_GPIO = 3,
};



void setup()
{
  Wire.begin();
  mcp.attach(Wire, 14); // 14 is the enable pin
  mcp.readRegisters();
  mcp.analogWrite(0, 0);
  mcp.analogWrite(1, 0);
  mcp.analogWrite(2, 0);
  mcp.analogWrite(3, 0);
  mcp.enable(true);
  mcp.readRegisters();
  delay(100);

  #ifdef DEBUG
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Ready");
  #endif
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void error(String error_string)
{
  Serial.println(error_string);
#ifdef DEBUG
  lcd.clear();
  lcd.printstr(error_string.c_str());
#endif
}

#ifdef DEBUG 
#define LCD_DEBUG(command, pin, value)  \
  lcd.clear(); \
  lcd.print(command); \
  lcd.print(" "); \
  lcd.print(pin); \
  lcd.print(" "); \
  lcd.print(value); \

#else
#define LCD_DEBUG(command) ;
#endif 


void loop()
{
  if (Serial.available() >= 3)
  {
    data[0] = Serial.read();
    data[1] = Serial.read();
    data[2] = Serial.read();
    Command c;
    c.reinit(data);
    count++;
    uint8_t command = c.command();
    uint8_t pin = c.pin();
    uint16_t value = c.value();

    // LCD_DEBUG(command, pin, value);

    switch (command)
    {
      case (WRITE_GPIO):
      {
        if (pin < DIGITAL_PIN_COUNT)
        {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, value);
        }
        else
        {
          error("GPIO PIN COUNT EXCEEDED");
        }
        break;
      }
      case (READ_GPIO):
      {
        lcd.clear();
        if (pin < DIGITAL_PIN_COUNT)
        {
          pinMode(pin, INPUT);
          int val = digitalRead(pin);
          Serial.println(val);
        }
        else
        {
          error("GPIO PIN COUNT EXCEEDED");
        }
        break;
      }
      case (WRITE_DAC):
      {
        if (pin < MCP_PIN_COUNT)
        {
          mcp.analogWrite(pin, value);
        }
        else
        {
          error("DAC PIN COUNT EXCEEDED");
        }
        break;
      }
      case (READ_ADC):
      {
        if (pin < ANALOG_PIN_COUNT)
        {
          int val = analogRead(pin);
          Serial.println(val);
        }
        else
        {
          error("ADC PIN COUNT EXCEEDED");
        }
        break;
      }
    }
  }
}


