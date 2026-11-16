#include <ti/devices/msp/msp.h>
#include "../../LP_MSPM0G3507/bsp.h"
#include "../../LP_MSPM0G3507/MKII.h"
#include "../../LP_MSPM0G3507/LCD.h"

#define FALSE 0
#define TRUE 1

uint32_t ClockFrequency = 32000000;
int main(void)
{
    uint32_t LoopDelay = 250*ONE_MILLISECOND_DELAY;

    InitializeBoosterpack(ClockFrequency);
    LCD_Init();
    LCD_FillScreen(LCD_BLACK);
    LCD_SetTextColor(LCD_CYAN);
    LCD_OutString("21\nDavid Holland\nMaddox Nichols");     

    while (TRUE) {
        BP_ToggleBlueLED();
        ClockDelay(LoopDelay);
    }
    return 0;
} 