//Stand 27.10.2017

#define LCD_RS 11               //LCD-Display Konstanten
#define LCD_E 10
#define LCD_D0 24
#define LCD_D1 5
#define LCD_D2 4
#define LCD_D3 1

#define knopfRechts 22
#define knopfMitte 2
#define knopflinks 7
#define knopfSHUTDOWN 23

#define SENDER 9
#define EMPFAENGER 14
#define RELAY 21

#define TrigEINS 29     //grünes Kabel      Konstanten für den Ultraschallsensor
#define TrigZWEI 28     //weißes Kabel
#define EchoEINS 26
#define EchoZWEI 27

int lcd;
int geschlecht;
int klassenstufe;
int sportart;
long notenstufen[16];
long beginnZeitmessung;

char unten[8];

char oben[8];


