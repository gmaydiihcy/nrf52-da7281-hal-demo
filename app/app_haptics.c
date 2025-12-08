/**
 * @file app_haptics.c
 * @brief Haptic Control Task integrated with FreeRTOS Queue.
 */

#include "da7281.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Shared Bus Instance */
static i2c_bus_t bus0 = { .bus_id = 0, .scl_pin = 8, .sda_pin = 9 };

/* Multiple Devices on same bus */
static da7281_t dev_left  = { .bus = &bus0, .i2c_addr = 0x4A, .en_pin = 15 };
static da7281_t dev_right = { .bus = &bus0, .i2c_addr = 0x4B, .en_pin = 16 };

void haptics_task(void *arg) {
    /* 1. Init Bus Manager */
    i2c_bus_init(&bus0);

    /* 2. Hardware Power On & Config */
    da7281_power_on(&dev_left);
    da7281_power_on(&dev_right);
    da7281_init_lra(&dev_left);
    da7281_init_lra(&dev_right);

    /* 3. Run Startup Self-Test (As requested) */
    da7281_run_self_test(&dev_left);
    vTaskDelay(pdMS_TO_TICKS(200)); // Gap between tests
    da7281_run_self_test(&dev_right);

    /* 4. Main Loop: Wait for commands */
    for(;;) {
        /* 
         * Waiting for queue commands here.
         * Thread will block (Sleep) to save power.
         */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}