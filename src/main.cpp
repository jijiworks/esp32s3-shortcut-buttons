#include <Arduino.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

static const int BTN_NOTE = D0;  // Button 1: Create a text file
static const int BTN_CMD  = D1;  // Button 2: Open Command Prompt
static const int BTN_LOCK = D2;  // Button 3: Lock the PC

bool prevNoteState = HIGH;
bool prevCmdState  = HIGH;
bool prevLockState = HIGH;

void tapKey(uint8_t key, uint16_t waitMs = 120) {
  Keyboard.press(key);
  delay(40);
  Keyboard.release(key);
  delay(waitMs);
}
void pressCombo(uint8_t mod, uint8_t key, uint16_t waitMs = 150) {
  Keyboard.press(mod);
  delay(30);
  Keyboard.press(key);
  delay(60);
  Keyboard.releaseAll();
  delay(waitMs);
}

void tapRawKey(uint8_t key, uint16_t waitMs = 80) {
  Keyboard.pressRaw(key);
  delay(40);
  Keyboard.releaseRaw(key);
  delay(waitMs);
}
void pressRawCombo(uint8_t key1, uint8_t key2, uint16_t waitMs = 80) {
  Keyboard.pressRaw(key1);
  Keyboard.pressRaw(key2);
  delay(40);
  Keyboard.releaseRaw(key2);
  Keyboard.releaseRaw(key1);
  delay(waitMs);
}

void openRunDialog(uint16_t waitMs = 500) {
  pressCombo(KEY_LEFT_GUI, 'r', waitMs);
}
void sendWinRCommand(const char* command, uint16_t openWait = 500, uint16_t afterEnterWait = 900) {
  openRunDialog(openWait);
  Keyboard.print(command);
  delay(150);
  tapKey(KEY_RETURN, afterEnterWait);
}

// Japanese [¥] key
void typeBackslashJP(uint16_t waitMs = 80) {
  tapRawKey(0x89, waitMs);
}
void typeDoubleBackslashJP(uint16_t waitMs = 80) {
  typeBackslashJP(waitMs);
  typeBackslashJP(waitMs);
}

// Japanese [&] key
void typeAndJP(uint16_t waitMs = 80) {
  pressRawCombo(0xE1, 0x23, waitMs); // Left Shift + Keyboard 6
}
void typeDoubleAndJP(uint16_t waitMs = 80) {
  typeAndJP(waitMs);
  typeAndJP(waitMs);
}

// // Types the Windows desktop path for a Japanese keyboard layout
void typePathUserDesktopJP() {
  Keyboard.print("%USERPROFILE%");
  typeBackslashJP();
  Keyboard.print("Desktop");
}

// ========== Functions assigned to the buttons start here ==========
void createTxtDemo() {
  openRunDialog(500);
  Keyboard.print("cmd /c cd /d ");
  typePathUserDesktopJP();
  Keyboard.print(" ");
  typeDoubleAndJP();
  Keyboard.print(" echo Hello.>Sample.txt");
  typeDoubleAndJP();
  Keyboard.print(" echo.>>Sample.txt");
  typeDoubleAndJP();
  Keyboard.print(" echo This text file was created by ESP32-S3.>>Sample.txt");
  tapKey(KEY_RETURN, 900);
}

void lockPC() {
  pressCombo(KEY_LEFT_GUI, 'l', 300);
}

void openCommandPrompt() {
  sendWinRCommand("cmd", 500, 900);
}
// ========== Functions assigned to the buttons end here ==========

void handleButton(int pin, bool &prevState, void (*action)()) {
  bool currentState = digitalRead(pin);

  // Assumes INPUT_PULLUP: run only when the button is pressed
  if (prevState == HIGH && currentState == LOW) {
    delay(20);  // Debounce
    if (digitalRead(pin) == LOW) {
      action();
    }
  }

  prevState = currentState;
}

void setup() {
  pinMode(BTN_NOTE, INPUT_PULLUP);
  pinMode(BTN_CMD,  INPUT_PULLUP);
  pinMode(BTN_LOCK, INPUT_PULLUP);

  USB.productName("USB Keyboard");
  USB.manufacturerName("Generic");

  Keyboard.begin();
  USB.begin();

  delay(3000);  // Wait for the PC to recognize the device
}

void loop() {
  handleButton(BTN_NOTE, prevNoteState, createTxtDemo);
  handleButton(BTN_CMD,  prevCmdState,  openCommandPrompt);
  handleButton(BTN_LOCK, prevLockState, lockPC);

  delay(5);
}