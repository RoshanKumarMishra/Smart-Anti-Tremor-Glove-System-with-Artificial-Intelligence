# Smart Anti-Tremor Glove System with Artificial Intelligence 🩺🧤🤖

An advanced, non-invasive, closed-loop medical device developed to provide real-time tremor suppression for individuals living with Parkinson's disease and Essential Tremor. 

Unlike traditional commercial devices that run on completely reactive principles (introducing massive tracking loops that cause up to a 90-degree phase error at 5 Hz, leading to tremor acceleration), this platform leverages **Predictive Phase-Lead Compensation (PPLC)** to neutralize physical muscle abnormalities seamlessly before they physically fully move the hand.

---

## 🔬 Core Engineering Breakthroughs

### 1. Predictive Phase-Lead Compensation (PPLC)
At typical Parkinsonian tremor limits (3 Hz - 8 Hz), an uncompensated electromechanical latency window of 50 ms can shift feedback calculations into destructive reinforcement loops. This system isolates real-time parameters using a **Hilbert Transform analytic signal** to compute the instantaneous frequency and phase sample-by-sample. By linearly projecting tracking data 50 ms into the future, it drives a phase-inverted counter-force immediately prior to muscle spike impacts.

### 2. Edge-AI Support Vector Machine (SVM) Classifier
To eliminate over-correction and limit haptic feedback spikes during intentional user tasks (e.g., writing, handling utensils), a 24-dimensional feature layer window is tracked via an SVM engine configured with a Radial Basis Function (RBF) kernel ($\gamma = 0.01$).
* **Training Calibration Metrics:** 98.24%
* **Testing Evaluation Accuracy:** 97.10%
* **Overfitting Margin Gap:** 1.14%
* **Deployment Topology:** Zero-dependency, pure stack-allocated C code compiled directly via `micromlgen` to execute bare-metal on ARM Cortex-M7 hardware with zero OS scheduling interference.

### 3. Integrated Sub-10 ms Control Loop
The entire analytical cycle—comprising IMU collection data loops, 24-dimensional statistical feature generation, edge SVM inference, Hilbert tracking algorithms, PPLC extrapolation calculations, $I^2C$ haptic commands, and mechanical actuator relaxation cycles—finishes in just **9.5 ms**, preserving a **40.5 ms safe operating cushion** within the 50 ms tracking timeline.

---

## 📂 Repository Tree Structure

Organize your operational directory space according to this standard tree structure:
```text
├── Hardware/               # System Schematic Diagrams & CAD Architecture Blueprints
│   ├── Schematics/         # High-resolution PDF circuit layouts for instant inspection
│   └── Gerber/             # Complete ZIP archives ready for board house execution
├── Firmware/               # Embedded Software Production Packages
│   ├── Teensy_Controller/  # High-Speed Master Signal Filter Engine Code (Teensy 4.1)
│   └── ESP32_Gateway/      # Asynchronous BLE Telemetry Processing Hub Code (ESP32)
├── AI_Model/               # Machine Learning Training Pipelines & Matrices
│   ├── train_svm.py        # Python Scikit-Learn training loop & micromlgen exporter
│   └── Dataset.csv         # Local copy of your windowed sensor data arrays
├── Docs/                   # System validation summaries, report chapters, and patent logs
└── README.md               # Landing landing documentation dashboard
```

---

## 📊 High-Level Component Bill of Materials (BOM)

| Component | Architecture Role | Native Bus Interface | Target Voltage | Operational Description |
| :--- | :--- | :--- | :--- | :--- |
| **Teensy 4.1** | Edge Processing DSP Node | Master System Link | 3.3V DC Logic | ARM Cortex-M7 running at 600 MHz with hardware FPU arrays. |
| **ESP32 Node** | Telemetry Gateway Receiver | Hardware UART (`Serial2`) | 3.3V DC Logic | Dual-Core 240 MHz logic engine handling asynchronous BLE logs. |
| **ICM-42688-P** | 6-Axis High-Resolution IMU | I2C Protocol (`0x68`) | 3.3V Signaling | 16-bit high-accuracy sampling engine evaluating rotational vectors. |
| **DRV2605L** | Haptic Output Controller | I2C Protocol | 3.3V / 5V Rails | Dedicated IC running built-in PWM waveform matrices for LRAs. |
| **LRA Coin Motors** | Physical Dampener Node | Analog Terminal Leg | Waveform Fed | Dual metacarpal mounting layout running at 175 Hz mechanical resonance. |
| **Flex Sensor Array** | Contextual Angle Monitor | Analog Pin A0 Link | 0 - 3.3V Variable | Provides finger articulation indexing to identify resting postures. |

---

## 📄 Patent Claim Architecture Overview (Section 6.2 Reference)
* **Claim 1 (Independent Broadest):** A wearable device for real-time tremor suppression comprising an IMU sensor array, a pathological state classifier, a phase predictor that extrapolates instantaneous oscillatory phase by a lead-time equal to the system's total electromechanical delay, and an actuator that applies a phase-inverted counter-force to the user's limb.
* **Claim 2 (Dependent Process):** Algorithmic execution of Butterworth passband isolation, Hilbert calculation structures, and confidence-gated variable scaling.
* **Claim 3 (Dependent Implementation):** Deploying bare-metal SVM architectures utilizing standalone C headers via `micromlgen` structures.
* **Claim 4 (Dependent Guardrails):** Dynamic counter-force damping loops ($F_c 	imes 0.5$) implemented immediately if phase confidence indicators scale under safe barriers ($c < 0.3$).

---

## 📄 License & Intellectual Compliance
This software repository and its electronic layout documentation files are maintained openly under the text terms of the standard MIT License. Review the bundled `LICENSE` parameters for details.
