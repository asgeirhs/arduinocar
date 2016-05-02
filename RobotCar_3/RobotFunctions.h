
//******************* Skilgreining á tengjum og breytum **************
int motorVpwm_=5;
int motorVgir_=9;
int motorVpwm;     //Geymir stöðu motorV  (analogWrite(motorVpwm_,motorVpwm);)

int motorHpwm_=6;
int motorHgir_=12; 
int motorHpwm;     //Geymir stöðu motorH  (analogWrite(motorHpwm_,motorHpwm);)
/*
int VgrunnV=125;   //Upphafsgildi mótoranna
int VgrunnH=125;
*/
int VgrunnV=150;   //Upphafsgildi mótoranna
int VgrunnH=150;

int TXspilari_=4;  //Sendir skipanir til mp3 spilarans
int RXspilari_=2;  //Tekur við skipunum frá mp3 spilara
unsigned long time;  //Notuð með millis() fallinu til að ákveða spilatíma laga

int servo;          //Geymir stöðu skrifaða á servo_
int pulsBreidd=75;  //Geymir púlsbreidd á servo_  75*20us=1,5ms => beint fram
int period=1000;    //Geymir útreiknaðan umferðatíma (1000*20us=20ms)

int sonarTrigger_=7;       //Sendir 10us breiðan 40kHz sónarpúls
int sonarEcho_=8;          //Tekur á móti SONAR púlsinum
unsigned int sonarLengd;   //Geymir mælda (reiknaða) lengd að endurvarpinu


int lagNr;                 //Synir hvaða lag er spilad
unsigned int lengdV;
unsigned int lengdH;
unsigned int lengdMax=0;
unsigned int lengdCheck;

int cnt_SONAR=0;     //Geymir stöðu SONAR teljara interrupt fallsins
int cnt_SERVO=0;     //Geymir stöðu SERVO teljara interrupt fallsins
int servo_=3;        //Geymir númer servo tengis
int snuningsAtt=-1;   //-1 ef servo orvhentur



//*************** Custom ISR Timer Routine (interrupt fallid" ***********
void timerIsr()                   
{ 
    cnt_SONAR++;          //cnt_SONAR Hækkar um 1 við hvert overflow
    cnt_SERVO++;          //cnt_SERVO Hækkar um 1 við hvert overflow
  
  //Smíða servo pwm merkið út frá pulsBreidd, period og cnt_SERVO teljaranum
    if(digitalRead(servo_)==1 && cnt_SERVO>=pulsBreidd)
    {
       digitalWrite(servo_,LOW);
    }
    if(cnt_SERVO>=period)
    {
        digitalWrite(servo_,HIGH);
        cnt_SERVO=0;
    }
}
//******************** unsigned intlengd() Reiknar fjarlægð að endurvarpi ***************
//Athugið að nota þetta fall ekki nema sónarinn sé tengdur annars bíður tölvan endalaust

unsigned int lengd() //Reikna lengd að endurvarpi
{
   unsigned int tmp;
   digitalWrite(sonarTrigger_,HIGH);       //Setja trigger útgang í hástöðu (Byrja StartPúls)
   delayMicroseconds(10);                  //bíða í 10 us  (Start púls er 10us breiður)
   digitalWrite(sonarTrigger_,LOW);        //Setja trigger útgang í lástöðu (enda startpuls)
   while(digitalRead(sonarEcho_)==0);      //Bíða þar til bergmal byrjar
   cnt_SONAR=0;                            //Stilla SONAR teljarann á 0
   
   while(digitalRead(sonarEcho_)==1);      //Bíða þar til bergmáli lýkur   
   tmp=cnt_SONAR;                          //Forða innihaldi cnt_SONAR yfir í tmp
   tmp=tmp*0.343;                          //tmp*20us*34300cm/s /2 =tmp*0,343cm/us
   
   return tmp;                //Skila lengdinni til baka til þess sem kallaði á fallið
}
//******** Skapa servopúls kvarðaðan í 20us einingum, út frá stefnu í gráðum **************

//Servopúlsinn umreiknaður úr gráðum í us.  -90°-> 600us 0°-> 1500us +90°->2400us
//Síðan er púlsbreiddin umreiknuð í fjölda interrupta er koma á 20us fresti
int reiknaPulsBreidd(int gradur)  //+1, snunAtt=-1 ef Servo orvhentur      
{  
    int temp;
    if(gradur<-90)      //koma í veg fyrir að servo eyðileggist
       gradur = -90;
    else if(gradur>90)
      gradur = 90;
   if(snuningsAtt==-1)  //Sumir servomotorar eru orvhentir
      gradur=gradur*-1;   
   temp=1500+gradur*10; //Hér er púlsbreiddin nú komin í us
   
   temp=temp*5/10;      //temp=temp/20  eða temp=temp*0,05 sem gerir sama                 
   temp=(temp+5)/10;    //leggja 5 við fyrir deilingu afrúnnar töluna                    
   return temp;         
}

void startCar()
{
   digitalWrite(motorVgir_,HIGH);
   digitalWrite(motorHgir_,LOW);
   analogWrite(motorVpwm_,VgrunnV);
   analogWrite(motorHpwm_,VgrunnH); 
}

void slowCar()
{
   digitalWrite(motorVgir_,HIGH);
   digitalWrite(motorHgir_,LOW);
   analogWrite(motorVpwm_,50);
   analogWrite(motorHpwm_,50); 
}

void breakCar()
{
    analogWrite(motorVpwm_,0);
    analogWrite(motorHpwm_,0);
}
void stopCar()
{  
   digitalWrite(motorVgir_,HIGH);
   digitalWrite(motorHgir_,LOW);
   analogWrite(motorVpwm_,180);
   analogWrite(motorHpwm_,180);
   delay(100);
   analogWrite(motorVpwm_,100);
   analogWrite(motorHpwm_,100);
   delay(100);
   analogWrite(motorVpwm_,0);
   analogWrite(motorHpwm_,0);
}
void backCar()
{
   stopCar();
   delay(500);
   digitalWrite(motorVgir_,LOW);
   digitalWrite(motorHgir_,HIGH);
   analogWrite(motorVpwm_,VgrunnV);
   analogWrite(motorHpwm_,VgrunnH); 
}

void driveLeft()
{
    digitalWrite(motorVgir_,LOW);
    digitalWrite(motorHgir_,LOW);
    analogWrite(motorVpwm_,VgrunnV);
    analogWrite(motorHpwm_,VgrunnH);  
}
void driveRight()
{
    digitalWrite(motorVgir_,HIGH);
    digitalWrite(motorHgir_,HIGH);
    analogWrite(motorVpwm_,VgrunnV);
    analogWrite(motorHpwm_,VgrunnH);  
}

