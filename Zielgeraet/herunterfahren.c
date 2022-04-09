#include <wiringPi.h>
#include "herunterfahren.h"

int herunterfahren()
{
    int i;
    if(digitalRead(knopfSHUTDOWN)==1)
    {    
        for(i=0;i<=20;i++)
        {
            if(digitalRead(knopfSHUTDOWN)==0)
            {
                i=30;
            }
            delay(50);
        }
        if(i!=30)
        {
            lcdClear(lcd);
            lcdPuts(lcd,"Stromversorgung entfernen");
            system("sudo shutdown -h now");
        }
    }
    return 0;
}
