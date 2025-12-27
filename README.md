# 🔧 ESP32 DALI Force Address & Hard Reset Tool

A specialized maintenance tool for **DALI Lighting Systems** running on **ESP32**.
This utility performs a **Hard Reset** sequence and forcefully assigns a specific Short Address to a DALI fixture, bypassing the standard random address allocation process.

> **⚠️ CRITICAL WARNING:** This tool uses **Broadcast Commands**. You must connect **ONLY ONE** lamp to the DALI bus when running this tool. If multiple lamps are connected, they will ALL be assigned the same address!

## 🎯 Purpose
* **Revive "Dead" Lamps:** Fix fixtures that are unresponsive to standard commissioning.
* **Resolve Conflicts:** Manually fix duplicate addresses.
* **Pre-Installation Setup:** Quickly assign a known address (e.g., Address 2) to a fixture before installing it in the ceiling.

## ⚙️ How It Works
The code implements a raw bit-banging protocol to execute the following sequence:

1.  **Hard Reset (`0x20`):** Sends a reset command twice to clear internal driver errors.
2.  **Initialise (`0xA5`):** Puts the driver into "Programming Mode".
3.  **Set DTR (`0xA3`):** Loads the desired address into the Data Transfer Register.
4.  **Store DTR (`0x80`):** Saves the DTR value as the permanent Short Address.
5.  **Terminate (`0xA1`):** Exits programming mode.
6.  **Verification Loop:** Blinks the lamp ON/OFF to confirm success.

## 🛠️ Hardware Setup

### Wiring Diagram
| ESP32 Pin | DALI Module | Function |
| :--- | :--- | :--- |
| **GPIO 21** | **TX** | DALI Transmit Line |
| **GND** | **GND** | Common Ground |
| **3.3V** | **VCC** | Logic Power |

**Note:** You also need a 16V DALI Bus Power Supply connected to the DALI bus wires.



### Signal Logic
The project uses Manchester Encoding (Bit-Banging) to communicate with the DALI bus without specialized hardware drivers (other than the level shifter).

## 💻 Configuration

Open `src/main.cpp` to change the target address:


// --- SETTINGS ---

const int DALI_TX_PIN = 21; 

// WHICH ADDRESS TO ASSIGN?

const byte TARGET_ADDRESS = 2; // Change this to 1, 5, etc.

## 📸 Verification

Check your Serial Monitor (115200 baud) for the success log:



--- DALI FORCE PROGRAMMER ---

Trying to FORCE address: 2

> Sending RESET (Unlock)...
> 
> Sending INITIALISE...
> 
> Setting DTR...
> 
> Saving Address...
> 
> Sending TERMINATE...
> 
Programming Sequence Finished.

Testing Address 2: ON...


## 🛒 Hardware Bill of Materials (BOM)

This project has been tested and verified with the following specific hardware components.

### 1. Microcontroller
* **Model:** ESP32-S3 (N16R8 Variant)
* **Specs:** Dual-core 240MHz, 16MB Flash, 8MB PSRAM.
* **Note:** The N16R8 version is recommended for stability, though standard ESP32 boards work.

### 2. DALI Interface Module
* **Model:** **Waveshare Pico-DALI2** (Adapted for ESP32 Pico)
* **Function:** Handles TTL (3.3V) to DALI (16V) logic conversion.
* **Connection:** Wired via jumper cables from the Pico header to the ESP32.

| Waveshare Pin | ESP32-S3 Pin | Description |
| :--- | :--- | :--- |
| **GPIO6 (Pin 9)** | **GPIO 21** | **TX** (Transmit DALI) |
| **VSYS (Pin 39)** | **5V** | Power Input |
| **GND (Pin 38)** | **GND** | Ground |
| **DALI Bus** | **DALI +/-** | To LED Drivers |

![Pico-DALI2-details-inter](https://github.com/user-attachments/assets/e71efed2-2cba-43d2-8d57-1e034cbc3b97)

### 3. DALI Bus Power Supply
* **Brand:** **Mean Well**
* **Model:** **DLP-04R**
* **Type:** DIN Rail Mount DALI Power Supply
* **Specs:** Output 16V DC / 240mA
* **Note:** Essential for powering the DALI bus communication line.

![S20dfc8ec6d304815b8aef36b51e602c3S](https://github.com/user-attachments/assets/11147d48-c4a1-4275-a6de-34e429af9bc8)



### 4. ✅ Verified LED Drivers
The code has been field-tested with the following drivers:

| Brand | Model | Type | Status |
| :--- | :--- | :--- | :--- |
| **QLT** | **PBX 150D** | Dimmable LED Driver | **VERIFIED** |
| **QLT** | **PBX 200D** | Dimmable LED Driver | **VERIFIED** |

> **Verification:** Drivers confirmed to accept Hard Reset (`0x20`) and Force Address commands correctly.

![PBOX200D2B-1024x736](https://github.com/user-attachments/assets/2de5d523-89f2-40d6-bc22-a11eec26e606)









