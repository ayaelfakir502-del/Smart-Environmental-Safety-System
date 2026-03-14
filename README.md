
 🛡️ Smart Safety Monitoring System

![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Embedded Systems](https://img.shields.io/badge/Embedded-Systems-E34F26?style=for-the-badge)

An intelligent monitoring and alert system designed to ensure home and industrial safety. This project integrates multiple sensors to detect gas leakages and abnormal humidity levels, featuring a custom **tiered alert logic** and a user-controlled **acknowledgment (mute) system**.

---

##  Key Features

* **Dual-Sensor Monitoring:** Real-time tracking of Gas concentration (MQ sensor) and Environmental data (DHT11).
* **Tiered Hazard Logic:**
    *  **Secure:** Normal operation with visual feedback.
    *  **Level 1 (Humidity):** High humidity warning with intermittent beeps.
    *  **Level 2 (Gas Hazard):** Critical gas detection with a high-frequency emergency siren.
* **Smart Mute Function:** A priority-based button system that allows users to acknowledge and silence alarms manually without disabling the system.
* **Intuitive UI:** A 20x4 I2C LCD displaying live data, hazard levels, and mute status.

---

##  Hardware Components

| Component | Purpose |
| :--- | :--- |
| **Arduino Uno/Nano** | The Brain of the system |
| **MQ Gas Sensor** | Detecting smoke/gas leakage |
| **DHT11** | Temperature and Humidity measurement |
| **I2C LCD 20x4** | Data visualization |
| **RGB LED** | Color-coded status feedback |
| **Passive Buzzer** | Acoustic alarm system |
| **Push Button** | Alarm acknowledgment (Mute) |

---

##  Logic & Implementation

The core logic uses a **Finite State Machine (FSM)** approach to manage hazard levels. The button logic implements **software debouncing** and **edge detection** to ensure a single press increments the `pStatus` (priority status), preventing accidental multiple triggers.

### Siren Patterns:
- **Gas Hazard:** Frequency oscillation (1000Hz - 600Hz) simulating an emergency siren.
- **Humidity Alert:** Short rhythmic bursts (500Hz).

---

##  Future Improvements

* **IoT Integration:** Connecting an ESP8266/ESP32 to send real-time mobile notifications via Blynk or MQTT.
* **Data Logging:** Implementation of an SD card module to store environmental history for safety audits.
* **Actuator Control:** Automatically triggering an exhaust fan when gas levels exceed the safety threshold.

---

##  How to use
1. Clone the repository.
2. Install `DHT` and `LiquidCrystal_I2C` libraries in your Arduino IDE.
3. Connect the hardware according to the pin definitions in the `.ino` file.
4. Upload the code and monitor your environment!

---

**Developed by Aya El Fakir** *Building bridges between Embedded Systems and Creative Logic.*
