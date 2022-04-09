//Stand 28.10.2017

#include <wiringPi.h>
#include "433Mhz_Grundlagen.h"
#include "daten.h"
#include "herunterfahren.h"


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
        digitalWrite(RELAY, HIGH);
        delay(10);
        digitalWrite(SENDER, HIGH);
        delay(100);
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
    int empfangen;
    long zeitanfang, zeitende, zeitdifferenz;
    
    empfangen=0;
    zeitanfang=0;
    zeitende=0;
    zeitdifferenz=0;

    while(empfangen==0)
    {
        herunterfahren();
        zeitanfang=micros();
        
        if(funkMessung()>=7)
        {
            while(funkMessung()>=1)
            {
                delay(5);
            }
        
            zeitende = micros();
            delay(5);
            zeitdifferenz = zeitende - zeitanfang;

           if(zeitdifferenz>850000 && zeitdifferenz<1050000)         //Genaue Zalen erst durch Messung der Länge der Störsignale möglich
            {
                empfangen=1;
                delay(20);
            }
            
            delay(10);
        }
    }
    lcdClear(lcd);
    lcdPrintf(lcd,"Startkommando");
    return 0;
}

