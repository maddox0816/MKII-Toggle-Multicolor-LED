#include <ti/devices/msp/msp.h>

#include "../../LP_MSPM0G3507/bsp.h"
#include "../../LP_MSPM0G3507/timer.h"
#include "../../LP_MSPM0G3507/mspm0g350x_int.h"
#include "../../LP_MSPM0G3507/tone_pitch.h"

// Buzzer tone settings
#define TONE_FREQUENCY  NOTE_A4         
#define PWM_PRESCALE    31              // 32 MHz 
#define PWM_TIMER       TIMA1

// Beep timing (Timer G8 interrupt)
#define DELAY_TIMER     TIMG8
#define DELAY_PRESCALE  31             
#define BEEP_TIME_US    500000         

// Function prototypes
void TimerG8CallbackFunction(void *CallbackObject);

// Global variable for Timer G8 interrupt callback function.
Callback_s TimerG8 = {(void *) 0, TimerG8CallbackFunction};

// Timer G8 used for the beep on/off timing.
Timer_ClockConfig DelayTimerClockConfig =
{
 .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
 .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
 .prescale = DELAY_PRESCALE
};

Timer_TimerConfig DelayTimerConfig =
{
 .period = 0,
 .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)
};

Timer_InterruptConfig DelayTimerInterruptConfig =
{
 .imask = GPTIMER_CPU_INT_IMASK_Z_SET,
 .priority = 1,
 .IRQnum = TIMG8_INT_IRQn
};

// Timer A1 drives the MKII buzzer
Timer_ClockConfig PWMTimerClockConfig =
{
 .clockSel = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE,
 .divideRatio = GPTIMER_CLKDIV_RATIO_DIV_BY_1,
 .prescale = PWM_PRESCALE
};

Timer_TimerConfig PWMTimerConfig =
{
 .period = 0,
 .timerMode = (GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1)
};

Timer_PWMConfig PWMTimerPWMConfig =
{
 .index = PB4INDEX,
 .iomuxMode = (IOMUX_PINCM_PC_CONNECTED | IOMUX_MODE4),
 .ccr = 0,
 .ccpd = GPTIMER_CCPD_C0CCP0_OUTPUT,
 .ccctl = GPTIMER_CCCTL_01_COC_COMPARE,
 .octl = GPTIMER_OCTL_01_CCPO_FUNCVAL,
 .ccact = (GPTIMER_CCACT_01_CDACT_CCP_LOW | GPTIMER_CCACT_01_LACT_CCP_HIGH),
 .duty = 0,
};

uint32_t ClockFrequency = 32000000;

int main(void)
{
    uint32_t ScalingFactor;
    uint32_t TimerClock;

    InitializeLaunchpad(ClockFrequency);

    // Configure Timer A1 to generate the tone on the buzzer.
    ScalingFactor = (PWMTimerClockConfig.divideRatio + 1) * (PWMTimerClockConfig.prescale + 1);
    TimerClock = ClockFrequency / ScalingFactor;
    PWMTimerConfig.period  = TimerClock / TONE_FREQUENCY;   
    PWMTimerPWMConfig.duty = PWMTimerConfig.period / 2;    

    InitializeTimerClock(PWM_TIMER, &PWMTimerClockConfig);
    InitializeTimerCompare(PWM_TIMER, &PWMTimerConfig);
    InitializeTimerPWM(PWM_TIMER, &PWMTimerPWMConfig);
    EnableTimer(PWM_TIMER);

    // Configure Timer G8 to interrupt every BEEP_TIME_US to toggle the tone.
    __disable_irq();
    ScalingFactor = (DelayTimerClockConfig.divideRatio + 1) * (DelayTimerClockConfig.prescale + 1);
    DelayTimerConfig.period = (ClockFrequency / ScalingFactor) / 1000000 * BEEP_TIME_US;
    InitializeTimerClock(DELAY_TIMER, &DelayTimerClockConfig);
    InitializeTimerCompare(DELAY_TIMER, &DelayTimerConfig);
    InitializeTimerInterrupt(DELAY_TIMER, &DelayTimerInterruptConfig);
    EnableTimer(DELAY_TIMER);
    __enable_irq();

    while (1) {
        __NOP(); // wait for interrupt
    }

    return 0;
}


void TimerG8CallbackFunction(void *CallbackObject)
{
    static uint8_t on = 1;

    if (DELAY_TIMER->CPU_INT.IIDX == GPTIMER_CPU_INT_IIDX_STAT_Z) {
        on = !on;
        PWMTimerPWMConfig.duty = on ? (PWMTimerConfig.period / 2) : 0;
        UpdateDutyCycle(PWM_TIMER, &PWMTimerPWMConfig);
    }
}