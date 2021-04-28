#include <xc.h> 
void fail(void);

void main (void)
{
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 1;
    TRISDbits.TRISD3 = 0; 
    TRISDbits.TRISD4 = 0;

   PORTDbits.RD0 = 0;
   PORTDbits.RD1 = 0;
   if(PORTDbits.RD2 == 0)
        fail();

   PORTDbits.RD0 = 0;
   PORTDbits.RD1 = 1;
   if(PORTDbits.RD2 == 0)
        fail(); 

   PORTDbits.RD0 = 1;
   PORTDbits.RD1 = 0;
   if(PORTDbits.RD2) == 0)
        fail();

   PORTDbits.RD0 = 1;
   PORTDbits.RD1 = 1;
   if(PORTDbits.RD2 == 0)
        fail();

while(1)
{ 
    PORTDbits.RD3 = 1;
    PORTDbits.RD4 = 0;
}
}

void fail(void)
{
while(1)
{ 
    PORTDbits.RD3 = 0; 
    PORTDbits.RD4 = 1;
}
}