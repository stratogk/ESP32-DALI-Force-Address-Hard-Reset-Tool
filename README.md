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

```cpp
// --- SETTINGS ---
const int DALI_TX_PIN = 21; 

// WHICH ADDRESS TO ASSIGN?
const byte TARGET_ADDRESS = 2; // Change this to 1, 5, etc.
