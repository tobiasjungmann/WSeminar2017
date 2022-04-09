#include "daten.h"


int lcd;
int wertungsart;
int geschlecht;
int klassenstufe;
int sportart;
long notenstufen[16];
long beginnZeitmessung;


char unten[8]={
                        0b00100,
                        0b00100,
                        0b00100,
                        0b00100,
                        0b00100,
                        0b11111,
                        0b01110,
                        0b00100};

char oben[8]={
                        0b00100,
                        0b01110,
                        0b11111,
                        0b00100,
                        0b00100,
                        0b00100,
                        0b00100,
                        0b00100};


