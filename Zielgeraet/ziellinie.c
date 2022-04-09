//Stand 27.10.2017
//Hier wir die Zielüberwachung mittels der Ultraschallsensoren bereitgestellt.


#include <stdio.h>
#include <lcd.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "Ziellinie.h"
#include "daten.h"


long timeOutEINS;
long timeOutZWEI;
int zustandEINS;                      //Wert des Signals am GPIO von Echos eins
int zustandZWEI;                      //Wert des Signals am GPIO von Echos 
int abbruch;                          //beendet Schleifen bei der Auswertung
int position;                         //zählt in der Auswertung, um die passende Note zu ermitteln, die Durchläufe.
int abgeschlossenEINS;
int abgeschlossenZWEI;                
int statusBahnEINS;                   //Gibt an, in welcher Position sich das erste switch befindet
int statusBahnZWEI;                   //Gibt an, in welcher Position sich das zweite switch befindet
int vorherig_ZielerreichtEINS;        //1 bedeutet durch vorherige Messung ermittelte Entfernung unter 1,7m
int vorherig_ZielerreichtZWEI;        //1 bedeutet durch vorherige Messung ermittelte Entfernung unter 1,7m
int ende;                             //beendet Wiederholung, sobald beide USS Ziel erkannt haben
    



long startzeitAktZWEI;              //Zeitpunkt erstes HIGH Signal des Echos aktuell
long startzeitAktEINS;              //Zeitpunkt erstes HIGH Signal des Echos aktuell
long laufzeitAktEINS;           //Zeitpunkt des aktuellen Zieleinlaufes Bahn Eins
long laufzeitAktZWEI;           //Zeitpunkt des aktuellen Zieleinlaufes Bahn Zwei
long zeitEINS;                  //gesammte benötige Zeit Bahn zwei
long zeitZWEI;                  //gesammte benötige Zeit Bahn zwei
long startPunktZWEI;                //Beginn der Zeitmessung          //unwichtig, da Startzeit von außen mittels daten.h kommt.
long startPunktEINS;                //Beginn der Zeitmessung
long endeZeitmessungEINS;
long endeZeitmessungZWEI;

long laufzeiten[30];
int laufzeitPos;
long laufzeitenDurchschnitt;



//StateMachine erzielt eine sehr kurze Durchlaufzeit, wodruch beide Ultraschallsensoren (USS) auf einmal messen können.
//dafür erhält jeder Sensor einen switch case, der jeweils einen Zustand abarbeitet. Jeder Zustand ist möglichst knapp gehalten.

int stateMachine()
{   
    switch(statusBahnEINS)
    {
        case 0:                             //Initialisierung der Messungsspezifischen Variablen
            digitalWrite(TrigEINS, LOW);
            timeOutEINS=0;
            statusBahnEINS=1;
            break;

        case 1:                             //warten, bis der USS den Trigger wargenommen hat(im Datenblatt vorgeschrieben).
            if(timeOutEINS<=25)
            {
                delayMicroseconds(1);
                timeOutEINS++;
            }
            else
            {
                statusBahnEINS=2;
                timeOutEINS=0;
            }
            break;
        
        
        case 2:                             //Triggern von Sensor EINS eigentlich mittels einer fallenden Flanke(-> LOW), durch den Transistor wird dieses Signal aber umgekehrt
            digitalWrite(TrigEINS, HIGH);
            statusBahnEINS=3;
            break;


        case 3:                             //Kurzes warten, sodass der GPIO sicher seinen Zustand wechselt und getriggert wird
            delayMicroseconds(1);
            statusBahnEINS=4;
            break;

        case 4:                             //Warten, bis erstes HIGH Signal gelesen wird
            zustandEINS=digitalRead(EchoEINS);
            if(zustandEINS==1)
            {
                startzeitAktEINS = micros();
                statusBahnEINS=5;
                timeOutEINS=0;
            }
            else if(timeOutEINS>1000)       //Abbruchbedingung, da der Sensor in diesem Zusand stecken bleiben kann.
            {
                delayMicroseconds(1);
                statusBahnEINS=0;
            }
            timeOutEINS++;
            delayMicroseconds(1);
            break;

        
        case 5:                             //Warten, das das HIGH Signal wieder verschwindet
            zustandEINS=digitalRead(EchoEINS);
            timeOutEINS++;
            if(zustandEINS == 0)
            {
                laufzeitAktEINS = micros();
                statusBahnEINS=6;
            }
            else if(timeOutEINS>4000)       //Timeout bricht ab falls Signal zu lange dauert, was ein weiter als 2 Meter entferntes Ziel voraussetzt.
            {
                statusBahnEINS=0;
                vorherig_ZielerreichtEINS=0;
            }
            break;

        case 6:                             //prüfen ob Messwert im gültigen Bereich
            if(vorherig_ZielerreichtEINS==1)
            {
                if((laufzeitAktEINS-startzeitAktEINS)<=9860)
                {
                    endeZeitmessungEINS=micros();
                    statusBahnEINS=7;
                    zustandEINS=0;
                }
                else
                {
                    statusBahnEINS=0;
                    vorherig_ZielerreichtEINS=0;
                }
            }
            else
            {
                if((laufzeitAktEINS-startzeitAktEINS)<=9860)
                {
                    statusBahnEINS=0;
                    vorherig_ZielerreichtEINS=1;
                }
                else
                {
                    statusBahnEINS=0;
                    vorherig_ZielerreichtEINS=0;
                }
            }
            break;

        case 7:                             //zwei gülitge Messungen auf Bahn EINS empfangen

            if(zustandEINS==0)
            {
                zustandEINS=1;
                abgeschlossenEINS=1;
            }
            break;
    }

   
    switch(statusBahnZWEI)                  //Dieser switch-Case läuft identisch wie der erste ab, weshalb auf genauere Erläuterungen verzichtet wird.
    {
        case 0:                             //Initialisierung der Messungsspezifischen Variablen
            digitalWrite(TrigZWEI, LOW);
            timeOutZWEI=0;
            statusBahnZWEI=1;
            break;

        case 1:
            if(timeOutZWEI<=25)
            {
                delayMicroseconds(1);
                timeOutZWEI++;
            }
            else
            {
                statusBahnZWEI=2;
                timeOutZWEI=0;
            }
            break;
        
        
        case 2:                             //Triggern von Sensor ZWEI
            digitalWrite(TrigZWEI, HIGH);
            statusBahnZWEI=3;
            break;


        case 3:                             
            delayMicroseconds(1);
            statusBahnZWEI=4;
            break;

        case 4:                             //Warten, bis erstes HIGH Signal gelesen wird
            zustandZWEI=digitalRead(EchoZWEI);
            if(zustandZWEI==1)
            {
                startzeitAktZWEI = micros();
                statusBahnZWEI=5;
                timeOutZWEI=0;
            }
            else if(timeOutZWEI>1000)
            {
                delayMicroseconds(1);
                statusBahnZWEI=0;
            }
            timeOutZWEI++;
            delayMicroseconds(1);
            break;

        
        case 5:                             //Warten, das das HIGH Signal wieder verschwindet
            zustandZWEI=digitalRead(EchoZWEI);
            timeOutZWEI++;
            if(zustandZWEI == 0)
            {
                laufzeitAktZWEI = micros();
                statusBahnZWEI=6;
            }
            else if(timeOutZWEI>4000)
            {
                //laufzeitAktZWEI = micros();
                statusBahnZWEI=0;
                vorherig_ZielerreichtZWEI=0;
            }
            break;

        case 6:                             //prüfen ob Messwert im gültigen Bereich
            if(vorherig_ZielerreichtZWEI==1)
            {
                if((laufzeitAktZWEI-startzeitAktZWEI)<=9860)
                {
                    endeZeitmessungZWEI=micros();
                    statusBahnZWEI=7;
                    zustandZWEI=0;
                }
                else
                {
                    statusBahnZWEI=0;
                    vorherig_ZielerreichtZWEI=0;
                }
            }
            else
            {
                if((laufzeitAktZWEI-startzeitAktZWEI)<=9860)
                {
                    statusBahnZWEI=0;
                    vorherig_ZielerreichtZWEI=1;
                }
                else
                {
                    statusBahnZWEI=0;
                    vorherig_ZielerreichtZWEI=0;
                }
            }
            break;
        

        case 7:                             //zwei gültige Messungen auf Bahn ZWEI empfangen
            if(zustandZWEI==0)
            {
                zustandZWEI=1;
                abgeschlossenZWEI=1;
            }
            break;
    }

//____________Auswertung______________________________________________________________________________________

    if(abgeschlossenEINS==1 && abgeschlossenZWEI==1)      //Auswertung: Ermitteln der passenden Note, sobald beide Läufer das Ziel durchquert haben
    {
        int i, druckEINS, druckZWEI;
        long druckHilfeEINS, druckHilfeZWEI;
        
        ende=1;
        abbruch=0;
        position=-1;
        i=1;
        laufzeitenDurchschnitt=0;
        
        zeitEINS=endeZeitmessungEINS-beginnZeitmessung; //Ermittlung der benötigten Zeit
        zeitZWEI=endeZeitmessungZWEI-beginnZeitmessung;

        druckEINS=zeitEINS/1000000;
        druckHilfeEINS=druckEINS*1000000;
        druckHilfeEINS=zeitEINS-druckHilfeEINS;
        druckHilfeEINS=druckHilfeEINS/100000;
        lcdPosition(lcd,8,0);
        lcdPrintf(lcd,"%i,%ld",druckEINS,druckHilfeEINS);
        
        druckZWEI=zeitZWEI/1000000;
        druckHilfeZWEI=druckZWEI*1000000;
        druckHilfeZWEI=zeitZWEI-druckHilfeZWEI;
        druckHilfeZWEI=druckHilfeZWEI/100000;
        lcdPosition(lcd,8,1);
        lcdPrintf(lcd,"%i,%ld",druckZWEI,druckHilfeZWEI);    


        laufzeiten[laufzeitPos]=zeitEINS;
        laufzeitPos++;
        if(laufzeitPos==30)
        {
            laufzeitPos=0;
        }

        laufzeiten[laufzeitPos]=zeitZWEI;
        laufzeitPos++;
        if(laufzeitPos==30)
        {
            laufzeitPos=0;
        }
        
        if(klassenstufe==1 || klassenstufe==2)          //Jahrgangsstufe 11 und 12 müssen aufgrund des anderen Notensystems seperat bestimmt werden.
        {
            while(abbruch==0)                           //ermitteln der Note für Bahn eins
            {
                position++;
                if(zeitEINS>=notenstufen[position])     //Es wird so lange wiederholt, bis die gelaufene Zeit zum ersten mal größer als der Wert von notenstufen[], ist
                {
                    abbruch=1;
                }
                if(position==16)                        //Diese abbruch Bedingung ist nötig, da die schlechteste Note kleiner als die niedrigste zeit sein kann => 0 Punkte
                {
                    abbruch=1;
                }
            }            
            lcdPosition(lcd,12,0);
            lcdPrintf(lcd,"%2i",15-position);
            delay(1);


            abbruch=0;
            position=-1;
            
            while(abbruch==0)
            {
                position++;
                if(zeitZWEI>=notenstufen[position])
                {
                    abbruch=1;
                }
                if(position==16)
                {
                    abbruch=1;
                }
            }
            
            lcdPosition(lcd,12,1);
            lcdPrintf(lcd,"%2i",15-position);
            delay(1);

            
        }
        else
        {
            abbruch=0;
            position=-1;

            while(abbruch==0)
            {
                position++;
                if(zeitZWEI>=notenstufen[position])
                {
                    abbruch=1;
                }
                if(position==5)
                {
                    abbruch=1;
                }
            }
            lcdPosition(lcd,12,0);
            lcdPrintf(lcd,"%2i",1+position);
            delay(1);

            abbruch=0;
            position=-1;

            while(abbruch==0)
            {
                position++;
                if(zeitZWEI>=notenstufen[position])
                {
                    abbruch=1;
                }
                if(position==5)
                {
                    abbruch=1;
                }
            }
            
            lcdPosition(lcd,12,1);
            lcdPrintf(lcd,"%2i",1+position);
            delay(1);
        }
        
        while(abbruch==0 && i<31)
        {
            laufzeitenDurchschnitt=laufzeitenDurchschnitt+laufzeiten[i];
            i++;
            if(laufzeiten[i]==0)                //notwendig, da sonnst der durchschnitt der ersten Messungen durch die Messungen ohne Wert(=0) verfälscht werden.
            {
                abbruch=1;
            }
        }

        laufzeitenDurchschnitt=laufzeitenDurchschnitt/i;

        
        lcdPosition(lcd,15,0);
        if(zeitEINS>=laufzeitenDurchschnitt)
        {
            lcdPutchar(lcd, 3);
            delay(1);
        }
        else
        {
            lcdPutchar(lcd, 2);
            delay(1);
        }

        lcdPosition(lcd,15,1);
        if(zeitZWEI>=laufzeitenDurchschnitt)
        {
            lcdPutchar(lcd, 3);
            delay(1);
        }
        else
        {
            lcdPutchar(lcd, 2);
            delay(1);
        }




        //while(digitalRead(knopfRechts)==0)  
        //{
        //    herunterfahren();
            if(digitalRead(knopflinks)==1)  
            {
                einstellungen();
            }
            delay(1);
        //}
        
        //while(digitalRead(knopfRechts)==1)          
        //{                                  
        //    delay(1);
        //}
    }
    return 0;
}


int zielkontrolle()
{   
    abgeschlossenEINS=0;
    abgeschlossenZWEI=0;
    statusBahnEINS=0;
    statusBahnZWEI=0;
    vorherig_ZielerreichtEINS=0;        //0 entspricht nicht erreicht, 1 bedeutet entfernung unter 1,7m
    vorherig_ZielerreichtZWEI=0;        //0 entspricht nicht erreicht, 1 bedeutet entfernung unter 1,7m
    laufzeitPos=0;

    ende = 0;
    
    digitalWrite(TrigEINS, LOW);
    digitalWrite(TrigZWEI, LOW);

    lcdClear(lcd);
    lcdPrintf(lcd,"<- Bahn      - --> Bahn      - -");                     //nur zu Testzwecken
    delay(1000);                
    
    while(ende==0)                  //Diese Schleife ruft bis zum Ende der Messung die Methode statemachine auf,
                                    //die die eigentliche Zielüberwachung übernimmt
    {
        stateMachine();
    }

    return 0;
}
