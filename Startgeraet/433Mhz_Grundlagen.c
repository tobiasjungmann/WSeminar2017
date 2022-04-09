//Stand 18.10.2017
//Version für das Startgerät, besitzt kleine Abweichungen gegenüber Zielgerät

#include <wiringPi.h>
#include "433Mhz_Grundlagen.h"
#include "daten.h"
#include "herunterfahren.h"
#include "StatusLEDs.h"


int funkMessung()
{
    int ergebnis=0;
    int i=0;
    
    for(i=0;i<10;i++)
    {
        ergebnis=ergebnis+digitalRead(EMPFAENGER);
        delay(1);
    }

    return ergebnis;
}


//_________________________________________________________________________


int senden()
{
    delay(1000);        //vielleicht nur zu testzwecken
    digitalWrite(RELAY, HIGH);          //nötig , da erstes Signal nicht zuverlässig ist.
    delay(10);
    digitalWrite(SENDER, HIGH);
    delay(100);                  //nur zu testzwecken
    digitalWrite(SENDER, LOW);
    delay(10);
    digitalWrite(RELAY, LOW);
    delay(1000);
    
    digitalWrite(RELAY, HIGH);
    delay(10);
    digitalWrite(SENDER, HIGH);
    delay(1000);
    digitalWrite(SENDER, LOW);
    delay(10);
    digitalWrite(RELAY, LOW);
    delay(1000);
    
    return 0;
}

//_________________________________________________________________________


int empfangen()
{
    int empfangen,i;
    long zeitanfang, zeitende, zeitdifferenz;


    statusROT();
    
    empfangen=0;
    zeitanfang=0;
    zeitende=0;
    zeitdifferenz=0;

    while(empfangen==0)
    {      
        herunterfahren();
        zeitanfang=micros();
        delayMicroseconds(5);
        
        if(funkMessung()>=7)
        {

            while(funkMessung()>=1)
            {
                delay(5);
            }
        
            zeitende = micros();
            delayMicroseconds(5);
            zeitdifferenz = zeitende - zeitanfang;

           if(zeitdifferenz>850000 && zeitdifferenz<1050000)    //Davor ende bei 9500000     //Genaue Zalen erst durch Messung der Länge der Störsignale möglich
            {
               empfangen=1;
               delay(10);
            }
            
            delay(10);
        }
    }
    return 0;
}

