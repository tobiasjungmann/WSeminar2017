//Stand 21.09.2017
//Startgerät final

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#include "herunterfahren.h"
#include "daten.h"
#include "StatusLEDs.h"
#include "433Mhz_Grundlagen.h"
#include "mp3_Startsignal.h"



int init();



int main()
{
    int i,abbruch;
    
    init();

    for(i=0;i<3;i++)
    {
        statusROT();
        delay(400);
        statusGRUEN();
        delay(150);
    }

    
    for(;;)
    {
        abbruch=0;

        statusROT();
        
        empfangen();

        statusGRUEN();
        delay(1000);            //Hier muss eine Sek gewartet werden, da das Zielgerät auch erst nach dieser Zeit
                                //beginnt auf ein Signal zu warten, da es sonnst ohne diese Pause das
                                //selbst gesendete Signal empfangen würde.
        statusGRUEN();
        while(abbruch==0)
        {
            herunterfahren();
            
            if(digitalRead(knopfAKTION)==1)
            {
                senden();           //vor dem Start Senden, Zeit im Ziel dazurechnen
                startsignal();
                abbruch=1;
                        

            }
        }
    }
    
    return 0;
}





/*
==============================Hier beginnen die Funktionen==============================
*/



int init()
{
    wiringPiSetup();                

    pinMode(EMPFAENGER,INPUT);
    pinMode(SENDER, OUTPUT);
    pinMode(RELAY,OUTPUT);
    
    pinMode(knopfAKTION,INPUT);
    pinMode(knopfSHUTDOWN,INPUT);
    
    pinMode(LEDgruen,OUTPUT);
    pinMode(LEDrot,OUTPUT);

    digitalWrite(LEDgruen, LOW);
    digitalWrite(LEDrot, LOW);
    digitalWrite(SENDER, LOW);
    digitalWrite(RELAY, LOW);

    
    pullUpDnControl(knopfAKTION,PUD_DOWN);
    pullUpDnControl(knopfSHUTDOWN,PUD_DOWN);
    pullUpDnControl(EMPFAENGER,PUD_DOWN);
    
    return 0;
}






