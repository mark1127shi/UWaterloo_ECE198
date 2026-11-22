# ECE 198 Arduino Joystick

An Arduino project that uses a joystick to navigate an LCD menu.

## Hardware

- Arduino UNO R4
- Joystick module
- Two-line LCD
- Jumper wires

## Setup

1. Connect the Arduino to your computer.
2. Open the sketch in Arduino IDE.
3. Select the board and port, then click **Upload**.

## Joystick Wiring

| Joystick | Arduino |
| --- | --- |
| VRX | A0 |
| VRY | A1 |
| SW | A2 |

## Code Example

Use `analogRead()` to read the joystick position.

```cpp
int x = analogRead(A0);
Serial.println(x);
```

[Arduino documentation](https://docs.arduino.cc/)
