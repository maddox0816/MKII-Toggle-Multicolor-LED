#include <ti/devices/msp/msp.h>
#include "../../LP_MSPM0G3507/MKII.h"

#define FALSE 0
#define TRUE 1
#define NOTE_C4 262U


int main(void)
{

    // Global variables
    uint32_t ClockFrequency = 32000000; // 32 MHz

    // Initialization section: MKII BoosterPack and variables.
    InitializeLaunchpad(ClockFrequency);
    InitializeBoosterpack(ClockFrequency);

    // Infinite loop
    while (TRUE) {
        if (BP_ReadS1() == 1) {
            while (BP_ReadS1() == 1);
            BP_ToggleBlueLED();
        }

        if (BP_ReadS2() == 1) {
            while (BP_ReadS2() == 1);
            BP_ToggleRedLED();
        }
       
        // BP_ToggleBuzzer();
        // BP_TurnOnBuzzer();
        // BP_TurnOffBuzzer();
        const uint32_t halfPeriodUs = 1000000U / (NOTE_C4 * 2U);
        const uint32_t halfPeriodCycles = halfPeriodUs * ONE_MICROSECOND_DELAY;

        BP_TurnOnBuzzer();
        for (uint32_t halfCycle = 0U; halfCycle < NOTE_C4; halfCycle++) {
            ClockDelay(halfPeriodCycles);
            BP_ToggleBuzzer();
        }
        BP_TurnOffBuzzer();
        



    }
    return 0;
}
