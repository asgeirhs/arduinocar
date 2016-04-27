// ******************* TestDFPlayer *******************************
#include <SoftwareSerial.h>
#include "MyDFPlayer.h"
                        
void setup()  
{
   mySerial.begin(9600);
   delay(100);
   mp3_set_volume(25);
   delay(100);
}

void loop()
{ 
   mp3_play_track(1);  // 1. Riding along in my automobile
   delay(10000);    
   mp3_play_track(2);  // 2. Pink Panther 
   delay(20000); 
   mp3_play_track(3);  // 3. Sultans of Swing
   delay(10000);
   mp3_play_track(4);  // 4. Bíll í gang (gamli bíllinn)
   delay(5000);
   mp3_play_track(5);  // 5. Gamaldags bílflauta 
   delay(5000);
   mp3_play_track(6);  // 6. Harley mótorhjáo + sultans of swing faide out
   delay(20000);
   mp3_play_track(7);  // 7. Beep Beep
   delay(5000);
}




