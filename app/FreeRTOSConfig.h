/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include "STC8x8K.h"

/* THE VALUE FOR configSTACK_START MUST BE OBTAINED FROM THE .MEM FILE. */
extern __data uint8_t _start__stack;
#define configSTACK_START ((uint8_t)(&_start__stack))

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION          1
#define configUSE_IDLE_HOOK           0
#define configUSE_TICK_HOOK           0
#define configUSE_TIME_SLICING        1
#define configCPU_CLOCK_HZ            ((uint32_t)11059200)
#define configCPU_CLOCK_1T            0
#define configTICK_RATE_HZ            ((TickType_t)1000)
#define configMAX_PRIORITIES          (4)
#define configMINIMAL_STACK_SIZE      128
#define configTOTAL_HEAP_SIZE         ((size_t)(MCU_XRAM_SIZE - 512))
#define configMAX_TASK_NAME_LEN       (16)
#define configUSE_TRACE_FACILITY      0
#define configTICK_TYPE_WIDTH_IN_BITS TICK_TYPE_WIDTH_16_BITS
#define configIDLE_SHOULD_YIELD       0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet      0
#define INCLUDE_uxTaskPriorityGet     0
#define INCLUDE_vTaskDelete           0
#define INCLUDE_vTaskCleanUpResources 0
#define INCLUDE_vTaskSuspend          0
#define INCLUDE_vTaskDelayUntil       0
#define INCLUDE_vTaskDelay            1

#endif /* FREERTOS_CONFIG_H */
