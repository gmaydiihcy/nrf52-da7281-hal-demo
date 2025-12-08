# nRF52 DA7281 Haptic Driver (HAL)

A production-grade, thread-safe HAL implementation for the Renesas DA7281 Haptic Driver, designed for the **Qorvo DWM3001C** module (nRF52833) running **FreeRTOS**.

## Key Features

1.  **Thread-Safe Architecture**: Implements a dedicated `i2c_manager` with Mutex protection, allowing multiple tasks to share the I2C bus safely.
2.  **Low Power Optimization**: Uses a **"Dynamic Power Gating"** strategy. The TWI peripheral is only enabled during transactions to save ~400µA idle current.
3.  **UWB Coexistence**: The driver logic is non-blocking where possible, designed to run alongside high-priority UWB TDoA tasks without disrupting timing.
4.  **Multi-Instance Support**: Supports multiple DA7281 devices on the same bus via the `da7281_t` handle.

## File Structure

- `drivers/inc/da7281.h`: Public API (HAL).
- `drivers/src/da7281.c`: Driver logic & LRA configuration (170Hz, 350mA).
- `platform/i2c_manager.c`: Thread-safe I2C wrapper with anti-lockup recovery.
- `app/app_haptics.c`: Demo task performing the required start-up self-test.

## Note on NDA
This repository serves as a **technical demonstration** of my coding standards and architectural approach. It does not contain any proprietary code from previous employers.