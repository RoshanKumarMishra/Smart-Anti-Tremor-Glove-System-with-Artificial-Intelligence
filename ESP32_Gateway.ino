/**
 * @file ESP32_Gateway.ino
 * @author Roshan Kumar Mishra & Bijendra Yadav (SRMIST, May 2026)
 * @brief Asynchronous Communications Subsystem & BLE Telemetry Gateway Node
 * @project Smart Anti-Tremor Glove System with Artificial Intelligence
 */

#define RXD2 16  // Physical UART Connection Interconnect mapping to Teensy Pin 1
#define TXD2 17  // Physical UART Connection Interconnect mapping to Teensy Pin 0

void setup() {
  Serial.begin(115200); // Initialize Local Host Terminal Diagnostics Connection Link
  while(!Serial);
  
  // Initialize Secondary Independent Hardware Bus Interconnect at 115200 bps
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println(F("=================================================="));
  Serial.println(F("   ESP32 ISOLATED TELEMETRY GATEWAY SUSBYSTEM     "));
  Serial.println(F("   Awaiting Stream Arrays from Master Core...     "));
  Serial.println(F("=================================================="));
}

void loop() {
  // Capture inbound frames asynchronously without locking execution timelines
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim(); // Clean trailing string bytes
    
    // Output pipeline metrics to localized serial monitors for telemetry observation
    Serial.print(F("[INGESTED METRICS FRAME]: "));
    Serial.println(data);
  }
}
