//******************** MySonar_og_3Servo ************************
#include <TimerOne.h>

int trigger=12;    //Geymir númer trigger tengisins, hér 12
int echo=11;       //Geymir númer echo tengisins, hér 11

int servoV_=4;     //Geymir númer servo tengisV, hér 4
int servoH_=7;     //Geymir númer servo tengisH, hér 7
int servoS_=8;     //Geymir númer servo tengisS, hér 8
int servoV;        //Geymir stöðuna sem skrifuð var á servoV_
int servoH;        //Geymir stöðuna sem skrifuð var á servoH_
int servoS;        //Geymir stöðuna sem skrifuð var á servoS_

int cnt_SONAR=0;    //Geymir talningu fyrir SONAR interrupt fallsins
int cnt_SERVO=0;    //Geymir talningu fyrir SONAR interrupt fallsins
int L=0;      //Geymir mælda vegalengd sonarsins
int pulsBreidd_V=0; //Geymir púlsbreidd servo_V fyrir vinstri mótor
int pulsBreidd_H=0; //Geymir púlsbreidd servo_H fyrir hægri mótor
int pulsBreidd_S=0; //Geymir púlsbreidd servo_S fyrir SONAR hausinn
int period=0;
  
unsigned int lengd() //Reikna lengd að endurvarpi
{
   unsigned int tmp;
   digitalWrite(trigger,HIGH);             //Setja trigger útgang í hástöðu (Byrja StartPúls)
   delayMicroseconds(10);                  //bíða í 10 us  (Start púls er 10us breiður)
   digitalWrite(trigger,LOW);              //Setja trigger útgang í lástöðu (enda startpuls)
   while(digitalRead(echo)==0);            //Bíða þar til bergmal byrjar
   cnt_SONAR=0;                            //Stilla SONAR teljarann á 0
   while(digitalRead(echo)==1);            //Bíða þar til bergmáli lýkur   
   tmp=cnt_SONAR;                          //Forða innihaldi cnt_SONAR yfir í tmp
   tmp=tmp*0.343+0,5;         //tmp*20us*34300cm/s /2=tmp*0,343cm/us
   return tmp;                //Skila lengdinni til baka til þess sem kallaði á fallið
}
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
  period=1000;
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT_PULLUP); 
  pinMode(servoV_,OUTPUT);
  pinMode(servoH_,OUTPUT);
  pinMode(servoS_,OUTPUT);
  servoV=1;
  digitalWrite(servoV_,HIGH);
  servoH=1;
  digitalWrite(servoH_,HIGH);
  servoS=1;
  digitalWrite(servoS_,HIGH);
  
  Timer1.initialize(20); // set a timer of length 20 microseconds 
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  Serial.begin(9600);
}
 
void loop()
{
   L=lengd();
   Serial.println(L);
   pulsBreidd_V=reiknaNigrip(600);
   delay(1000);
   pulsBreidd_H=reiknaNigrip(1500);
   delay(1000);
   pulsBreidd_S=reiknaNigrip(2400);
   delay(1000);
}
 
/// --------------------------         //digitalWrite(trigger,LOW); digitalRead(echo
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()                          //MUNA digitalWrite og digital read_____________________________--
{ 
    cnt_SONAR++;          //cnt_SONAR Hækkar um 1 við hvert overflow
    cnt_SERVO++;          //cnt_SERVO Hækkar um 1 við hvert overflow
    if(servoV==1 && cnt_SERVO>=pulsBreidd_V)
    {
       servoV=0;
       digitalWrite(servoV_,LOW);
    }
    if(servoH==1 && cnt_SERVO>=pulsBreidd_H)
    {
       servoH=0;
       digitalWrite(servoH_,LOW);
    }
    if(servoS==1 && cnt_SERVO>=pulsBreidd_S)
    {
       servoS=0;
       digitalWrite(servoS_,LOW);
    }
    if(cnt_SERVO>=period)
    {
        servoV=1;
        digitalWrite(servoV_,HIGH);
        servoH=1;
        digitalWrite(servoH_,HIGH);
        servoS=1;
        digitalWrite(servoS_,HIGH);
        cnt_SERVO=0;
    }
}





