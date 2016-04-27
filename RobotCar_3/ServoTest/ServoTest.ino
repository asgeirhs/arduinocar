//******************** MySonar_og_3Servo ************************
#include <TimerOne.h>

int servo_=4;     //Geymir númer servo tengisV, hér 4
int servo;        //Geymir stöðuna sem skrifuð var á servo_

int cnt_SERVO;    //Geymir talningu fyrir SERVO interrupt fallsins
int pulsBreidd=75; //Geymir púlsbreidd servo
int period=0;
  
//Hér er servopúlsinn gefinn í us
int reiknaNigrip(int servoPuls) //púlsbreiddarsviðið er 600us-1500us-2400us og 2400us/20us=0,05*2400 
{                               //t.d. þá er 0,05*600us=30 interrupt og 0,05*2400us=120 interrupt
   int temp;                    //period=20ms=20000us  0,05*20000=1000 interrupt
   temp=servoPuls*5/10;
   temp=(temp+5)/10; //að leggja 5 við fyrir deilinguna afrúnnar útkomuna
   return temp;
}
 
 
void setup() 
{ 
  cnt_SERVO=0;
  period=1000;
  pinMode(servo_,OUTPUT);
  servo=1;
  digitalWrite(servo_,HIGH);
  
  Timer1.initialize(20); // set a timer of length 20 microseconds 
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  Serial.begin(9600);
}
 
void loop()
{
   pulsBreidd=reiknaNigrip(600);
   delay(1000);
   pulsBreidd=reiknaNigrip(1500);
   delay(1000);
   pulsBreidd=reiknaNigrip(2400);
   delay(1000);
}
 
/// --------------------------         //digitalWrite(trigger,LOW); digitalRead(echo
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()                          //MUNA digitalWrite og digital read_____________________________--
{
    cnt_SERVO++;          //cnt_SERVO Hækkar um 1 við hvert overflow
    if(servo==1 && cnt_SERVO>=pulsBreidd)
    {
       servo=0;
       digitalWrite(servo_,LOW);
    }
    if(cnt_SERVO>=period)
    {
        servo=1;
        digitalWrite(servo_,HIGH);
        cnt_SERVO=0;
    }
}





