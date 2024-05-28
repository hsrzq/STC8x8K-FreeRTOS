/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

__sfr __at(0x93) P0M1;
__sfr __at(0x94) P0M0;

void prvSetupHardware(void)
{
    /* 设置P0为强推挽输出 */
    P0M0 = 0xff;
    P0M1 = 0x00;
    P0   = 0x00;
}

static portTASK_FUNCTION(vFLOPCheck0, pvParameters)
{
    pvParameters;
    while (1) {
        vTaskDelay(500);
        P0_0 = !P0_0;
    }
}

static portTASK_FUNCTION(vFLOPCheck1, pvParameters)
{
    pvParameters;
    while (1) {
        vTaskDelay(999);
        P0_1 = 1;
        vTaskDelay(1);
        P0_1 = 0;
    }
}

static portTASK_FUNCTION(vFLOPCheck2, pvParameters)
{
    pvParameters;
    while (1) {
        vTaskDelay(4500);
        P0_2 = 1;
        vTaskDelay(500);
        P0_2 = 0;
    }
}

void main(void)
{
    prvSetupHardware();

    /* ========== ========== ========== ========== ==========
     * !!!!! 这是最重要的代码 !!!!!
     * 重置任务状态，初始化内存块。
     * XRAM内存似乎并没有被正确初始化，任务状态、堆内存等变量值都是随机的，
     * 这导致任务根本无法正常运行。好在系统的这两个函数可以重置大部分状态。
     * 至于为什么会这样，我也不知道，可能是我对XRAM的使用方式有误？
     */
    vTaskResetState();
    vPortInitialiseBlocks();

    xTaskCreate(vFLOPCheck0, "Check0", 128, NULL, (tskIDLE_PRIORITY + 1), (TaskHandle_t *)NULL);
    xTaskCreate(vFLOPCheck1, "Check1", 128, NULL, (tskIDLE_PRIORITY + 1), (TaskHandle_t *)NULL);
    xTaskCreate(vFLOPCheck2, "Check2", 128, NULL, (tskIDLE_PRIORITY + 1), (TaskHandle_t *)NULL);

    vTaskStartScheduler();
}
