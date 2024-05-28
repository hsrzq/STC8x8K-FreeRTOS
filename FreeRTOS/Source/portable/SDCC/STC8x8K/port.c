/* Standard includes. */
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

__data static uint8_t ucStackBytes;
__xdata static StackType_t *__data pxXRAMStack;
__data static StackType_t *__data pxRAMStack;

typedef void TCB_t;
extern volatile TCB_t *volatile pxCurrentTCB;

#define portCOPY_STACK_TO_XRAM()                                                       \
    {                                                                                  \
        pxXRAMStack  = (__xdata StackType_t *)*((__xdata StackType_t **)pxCurrentTCB); \
        pxRAMStack   = (__data StackType_t * __data)(configSTACK_START - 1);           \
        ucStackBytes = SP - (configSTACK_START - 1);                                   \
        *pxXRAMStack = ucStackBytes;                                                   \
        while (ucStackBytes--) {                                                       \
            *(++pxXRAMStack) = *(++pxRAMStack);                                        \
        }                                                                              \
    }

#define portCOPY_XRAM_TO_STACK()                                                       \
    {                                                                                  \
        pxXRAMStack  = (__xdata StackType_t *)*((__xdata StackType_t **)pxCurrentTCB); \
        pxRAMStack   = (__data StackType_t * __data)(configSTACK_START - 1);           \
        ucStackBytes = pxXRAMStack[0];                                                 \
        while (ucStackBytes--) {                                                       \
            *(++pxRAMStack) = *(++pxXRAMStack);                                        \
        }                                                                              \
        SP = (uint8_t)pxRAMStack;                                                      \
    }

// clang-format off
#define portSAVE_CONTEXT()          \
    {                               \
        __asm                       \
        push ACC                    \
        push IE                     \
        clr _EA                     \
        push DPL                    \
        push DPH                    \
        push b                      \
        push ar2                    \
        push ar3                    \
        push ar4                    \
        push ar5                    \
        push ar6                    \
        push ar7                    \
        push ar0                    \
        push ar1                    \
        push PSW                    \
        __endasm;                   \
        PSW = 0x00;                 \
        __asm                       \
        push _bp                    \
        __endasm;                   \
    }

#define portRESTORE_CONTEXT()       \
    {                               \
        __asm                       \
        pop _bp                     \
        pop PSW                     \
        pop ar1                     \
        pop ar0                     \
        pop ar7                     \
        pop ar6                     \
        pop ar5                     \
        pop ar4                     \
        pop ar3                     \
        pop ar2                     \
        pop b                       \
        pop DPH                     \
        pop DPL                     \
        pop ACC                     \
        clr _EA                     \
        jnb ACC.7, 4000$            \
        setb _EA                    \
    4000$:                          \
        pop ACC                     \
        __endasm;                   \
    }
// clang-format on

StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    uint32_t ulAddress;
    StackType_t *pxStartOfStack = pxTopOfStack;

    ulAddress         = (uint32_t)pxCode;                             // 任务入口地址
    *(++pxTopOfStack) = (StackType_t)ulAddress;                       // 任务入口地址低字节
    *(++pxTopOfStack) = (StackType_t)(ulAddress >> 8);                // 任务入口地址高字节
    *(++pxTopOfStack) = 0xaa;                                         // ACC
    *(++pxTopOfStack) = 0x80;                                         // IE
    ulAddress         = (uint32_t)pvParameters;                       // 任务参数地址
    *(++pxTopOfStack) = (StackType_t)(ulAddress & 0xFF);              // DPL
    *(++pxTopOfStack) = (StackType_t)(ulAddress >> 8);                // DPH
    *(++pxTopOfStack) = (StackType_t)(ulAddress >> 16);               // B
    *(++pxTopOfStack) = 0x02;                                         // R2
    *(++pxTopOfStack) = 0x03;                                         // R3
    *(++pxTopOfStack) = 0x04;                                         // R4
    *(++pxTopOfStack) = 0x05;                                         // R5
    *(++pxTopOfStack) = 0x06;                                         // R6
    *(++pxTopOfStack) = 0x07;                                         // R7
    *(++pxTopOfStack) = 0x00;                                         // R0
    *(++pxTopOfStack) = 0x01;                                         // R1
    *(++pxTopOfStack) = 0x00;                                         // PSW
    *(++pxTopOfStack) = 0xbb;                                         // BP
    *pxStartOfStack   = (StackType_t)(pxTopOfStack - pxStartOfStack); // 任务栈长度

    return pxStartOfStack;
}

void prvSetupTimerInterrupt(void)
{
#if (configCPU_CLOCK_1T == 1)
    AUXR |= 0x80; // 定时器时钟1T模式
    const uint16_t usTick = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
#else
    AUXR &= 0x7F; // 定时器时钟12T模式
    const uint16_t usTick = configCPU_CLOCK_HZ / configTICK_RATE_HZ / 12;
#endif
    const uint16_t usInit = 0x10000 - usTick;

    TMR0 = usInit; // 定时器自动重装初值
    TMOD = 0x03;   // 定时器0工作在模式3
    ET0  = 1;      // 使能定时器中断
    // EA   = 1;   // 模式3无需打开总中断
    TR0  = 1;      // 启动定时器
}

BaseType_t xPortStartScheduler(void)
{
    prvSetupTimerInterrupt();

    portCOPY_XRAM_TO_STACK();
    portRESTORE_CONTEXT();

    return pdTRUE;
}

void vPortEndScheduler(void)
{
}

void vPortYield(void) __naked
{
    portSAVE_CONTEXT();
    portCOPY_STACK_TO_XRAM();
    vTaskSwitchContext();
    portCOPY_XRAM_TO_STACK();
    portRESTORE_CONTEXT();

    __asm__("ret");
}

#if configUSE_PREEMPTION == 0
void vTimer0ISR(void) __interrupt(1)
{
    xTaskIncrementTick();
}
#else
void vTimer0ISR(void) __interrupt(1) __naked
{
    portSAVE_CONTEXT();
    if (xTaskIncrementTick() != pdFALSE) {
        portCOPY_STACK_TO_XRAM();
        vTaskSwitchContext();
        portCOPY_XRAM_TO_STACK();
    }
    portRESTORE_CONTEXT();

    __asm__("reti");
}
#endif
