#define TagID 2 //Number of Tags
int Granted=12, Denied=13; //LEDs for RFID
long AuthorizedTagID[TagID] ={5902508,7927290};//Authorized TagID
int flag=0,RX_Flag=0;//flag: start storing data, RX_flag: reading process finished
char Code[14];
long Num=0; //the requesting TagID

void setup() 
{
  pinMode(Granted,OUTPUT);
  pinMode(Denied,OUTPUT);
  Serial.begin(9600);
  Serial.println("This is a test for access control system. Password: ");
  Serial.println("--------------------");
}

void Read_ID(void) //The read Tag is Code[]
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

void AccessGranted(void)
{
  Serial.println("Access Granted");
  Serial.println("--------------------");
  digitalWrite(12,HIGH);
  delay(2000);
  digitalWrite(12,LOW);
}

void AccessDenied(void)
{
  Serial.println("Access Denied");
  Serial.println("--------------------");
  digitalWrite(13,HIGH);
  delay(2000);
  digitalWrite(13,LOW);
}

void loop()
{
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
  }
  while(Serial.read()>0){;}
  RX_Flag=0;
  Num=0;
}
