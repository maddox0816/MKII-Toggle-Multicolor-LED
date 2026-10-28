#include <ti/devices/msp/msp.h>
#include "../../LP_MSPM0G3507/MKII.h"

#define FALSE 0
#define TRUE 1

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
    }
    return 0;
}