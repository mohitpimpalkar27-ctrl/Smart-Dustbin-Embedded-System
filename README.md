# 🗑️ Smart Dustbin – Automatic Waste Management System

<p align="center">
  <img src="images/prototype.jpg" alt="Smart Dustbin" width="500">
</p>

<p align="center">
  <b>An Arduino-based smart dustbin with automatic, contactless lid operation.</b>
</p>

<p align="center">

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge\&logo=arduino\&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge\&logo=cplusplus\&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Technology-blue?style=for-the-badge)
![Embedded Systems](https://img.shields.io/badge/Embedded-Systems-orange?style=for-the-badge)

</p>

---

## 📌 About The Project

The **Smart Dustbin** is an Arduino-based automated waste-management system designed to provide a **contactless and hygienic waste disposal experience**.

The system uses an **HC-SR04 ultrasonic sensor** to detect an approaching object. When an object is detected within the predefined range, the Arduino commands a **servo motor** to automatically open the dustbin lid.

An **OLED display** provides real-time information about the system status, while a **push button** can be used for manual operation.

### 🎯 Main Objective

To develop a low-cost smart dustbin that reduces unnecessary physical contact with waste bins and demonstrates the practical application of **sensors, actuators, microcontrollers, and embedded systems**.

---

## ✨ Features

* 🚮 **Automatic Lid Opening**
* 📡 **Ultrasonic Object Detection**
* ⚙️ **Servo Motor Controlled Lid**
* 🖥️ **OLED Status Display**
* 🔘 **Manual Push-Button Control**
* 🧼 **Contactless Operation**
* ⚡ **Arduino-Based Control**
* 🔄 **Automatic Lid Closing**
* 💰 **Low-Cost Prototype**
* 🛠️ **Easy to Upgrade**

---

## 🧰 Hardware Components

| Component                 | Purpose           |
| ------------------------- | ----------------- |
| Arduino Uno               | Main controller   |
| HC-SR04 Ultrasonic Sensor | Object detection  |
| Servo Motor               | Lid movement      |
| 0.96" OLED 128×64         | Status display    |
| Push Button               | Manual control    |
| Jumper Wires              | Connections       |
| Breadboard                | Prototyping       |
| Smart Dustbin Body        | Project structure |
| Power Supply              | Provides power    |

---

## 💻 Software & Technologies

* **Arduino IDE**
* **Embedded C/C++**
* **Arduino Uno**
* **Ultrasonic Sensing**
* **Servo Motor Control**
* **I2C Communication**
* **OLED Display**

### 📚 Libraries Used

```text
Servo.h
Wire.h
Adafruit_GFX.h
Adafruit_SSD1306.h
```

---

# 🔌 Circuit Diagram

<p align="center">
  <img src="images/circuit.jpg" alt="Smart Dustbin Circuit Diagram" width="700">
</p>

### Basic Connections

| Component    | Arduino Connection                |
| ------------ | --------------------------------- |
| HC-SR04 VCC  | 5V                                |
| HC-SR04 GND  | GND                               |
| HC-SR04 TRIG | Digital Pin                       |
| HC-SR04 ECHO | Digital Pin                       |
| Servo Signal | Digital PWM Pin                   |
| OLED SDA     | A4                                |
| OLED SCL     | A5                                |
| OLED GND     | GND                               |
| OLED VCC     | According to module specification |
| Push Button  | Digital Input                     |

> ⚠️ **Note:** If the servo requires more current than the Arduino can safely supply, use a separate 5V supply for the servo and connect the external supply GND to Arduino GND.

---

# ⚙️ How It Works

```text
        👤 Object Approaches
                │
                ▼
       📡 Ultrasonic Sensor
                │
                ▼
       Object Within Range?
          ┌─────┴─────┐
         YES           NO
          │             │
          ▼             │
    ⚙️ Servo Opens      │
       The Lid          │
          │             │
          ▼             │
    🖥️ OLED: OPEN       │
          │             │
          ▼             │
       Wait/Delay       │
          │             │
          ▼             │
    ⚙️ Servo Closes ◄───┘
       The Lid
          │
          ▼
    🖥️ OLED: CLOSED
          │
          ▼
        Repeat
```

---

# 🧪 Working Demonstration

## 📸 Prototype

<p align="center">
  <img src="images/prototype.jpg" alt="Smart Dustbin Prototype" width="600">
</p>

## 🔧 Hardware Setup

<p align="center">
  <img src="images/hardware.jpg" alt="Hardware Setup" width="600">
</p>

## 🖥️ OLED Display

<p align="center">
  <img src="images/oled.jpg" alt="OLED Display" width="400">
</p>

---

# 🎥 Project Demonstration Video

### ▶️ Working Video

Upload your demonstration video to **YouTube** and replace the link below:

```text
https://www.youtube.com/watch?v=YOUR_VIDEO_ID
```

Then add:

```markdown
[![Smart Dustbin Working](images/prototype.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)
```

> 💡 Clicking the project image will open the demonstration video on YouTube.

---

# 🚀 Getting Started

## 1️⃣ Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Smart-Dustbin.git
```

## 2️⃣ Open the Project

Open:

```text
Smart_Dustbin.ino
```

using **Arduino IDE**.

## 3️⃣ Install Required Libraries

From Arduino IDE:

```text
Sketch
→ Include Library
→ Manage Libraries
```

Install:

```text
Adafruit GFX Library
Adafruit SSD1306
```

The `Servo` and `Wire` libraries are generally available with the Arduino environment.

## 4️⃣ Connect the Hardware

Connect all components according to the circuit diagram provided above.

## 5️⃣ Upload the Code

Select:

```text
Tools → Board → Arduino Uno
```

Then select the appropriate COM port and click **Upload**.

## 6️⃣ Test the System

Bring your hand or an object near the ultrasonic sensor.

The dustbin should automatically open and then close after the configured delay.

---

# 📂 Project Structure

```text
Smart-Dustbin/
│
├── README.md
├── Smart_Dustbin.ino
│
├── images/
│   ├── prototype.jpg
│   ├── circuit.jpg
│   ├── hardware.jpg
│   └── oled.jpg
│
└── LICENSE
```

---

# 🔮 Future Enhancements

The current prototype can be further upgraded with:

* 📱 Mobile application control
* ☁️ IoT/cloud monitoring
* 📊 Waste-level monitoring
* 🔔 Full-bin notification
* ♻️ Wet/dry waste segregation
* 🤖 AI-based waste classification
* 📷 Camera-based waste detection
* 🌐 Web dashboard
* 🔋 Solar/battery operation
* 📍 Multiple-bin monitoring

---

# 🌍 Applications

The Smart Dustbin can be deployed in:

* 🏠 Homes
* 🏫 Schools & Colleges
* 🏢 Offices
* 🏥 Hospitals
* 🏨 Hotels
* 🛍️ Shopping Malls
* 🚉 Public Places
* 🏭 Industrial Areas

---

# 📈 Advantages

✅ Hygienic contactless operation
✅ Simple and affordable design
✅ Automatic lid control
✅ Easy to maintain
✅ Low power requirements
✅ Suitable for educational projects
✅ Easily expandable with IoT features

---

# 👨‍💻 Project Team

### 🚀 Team Members

| Name                      | Role                     | GitHub                                     |
| ------------------------- | ------------------------ | ------------------------------------------ |
| **Mohit Sunil Pimpalkar** | Project Lead / Developer | [GitHub](https://github.com/YOUR_USERNAME) |
| **Team Member 2**         | Hardware & Electronics   | [GitHub](https://github.com/USERNAME)      |
| **Team Member 3**         | Software & Testing       | [GitHub](https://github.com/USERNAME)      |
| **Team Member 4**         | Documentation & Design   | [GitHub](https://github.com/USERNAME)      |

> Replace the placeholder names and GitHub usernames with your actual team details.

---

# 👨‍🏫 Project Guidance

**Project Guide:**
`Your Professor's Name`

**Department:** Computer Engineering

**College:** `Your College Name`

**Academic Year:** 2026–2027

---

# 📊 Project Status

![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

| Module               | Status         |
| -------------------- | -------------- |
| Arduino Programming  | ✅ Completed    |
| Ultrasonic Detection | ✅ Completed    |
| Servo Control        | ✅ Completed    |
| OLED Display         | ✅ Completed    |
| Push Button          | ✅ Completed    |
| Automatic Lid        | ✅ Completed    |
| Hardware Integration | ✅ Completed    |
| Testing              | ✅ Completed    |
| Documentation        | 🚧 In Progress |

---

# 📜 License

This project is licensed under the **MIT License**.

You are free to use, modify, and distribute this project for educational and development purposes.

---

# ⭐ Support

If you found this project useful:

⭐ **Star this repository**
🍴 **Fork the repository**
🐛 **Report issues**
💡 **Suggest improvements**

---

## 🙏 Acknowledgement

We would like to thank our project guide, faculty members, and everyone who supported us during the development and testing of this project.

---

<p align="center">

### 🗑️ Smart Dustbin

**Building a cleaner and smarter future with technology. 🌱**

</p>

