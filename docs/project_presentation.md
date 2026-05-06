---
marp: true
theme: uncover
class: invert
paginate: true
header: 'Water Level Monitoring System'
footer: '© 2026 Uday V'
---

# REAL-TIME WATER LEVEL MONITORING SYSTEM
### Using Ultrasonic Sensor & Arduino

---

## 🌊 Problem Statement

- Water overflow leads to **wastage and damage**
- Manual monitoring is **inefficient and unreliable**
- Lack of real-time awareness in tank systems
- Need for a **low-cost automated solution**

---

## 🎯 Objectives

- Monitor water level in **real-time**
- Display level using **LCD interface**
- Provide **alert when threshold is reached**
- Ensure **accuracy and stability**
- Develop a **cost-effective system**

---

## 🧩 System Components

- **Arduino Uno** (Controller)
- **HC-SR04** (Ultrasonic Sensor)
- **I2C LCD Display** (16x2)
- **Buzzer** (Alert system)
- Jumper wires & Breadboard

---

## ⚙️ Working Principle

1. Ultrasonic sensor emits sound waves
2. Waves reflect from water surface
3. Time is measured → distance calculated
4. Distance → water level (%)
5. LCD displays real-time data
6. Buzzer activates at threshold

---

## 📐 Mathematical Model

**Distance Calculation:**
`Distance = (Time × 0.034) / 2`

**Water Level Calculation:**
`Level (%) = ((Tank Height − Distance) / Tank Height) × 100`

**Threshold Condition:**
`If Level ≥ Threshold → Alert Activated`

---

## 🔌 Circuit Connections

- **HC-SR04:** TRIG → D9 | ECHO → D10
- **I2C LCD:** SDA → A4 | SCL → A5
- **Buzzer:** Positive → D8
- **Power:** 5V & GND to all components

---

## 📊 Results & Conclusion

- Real-time monitoring achieved
- Accurate and stable readings using filtering
- Buzzer alert prevents overflow
- Low-cost and easy implementation

**Conclusion:**
Efficient solution for water level monitoring with potential for IoT extension.

---

# THANK YOU!
### Questions?
