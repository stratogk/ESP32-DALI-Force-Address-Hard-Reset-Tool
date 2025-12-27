#include <Arduino.h>

// --- ΡΥΘΜΙΣΕΙΣ ---
const int DALI_TX_PIN = 21; 

// ΠΟΙΑ ΔΙΕΥΘΥΝΣΗ ΘΕΛΟΥΜΕ ΝΑ ΔΩΣΟΥΜΕ;
const byte TARGET_ADDRESS = 2; // Βάλε 1 για το δεύτερο τροφοδοτικό

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
  sendBit(true); 
  for (int i = 7; i >= 0; i--) sendBit((byte1 >> i) & 1);
  for (int i = 7; i >= 0; i--) sendBit((byte2 >> i) & 1);
  digitalWrite(DALI_TX_PIN, LOW); 
  delay(20); // Αυξήσαμε λίγο το delay για ασφάλεια
}

void forceProgramAddress(byte newShortAddress) {
  Serial.printf("Trying to FORCE address: %d\n", newShortAddress);
  byte formattedAddress = (newShortAddress << 1) | 1;

  // ΒΗΜΑ 0: HARD RESET (Εντολή 32) - Για να καθαρίσει τυχόν κολλήματα
  // Στέλνουμε RESET Broadcast
  Serial.println("> Sending RESET (Unlock)...");
  sendDaliCmd(0xFF, 0x20); 
  delay(10);
  sendDaliCmd(0xFF, 0x20); // Πάντα διπλή εντολή το Reset
  delay(200); // Περιμένουμε να ξυπνήσει ο driver

  // ΒΗΜΑ 1: INITIALISE (Εντολή A5 00) - Μπες σε mode προγραμματισμού
  Serial.println("> Sending INITIALISE...");
  sendDaliCmd(0xA5, 0x00);
  delay(10);
  sendDaliCmd(0xA5, 0x00);
  delay(100);

  // ΒΗΜΑ 2: Set DTR με τη νέα διεύθυνση
  Serial.println("> Setting DTR...");
  sendDaliCmd(0xA3, formattedAddress);
  delay(100);
  // Το στέλνουμε και 2η φορά για σιγουριά
  sendDaliCmd(0xA3, formattedAddress);
  delay(100);

  // ΒΗΜΑ 3: STORE DTR AS SHORT ADDRESS
  Serial.println("> Saving Address...");
  sendDaliCmd(0xFF, 0x80); 
  delay(50);
  sendDaliCmd(0xFF, 0x80);
  delay(100);

  // ΒΗΜΑ 4: TERMINATE (Βγες από το mode)
  Serial.println("> Sending TERMINATE...");
  sendDaliCmd(0xA1, 0x00);
  delay(100);
  
  Serial.println("Programming Sequence Finished.");
}

void setup() {
  delay(6000);
  Serial.begin(115200);
  setupDALI();
  delay(3000); // Περίμενε 3 δευτερόλεπτα να σταθεροποιηθεί το ρεύμα

  Serial.println("--- DALI FORCE PROGRAMMER ---");
  
  // Εκτέλεση προγραμματισμού
  forceProgramAddress(TARGET_ADDRESS);
}

void loop() {
  // Blink στη συγκεκριμένη διεύθυνση
  byte targetByte = (TARGET_ADDRESS << 1);

  Serial.printf("Testing Address %d: ON\n", TARGET_ADDRESS);
  sendDaliCmd(targetByte, 0x05); 
  delay(1500);

  Serial.printf("Testing Address %d: OFF\n", TARGET_ADDRESS);
  sendDaliCmd(targetByte, 0x00); 
  delay(1500);
}