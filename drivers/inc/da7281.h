#ifndef DA7281_H_
#define DA7281_H_

#include <stdint.h>
#include <stdbool.h>
#include "i2c_manager.h"

/* Error Codes */
#define DA7281_OK                   (0)
#define DA7281_ERR_I2C              (-1)
#define DA7281_ERR_INVALID          (-3)

/* DA7281 Device Object - Supports multiple instances */
typedef struct {
    i2c_bus_t *bus;             /**< Pointer to the shared I2C bus manager */
    uint8_t i2c_addr;           /**< 7-bit I2C Address */
    uint32_t en_pin;            /**< GPIO pin for Enable/Power control */
} da7281_t;

/* Public API */
int da7281_power_on(da7281_t *dev);
int da7281_init_lra(da7281_t *dev);
int da7281_set_operation_mode(da7281_t *dev, uint8_t mode);
int da7281_set_standby(da7281_t *dev, bool enable);
int da7281_set_override_value(da7281_t *dev, uint8_t val);
int da7281_run_self_test(da7281_t *dev);

#endif /* DA7281_H_ */