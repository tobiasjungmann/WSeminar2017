//Stand 11.09.2017

#include <wiringPi.h>
#include "StatusLEDs.h"
#include "daten.h"


int statusGRUEN()
{
    digitalWrite(LEDgruen, LOW);
    digitalWrite(LEDrot, LOW);
    
    digitalWrite(LEDgruen,HIGH);
    return 0;
}

int statusROT()
{
    digitalWrite(LEDgruen, LOW);
    digitalWrite(LEDrot, LOW);
    
    digitalWrite(LEDrot,HIGH);
    return 0;
}


int statusAUS()
{
    digitalWrite(LEDgruen, LOW);
    digitalWrite(LEDrot, LOW);
    
    return 0;
}




