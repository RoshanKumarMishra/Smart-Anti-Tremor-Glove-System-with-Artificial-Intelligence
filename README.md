# Smart Anti-Tremor Glove System with Artificial Intelligence 🩺🧤🤖

An advanced, non-invasive, closed-loop wearable medical device designed to provide real-time tremor suppression for individuals living with Parkinson's disease and Essential Tremor. 

Unlike traditional commercial options that act *reactively* (causing a 90-degree phase delay at 5 Hz that can unintentionally amplify tremors), this system implements **Predictive Phase-Lead Compensation (PPLC)** to anticipate and cancel the tremor waveform exactly as it hits the physical boundary of the hand.

---

## ⚡ The Core Engineering Breakthroughs

### 1. Predictive Phase-Lead Compensation (PPLC)
At a typical Parkinsonian tremor frequency of 5 Hz, an uncompensated 50 ms electromechanical delay results in a catastrophic 90° phase error, turning intended cancellation forces into amplification. PPLC resolves this by calculating the instantaneous phase and frequency of the tremor signal sample-by-sample using a **Hilbert Transform** analytic signal, extrapolating the waveform 50 ms into the future, and deploying a phase-inverted counter-force immediately before the physical tremor peak arrives.

### 2. Edge-AI Support Vector Machine (SVM) Classifier
To prevent over-correction and unwanted haptic actuation during voluntary movements (e.g., writing, drinking), a 24-dimensional feature vector is processed using a highly optimized SVM classifier with a Radial Basis Function (RBF) kernel ($\gamma = 0.01$). 
* **Training Accuracy:** 98.24%
* **Testing Accuracy:** 97.10%
* **Train-Test Gap:** 1.14% (Zero Overfitting)
* **Execution Environment:** Pure bare-metal C arithmetic compiled via `micromlgen` onto ARM Cortex-M7 hardware with zero OS overhead.

### 3. Sub-10 ms Deterministic Latency Pipeline
The entire loop—encompassing IMU acquisition, 24-dimensional feature extraction, SVM inference, Hilbert calculations, PPLC phase tracking, $I^2C$ writing, and LRA mechanical settling—completes in just **9.5 ms** on the Teensy 4.1 hardware, leaving a comfortable **40.5 ms safety margin** within the 50 ms prediction horizon.

---

## 📂 Repository Directory Layout

Organize your project files according to the standard architecture below:

```text
├── Hardware/               # Schematic Capture & Layout Blueprints
│   ├── Schematics/         # High-resolution PDF circuit drawings 
│   └── Gerber/             # Complete ZIP archives ready for PCB manufacturing
├── Firmware/               # Embedded Software Production Implementations
│   ├── Teensy_Controller/  # Main Edge-AI & PPLC Prediction Core (Teensy 4.1)
│   └── ESP32_Gateway/      # Asynchronous BLE Telemetry Module (ESP32)
├── AI_Model/               # Python Training Script & Dataset Processing Pipeline
│   ├── train_svm.py        # Scikit-Learn training loop & micromlgen export script
│   └── Dataset.csv         # Labelled 6-axis IMU raw data window streams
├── Docs/                   # System design reports, graphs, and patent architecture logs
└── README.md               # Repository documentation landing page
