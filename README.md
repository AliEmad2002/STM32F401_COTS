# STM32F401x_10x_COTS
In this project I've covered most of LIB, MCAL, HAL (ECUAL) drivers for STM32F401 and STM32F103 (Black-pill and Blue-pill).

The layering model used here is the simplist (no-RTOS):
# ![image](https://user-images.githubusercontent.com/99054912/212318302-c65479f6-719d-4bc9-8d42-e5014700f8d5.png)

Following will list all available interfaces in each layer.

# LIB layer:
  - Std_Types:
    contains basic datatypes used in MCU coding.
    
  - Bit_Math
    
    
  - Stack
  - Img
  - Target_config
  - random
  - My_String
  - My_Math
  - Hex_Parser
  - Delay
  - Debug_active
  - Error_Handler
