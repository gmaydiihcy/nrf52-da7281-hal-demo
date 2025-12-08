/**
 * @file i2c_manager.h
 * @brief Thread-safe I2C Bus Manager for nRF52.
 *        Abstracts low-level implementation and ensures power optimization.
 * @author [qbc]
 */

#ifndef I2C_MANAGER_H_
#define I2C_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"

/* Error Codes */
#define I2C_OK              (0)
#define I2C_ERR_BUSY        (-1)
#define I2C_ERR_TIMEOUT     (-2)
#define I2C_ERR_HAL         (-3)

/**
 * @brief I2C Bus Handle
 * Represents a physical I2C bus shared by multiple tasks/devices.
 */
typedef struct {
    uint8_t bus_id;             /**< Hardware ID (e.g., TWI0, TWI1) */
    void *hw_instance;          /**< Pointer to nRF SDK instance struct */
    SemaphoreHandle_t lock;     /**< Mutex for thread-safety protection */
    uint32_t scl_pin;
    uint32_t sda_pin;
    bool is_initialized;
} i2c_bus_t;

/**
 * @brief Initializes the I2C bus and creates the protection Mutex.
 *        Performs bus recovery sequence if the bus is stuck low.
 */
int i2c_bus_init(i2c_bus_t *bus);

/**
 * @brief Thread-safe Register Write.
 *        Takes Mutex -> Enables TWI -> Writes Data -> Disables TWI (Save Power) -> Gives Mutex.
 */
int i2c_bus_write_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg, uint8_t data);

/**
 * @brief Thread-safe Register Read.
 */
int i2c_bus_read_reg(i2c_bus_t *bus, uint8_t dev_addr, uint8_t reg, uint8_t *data);

#endif /* I2C_MANAGER_H_ */