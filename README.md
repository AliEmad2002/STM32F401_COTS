# STM32F401x_10x_COTS
In this project I've covered most of LIB, MCAL, HAL (ECUAL) drivers for STM32F401 and STM32F103 (Black-pill and Blue-pill).

The layering model used here is the simplist (no-RTOS):
# ![image](https://user-images.githubusercontent.com/99054912/212318302-c65479f6-719d-4bc9-8d42-e5014700f8d5.png)
Following will list all available interfaces in each layer.

# LIB layer:
  - Std_Types * Basic datatypes used in MCU coding.
    
  - Bit_Math
    * Macros to commonly used logic operations.
    
  - Stack
    * A structure representing FIFO stack.
  
  - Img
    * Colors in 565 format.
    * "Frame_t" struct.
    
  - Target_config
    * Target selection that is useful when switching between the two covered targets for the same application.
    
  - random
    * Random number generation using HW methods (to get a non-math-predicted values).
    * Useful in games.
    
  - My_String
    * Useful functions in dealing with strings.
    
  - My_Math
    * Useful math functions.
    
  - Hex_Parser
    * Parses array of "char" to a hexadicimal numerical value.
    * Checksum calculations.

  - Delay
    * "NOP" instruction based SW delay.
    
  - Debug_active
    * Enables/Disables the console printf's
    * Saves time spent in writing test cases and printf statements, they are only written once.
    * Saves time and memoey when not Debugging nor Tracing.
   
  - Error_Handler
    * Usually used when "Debug_active" is enabled.
    * Useful in debugging.
