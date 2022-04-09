//Stand 27.10.2017

#include <wiringPi.h>               //einfügen der nötigen Bibliotheken
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>

#include "433Mhz_Grundlagen.h"      //einfügen von 
#include "daten.h"
#include "Ziellinie.h"
#include "Einstellungen.h"


//_______________________________________ Funktionsdeklarationen ________________________________________________________________

int init();

int sprintsequenz();

int laufsequenz();



int main()
{
    init();
    einstellungen();
    return 0;
}





/*
==============================Hier beginnen die Funktionen==============================
*/

//init() initialisiert alle GPIOs, die Bibliothek wiringPi sowie dem LCD-Display

int init()
{
    wiringPiSetup();                

    lcd=lcdInit(2,16,4,LCD_RS,LCD_E,LCD_D0,LCD_D1,LCD_D2,LCD_D3,0,0,0,0);

    lcdCharDef(lcd, 2, unten);
    lcdCharDef(lcd, 3, oben);

    pinMode(SENDER,OUTPUT);
    pinMode(EMPFAENGER,INPUT);
    pinMode(RELAY, OUTPUT);
    
    pinMode(knopflinks,INPUT);
    pinMode(knopfRechts,INPUT);
    pinMode(knopfMitte,INPUT);
    pinMode(knopfSHUTDOWN,INPUT);
    
    pinMode(EchoEINS, INPUT);
    pinMode(EchoZWEI, INPUT);
    pinMode(TrigEINS , OUTPUT);
    pinMode(TrigZWEI , OUTPUT);

    pullUpDnControl(EMPFAENGER,PUD_DOWN);
    pullUpDnControl(knopflinks,PUD_DOWN);    
    pullUpDnControl(knopfRechts,PUD_DOWN);
    pullUpDnControl(knopfMitte,PUD_DOWN);
    pullUpDnControl(knopfSHUTDOWN,PUD_DOWN);
    pullUpDnControl(EchoEINS,PUD_DOWN);
    pullUpDnControl(EchoZWEI,PUD_DOWN);

    digitalWrite(SENDER, LOW);
    digitalWrite(RELAY, LOW);
    digitalWrite(TrigEINS, HIGH);
    digitalWrite(TrigZWEI, HIGH);
    
    beginnZeitmessung=0;
    delay(100);
    return 0;
}



//_________________________________________________________________________


//sprintsequenz() ermöglicht eine Zeitmessung bei einer Sprintsportart. Sie wird automatisch
//nach tätigen der Einstellungen aufgerufen. Sie führt auf Knopfdruck entweder eine neue Messung aus,
//ruft die Einstellungen erneut auf oder fährt das Gerät herunter.

int sprintsequenz()
{
   
    lcdClear(lcd);
    lcdPuts(lcd,"  startbereit  ");

    while(digitalRead(knopfRechts)==1)          //Wartet, bis der Knopf nicht mehr gedrückt wird, 
    {                                           //damit nicht automatisch die nächste Aktion ausgelöst wird
        delay(1);
    }
    
    for(;;)                                     //Sich ewig wiederholende Schleife, die auf Knopfdruck 
    {                                           //verschiedene Funktionen bereitstellt
        if(digitalRead(knopfRechts)==1)         //Der rechte Knopf startet einen Lauf
        {
            delay(200);
            lcdClear(lcd);
            lcdPuts(lcd,"warten auf      Rueckmeldung");
            delay(10);

            delay(1000);
            senden();

          
            delay(1000);
            empfangen();
            
            delay(7350);                       //Diese Zeit benötigt das Startsignal und der Sendevorgang. Wenn die Geräte näher zusammenstehen ist diese Zahl kleiner
            beginnZeitmessung=micros();

            zielkontrolle();                    //Danach beginnt die eigentliche Überwachung des Ziels.
           // lcdClear(lcd);                      //Als letztes wird der Ausgangszustand wiederhergestellt.
           // lcdPuts(lcd,"  startbereit  ");
        }

        if(digitalRead(knopflinks)==1)          //Der linke Knopf ermöglicht die Einstellungen erneut zu tätigen.
        {
            einstellungen();
        }

        herunterfahren();                       //Hier wird, sollte der "Herunterfahren-" Knopf lange genüg gedrückt sein,
                                                //der Raspberry Pi heruntergefahren.

        delay(20);                 //nur zu Testzwecken
    }
    printf("\nFehler laufsequenz\n");           //nur zu Testzwecken
    return 0;
}



//______________________________________________________________________________________________________________________________


//laufsequenz zeigt bei den verschiedenen Dauerläufendie bereits gelaufene Zeit, sowie die entsprechende Note, an.
//Sie dient lediglich der aAusgabe der Zeit und einer Auswertung

int laufsequenz()
{
    int abbruch,stop, min, sek, hndrsek,i,position,pause;
    long zeitStart, zeitEnde, zeitDiff;

    abbruch=0;

    lcdClear(lcd);
    lcdPrintf(lcd,"Zeit:  0: 0: 0");
    lcdPosition(lcd,0,1);
    
    for(;;) 
    {
        if(digitalRead(knopfRechts)==1)                 //Durch drücken des rechten Tasters wird ein Lauf gestartet.
        {
            zeitStart=micros();
            stop=0;
            
            lcdClear(lcd);
            lcdPrintf(lcd,"Zeit:  0: 0: 0");
            
            if(klassenstufe==1 || klassenstufe==2)      //Je nach Wertungssystem (Notenpunkte/Noten) erscheint ein anderer, Text auf dem Display
            {
                position=0;
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd,"Notenpunkte: ");
                lcdPosition(lcd,14,1);
                lcdPrintf(lcd,"15");
            }
            else
            {
                position=6;
                lcdPosition(lcd,0,1);
                lcdPrintf(lcd,"Note: ");
                lcdPosition(lcd,7,1);
                lcdPrintf(lcd,"6");
            }
            
            while(stop==0)
            {             
                if(digitalRead(knopfMitte)==1)                  //ermöglicht den Lauf zu stoppen und ohne Verfälschung der Zeit diesen später fortzusetzten
                {
                    while(digitalRead(knopfMitte)==1)          //Wartet, bis der Knopf nicht mehr gedrückt wird, damit nicht automatisch die nächste Aktion ausgelöst wird
                    {
                        delay(50);
                    }
                }
                
                zeitEnde=micros();
                zeitDiff=zeitEnde-zeitStart;        
                
                hndrsek=(zeitDiff/100000)%10;                   //Hier wird die Zeitdifferenz in Mikrosekunden  in Minuten, Sekunden und zehntel Sekunden ausgegeben.
                sek=(zeitDiff/1000000)%60;
                min=(zeitDiff/1000000)/60;

                lcdPosition(lcd,7,0);
                lcdPrintf(lcd,"%2i:%2i:%2i",min,sek,hndrsek);
                delay(10);

                if(klassenstufe==1 || klassenstufe==2)           //Zur aktuell gelaufenen Zeit wird die, dieser entsprechend, die Note bestimmt.
                {
                    if(zeitDiff>=notenstufen[position])
                    {                   
                        position++;
                        lcdPosition(lcd,14,1);
                        if(position==16)
                        {
                            stop=1;
                        }
                        else
                        {
                            lcdPrintf(lcd,"%2i",15-position);
                        }
                    }
                }
                else
                {
                    if(zeitDiff>=notenstufen[position-1])       //Falschherum fängt bei zeit für eins an
                    {                   
                        lcdPosition(lcd,7,1);
                        lcdPrintf(lcd,"%2i",position);

                        position--;
                        
                        if(position==0)
                        {
                            stop=1;
                        }
                    }
                }

                herunterfahren();
                if(digitalRead(knopflinks)==1)  
                {
                    einstellungen();
                }
                
                delay(50);
            }
        }

        if(digitalRead(knopflinks)==1)
        {
            einstellungen();
        }

        herunterfahren();
    }
    laufsequenz();
    return 0;
}
    

