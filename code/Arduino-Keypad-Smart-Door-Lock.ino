#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myServo;

String password = "15151"; // Default password
String inputPassword = "";
const int buzzerPin = 12;
const int ledPin = 11;
const int alarmLightPin = 13; // Alarm light pin

const int ledA0 = A0;
const int ledA1 = A1;
const int ledA2 = A2;

const String secretCode = "1122"; // Secret code for changing password
String inputSecretCode = "";
bool enteringSecretCode = false;

int incorrectAttempts = 0;
bool isDoorLocked = true;
unsigned long lastKeyPressTime = 0;
const unsigned long passwordEntryTimeout = 2000;
const int maxIncorrectAttempts = 3;
const unsigned long lockoutDuration = 4000;
unsigned long lockoutStartTime = 0;
bool isLockedOut = false;
bool alarmActive = false;
bool alarmTriggered = false;

unsigned long doorUnlockTime = 0;
const unsigned long doorUnlockDuration = 5000;

unsigned long sirenStartTime = 0;
bool sirenOn = false;

bool changingPassword = false; // Flag for changing password
String newPassword = ""; // Variable to store new password

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  myServo.attach(10);
  myServo.write(0);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(alarmLightPin, OUTPUT); // Set alarm light pin as output

  pinMode(ledA0, OUTPUT);
  pinMode(ledA1, OUTPUT);
  pinMode(ledA2, OUTPUT);

  Serial.println("Setup Complete");
  enterPassword();
}

void loop() {
  if (isLockedOut) {
    if (millis() - lockoutStartTime > lockoutDuration) {
      isLockedOut = false;
      Serial.println("Lockout period over.");
      enterPassword();
    } else {
      return;
    }
  }

  char key = keypad.getKey();
  if (key) {
    lastKeyPressTime = millis();
    handleKeyPress(key);
  }

  if (alarmTriggered) {
    runSiren();
  }

  if (inputPassword.length() > 0 && millis() - lastKeyPressTime > passwordEntryTimeout) {
    inputPassword = "";
    Serial.println("Password entry timeout.");
    soundBuzzerMultipleTimes(2);
    enterPassword();
  }

  if (!isDoorLocked && millis() - doorUnlockTime > doorUnlockDuration) {
    Serial.println("Auto-locking the door.");
    myServo.write(0);
    digitalWrite(ledPin, LOW);
    isDoorLocked = true;
  }
}

void handleKeyPress(char key) {
 if (changingPassword) {
  if (key == '#') { // Check if the save key is pressed
    if (newPassword.length() == 5) { // Ensure the new password is complete
      Serial.println("New password set.");
      password = newPassword; // Update the password
      newPassword = ""; // Reset new password
      changingPassword = false; // Exit password change mode
      blinkLights(3); // Blink lights after setting new password
      Serial.println("Password change successful. Returning to normal mode.");
      enterPassword(); // Return to password entry mode
    } else {
      Serial.println("New password must be 5 digits.");
      soundBuzzerMultipleTimes(2);
    }
    return;
  }

  if (newPassword.length() < 5) {
    newPassword += key; // Add the key to the new password
    soundBuzzerOnce();
    if (newPassword.length() == 5) {
      Serial.println("New password complete. Press '#' to save.");
    }
  }
  return;
}

  

  if (enteringSecretCode) {
    inputSecretCode += key;
    soundBuzzerOnce();

    if (inputSecretCode.length() == 4) {
      if (inputSecretCode == secretCode) {
        Serial.println("Secret code correct. Resetting incorrect attempts.");
        incorrectAttempts = 0;
        isLockedOut = false;
        blinkLights(3); // Blink lights after correct secret code
        changingPassword = true; // Start changing password
        Serial.println("Enter new password:");
      } else {
        Serial.println("Incorrect secret code.");
        soundBuzzerMultipleTimes(2);
      }
      inputSecretCode = ""; // Reset input after checking
      enteringSecretCode = false; // Reset state
      enterPassword();
    }
    return;
  }

  if (key == 'C') { // Cancel password change
  changingPassword = false;
  newPassword = "";
  Serial.println("Password change canceled.");
  enterPassword();
  return;
}
 else if (key == 'D') {
    enteringSecretCode = true;
    inputSecretCode = ""; // Reset input for secret code
    Serial.println("Enter secret code:");
  } else if (key == 'B') { // Start password change process
    changingPassword = true;
    inputPassword = ""; // Reset input password
    Serial.println("Enter current password:");
  } else {
    inputPassword += key;
    soundBuzzerOnce();

    if (inputPassword.length() == 5) {
      if (inputPassword == password) {
        if (alarmActive || alarmTriggered) {
          stopAlarm();
        } else {
          toggleDoorLock();
        }
        incorrectAttempts = 0;
      } else {
        incorrectAttempts++;
        Serial.println("Wrong password!");

        if (incorrectAttempts >= maxIncorrectAttempts) {
          Serial.println("Alarm triggered!");
          alarmTriggered = true;
          sirenStartTime = millis();
          incorrectAttempts = 0;
          isLockedOut = true;
          lockoutStartTime = millis();
          Serial.println("You are locked out for 1 minute.");
        }
      }
      inputPassword = "";
      enterPassword();
    }
  }
}

void runSiren() {
  if (!sirenOn) {
    sirenOn = true;
    Serial.println("Activating siren...");
    sirenStartTime = millis();
  }

  while (millis() - sirenStartTime < 4000) {
    digitalWrite(alarmLightPin, HIGH);
    digitalWrite(ledA0, HIGH);
    tone(buzzerPin, 2000);
    delay(300);
    digitalWrite(ledA0, LOW);
    digitalWrite(alarmLightPin, LOW);
    
    digitalWrite(alarmLightPin, HIGH);
    digitalWrite(ledA1, HIGH);
    delay(300);
    digitalWrite(ledA1, LOW);
    digitalWrite(alarmLightPin, LOW);
    
    digitalWrite(alarmLightPin, HIGH);
    digitalWrite(ledA2, HIGH);
    delay(300);
    digitalWrite(ledA2, LOW);
    digitalWrite(alarmLightPin, LOW);
    
    noTone(buzzerPin);
    delay(200);
  }

  stopAlarm();
}

void stopAlarm() {
  Serial.println("Stopping alarm...");
  alarmActive = false;
  alarmTriggered = false;
  sirenOn = false;
  noTone(buzzerPin);
  digitalWrite(ledA0, LOW);
  digitalWrite(ledA1, LOW);
  digitalWrite(ledA2, LOW);
  digitalWrite(alarmLightPin, LOW); // Turn off alarm light
  Serial.println("Alarm stopped.");
  enterPassword();
}

void soundBuzzerOnce() {
  tone(buzzerPin, 1000, 200);
  delay(200);
  noTone(buzzerPin);
}

void soundBuzzerMultipleTimes(int times) {
  for (int i = 0; i < times; i++) {
    tone(buzzerPin, 1000, 200);
    delay(300);
    noTone(buzzerPin);
    delay(100);
  }
}

void toggleDoorLock() {
  if (isDoorLocked) {
    Serial.println("Unlocking door...");
    myServo.write(90);
    digitalWrite(ledPin, HIGH);
    isDoorLocked = false;
    doorUnlockTime = millis();
  } else {
    Serial.println("Locking door...");
    myServo.write(0);
    digitalWrite(ledPin, LOW);
    isDoorLocked = true;
  }
}

void enterPassword() {
  Serial.println("Enter password:");
  inputPassword = ""; // Reset input password
}

void blinkLights(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledA0, HIGH);
    digitalWrite(ledA1, HIGH);
    digitalWrite(ledA2, HIGH);
    delay(200); // Keep lights on for 200 ms
    digitalWrite(ledA0, LOW);
    digitalWrite(ledA1, LOW);
    digitalWrite(ledA2, LOW);
    delay(200); // Keep lights off for 200 ms
  }
}