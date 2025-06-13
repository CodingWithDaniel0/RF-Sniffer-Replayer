# RF Remote Sniffer & Replayer
#### Author: Bocaletto Luca
A turnkey Arduino-based solution to capture (sniff) ASK/OOK 315 MHz or 433 MHz remote‐control codes and replay them on demand. Perfect for testing, reverse‐engineering or automating RF remotes (garage doors, wireless switches, sockets).

**Files**  
- `README.md`  
- `RF_Sniffer_Replayer.ino`

---

## 1. Concept

1. **Sniff Mode**: Arduino listens on a 315 MHz or 433 MHz ASK/OOK receiver and prints any received codes to Serial.  
2. **Replay Mode**: After capturing, you can send the last sniffed code back out via a paired transmitter module by pressing a push-button.  

Key features:  
- Auto-print raw code and protocol parameters (pulse length, bit count)  
- Store the last code in RAM for instant replay  
- LED feedback on sniff and transmit events  
- Works with fixed-code remotes (rolling-code requires custom logic)

---

## 2. Bill of Materials

| Qty | Component                        | Notes                               |
|-----|----------------------------------|-------------------------------------|
| 1   | Arduino Uno / Nano / Pro Mini    | 5 V version                         |
| 1   | 433 MHz ASK Receiver module      | e.g. XY-MK-5V                      |
| 1   | 433 MHz OOK Transmitter module   | e.g. MX-05V                        |
| 1   | Push-button                      | for manual replay trigger           |
| 1   | LED (5 mm) + 220 Ω resistor      | for status indicator                |
|     | Jumper wires & breadboard        |                                     |
|     | USB cable                        | for programming & power            |

---

## 3. Wiring Diagram

        Arduino        Receiver           Transmitter         Push-Button         LED
        ┌──────────────────────────────────────────────────────────────────────────┐
        │   ┌───┐                                                                  │
        │   D2 ──► DATA_IN     Receiver DATA OUT ─► D3                             │
        │   D3 ◄─ DATA_OUT? [not used]                                             │
        │   5 V ──► VCC         VCC Transmitter VCC ─► 5 V                          │
        │   GND ──► GND         GND Transmitter GND ─► GND                         │
        │                                                                          │
        │   D4 ──► TX_ENABLE (optional)                                            │
        │   D5 ◄─ BUTTON INPUT ── push to GND (with pull-up)                       │
        │   D6 ──► LED (via 220 Ω resistor)                                        │
        └──────────────────────────────────────────────────────────────────────────┘


- DATA_IN pin of receiver → D2  
- DATA_OUT pin of transmitter → D4 (some modules need enable; if not, tie VCC)  
- Button between D5 and GND; configure internal PULLUP  
- LED on D6 through 220 Ω to GND  

---

## 4. Software Setup

1. **Install Arduino IDE** ≥ 1.8.x.  
2. **Library**: open Library Manager → install **rc-switch** by **sui77**.  
3. Create a sketch folder named `RF_Sniffer_Replayer` and place `RF_Sniffer_Replayer.ino` inside.  
4. Open the sketch in the IDE.  
5. Select your board (e.g. Uno) and correct COM port.

---

## 5. Sketch Usage

1. Upload the sketch.  
2. Open **Serial Monitor** at **115200 baud**.  
3. Set your transmitter frequency (315 or 433) at top of code.  
4. **Sniff Mode**: any RF code received will print as:  Received: 12345678 Protocol: 1 PulseLen: 350
5. **Replay Mode**: press the push-button → Arduino retransmits the last captured code; LED flashes to indicate transmission.

---

## 6. Notes & Extensions

- To capture rolling-code systems you’ll need to reverse-engineer algorithmic sequence, store dynamic code and state.  
- You can adapt this to 315 MHz by changing `#define RF_FREQ 315` and wiring a 315 MHz pair.  
- Add EEPROM storage to save multiple codes and select via menus.  

---

Proceed to the code file below.  
