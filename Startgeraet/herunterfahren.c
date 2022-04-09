//Stand 21.09.17

#include <wiringPi.h>
#include "herunterfahren.h"
#include "daten.h"
#include "StatusLEDs.h"

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
            statusAUS();
            system("sudo shutdown -h now");
        }
    }
    return 0;
}
