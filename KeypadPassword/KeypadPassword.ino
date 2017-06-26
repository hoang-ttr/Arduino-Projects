/* 
 || @file Keypad Password
 || @version 1.0
 || @author Andrew Mascolo
 || @Editor Hoang Thien Tran
 || @date May 7, 2013
 ||
 || @description
 || Simple use of keypad, password and LCD
*/
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define Password_Lenght 7 // Give enough room for six chars + NULL char

LiquidCrystal_I2C lcd(0x27,16,2);
char Master[Password_Lenght] = "123456"; 
char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
byte data_count=0, master_count=0, incorrect_count=0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {
  9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  5,4,3,2}; //connect to the column pinouts of the keypad
//Keypad (Left -> Right): 9 8 7 6 5 4 3 2

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad 

void setup()
{
  lcd.init();// initialize the lcd 
  lcd.backlight();
}

void loop()
{
  lcd.setCursor(1,0);
  lcd.print("Enter Password");

  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count+5, 1); // move cursor to show each new char
    lcd.print("*"); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if(data_count == Password_Lenght-1) // if the array index is equal to the number of expected chars, compare data to master
  {
    lcd.clear();
    if(!strcmp(Data, Master)) //  Password is incorrect
    {
      incorrect_count=0;
      lcd.setCursor(5, 0);
      lcd.print("Correct");
    }
    else
    {
      lcd.setCursor(4, 0);
      lcd.print("Incorrect");
      incorrect_count++;
    }
    delay(2000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    lcd.clear();
    clearData();
  }
  if(incorrect_count==3)
  {
    incorrect_count=0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please wait 10s");
    lcd.setCursor(2,1);
    lcd.print("to try again");
    delay(10000);
    lcd.clear();
  }
}

void clearData()
{
  while(data_count !=0)
  {   // This can be used for any array size, 
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}
