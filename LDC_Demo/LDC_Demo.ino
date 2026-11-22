//Demo of Liquid Crystal Display(LCD)

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

void setup(){
  int status = lcd.begin(16, 2);
  if (status != 0){
    hd44780::fatalError(status);
  }

  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Arduino booting");
  
  lcd.setCursor(0,1);
  lcd.print("------------------");

  for (int column = 0; column < 16; ++column){
    delay(120);
    lcd.setCursor(column,1);
    lcd.print('#');
  }

  delay(400);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Hello, World");
  
  lcd.setCursor(0,1);
  lcd.print("Kerry is FURRY ");


}

void loop(){
  const char message[] = "                Kerry is FURRY!!!!!!!";
  const int length = sizeof(message) -1;
  static int offset = 0;

  lcd.setCursor(0,1);

  for (int column = 0; column < 16; ++column){
    int index = (offset + column) %length;
    lcd.print(message[index]);
  }

  offset = (offset + 1) % length;
  delay(300);
}







