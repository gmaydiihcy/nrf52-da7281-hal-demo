/**
 * @file app_uwb_tdoa.c
 * @brief Simulated UWB TDoA Blink Task with "Race-to-Sleep" architecture.
 */

#include "FreeRTOS.h"
#include "task.h"

/* 
 * TDoA Blink Interval: 2000ms.
 * MCU sleeps >99% of the time (Tickless Idle).
 */
#define UWB_BLINK_INTERVAL_MS   (2000)

void uwb_tdoa_task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(UWB_BLINK_INTERVAL_MS);

    for (;;) {
        /* STEP 1: Deep Sleep (WFE) until next slot */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        /* STEP 2: Wake up UWB Chip (Simulated SPI) */
        // dw3000_wakeup();

        /* STEP 3: Trigger TX (Race-to-Sleep) */
        // dw3000_send_blink();
        
        /* STEP 4: Immediately return to Sleep */
        // dw3000_enter_sleep();
    }
}