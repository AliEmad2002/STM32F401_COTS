# STM32F401x_10x_COTS
In this project I've covered most of LIB, MCAL, HAL drivers for STM32F401 and STM32F103 (Black-pill and Blue-pill).

The layering model used here is the simplist (no-RTOS):
# ![image](https://user-images.githubusercontent.com/99054912/212324211-ecfec1b8-2d4c-4b80-b461-f707e95aa85d.png)
Following will list all available interfaces in each layer.

# LIB layer:
  - Std_Types
    * Basic datatypes used in MCU coding.
    
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
   
# MCAL (Micro-Controller Abstraction Layer):
following names are peripherals' abbervation in STM datasheets
  - ADC
  - BKP
  - DMA
  - EXTI
  - FPEC
  - FPU
  - GPIO
  - DAC
  - NVIC
  - PWR
  - RCC
  - SCB
  - SPI
  - STK
  - SYSCFG
  - TIM
  - UART

# HAL (Hardware Abstraction Layer):
Note that all of these drivers have dependancies on previously mentioned LIB and MCAL layers.
  - DAC
    * Based on R-2R implementation.
  - ESC (Electronic Speed Control)
    * Interfaces bluerobotics basic ESC.
  - IR
    * Non-blocking IR data reciever.
  - Keypad
  - LCD
  - TFT
  - LED_Matrix
  - RGB_LED
  - Seven_Segment
  - Shift_Register
  - Stepper_Motor
  - WiFi (With FTP upload / download examples).
