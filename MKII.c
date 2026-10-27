#include "MKII.h"

void InitializeBoosterpack(uint32_t ClockFrequency)
{

  ActivatePortAandB();

  // IOMUX configuration for RGB LED on BoosterPack.
    
    IOMUX->SECCFG.PINCM[BP_RGB_R_INDEX] = (IOMUX_MODE1 | IOMUX_PINCM_PC_CONNECTED);
    IOMUX->SECCFG.PINCM[BP_RGB_G_INDEX] = (IOMUX_MODE1 | IOMUX_PINCM_PC_CONNECTED);
    IOMUX->SECCFG.PINCM[BP_RGB_B_INDEX] = (IOMUX_MODE1 | IOMUX_PINCM_PC_CONNECTED);
  // Red: Peripheral connected and IOMUX mode 1
  // Green: Peripheral connected and IOMUX mode 1
  // Blue: Peripheral connected and IOMUX mode 1

  // IOMUX configuration for S1 and S2 on BoosterPack.
  // S1: Peripheral connected, input enabled, internal resistor enabled, IOMUX mode 1
  // S2: Peripheral connected, input enabled, internal resistor enabled, IOMUX mode 1
    IOMUX->SECCFG.PINCM[BP_S1_INDEX] = (IOMUX_MODE1 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_PIPU_ENABLE);
    IOMUX->SECCFG.PINCM[BP_S2_INDEX] = (IOMUX_MODE1 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE | IOMUX_PINCM_PIPU_ENABLE);


  // Enable RGB LED pins as outputs using Data Output Enable (DOE) register
    BP_RGB_R_PORT->DOE31_0 |= BP_RGB_R_MASK;
    BP_RGB_B_PORT->DOE31_0 |= BP_RGB_B_MASK;
    BP_RGB_G_PORT->DOE31_0 |= BP_RGB_G_MASK;

  // Turn off red RGB LED
  // Turn off green RGB LED
  // Turn off blue RGB LED
  BP_TurnOffRedLED();
  BP_TurnOffGreenLED();
  BP_TurnOffBlueLED();

  // Set the clock frequency
  switch (ClockFrequency) {
  case 40000000: ClockInitialization_40MHz(); break;
  case 80000000: ClockInitialization_80MHz(0); break; // disable clk_out
  default: ClockInitialization_4MHz_32MHz(ClockFrequency);
  }
}

uint32_t BP_ReadS1()
{
  uint32_t ReturnValue = 0;
  ReturnValue = (BP_S1_PORT->DIN31_0 & BP_S1_MASK) >> BP_S1_PIN;

  ReturnValue = !(ReturnValue);

  return ReturnValue;
}

uint32_t BP_ReadS2()
{
}

void BP_TurnOnRedLED(void)
{
  BP_RGB_R_PORT->DOUT31_0 |= BP_RGB_R_MASK;
}

void BP_TurnOffRedLED(void)
{ 
  BP_RGB_R_PORT->DOUT31_0 &= ~BP_RGB_R_MASK;
}

void BP_ToggleRedLED(void)
{
  BP_RGB_R_PORT->DOUT31_0 ^= BP_RGB_R_MASK; 
}

void BP_TurnOnGreenLED(void)
{

}

void BP_TurnOffGreenLED(void)
{

}

void BP_ToggleGreenLED(void)
{

}

void BP_TurnOnBlueLED(void)
{

}

void BP_TurnOffBlueLED(void)
{

}

void BP_ToggleBlueLED(void)
{
 
}
