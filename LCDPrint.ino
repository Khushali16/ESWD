//LCD I2C library for Arduino to print Counter loop

#include<LCD_I2C.h>
 LCD_I2C lcd(0x27);

 void setup()
 {
  lcd.begin();
  lcd.print("");
  lcd.print("Counter: ");
 }

 void loop()
 {
  for(int i = 0; i<100000;i++)
  {
    lcd.setCursor(7,0);
    lcd.print(i);
  }
 }


 //print hello world on LCD

 #include<LCD.I2C.h>
 LCD_I2C lcd(0x27);

 void setup()
 {
  lcd.begin();
  lcd.print("Hello World")
  lcd.backlight();
 }

 void loop()
 {

 }