#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define Password_Lenght 7 // Give enough room for six chars + NULL char
#define TagID 2 //Number of Tags

byte Granted=12, Denied=13; //Signals OUTPUT for RFID
//----------Variable for KeypadPassword----------//
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
byte rowPins[ROWS] = {10,9,8,7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,5,4,3}; //connect to the column pinouts of the keypad
//Keypad (Left -> Right): 10 9 8 7 6 5 4 3

//----------Variable for FRID----------//
long AuthorizedTagID[TagID] ={11701234};//Authorized TagID
int flag=0,RX_Flag=0;//flag: start storing data, RX_flag: reading process finished
char Code[14];
long Num=0; //the requesting TagID
//--------------------//
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad 

void setup()
{
  pinMode(Granted,OUTPUT);
  pinMode(Denied,OUTPUT);
  Serial.begin(9600);
  lcd.init();// initialize the lcd 
  lcd.backlight();
  Serial.println("This is used to check and verify the RFID Tag. RFID Tag: 11701234");
  Serial.println("--------------------");
}

void Read_ID() //The read Tag is Code[]
{
    int i;
    for(i=0;(Serial.available()>0);i++)
    {
      char temp=Serial.read();
      delay(4);             //DO NOT delete this delay
      if(temp==2)  //recieve the start bit
      {
        flag=1;i=0;
      }
      if(flag==1)//detect the start bit and recieve data
      {
        if(temp==3)//detect the end bit
        {
          flag=0;  //zero clearing
          RX_Flag=1;  //complete the reading process, set RX_Flag
          break;  //break out of for loop
        }
        Code[i]=temp;
      }
    }    
}

void AccessGranted()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  incorrect_count=0;
  //----Serial Monitor----//
  Serial.println("Access Granted");
  Serial.println("--------------------");
  
  digitalWrite(Granted,HIGH);
  digitalWrite(Denied,LOW);
}

void AccessDenied()
{
  incorrect_count++;
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Denied");
  //----Serial Monitor----//
  Serial.println("Access Denied");
  Serial.println("--------------------");
  
  digitalWrite(Denied,HIGH);
  digitalWrite(Granted,LOW);
}

void clearData()
{
  while(data_count !=0)
  {   // This can be used for any array size, 
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

void lockGate()
{
  digitalWrite(Granted,LOW);
  digitalWrite(Denied,LOW);
  customKey=0;
  clearData();
  RX_Flag=0;
  Num=0;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Gate locked");
  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(1,0);
  lcd.print("Enter Password");

  customKey = customKeypad.getKey();
  if (customKey=='#') lockGate();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count+5, 1); // move cursor to show each new char
    lcd.print("*"); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if(data_count == Password_Lenght-1) // if the array index is equal to the number of expected chars, compare data to master
  {
    if(!strcmp(Data, Master)) //  Password is correct
    {
      AccessGranted();
    }
    else
    {
      AccessDenied();
    }
    delay(2000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    lcd.clear();
    clearData();
  }

  //----------Program section for the RFID----------//
  int i;
  Read_ID();
  if(RX_Flag==1)
  {
    Serial.print("Decrypted ID Code: ");
    for(i=5;i<11;i++) //Convert char to integer    //Refer to ASCII chart //Only take character from 5->11 of the Code[]
    {
      Serial.print(Code[i]);
      Num<<=4;
      if(Code[i]>64)  Num+=((Code[i])-55); //('A'=65)
      else Num+=((Code[i])-48); //char 0:9 <-> int 48:57
    }
    Serial.println("");
    Serial.print("Requesting ID: ");  Serial.println(Num);
    if((Num==AuthorizedTagID[0])||(Num==AuthorizedTagID[1]))  AccessGranted();
    else  AccessDenied();
    delay(2000);// added 1 second delay to make sure the password is completely shown on screen before it gets cleared.
    lcd.clear();
    clearData();
  }
  while(Serial.read()>0){;}
  RX_Flag=0;
  Num=0;
  //---Overflowed with error---//
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

