# Supported Commands:
READ ADC | GPIO PIN_NUM
  - The ADC switch specifies to read from an ADC pin
  - The GPIO switch specifies to read from a digital pin
  
WRITE DAC | GPIO PIN_NUM VALUE
  - DAC specifies to write the value to the DAC module (connected over I2C)
  - GPIO specifies to write the digital pin to VALUE 
    - 1 denotes HIGH, 0 denotes LOW