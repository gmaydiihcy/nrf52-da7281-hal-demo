#ifndef DA7281_REG_H_
#define DA7281_REG_H_

/* Register Map */
#define DA7281_REG_CHIP_ID          (0x00)
#define DA7281_REG_CHIP_REV         (0x01)
#define DA7281_REG_IRQ_EVENT1       (0x03)
#define DA7281_REG_TOP_CFG1         (0x13)
#define DA7281_REG_TOP_CFG2         (0x14)
#define DA7281_REG_TOP_CFG4         (0x16) /* OPERATION_MODE */
#define DA7281_REG_TOP_CTL1         (0x22) /* STANDBY_EN */
#define DA7281_REG_TOP_CTL2         (0x23) /* OVERRIDE_VAL */

/* LRA Specific Config Registers */
#define DA7281_REG_ACTUATOR1        (0x2C) /* NOMMAX / IMAX */
#define DA7281_REG_ACTUATOR2        (0x2D) /* ABSMAX / V2I_FACTOR_L */
#define DA7281_REG_ACTUATOR3        (0x2E) /* V2I_FACTOR_H */
#define DA7281_REG_LRA_PER_L        (0x5E)
#define DA7281_REG_LRA_PER_H        (0x5F)

/* Bit Masks & Values */
#define DA7281_CHIP_ID_VAL          (0xE0) 
#define DA7281_OP_MODE_DRO          (0x01)
#define DA7281_OP_MODE_INACTIVE     (0x00)

#endif /* DA7281_REG_H_ */