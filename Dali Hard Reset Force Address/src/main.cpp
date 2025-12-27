/**
 * ESP32 DALI Force Address Tool
 * * Description: Hard resets a DALI lamp and forces a specific Short Address.
 * WARNING: Connect only ONE lamp at a time.
 */

#include <Arduino.h>

// --- USER SETTINGS ---
const int DALI_TX_PIN = 21; 

// WHICH ADDRESS DO YOU WANT TO ASSIGN?
const byte TARGET_ADDRESS = 2; // Example: Force lamp to Address 2

// Timing for Manchester Encoding (1200 baud -> 833us bit time -> 416us half bit)
const int TE = 416; 

void setupDALI() {
  pinMode(DALI_TX_PIN, OUTPUT);
  digitalWrite(DALI_TX_PIN, LOW); 
}

void sendBit(bool b) {
  if (b) {
    digitalWrite(DALI_TX_PIN, HIGH); delayMicroseconds(TE);
    digitalWrite(DALI_TX_PIN, LOW);  delayMicroseconds(TE);
  } else {
    digitalWrite(DALI_TX_PIN, LOW);  delayMicroseconds(TE);
    digitalWrite(DALI_TX_PIN, HIGH); delayMicroseconds(TE);
  }
}

void sendDaliCmd(byte byte1, byte byte2) {
  sendBit(true); // Start Bit
  for (int i = 7; i >= 0; i--) sendBit((byte1 >> i) & 1);
  for (int i = 7; i >= 0; i--) sendBit((byte2 >> i) & 1);
  digitalWrite(DALI_TX_PIN, LOW); 
  delay(20); // Safety delay between commands
}

void forceProgramAddress(byte newShortAddress) {
  Serial.printf("Trying to FORCE address: %d\n", newShortAddress);
  byte formattedAddress = (newShortAddress << 1) | 1;

  // STEP 0: HARD RESET (Cmd 32) - Clears glitches
  // Broadcast to ALL connected devices (Connect only one!)
  Serial.println("> Sending RESET (Unlock)...");
  sendDaliCmd(0xFF, 0x20); 
  delay(10);
  sendDaliCmd(0xFF, 0x20); // Reset requires double command
  delay(200); // Wait for driver to restart

  // STEP 1: INITIALISE (Cmd A5 00) - Enter Programming Mode
  Serial.println("> Sending INITIALISE...");
  sendDaliCmd(0xA5, 0x00);
  delay(10);
  sendDaliCmd(0xA5, 0x00);
  delay(100);

  // STEP 2: SET DTR (Cmd A3) - Load new address to register
  Serial.println("> Setting DTR...");
  sendDaliCmd(0xA3, formattedAddress);
  delay(100);
  sendDaliCmd(0xA3, formattedAddress); // Twice for safety
  delay(100);

  // STEP 3: STORE DTR AS SHORT ADDRESS (Cmd 80)
  Serial.println("> Saving Address...");
  sendDaliCmd(0xFF, 0x80); 
  delay(50);
  sendDaliCmd(0xFF, 0x80);
  delay(100);

  // STEP 4: TERMINATE (Cmd A1) - Exit Programming Mode
  Serial.println("> Sending TERMINATE...");
  sendDaliCmd(0xA1, 0x00);
  delay(100);
   
  Serial.println("Programming Sequence Finished.");
}

void setup() {
  delay(5000); // Time to open Serial Monitor
  Serial.begin(115200);
  setupDALI();
  
  Serial.println("Waiting for power stabilization...");
  delay(2000); 

  Serial.println("--- DALI FORCE PROGRAMMER ---");
   
  // Execute the programming sequence once on boot
  forceProgramAddress(TARGET_ADDRESS);
}

void loop() {
  // Verification Loop: Blink the target address
  byte targetByte = (TARGET_ADDRESS << 1);

  Serial.printf("Testing Address %d: ON\n", TARGET_ADDRESS);
  sendDaliCmd(targetByte, 0x05); // Cmd 05: RECALL MAX LEVEL
  delay(1500);

  Serial.printf("Testing Address %d: OFF\n", TARGET_ADDRESS);
  sendDaliCmd(targetByte, 0x00); // Cmd 00: OFF
  delay(1500);
}
