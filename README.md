# Specs
* Receives commands in 3 byte strings
    - Byte 0: Command (2 bits) | Pin number (4 bits)
    - Byte 1: First 8 bits of value setting (in order to support writing to the DAC's full 12 bits)
    - Byte 2: Second 8 bits of value setting
    - Example: `set-pin DOUT 3 1;` would correspond to the command `51 0 1` being sent to the serial device

* Will respond with an error over the serial line if the pin number exceeded the max pins for that type
