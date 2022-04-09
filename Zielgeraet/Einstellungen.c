//Stand 27.10.2017
//Diese Bibliothek ermöglicht die nötigen Einstellungen  durch eine Benutzer zu tätigen.
//Die Methode Einstellungen dient der Übersichtlichkeit und verwaltet die anderen vier Methoden
//sobald jede dieser Funktionen einmal durchlaufen wurde entscheidet sie ob es sich um eine Sprintsportart oder eine
//Ausdauersportart handelt. je nach dem wird dann die passende Methode, die sich in Zielgeraet.c befindet ausgeführt.


#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include "daten.h"
#include "Einstellungen.h"
#include "herunterfahren.h"


int einstellungen()
{
    int einstellung, i;                     //int i nur für testzwecke, wird in finaler Version nicht mehr benötigt

    einstellung=0;
    
    lcdClear(lcd);
    lcdPuts(lcd,"Einstellungen");
    
    while(einstellung==0)                       //Menü zu Beginn: Knopf 1: Klassenstufe, Sportart und Geschlecht muss eingegeben werden
    {                                            //                Knopf 3: Es wird nur die Zeit gemessen, keine Note wird ausgegeben
    
        if(digitalRead(knopfRechts) == 1)       //Knopf 1: Klassenstufe, Sportart und Geschlecht muss eingegeben werden
        {
            klassenstufeEingeben();
            geschlechtEingeben();
            sportartEingeben();
            einstellung=1;

            auslesen();
        }

        herunterfahren();
    }

    if(klassenstufe>=5 && klassenstufe<=9 && sportart==0 ||  klassenstufe>=0 && klassenstufe<=2 && sportart==2 || klassenstufe==9 && sportart==3 || klassenstufe>=1 && klassenstufe<=2 && sportart==1)
    {
        laufsequenz();
    }
    else
    {
        sprintsequenz();
    }

    return 0;
}
    


//_________________________________________________________________________

//KlassenstufeEingeben() wartet nach initialisieren der Variablen bis der rechte Knopf nicht mehr gedrückt wird.
//Daraufhin wird die aktuelle Klassenstufe (5-12) Durch den mittleren und den linken Knopf verkleinert oder vergrößert.
//Sollte dabei die Klassenstufe 13 erreicht werden erscheint automatisch die Klassenstufe 5. Sollte sich eine Klassenstufe
//von 4 ergeben erscheint automatisch die Klassenstufe 13. Durch drücken des rechten Tasters wird diese Schleife beendet.
//Um die spätere Auswertung zu vereinfachen werden die Klassenstufen mit nur einer Ziffer angegeben. Dafür wird bei allen
//zweistelligen die erste Ziffer entfernt.

int klassenstufeEingeben()
{   
    klassenstufe=5;
        
    lcdClear(lcd);
    lcdPuts(lcd,"Klassenstufe: ");
    lcdPosition(lcd,13,0);
    lcdPrintf(lcd,"%2i",klassenstufe);

    while(digitalRead(knopfRechts)==1)
    {
        delay(1);
    }

    while(digitalRead(knopfRechts)==0)
    {
        if(digitalRead(knopflinks) == 1)
        {
            klassenstufe++;

            if(klassenstufe>12)
            {
                klassenstufe=5;
            }

            lcdPosition(lcd,13,0);
            lcdPrintf(lcd,"%2i",klassenstufe);
            
            while(digitalRead(knopflinks) == 1)
            {
                delay(1);
            }
        }
        
        if(digitalRead(knopfMitte)==1)
        {
            klassenstufe--;

            if(klassenstufe<5)
            {
                klassenstufe=12;
            }

            lcdPosition(lcd,13,0);
            lcdPrintf(lcd,"%2i",klassenstufe);
            
            while(digitalRead(knopfMitte) == 1)
            {
                delay(1);
            }
        }
        
        delay(10);
    }
    
    if(klassenstufe==10)
    {
        klassenstufe = 0;
    }

    if(klassenstufe==11)
    {
        klassenstufe = 1;
    }

    if(klassenstufe==12)
    {
        klassenstufe = 2;
    }

    return 0;
}




//_________________________________________________________________________

//GeschlechtEingeben() verwendet das gleiche Prinzip wie KlssenstufeEingeben().
//männlich -> 0
//weiblich -> 1

int geschlechtEingeben()
{
    geschlecht = 0;
       
    lcdClear(lcd);
    lcdPuts(lcd,"Geschlecht: ");
    lcdPosition(lcd,0,1);
    lcdPuts(lcd,"maennlich");

    while(digitalRead(knopfRechts)==1)
    {
        delay(1);
    }

    while(digitalRead(knopfRechts)==0)
    {
        if(digitalRead(knopflinks) == 1|| digitalRead(knopfMitte)==1)
        {
            if(geschlecht==0)
            {
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"weiblich ");
                geschlecht=1;
            }
            else if (geschlecht==1)
            {
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"maennlich");
                geschlecht=0;
            }
            
            while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
            {
                delay(1);
            }
        }               
    }
    
    return 0;
}


//_________________________________________________________________________

//sportartEingeben() ermittelt anhand des Geschlechtes und der Klassenstufe die zur verfügung stehenden Sportarten.
//Diese stehen in einer Schleife ähnlich der in klassenstufeEingeben(), dem Nutzer zur Verfügung 


int sportartEingeben()
{
    int veraendert,strecke,huerde;

    sportart=0;
    veraendert=0;
       
    lcdClear(lcd);
    lcdPuts(lcd,"Sportart: ");
    

    while(digitalRead(knopfRechts)==1)
    {
        delay(1);
    }


    switch(klassenstufe)
    {
        case 5: case 6:
                sportart=1;
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"50m Sprint ");

                while(digitalRead(knopfRechts)==0)
                {
                    if(digitalRead(knopflinks) == 1 || digitalRead(knopfMitte)==1)
                    {
                        if(sportart==0)
                        {
                            sportart=1;
                            lcdPosition(lcd,0,1);
                            lcdPuts(lcd,"50m Sprint");
                        }
                        else
                        {
                            sportart=0;
                            lcdPosition(lcd,0,1);
                            lcdPuts(lcd,"Dauerlauf ");
                        }

                        while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;

        
        case 7:
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"Dauerlauf ");

                while(digitalRead(knopfRechts)==0)
                {       
                    if(digitalRead(knopflinks) == 1)
                    {
                        sportart--;
                        if(sportart==-1)
                        {
                            sportart=2;
                        }
                        veraendert++;
                    }

                    if(digitalRead(knopfMitte)==1)
                    {
                        sportart++;
                        if(sportart==3)
                        {
                            sportart=0;
                        }
                        veraendert++;
                    }
            
                    if(veraendert==1)
                    {
                        switch(sportart)
                        {
                            case 0:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"Dauerlauf  ");
                                break;

                            case 1:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"50m Sprint  ");
                                break;
                                
                            case 2:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"75m Sprint   ");                                
                                break;

                            default:
                                break;
                        }
                        veraendert=0;
                        
                        while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;           

                
        case 8:
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"Dauerlauf ");

                while(digitalRead(knopfRechts)==0)
                {
                    if(digitalRead(knopflinks) == 1 || digitalRead(knopfMitte)==1)
                    {
                        if(sportart==0)
                        {
                            sportart=1;
                            lcdPosition(lcd,0,1);
                            lcdPuts(lcd,"75m Sprint");
                        }
                        else
                        {
                            sportart=0;
                            lcdPosition(lcd,0,1);
                            lcdPuts(lcd,"Dauerlauf ");
                        }

                        while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;


       case 9:
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"Dauerlauf ");

                if(geschlecht==0)       //männlich
                    {
                        strecke=1000;
                    }
                    else
                    {
                        strecke=800;
                    }

                while(digitalRead(knopfRechts)==0)
                {       
                    if(digitalRead(knopflinks) == 1)
                    {
                        sportart--;
                        if(sportart==-1)
                        {
                            sportart=3;
                        }
                        veraendert++;
                    }

                    if(digitalRead(knopfMitte)==1)
                    {
                        sportart++;
                        if(sportart==4)
                        {
                            sportart=0;
                        }
                        veraendert++;
                    }
            
                    if(veraendert==1)
                    {
                        switch(sportart)
                        {
                            case 0:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"Dauerlauf  ");
                                break;

                            case 1:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"75m Sprint  ");
                                break;

                            case 2:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"100m Sprint  ");
                                break;
                            
                            case 3:
                                lcdPosition(lcd,0,1);
                                lcdPrintf(lcd,"%im Lauf   ",strecke);                                
                                break;

                            default:
                                break;
                        }
                        veraendert=0;
                        
                        while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;


        
       case 0:
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"Dauerlauf ");

                if(geschlecht==0)       //männlich
                    {
                        strecke=1000;
                    }
                    else
                    {
                        strecke=800;
                    }

                while(digitalRead(knopfRechts)==0)
                {       
                    if(digitalRead(knopflinks) == 1)
                    {
                        sportart--;
                        if(sportart==-1)
                        {
                            sportart=2;
                        }
                        veraendert++;
                    }

                    if(digitalRead(knopfMitte)==1)
                    {
                        sportart++;
                        if(sportart==3)
                        {
                            sportart=0;
                        }
                        veraendert++;
                    }
            
                    if(veraendert==1)
                    {
                        switch(sportart)
                        {
                            case 0:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"Dauerlauf  ");
                                break;

                            case 1:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"100m Sprint  ");
                                break;
                                
                            case 2:
                                lcdPosition(lcd,0,1);
                                lcdPrintf(lcd,"%im Lauf   ",strecke);                                
                                break;

                            default:
                                break;
                        }
                        veraendert=0;
                        
                        while(digitalRead(knopfMitte)==1||digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;


        case 1: case 2:
                lcdPosition(lcd,0,1);
                lcdPuts(lcd,"100m Sprint ");

                if(geschlecht==0)       //männlich
                    {
                        strecke=1000;
                        huerde=110;
                    }
                    else
                    {
                        strecke=800;
                        huerde=100;
                    }

                while(digitalRead(knopfRechts)==0)
                {       
                    if(digitalRead(knopflinks) == 1)
                    {
                        sportart--;
                        if(sportart==-1)
                        {
                            sportart=3;
                        }
                        veraendert++;
                    }

                    if(digitalRead(knopfMitte)==1)
                    {
                        sportart++;
                        if(sportart==4)
                        {
                            sportart=0;
                        }
                        veraendert++;
                    }
            
                    if(veraendert==1)
                    {
                        switch(sportart)
                        {
                            case 0:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"100m Sprint  ");
                                break;

                            case 1:
                                lcdPosition(lcd,0,1);
                                lcdPrintf(lcd,"%4im Lauf   ",strecke); 
                                break;

                            case 2:
                                lcdPosition(lcd,0,1);
                                lcdPuts(lcd,"3000m Lauf  ");
                                break;
                            
                            case 3:
                                lcdPosition(lcd,0,1);
                                lcdPrintf(lcd,"%im Huerden   ",huerde);                                
                                break;

                            default:
                                break;
                        }
                        veraendert=0;
                        
                        while(digitalRead(knopfMitte)==1|| digitalRead(knopflinks)==1)
                        {
                           delay(1);
                        }
                    }
                }
                break;
       }
    return 0;
}


//______________________________________________________________________________________________________________________________

//auslesen() liest, aus einem File auf einem USB-Stick, die Zeiten, die zu den eben getätigten Einstellungen passen, ein


int auslesen()
{
    FILE *datei;
    int inhalt,i,u,abbruch,zahlwerte[70],ascwert;
    char werte[70];
    char cwert[1];

    i=0;
    u=0;
    abbruch=0;

    datei=fopen("/media/W_SEMINAR/daten.txt","r");         //Die Datei daten.txt wird geöffnet
    if(datei==NULL)                                             //Ob die Datei Ordungsgemäß geöffnet wurde wird hier geprüft.
    {
        lcdClear(lcd);
        lcdPuts(lcd,"USB-Stick   einstecken");
    }
    else
    {
        while(fgets(werte,65,datei)!=NULL && abbruch==0)        //diese Schleife liest so lange Zeile für Zeile ein,
                                                                //bis entweder die leztte Zeile erreicht ist oder die
                                                                //Richtige gefunden wird.
        {
            puts(werte);                                //nur zu Testzwecken
            for(i=0;i<3;i++)
            {
                ascwert=werte[i];         
                cwert[0]=(char)ascwert;     
                zahlwerte[i]=atoi(cwert);
            }
            
            if(zahlwerte[0]==geschlecht && zahlwerte[1]==klassenstufe && zahlwerte[2]==sportart)
            {                                                   //Sollten die ersten deri Ziffern einer Zeile mit denen
                                                                //der Einstellungen übereinstimmen ist die passende Zeile erreicht.
                abbruch=1;
                
               for(i=3;i<65;i++)                                //Der ausgelesene Wert besteht aus einem Char-Feld. Diese werden bei einer direkten Umwandlung in einen int als Assci-Code ausgegeben.
               {                                                //Durch einen Typecast des int zurück in ein char-Feld umgeht dieses Problem, da die Ziffer nun nicht mehr als Assci-Code gespeichert wird.
                    ascwert=werte[i];                           //Dieser neune char kann mithilfe eines Typcastes in den passenden Integer gewandelt werden.         
                    cwert[0]=(char)ascwert;     
                    zahlwerte[i]=atoi(cwert);
                }

               if(klassenstufe>3||klassenstufe==0)              //Je nachdem wie das Wertungssystem aussieht (Punkte/Noten) müssen unterschiedlich viele "Notenwerte erzeugt werden."
               {
                    for(u=0;u<=4;u++)
                    {
                        notenstufen[u]=zahlwerte[u*4+3]*1000+zahlwerte[u*4+4]*100+zahlwerte[u*4+5]*10+zahlwerte[u*4+6];
                        printf("\n%i",notenstufen[u]);
                    }
                }
                else
                {
                   for(u=0;u<=14;u++)
                   {
                       notenstufen[u]=zahlwerte[u*4+3]*1000+zahlwerte[u*4+4]*100+zahlwerte[u*4+5]*10+zahlwerte[u*4+6];
                        printf("\n%i",notenstufen[u]);
                    }
                }
                                                                    //Hier werden die jeweiligen Zeiten in Mikrosekunden umgerechnet um bei der Auswerung später nicht umrechnen zu müssen, wodurch diese schneller abarbeitbar ist.
                                                                    //Je nachdem in welcher Genauikeit(Minuten, Sekunden und Zehntelsekunden) die Zeiten angegeben sind müssen sie mit unterschiedlichen Werten multipliziert werden.
                if(klassenstufe!=1 && klassenstufe!=2 && sportart==0)
                {
                    for(i=0;i<=15;i++)
                    {
                        notenstufen[i]=notenstufen[i]*60000000;
                        printf("%ld\n",notenstufen[i]);     //nur zu Testzwecken
                        delay(50);
                    }
                }
                else if((klassenstufe<4 && sportart==2) || (klassenstufe==9 && sportart==3))      
                {
                    printf("Kategorie: blau\n");
                    for(i=0;i<=15;i++)
                    {
                       notenstufen[i]=notenstufen[i]*1000000;
                       printf("%ld\n",notenstufen[i]);
                    }
                }
                else
                {
                    printf("Kategorie: grün\n");
                    for(i=0;i<=15;i++)
                    {
                        notenstufen[i] = notenstufen[i]*100000;                                 
                        printf("%ld\n",notenstufen[i]);
                    }
                }
            }              
         }
        
         fclose(datei);
    }
    return 0;
}

