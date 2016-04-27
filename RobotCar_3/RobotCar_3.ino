//**************************** RobotCar_3 ********************************
// Only files with the .ino extension are scanned for include files.     *
// This is by design, for simplicity for one thing and so that a         *
// library can't include another library that includes another library   *
// and so on.                                                            *
// Athugid ad setja snuningsAtt i RobotFunctions.h a -1 ef servomotor    *
// er orvhentur     test     aron                                                 *
//***********************************************************************/
#include "Arduino.h"
#include <TimerOne.h>
#include "RobotFunctions.h"
#include <SoftwareSerial.h>
#include "MyDFPlayer.h"

//************************** uppsetning á forritinu *****************
void setup() 
{ 
    pinMode(motorVpwm_,OUTPUT);
    pinMode(motorVgir_,OUTPUT);
    pinMode(motorHpwm_,OUTPUT);
    pinMode(motorHgir_,OUTPUT);
  
    pinMode(servo_,OUTPUT);
  
    pinMode(sonarTrigger_,OUTPUT);
    pinMode(sonarEcho_,INPUT_PULLUP); //Allir inngangar verda að hafa pullup vidnam 
                                      //her notum við internal pullup (20kohm) orgjorvans 
    pinMode(TXspilari_,OUTPUT);
    pinMode(RXspilari_,INPUT_PULLUP); //Notum aftur Internal pullup vidnam
    
    period=1000;    //1000 interrupt gera 1000*20us=20ms sem er umferðartími servosins
    pulsBreidd=75;  //Nauðsynlegt byrjunargildi til að skemma ekki servomótorinn
    digitalWrite(servo_,HIGH);
    
    Timer1.initialize(20); // set a timer of length 20 microseconds 
    Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  
    Serial.begin(9600); 
    //******************** Setja upp MDFPlayer *************************
    mySerial.begin(9600);
    delay(100);
    //mp3_set_volume(25);
    mp3_set_volume(20);
    delay(100); 
     mp3_play_track(17);  // 17. Bíll í gang (gamli bíllinn)
     delay(5000); 
     mp3_play_track(1);     // 1. Riding along in my automobile 
     lagNr=2;
     time=millis();         //Setur time breytuna á tíma liðinn frá starti
     
}//End of setup *********

//*************************** Keyrslulykkjan **********************
void loop()
{
    pulsBreidd=reiknaPulsBreidd(-10);
    delay(100);
    startCar();
    delay(100); 
    
    if((millis()-time)>= 30000) //Spila lögin í 30 sek
    {
       if(lagNr ==17)
          lagNr=1;
       mp3_play_track(lagNr++); 
           time=millis();                               
    }
    
    
    while(lengd() < 50)    
    {   
        if(lengd()<30)
        {
           backCar();  
           delay(600);  //Bakka í 600ms
        }
        stopCar();
        pulsBreidd=reiknaPulsBreidd(-80);
        delay(1000); //Servo motorar eru misjafnlega hraðvirkir
        lengdV=lengd();
        delay(200);
        pulsBreidd=reiknaPulsBreidd(+80);
        delay(1000); //Servo motorar eru misjafnlega hraðvirkir
        lengdH=lengd();
        delay(200);
        pulsBreidd=reiknaPulsBreidd(-10);  //Stefna beint fram
        delay(400);        
        if(lengdV > lengdH)
        {          
          while(lengd()<100)
          {
             driveLeft();  
             delay(50);
             breakCar();
             delay(20);
          }
        }
        else
        {
           while(lengd() <100)
           {
               driveRight();
               delay(50);
               breakCar();
               delay(20);
           }
        }          
   }
}//End of loop *********************

