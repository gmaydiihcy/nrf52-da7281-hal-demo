/**
 * @file da7281.c
 * @brief Implementation of DA7281 Driver for nRF52833.
 */

#include "da7281.h"
#include "da7281_reg.h"
#include "FreeRTOS.h"
#include "task.h"

/* Helper macro for register writing via manager */
#define WRITE_REG(dev, reg, val) \
    i2c_bus_write_reg(dev->bus, dev->i2c_addr, reg, val)

#define READ_REG(dev, reg, val_ptr) \
    i2c_bus_read_reg(dev->bus, dev->i2c_addr, reg, val_ptr)

int da7281_power_on(da7281_t *dev) {
    uint8_t chip_id = 0;
    
    /* 1. Enable Power Rail via GPIO (Placeholder) */
    // gpio_set(dev->en_pin, 1);

    /* 2. Wait 1.5ms as per spec (Rounded to 2ms ticks) */
    vTaskDelay(pdMS_TO_TICKS(2));

    /* 3. Verify device presence */
    if (READ_REG(dev, DA7281_REG_CHIP_ID, &chip_id) != I2C_OK) {
        return DA7281_ERR_I2C;
    }

    /* 4. Ensure Standby Mode initially */
    return da7281_set_standby(dev, true);
}

int da7281_init_lra(da7281_t *dev) {
    if (!dev || !dev->bus) return DA7281_ERR_INVALID;

    /* 
     * Configurations based on Client Spec:
     * ACTUATOR_NOMMAX = 2.5 V RMS, ABSMAX = 3.5 V Peak
     * IMAX = 350 mA, Z = 6.75 Ohm, Freq = 170 Hz
     */
    
    // Step 1: Set Actuator Type = LRA
    WRITE_REG(dev, DA7281_REG_TOP_CFG1, 0x00); 

    // Step 2: Set Electrical Limits (Values to be calculated based on Z)
    WRITE_REG(dev, DA7281_REG_ACTUATOR1, 0x58); // Example for 2.5V
    WRITE_REG(dev, DA7281_REG_ACTUATOR2, 0x76); // Example for 3.5V

    // Step 3: Set Frequency (170Hz)
    // Period = 1 / (170 * 1333.32e-9) = ~4411 (0x113B)
    WRITE_REG(dev, DA7281_REG_LRA_PER_L, 0x3B);
    WRITE_REG(dev, DA7281_REG_LRA_PER_H, 0x11);

    return DA7281_OK;
}

int da7281_set_operation_mode(da7281_t *dev, uint8_t mode) {
    return WRITE_REG(dev, DA7281_REG_TOP_CFG4, mode & 0x07);
}

int da7281_set_standby(da7281_t *dev, bool enable) {
    return WRITE_REG(dev, DA7281_REG_TOP_CTL1, enable ? 0x08 : 0x00);
}

int da7281_set_override_value(da7281_t *dev, uint8_t val) {
    return WRITE_REG(dev, DA7281_REG_TOP_CTL2, val & 0x7F);
}

int da7281_run_self_test(da7281_t *dev) {
    /* Sequence per client spec */
    da7281_set_operation_mode(dev, DA7281_OP_MODE_INACTIVE);
    da7281_set_override_value(dev, 0x7F); // Max value
    
    /* Enter DRO Mode */
    da7281_set_operation_mode(dev, DA7281_OP_MODE_DRO);

    /* Drive for 100ms */
    vTaskDelay(pdMS_TO_TICKS(100));

    /* Return to Inactive & Standby */
    da7281_set_operation_mode(dev, DA7281_OP_MODE_INACTIVE);
    da7281_set_standby(dev, true);

    return DA7281_OK;
}