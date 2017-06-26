/*
  Scrolling text demonstration sketch for Freetronics DMD.
 See http://www.freetronics.com/dmd for resources and a getting started guide.
 Note that the DMD library uses the SPI port for the fastest, low overhead writing to the
 display. Keep an eye on conflicts if there are any other devices running from the same
 SPI port, and that the chip select on those devices is correctly set to be inactive
 when the DMD is being written to.  
 */

// you always need the code from here...........
#include <DMD.h> // for DMD
#include <SPI.h> // SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <TimerOne.h> 
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#define DISPLAYS_ACROSS 4 // change to 2 for two screens, etc. 
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN); // creates instance of DMD to refer to in sketch

#define max_char 100
char message[max_char];
char message_1[max_char] = "WELCOME TO EE 🙂";
char r_char;
byte index = 0;
int i;
void ScanDMD() // necessary interrupt handler for refresh scanning of DMD
{ 
  dmd.scanDisplayBySPI();
  dmd.setBrightness(254);
 
}

void setup()
{
   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()  
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   Serial.begin(9600);
}


void loop()
{
  Serial.flush();
  if(Serial.available()){
    for(i=0; i<99; i++){
    message[i] = '\0';
  }
  index=0;
}
   while(Serial.available() > 0){
    if(index < (max_char-1)) {
      r_char = Serial.read();
      message[index] = r_char;
      index++;
      message[index] = '\0';
  }
}


Serial.println(message);
   dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16);

 
   dmd.drawMarquee(message_1, max_char,(32*DISPLAYS_ACROSS)-1 ,0);
   long start=millis();
   long timer=start;
   boolean ret=false;
   while(!ret){
   
   if ((timer+30) < millis()) {
     ret=dmd.stepMarquee(-1,0);
     timer=millis();
   }
  }
}
