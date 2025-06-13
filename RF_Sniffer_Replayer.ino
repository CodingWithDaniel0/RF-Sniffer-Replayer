// File: RF_Sniffer_Replayer.ino
// Arduino RF Sniffer & Replayer (ASK/OOK 315/433 MHz)
// Requires: rc-switch library (https://github.com/sui77/rc-switch)

#include <RCSwitch.h>

#define RF_FREQ      433      // Set to 315 or 433
#define PIN_RX       2        // Data pin for receiver
#define PIN_TX       4        // Data pin for transmitter (enable TX)
#define PIN_BUTTON   5        // Replay trigger button (to GND)
#define PIN_LED      6        // Status LED

RCSwitch rf = RCSwitch();

// Last received code info
unsigned long lastCode = 0;
int lastProtocol = 0;
int lastPulseLen = 0;

// Debounce
unsigned long lastBtnTime = 0;
const unsigned long DEBOUNCE_MS = 50;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // RX setup
  rf.enableReceive(PIN_RX);

  // TX setup
  rf.enableTransmit(PIN_TX);
  rf.setRepeatTransmit(10);       // send each code 10×
  if (RF_FREQ == 315) rf.setProtocol(1);
  else                 rf.setProtocol(1);

  Serial.println();
  Serial.println("=== RF Sniffer & Replayer ===");
  Serial.print("Frequency: "); Serial.print(RF_FREQ); Serial.println(" MHz");
}

void loop() {
  // 1) Sniff incoming codes
  if (rf.available()) {
    lastCode     = rf.getReceivedValue();
    lastProtocol = rf.getReceivedProtocol();
    lastPulseLen = rf.getReceivedDelay();
    Serial.print("Received: "); Serial.print(lastCode);
    Serial.print("   Protocol: "); Serial.print(lastProtocol);
    Serial.print("   PulseLen: "); Serial.println(lastPulseLen);
    // LED flash
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);

    rf.resetAvailable();
  }

  // 2) Check replay button (simple debounce)
  if (digitalRead(PIN_BUTTON) == LOW && millis() - lastBtnTime > DEBOUNCE_MS) {
    lastBtnTime = millis();
    if (lastCode != 0) {
      Serial.print("Replaying code: "); Serial.println(lastCode);
      // Optionally reconfigure
      rf.setProtocol(lastProtocol);
      rf.setPulseLength(lastPulseLen);
      rf.send(lastCode);
      // LED indication
      digitalWrite(PIN_LED, HIGH);
      delay(200);
      digitalWrite(PIN_LED, LOW);
    } else {
      Serial.println("No code captured yet.");
    }
  }
}
