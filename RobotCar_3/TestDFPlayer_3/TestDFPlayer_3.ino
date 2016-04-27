//**************************** RobotCar_3 ********************************
// Only files with the .ino extension are scanned for include files.     *
// This is by design, for simplicity for one thing and so that a         *
// library can't include another library that includes another library   *
//and so on.                                                             *
//***********************************************************************/
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "MyDFPlayer.h"

int TXspilari_=4;  //Sendir skipanir til mp3 spilarans
int RXspilari_=2;  //Tekur við skipunum frá mp3 spilara
int lagNr;
unsigned long time;

//************************** uppsetning á forritinu *****************
void setup() 
{ 
    pinMode(TXspilari_,OUTPUT); 
    mySerial.begin(9600);
    delay(100);
    mp3_set_volume(30);
    delay(100); 
     mp3_play_track(17);  // 17. Bíll í gang (gamli bíllinn)
     delay(10000); 
     mp3_play_track(1);     // 1. Riding along in my automobile 
     lagNr=2;
     time=millis();         //Setur time breytuna á tíma liðinn frá starti
}//End of setup *********

//*************************** Keyrslulykkjan **********************
void loop()
{
    if((millis()-time)>= 30000) //Spila lögin í 30 sek
    {
       if(lagNr ==17)
          lagNr=1;
       mp3_play_track(lagNr++); 
           time=millis();                               
    }
}//End of loop *********************


