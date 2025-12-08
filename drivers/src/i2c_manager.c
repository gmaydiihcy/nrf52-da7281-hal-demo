/**
 * @file i2c_manager.c
 * @brief Implementation of I2C Manager with Dynamic Power Gating.
 */

#include "i2c_manager.h"

/* 
 * NOTE: 
 * On nRF52 series, Hardware TWI can sometimes enter a lock-up state 
 * if a slave holds SDA low during a reset. 
 * A "Bus Clear" sequence (9 clocks on SCL) is recommended before init.
 */
static void _i2c_bus_recovery_sequence(uint32_t scl_pin, uint32_t sda_pin) {
    // Implementation of 9-clock cycle bit-banging to release SDA
    // 1. Configure pins as GPIO
    // 2. Toggle SCL 9 times
}

int i2c_bus_init(i2c_bus_t *bus) {
    if (bus == NULL) return I2C_ERR_HAL;

    /* 1. Create Mutex for Thread Safety */
    if (bus->lock == NULL) {
        bus->lock = xSemaphoreCreateMutex();
        if (bus->lock == NULL) {
            return I2C_ERR_HAL; // Memory error
        }
    }

    /* 2. Perform Bus Recovery (Anti-lockup) */
    _i2c_bus_recovery_sequence(bus->scl_pin, bus->sda_pin);

    bus->is_initialized = true;
    return I2C_OK;
}

int i2c_bus_write_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg, uint8_t data) {
    int ret = I2C_OK;
    uint8_t buffer[2] = {reg, data};

    /* CRITICAL SECTION START */
    if (xSemaphoreTake(bus->lock, portMAX_DELAY) != pdTRUE) {
        return I2C_ERR_BUSY;
    }

    /* 
     * POWER OPTIMIZATION STRATEGY:
     * 1. Enable TWI peripheral (Requests HFCLK, ~400uA)
     * 2. Perform Transfer
     * 3. Disable TWI peripheral (Releases HFCLK, ~0uA)
     */
    
    // nrf_drv_twi_enable(bus->hw_instance);
    // ret = nrf_drv_twi_tx(bus->hw_instance, dev_addr, buffer, 2, false);
    // nrf_drv_twi_disable(bus->hw_instance);

    /* CRITICAL SECTION END */
    xSemaphoreGive(bus->lock);

    return ret;
}

/* Read implementation omitted for brevity, follows same logic */
int i2c_bus_read_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg, uint8_t *data) {
    // Similar logic: Take Mutex -> Enable -> TX/RX -> Disable -> Give Mutex
    return I2C_OK;
}