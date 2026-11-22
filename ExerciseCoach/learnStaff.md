## This is the place for new things explained in detail
# I2c
I2c is a way for electronic components to communicate using two signal wires.
# ADC
**analog-to-digital converter**
For example, on the Arduino, we can use `analogRead()` to read the X-axis output of the ADXL335 accelerometer connected to `A0`.
```cpp
int xRaw = analogRead(A0);// something like this
```