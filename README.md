# nRF52 DA7281 Haptic Driver (HAL) for UWB Tags

A production-grade, thread-safe HAL implementation for the Renesas **DA7281 Haptic Driver**, designed specifically for the **Qorvo DWM3001C** module (nRF52833) running **FreeRTOS**.

This repository demonstrates a **"Race-to-Sleep"** architecture optimized for battery-powered UWB tracking tags.

---

## 1. Software Architecture

The design follows a strict layered approach to ensure **Thread Safety** and **Hardware Abstraction**. A dedicated `I2C Manager` middleware handles bus arbitration (Mutex) and dynamic power gating.

```text
+-------------------------+       +-------------------------+
|  Task: UWB TDoA Logic   |       |   Task: Haptic Ctrl     |
| (High Priority / Real)  |       | (Low Priority / Queue)  |
+------------+------------+       +------------+------------+
             |                             |
             v                             v
+-----------------------------------------------------------+
|               DA7281 Device Driver (HAL)                  |
|        (Stateless, Reentrant, Logic Translation)          |
+--------------------------+--------------------------------+
                           |
                           v
+-----------------------------------------------------------+
|           I2C Bus Manager (Middleware Layer)              |
|   [xSemaphoreMutex]  Protects shared bus access           |
|   [Power Gating]     Enables/Disables TWI dynamically     |
|   [Bus Recovery]     Anti-lockup sequence on init         |
+--------------------------+--------------------------------+
                           |
                           v
+-----------------------------------------------------------+
|                nRF52 TWI Hardware Driver                  |
|             (Nordic SDK / Qorvo HAL / Zephyr)             |
+-----------------------------------------------------------+
```

---

## 2. Power Management Strategy (Race-to-Sleep)

To minimize power consumption for UWB tags, the driver is designed to keep the MCU in **System ON Idle (WFE)** mode for >99% of the time.

```text
   Current
      ^
      | [UWB SPI Burst]      [Haptic I2C CMD]
 High |    +---+                  +--+
      |    |   |                  |  |
      |    |   |                  |  |
      |----+   +------------------+  +--------------------
 Low  |          (Deep Sleep / Tickless Idle)
      |          HFCLK OFF / TWI Uninitialized
      +---------------------------------------------------> Time

State:  ACTIVE      SLEEP       ACTIVE       SLEEP
```

*   **Dynamic Peripheral Gating:** The I2C (TWI) peripheral is only enabled/initialized during the actual transaction. This saves **~400µA** base current that would otherwise be consumed by the EasyDMA/HFCLK.
*   **DRO Mode:** The driver utilizes the DA7281's *Direct Register Override* mode to trigger hardware-sequenced waveforms, allowing the MCU to return to sleep immediately.

---

## 3. Key Features

*   **Multi-Instance Support:** Supports multiple DA7281 devices on the same bus (e.g., Left/Right actuators) via the `da7281_t` handle.
*   **Startup Self-Test:** Implements the strict power-on sequence and 100ms diagnostic pulse required by the datasheet.
*   **UWB Coexistence:** Non-blocking design ensures haptic feedback does not interfere with critical UWB TDoA timing.
*   **Clean Code:** Fully commented Doxygen-style documentation, no magic numbers.

---

## 4. File Structure

- `drivers/inc/da7281.h`: Public API definition.
- `drivers/src/da7281.c`: Driver implementation & LRA config (170Hz, 350mA).
- `platform/i2c_manager.c`: Thread-safe wrapper with Mutex & Power Gating.
- `app/app_haptics.c`: Demo task performing the start-up sequence.
- `app/app_uwb_tdoa.c`: Mock UWB task demonstrating the Race-to-Sleep logic.

---

## ⚠️ Note on Code Confidentiality

**To the Hiring Team:**

Throughout my 15+ years career (including my tenure at **Huawei**), my work has been strictly confidential under NDAs. I cannot share proprietary source code from previous commercial projects.

**This repository was created specifically for this proposal** to demonstrate my coding standards, architectural thinking, and ability to deliver production-ready firmware for the nRF52 platform.