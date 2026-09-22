# Arduino Keypad Smart Door Lock

An Arduino-based smart door lock system that uses a **4×4 keypad** for password authentication and a **servo motor** to control the door lock.

The system allows the user to enter a password to unlock the door. After successful authentication, the door remains unlocked for 5 seconds and then automatically locks again.

For security, the system counts incorrect password attempts. After three incorrect attempts, a security alarm is activated and the system temporarily locks the user out.

The project also includes a password-changing feature, secret-code authentication, buzzer feedback, LEDs, and a visual alarm sequence.

---

## Features

* Password-based door authentication
* 4×4 matrix keypad
* Servo-controlled door lock
* Automatic door locking after 5 seconds
* Password entry timeout
* Incorrect password attempt counter
* Security alarm after 3 incorrect attempts
* Temporary user lockout
* Buzzer feedback for key presses
* Alarm buzzer/siren
* Multiple alarm LEDs
* Alarm warning light
* Password changing feature
* Secret code for password reset/change
* Password confirmation using `#`
* Password-change cancellation using `C`
* Serial Monitor status messages

---

## Hardware Used

| Component    | Purpose                 |
| ------------ | ----------------------- |
| Arduino Uno  | Main controller         |
| 4×4 Keypad   | Password input          |
| Servo Motor  | Locks/unlocks the door  |
| Buzzer       | Key and alarm sound     |
| LED          | Door unlocked indicator |
| Alarm Light  | Security warning        |
| LED 1        | Alarm sequence          |
| LED 2        | Alarm sequence          |
| LED 3        | Alarm sequence          |
| Jumper Wires | Circuit connections     |
| Breadboard   | Circuit assembly        |

---

## Software Used

The project is programmed using **Arduino C/C++**.

### Required Library

The following libraries are required:

* Keypad
* Servo

The project does not require an external display because status messages are currently shown through the **Arduino Serial Monitor**.

---

# Pin Connections

## 4×4 Keypad

The keypad contains four row pins and four column pins.

### Row Connections

| Keypad Pin | Arduino Uno |
| ---------- | ----------- |
| Row 1      | D2          |
| Row 2      | D3          |
| Row 3      | D4          |
| Row 4      | D5          |

### Column Connections

| Keypad Pin | Arduino Uno |
| ---------- | ----------- |
| Column 1   | D6          |
| Column 2   | D7          |
| Column 3   | D8          |
| Column 4   | D9          |

The code defines these connections as:

```cpp
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
```

---

## Servo Motor

| Servo Pin | Arduino Uno |
| --------- | ----------- |
| Signal    | D10         |
| VCC       | 5V          |
| GND       | GND         |

The servo is controlled using:

```cpp
myServo.attach(10);
```

The current servo positions are:

```text
0°  → Door Locked
90° → Door Unlocked
```

> The actual servo angle may need to be adjusted according to the physical door-lock mechanism.

---

## Buzzer

| Buzzer Pin        | Arduino Uno |
| ----------------- | ----------- |
| Positive / Signal | D12         |
| Negative          | GND         |

The code defines:

```cpp
const int buzzerPin = 12;
```

The buzzer provides:

* Key press feedback
* Wrong password feedback
* Password timeout warning
* Security alarm sound

---

## Door Status LED

| LED              | Arduino Uno          |
| ---------------- | -------------------- |
| LED Signal/Anode | D11                  |
| Cathode          | GND through resistor |

The LED turns ON when the door is unlocked.

The code defines:

```cpp
const int ledPin = 11;
```

---

## Alarm Light

| Component          | Arduino Uno |
| ------------------ | ----------- |
| Alarm Light Signal | D13         |
| GND                | GND         |

The code defines:

```cpp
const int alarmLightPin = 13;
```

This light is activated during the security alarm.

---

## Alarm LEDs

The project uses three additional LEDs connected to the analog pins.

The analog pins are being used as **digital output pins**.

| Alarm LED | Arduino Pin |
| --------- | ----------- |
| LED A0    | A0          |
| LED A1    | A1          |
| LED A2    | A2          |

The code defines:

```cpp
const int ledA0 = A0;
const int ledA1 = A1;
const int ledA2 = A2;
```

These LEDs are activated one after another during the alarm sequence.

### Important

Each LED should use an appropriate **current-limiting resistor**, typically around 220Ω–330Ω.

---

# Complete Pin Summary

| Component   | Pin / Function | Arduino Pin |
| ----------- | -------------- | ----------- |
| Keypad      | Row 1          | D2          |
| Keypad      | Row 2          | D3          |
| Keypad      | Row 3          | D4          |
| Keypad      | Row 4          | D5          |
| Keypad      | Column 1       | D6          |
| Keypad      | Column 2       | D7          |
| Keypad      | Column 3       | D8          |
| Keypad      | Column 4       | D9          |
| Servo       | Signal         | D10         |
| Door LED    | Signal         | D11         |
| Buzzer      | Signal         | D12         |
| Alarm Light | Signal         | D13         |
| Alarm LED 1 | Signal         | A0          |
| Alarm LED 2 | Signal         | A1          |
| Alarm LED 3 | Signal         | A2          |

---

# How the System Works

The system starts with the door locked.

The servo is initially positioned at:

```cpp
myServo.write(0);
```

The Serial Monitor displays:

```text
Enter password:
```

The user enters the password using the keypad.

---

# Default Password

The default password in the current code is:

```text
15151
```

The password is stored in:

```cpp
String password = "15151";
```

The password contains **5 digits**.

---

# Entering the Password

Each time the user presses a keypad button:

1. The key is detected.
2. A short buzzer sound is produced.
3. The key is added to the entered password.
4. Once 5 characters are entered, the system checks the password.

### Correct Password

If the entered password matches the stored password:

```text
Access Granted
```

The door lock is toggled.

If the door was locked:

```text
Servo → 90°
LED → ON
Door → Unlocked
```

The door remains unlocked for approximately **5 seconds**.

After 5 seconds:

```text
Servo → 0°
LED → OFF
Door → Locked
```

---

# Automatic Door Lock

The project automatically locks the door after 5 seconds.

The duration is defined by:

```cpp
const unsigned long doorUnlockDuration = 5000;
```

Therefore:

```text
5000 milliseconds = 5 seconds
```

This means the user does not need to manually lock the door after opening it.

---

# Wrong Password Protection

Every time an incorrect 5-digit password is entered, the incorrect-attempt counter increases.

```cpp
incorrectAttempts++;
```

The maximum allowed attempts are:

```cpp
const int maxIncorrectAttempts = 3;
```

The system therefore allows three incorrect attempts before activating the alarm.

---

# Security Alarm

After three incorrect password attempts:

```text
Incorrect Attempt 1
        ↓
Incorrect Attempt 2
        ↓
Incorrect Attempt 3
        ↓
Security Alarm
```

The system activates:

* Buzzer
* Alarm light
* Alarm LEDs

The Serial Monitor displays:

```text
Alarm triggered!
```

The alarm LEDs are activated in sequence:

```text
LED A0
  ↓
LED A1
  ↓
LED A2
```

This creates a visual alarm effect.

---

# Lockout System

After the alarm is triggered, the user is temporarily locked out.

The lockout duration is defined as:

```cpp
const unsigned long lockoutDuration = 4000;
```

Therefore, the current code uses a **4-second lockout**.

During the lockout period, keypad input is ignored.

After the lockout period finishes, the system allows password entry again.

> The Serial Monitor currently says "locked out for 1 minute," but the actual code uses `4000` milliseconds, which is only 4 seconds. If you want a 1-minute lockout, change the value to `60000`.

---

# Password Entry Timeout

The project also has a password-entry timeout.

The timeout is:

```cpp
const unsigned long passwordEntryTimeout = 2000;
```

This means the user has approximately **2 seconds between key presses**.

If the user starts entering a password but stops pressing keys for more than 2 seconds:

1. The entered password is cleared.
2. The buzzer sounds twice.
3. The system asks for the password again.

This prevents an incomplete password from remaining in memory indefinitely.

---

# Password Changing Feature

The project supports changing the current password.

The user can press:

```text
B
```

to start the password-change process.

The system then enters password-changing mode.

A new password must contain exactly:

```text
5 digits
```

After entering the new 5-digit password, the user presses:

```text
#
```

to save it.

The new password then replaces the old password.

---

# Secret Code

The project also contains a secret code:

```text
1122
```

The code is defined as:

```cpp
const String secretCode = "1122";
```

The user can press:

```text
D
```

to enter secret-code mode.

If the correct secret code is entered:

```text
1122
```

the system:

1. Resets incorrect attempts.
2. Clears the lockout state.
3. Starts password-changing mode.
4. Allows the user to enter a new password.

---

# Password Change Cancellation

The password-changing process can be cancelled using:

```text
C
```

When `C` is pressed:

* Password change mode is cancelled.
* The new password is cleared.
* The system returns to normal password entry.

---

# Password Change Flow

```text
Press B
   |
   v
Password Change Mode
   |
   v
Enter New Password
   |
   v
Enter 5 Digits
   |
   v
Press #
   |
   v
New Password Saved
   |
   v
System Returns to Normal Mode
```

---

# Secret Code Flow

```text
Press D
   |
   v
Enter Secret Code
   |
   v
Is Code 1122?
   |
   +----------+
   |          |
  Yes         No
   |          |
   v          v
Reset       Show Error
Attempts
   |
   v
Change Password
```

---

# Alarm Flow

```text
System Ready
     |
     v
Enter Password
     |
     v
Wrong Password
     |
     v
Attempt Counter +1
     |
     v
3 Wrong Attempts?
     |
    Yes
     |
     v
Alarm Triggered
     |
     +----------------+
     |                |
     v                v
Buzzer           Alarm LEDs
     |                |
     +-------+--------+
             |
             v
       Alarm Sequence
             |
             v
        Stop Alarm
             |
             v
       System Ready
```

---

# Keypad Functions

| Key   | Function               |
| ----- | ---------------------- |
| `0-9` | Enter password/code    |
| `B`   | Start password change  |
| `C`   | Cancel password change |
| `D`   | Enter secret-code mode |
| `#`   | Save new password      |

The `A` key is currently not assigned a special function.

The `*` key is also currently not assigned a special function.

---

# Serial Monitor

The project uses the Arduino Serial Monitor for system messages.

The baud rate is:

```cpp
Serial.begin(9600);
```

Set the Serial Monitor to:

```text
9600 baud
```

Example messages include:

```text
Initializing...
Setup Complete
Enter password:
Unlocking door...
Locking door...
Wrong password!
Alarm triggered!
Activating siren...
Stopping alarm...
Alarm stopped.
```

---

# Installation

## 1. Install Arduino IDE

Install the Arduino IDE on your computer.

## 2. Install Required Libraries

Open:

```text
Arduino IDE
→ Library Manager
```

Search for and install:

```text
Keypad
Servo
```

## 3. Connect the Components

Connect the keypad, servo, buzzer, LEDs, and alarm light according to the pin-connection tables in this README.

## 4. Open the Arduino File

Open the project `.ino` file in Arduino IDE.

## 5. Select Board

Select:

```text
Arduino Uno
```

## 6. Select COM Port

Select the COM port where your Arduino Uno is connected.

## 7. Upload

Upload the program to the Arduino.

## 8. Open Serial Monitor

Open the Serial Monitor and set the baud rate to:

```text
9600
```

---

# Project Structure

A recommended GitHub repository structure is:

```text
Arduino-Keypad-Smart-Door-Lock/
│
├── Arduino-Keypad-Smart-Door-Lock.ino
├── README.md
│
├── circuit/
│   └── circuit-diagram.png
│
├── images/
│   ├── project.jpg
│   ├── keypad.jpg
│   └── circuit.jpg
│
└── documentation/
    └── project-report.pdf
```

---

# Important Configuration

## Change Default Password

Find:

```cpp
String password = "15151";
```

Change it to your desired 5-digit password.

For example:

```cpp
String password = "12345";
```

---

## Change Secret Code

Find:

```cpp
const String secretCode = "1122";
```

Change it to your desired secret code.

---

## Change Door Unlock Time

Find:

```cpp
const unsigned long doorUnlockDuration = 5000;
```

The value is in milliseconds.

Examples:

```text
5000  → 5 seconds
10000 → 10 seconds
30000 → 30 seconds
```

---

## Change Lockout Duration

Find:

```cpp
const unsigned long lockoutDuration = 4000;
```

For a 1-minute lockout:

```cpp
const unsigned long lockoutDuration = 60000;
```

---

## Change Maximum Incorrect Attempts

Find:

```cpp
const int maxIncorrectAttempts = 3;
```

For example, to allow 5 attempts:

```cpp
const int maxIncorrectAttempts = 5;
```

---

# Important Note About the Current Code

The Serial Monitor contains this message:

```text
You are locked out for 1 minute.
```

However, the actual value is:

```cpp
const unsigned long lockoutDuration = 4000;
```

`4000` milliseconds equals **4 seconds**, not 1 minute.

If the intended lockout is one minute, use:

```cpp
const unsigned long lockoutDuration = 60000;
```

---

# Future Improvements

Possible improvements for future versions include:

* Add an OLED/LCD display
* Hide password characters while typing
* Add `*` as a clear/backspace key
* Add an admin mode
* Store the password in EEPROM
* Keep the password after Arduino power loss
* Add RFID authentication
* Add fingerprint authentication
* Add Wi-Fi notifications
* Add mobile-app control
* Store access logs
* Add a door-position sensor
* Add battery backup
* Add multiple user passwords
* Add different access levels for users
* Increase lockout time after repeated attacks

---

# Project Flow Summary

```text
Power ON
   |
   v
Initialize System
   |
   v
Door Locked
   |
   v
Enter Password
   |
   v
Check Password
   |
   +------------------+
   |                  |
 Correct            Incorrect
   |                  |
   v                  v
Unlock Door       Attempts +1
   |                  |
   v                  v
5 Seconds       3 Attempts?
   |                  |
   v                 Yes
Auto Lock             |
   |                  v
   |                Alarm
   |                  |
   |                  v
   |               Lockout
   |                  |
   +--------<---------+
            |
            v
       System Ready
```


# Password and Code Management

The system supports changing the door password in two different ways.

There are currently **two methods** available:

1. Direct password change using `B`
2. Secret-code authentication using `D`

The new password must always contain **exactly 5 digits**.

---

# Method 1 — Change Password Using `B`

The first method allows the user to start password-changing mode directly by pressing:

```text
B
```

### Step-by-Step Process

### Step 1: System Ready

The door is normally locked and the system is waiting for a password.

```text
Enter password:
```

### Step 2: Press `B`

Press:

```text
B
```

The system enters password-changing mode.

The Serial Monitor displays:

```text
Enter current password:
```

> In the current code, although the message says "Enter current password", the program does not actually verify the old/current password before allowing the new password to be entered.

### Step 3: Enter New Password

Enter a new password using the number keys.

For example:

```text
25896
```

The password must contain exactly **5 digits**.

While entering the password, the buzzer gives feedback for each key.

### Step 4: Save the New Password

After entering all 5 digits, press:

```text
#
```

The system checks whether the new password contains exactly 5 digits.

If valid:

```text
New password set.
```

The new password replaces the old password.

For example:

```text
Old Password:
15151

New Password:
25896
```

After saving, the system returns to normal password-entry mode.

### Complete Sequence

```text
System Ready
     ↓
Press B
     ↓
Password Change Mode
     ↓
Enter 5-digit New Password
     ↓
Press #
     ↓
New Password Saved
     ↓
3 LED Blink Confirmation
     ↓
System Ready
```

---

# Method 2 — Change Password Using Secret Code

The second method uses a secret code before allowing the password to be changed.

The current secret code is:

```text
1122
```

The secret code is stored in the program as:

```cpp
const String secretCode = "1122";
```

### Step 1: System Ready

The system is waiting for the normal password.

```text
Enter password:
```

### Step 2: Press `D`

Press:

```text
D
```

This activates secret-code mode.

The Serial Monitor displays:

```text
Enter secret code:
```

### Step 3: Enter Secret Code

Enter:

```text
1122
```

The system compares the entered code with the stored secret code.

### If the Secret Code Is Correct

The system:

1. Resets the incorrect-attempt counter.
2. Removes the lockout state.
3. Gives a three-light blink indication.
4. Starts password-changing mode.

The Serial Monitor displays:

```text
Secret code correct. Resetting incorrect attempts.
Enter new password:
```

### Step 4: Enter New Password

Enter a new 5-digit password.

Example:

```text
73921
```

### Step 5: Press `#`

After entering the 5 digits, press:

```text
#
```

The system saves the new password.

The Serial Monitor displays:

```text
New password set.
Password change successful. Returning to normal mode.
```

The system then returns to normal operation.

### Complete Sequence

```text
System Ready
     ↓
Press D
     ↓
Enter Secret Code
     ↓
Enter 1122
     ↓
Secret Code Correct
     ↓
Enter New 5-Digit Password
     ↓
Press #
     ↓
Password Saved
     ↓
3 LED Blink Confirmation
     ↓
System Ready
```

---

# What Happens If the Secret Code Is Wrong?

If the user enters an incorrect secret code, the password is not changed.

For example:

```text
D
1234
```

The system displays:

```text
Incorrect secret code.
```

The buzzer sounds twice.

The system then returns to normal password-entry mode.

```text
Secret Code
     ↓
Wrong
     ↓
Warning
     ↓
Return to Normal Mode
```

---

# Cancel Password Change

Password changing can be cancelled using:

```text
C
```

If `C` is pressed while entering a new password:

1. Password-changing mode is cancelled.
2. The new password entered so far is deleted.
3. The old password remains unchanged.
4. The system returns to normal mode.

### Example

```text
Press B
     ↓
Enter New Password
     ↓
2 5 8
     ↓
Press C
     ↓
Password Change Cancelled
     ↓
Old Password Still Active
```

The incomplete password is **not saved**.

---

# Save Key

The `#` key is used to save a new password.

The user must first enter exactly 5 digits.

Example:

```text
2 5 8 9 6 #
```

The system saves:

```text
25896
```

If fewer than 5 digits are entered and `#` is pressed, the system displays:

```text
New password must be 5 digits.
```

The new password is not saved.

---

# New Password Requirements

The current program requires:

* Exactly 5 characters
* Digits are normally used as the password
* `#` is used to save
* `C` can cancel password changing

Examples of valid 5-digit passwords:

```text
12345
25896
73921
98765
```

---

# Changing the Secret Code

The **secret code itself is not currently changeable from the keypad**.

It is stored directly inside the Arduino program:

```cpp
const String secretCode = "1122";
```

To change the secret code:

### Step 1

Open the Arduino `.ino` file.

### Step 2

Find:

```cpp
const String secretCode = "1122";
```

### Step 3

Replace `1122` with your new secret code.

For example:

```cpp
const String secretCode = "4567";
```

### Step 4

Save the Arduino code.

### Step 5

Connect the Arduino Uno to the computer.

### Step 6

Upload the updated program.

The new secret code will then be:

```text
4567
```

---

# Changing the Default Password in Code

The default password is also stored directly inside the Arduino program.

The current password is:

```cpp
String password = "15151";
```

To change it manually:

### Step 1

Open the `.ino` file.

### Step 2

Find:

```cpp
String password = "15151";
```

### Step 3

Replace it with another 5-digit password.

For example:

```cpp
String password = "73921";
```

### Step 4

Save the code.

### Step 5

Upload the program to Arduino.

The new default password will be:

```text
73921
```

> This method changes the password stored in the program. However, because the current project does not save the password in EEPROM, a password changed through the keypad will be lost if the Arduino is reset or powered off.

---

# Password Change Methods Summary

| Method             | Key         | Authentication       | Can Change Password? |
| ------------------ | ----------- | -------------------- | -------------------- |
| Direct Method      | `B`         | No secret-code check | Yes                  |
| Secret-Code Method | `D`         | Secret code `1122`   | Yes                  |
| Arduino Code       | Edit `.ino` | Computer/code access | Yes                  |
| Cancel             | `C`         | N/A                  | Cancels change       |
| Save               | `#`         | Requires 5 digits    | Saves new password   |

---

# Complete Password Management Flow

```text
                    SYSTEM READY
                         |
                         v
                  Enter Password
                         |
             +-----------+-----------+
             |                       |
             B                       D
             |                       |
             v                       v
       Change Mode             Secret Code Mode
             |                       |
             |                  Enter 1122
             |                       |
             |                +------+------+
             |                |             |
             |              Correct       Wrong
             |                |             |
             |                v             v
             |          Change Mode      Cancel
             |                |
             +--------+-------+
                      |
                      v
              Enter New Password
                      |
                 5 Digits?
                 /       \
               No         Yes
               |           |
               v           v
             Error       Press #
                           |
                           v
                    Password Saved
                           |
                           v
                     3 LED Blinks
                           |
                           v
                      System Ready
```

---

# Important Security Note

In the current version of the project, pressing:

```text
B
```

starts password-changing mode **without checking the current password or secret code**.

Therefore, if anyone has physical access to the keypad, they can potentially change the password using the `B` method.

For a more secure version, the recommended logic would be:

```text
Press B
   ↓
Enter Current Password
   ↓
Check Current Password
   ↓
Correct?
   ├── No → Reject
   └── Yes
        ↓
   Enter New Password
        ↓
   Confirm New Password
        ↓
   Save Password
```

An even more secure version would require:

```text
Current Password
       +
Admin/Secret Code
       ↓
Change Password
```

---

# Password Persistence

The current code stores the password in a normal RAM variable:

```cpp
String password = "15151";
```

This means that a password changed during runtime is not permanently stored.

For example:

```text
Original Password
15151
     ↓
Change through keypad
     ↓
25896
     ↓
Arduino running
     ↓
25896 works
```

But after resetting/powering off the Arduino:

```text
Arduino Restart
     ↓
Program starts again
     ↓
password = "15151"
```

The password returns to the value written in the source code.

### Future Improvement

The project can be upgraded to use **EEPROM** so that a password changed through the keypad remains saved even after:

* Arduino reset
* Power off
* Power on

The improved flow would be:

```text
New Password
     ↓
Save to EEPROM
     ↓
Power OFF
     ↓
Power ON
     ↓
Read Password from EEPROM
     ↓
New Password Still Works
```

This would make the password-management system much more practical for a real smart-lock application.

---

# Author

**Aliyan Hussain**

Arduino / Embedded Systems Project

---

# License

This project is created for educational and learning purposes.
