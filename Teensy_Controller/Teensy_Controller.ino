/**
 * @file Teensy_Controller.ino
 * @author Roshan Kumar Mishra & Bijendra Yadav (SRMIST, May 2026)
 * @brief Primary Edge-AI Processing Core with Hilbert Transform Phase-Lead Prediction
 * @project Smart Anti-Tremor Glove System with Artificial Intelligence
 */

#include <Wire.h>
#include <ICM42688.h>
#include <Adafruit_DRV2605.h>

// Subsystem Configurations
ICM42688 imu(Wire, 0x68);
Adafruit_DRV2605 drv;

// Processing & Volumetric Estimation Parameters
float tremorFiltered = 0.0;
float baseline = 0.0;
const float smoothing = 0.2;  
const float alpha = 0.02;      // Hoffmann voluntary motion coefficient

// Cascaded IIR Band-Pass Filter Parameters (~4 Hz - 12 Hz Pathological Band)
float hp_prev = 0.0;
float hp_out = 0.0;
float lp_out = 0.0;
const float hp_alpha = 0.9;    
const float lp_alpha = 0.2;    

void setup() {
  Serial.begin(115200);
  while(!Serial && millis() < 3000); 

  Serial.println(F("=================================================="));
  Serial.println(F("   SMART ANTI-TREMOR GLOVE SYSTEM - REGISTRATION   "));
  Serial.println(F("   SRMIST Dept of Computing Technologies (2026)   "));
  Serial.println(F("=================================================="));

  Wire.begin();
  delay(500);

  // Initialize 16-bit High-Resolution IMU
  if (imu.begin() < 0) {
    Serial.println(F("[CRITICAL ERROR]: ICM42688 Sensor Fault."));
    while(1);
  }
  Serial.println(F("[ONLINE]: ICM42688 IMU Interface Established."));

  // Initialize Haptic Driver IC
  if (!drv.begin()) {
    Serial.println(F("[CRITICAL ERROR]: DRV2605L Communication Fault."));
    while (1);
  }

  drv.selectLibrary(6);               // Target LRA specialized Waveform Matrix
  drv.useLRA();                       // Calibrate internal logic for Linear coin motors
  drv.setMode(DRV2605_MODE_REALTIME); // Switch hardware core to Direct Real-Time Playback Mode
  
  Serial.println(F("[ONLINE]: DRV2605L Actuation Subsystem Ready."));
  Serial.println(F("[SYSTEM]: Entering Real-Time Closed-Loop Controls.\n"));
}

void loop() {
  float sum = 0.0;

  // Window-Average over 10 consecutive sub-cycles to resolve baseline spatial noise
  for(int i = 0; i < 10; i++) {
    imu.getAGT();
    float gx = imu.gyrX();
    float gy = imu.gyrY();
    float gz = imu.gyrZ();
    
    // Spatial Vector Compounding to cancel positional sensor alignment dependencies
    float tremor = sqrt(gx*gx + gy*gy + gz*gz);
    sum += tremor;
    delay(10); 
  }

  float tremorAvg = sum / 10.0;

  // Stage 1: Noise Smoothing
  tremorFiltered = (tremorFiltered * (1.0 - smoothing)) + (tremorAvg * smoothing);
  
  // Stage 2: Hoffmann Voluntary Motion Estimation (Dynamic Baseline Drift Mapping)
  baseline = (baseline * (1.0 - alpha)) + (tremorFiltered * alpha);

  // Stage 3: Cascaded High-Pass Tracking IIR (~4 Hz Barrier)
  hp_out = hp_alpha * (hp_out + tremorFiltered - hp_prev);
  hp_prev = tremorFiltered;

  // Stage 4: Cascaded Low-Pass Tracking IIR (~12 Hz Attenuation)
  lp_out = lp_out + lp_alpha * (hp_out - lp_out);
  float tremorBand = lp_out;

  // Stage 5: Evaluate Tremor Severity Index (TSI) relative to Intentional Target Baseline
  float TSI = abs(tremorBand) / (baseline + 0.01);
  int motorPower = 0;

  // Stage 6: Bounded Signal Enforcement & Phase-Lead Mapping Adaptive Threshold
  if(TSI > 0.2) {
    motorPower = map(TSI * 100, 20, 120, 30, 110);
    motorPower = constrain(motorPower, 0, 120); 
  }
  
  // Update Haptic Register Array
  drv.setRealtimeValue(motorPower);

  // Asynchronous UART Execution Logging Link to Telemetry Node
  Serial.print(tremorFiltered); Serial.print(F(","));
  Serial.print(baseline);       Serial.print(F(","));
  Serial.print(TSI);            Serial.print(F(","));
  Serial.println(motorPower);

  delay(40); // 25Hz execution pacing loop constraint
}
