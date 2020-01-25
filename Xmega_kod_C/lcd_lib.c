#include <avr/io.h>
#include "lcd_lib.h"

void delay_loop(uint16_t __count)
{
	__asm__ volatile (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"
	: "=w" (__count)
	: "0" (__count)
	);
}


// zmodyfikowane (bardzo uproszczone) funkcje dostêpne w pliku delay.h
void  _delay_us(uint16_t count)
{
   count = count << 2;
   delay_loop(count);		   // count * 4, bo   count * (4 * 4TCLK) = count us   dla fosc = 16 MHz
}

void  _delay_ms(uint16_t count)
{
   if (count > 16) count = 16;	   // zabezpieczenie przed przekroczeniem zakresu int

   count = (count << 2) * 1000;
   delay_loop(count);		   // count * 4, bo   count * (4 * 4TCLK) = count us

}


/*  ***********   Blok funkcji obs³ugi wyœwietlacza LCD  ***********  */


//Funkcja inicju¹ca wyœwietlacz LCD - kod dla ATxmega32A4 - obs³uga LCD na PORTC
/*
PC0 - nieu¿ywane = 0;
PC1 - RS,  0 - instrukcja, 1 - dana
PC2 - R/W, 0 - zapis, 1 - odczyt
PC3 - E,  _|--|_ - strob zapisu/odczytu
-----------------------------------------
PC4-7 - 4-bitowa magistrala danych
*/

void LCDinit(void)
{
       PORTC_OUT = 0x00;           // E=0, R/W=0, RS=0, D4-7=0x00
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(16);

    //--------- Write 0x03 -----------
       PORTC_OUT = 0x30;           // E=0, R/W=0, RS=0, D4-7=0x03
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(5);

    //--------- Write 0x03 -----------
       PORTC_OUT = 0x30;           // E=0, R/W=0, RS=0, D4-7=0x03
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

    //--------- Write 0x03 -----------
       PORTC_OUT = 0x30;           // E=0, R/W=0, RS=0, D4-7=0x03
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

    //--------- Enable Four Bit Mode ----------- 
       PORTC_OUT = 0x20;           // E=0, R/W=0, RS=0, D4-7=0x02 
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

    //---------- Set Interface Length ----------
       PORTC_OUT = 0x20;           // E=0, R/W=0, RS=0, D4-7=0x02 - Write 0x2 - 4 bits
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

       PORTC_OUT = 0x80;           // E=0, R/W=0, RS=0, D4-7=0x08 - Write 0x8 - 2 lines, 5x7 
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

   //---------- Turn off the Display ----------
       PORTC_OUT = 0x00;           // E=0, R/W=0, RS=0, D4-7=0x00 - Write 0x0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

       PORTC_OUT = 0x80;           // E=0, R/W=0, RS=0, D4-7=0x08 - Write 0x8
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

   //------------ Clear the Display -----------
       PORTC_OUT = 0x00;           // E=0, R/W=0, RS=0, D4-7=0x00 - Write 0x0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

       PORTC_OUT = 0x10;                  // E=0, R/W=0, RS=0, D4-7=0x01 - Write 0x1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

   //-------- Set Cursor Move Direction --------
       PORTC_OUT = 0x00;           // E=0, R/W=0, RS=0, D4-7=0x00 - Write 0x0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

       PORTC_OUT = 0x60;           // E=0, R/W=0, RS=0, D4-7=0x06 - Write 0x6 - Increment the Cursor
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

   //---------- Enable Display/Cursor ----------
       PORTC_OUT = 0x00;           // E=0, R/W=0, RS=0, D4-7=0x00 - Write 0x0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

       PORTC_OUT = 0xF0;                  // E=0, R/W=0, RS=0, D4-7=0x0F - Write 0xF - Display on, cursor on, blink on 
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
}			


// Funkcja wysy³a pojedynczy znak do LCD - kod dla ATxmega32A4 - obs³uga LCD na PORTC
/*
PC0 - nieu¿ywane = 0;
PC1 - RS,  0 - instrukcja, 1 - dana
PC2 - R/W, 0 - zapis, 1 - odczyt
PC3 - E,  _|--|_ - strob zapisu/odczytu
-----------------------------------------
PC4-7 - 4-bitowa magistrala danych
*/

void LCDsendChar(unsigned char ch)		
{	 
 
// starszy nibl

       PORTC_OUT = (ch & 0xF0) | 0x02;   // PC4-7 = starszy nibl ch // E=0, R/W=0, RS=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
      _delay_ms(1);

// m³odszy nibl

       PORTC_OUT = ((ch<<4) & 0xF0) | 0x02;     // PC4-7 = m³odszy nibl ch
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
}


// Funkcja wysy³a komendê do LCD - kod dla ATxmega32A4 - obs³uga LCD na PORTC
/*
PC0 - nieu¿ywane = 0;
PC1 - RS,  0 - instrukcja, 1 - dana
PC2 - R/W, 0 - zapis, 1 - odczyt
PC3 - E,  _|--|_ - strob zapisu/odczytu
-----------------------------------------
PC4-7 - 4-bitowa magistrala danych
*/

void LCDsendCommand(unsigned char cmd)	
{

// starszy nibl

       PORTC_OUT = (cmd & 0xF0);  // PC4-7 = starszy nibl ch // E=0, R/W=0, RS=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
      _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);

 // m³odszy nibl

       PORTC_OUT = ((cmd<<4) & 0xF0);    // PC4-7 = m³odszy nibl ch
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       PORTC_OUTSET = 0x08;		   // E=1
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
       PORTC_OUTCLR = 0x08;		   // E=0
       PORTA_OUTSET = 0x10;        // aktywacja uk³adu IC1 (od wysw. LCD)
       PORTA_OUTCLR = 0x10;        // zatrzaœniêcie stanu na wyjœciu IC1
       _delay_ms(1);
}


//Clears LCD
void LCDclr(void)				
{
	LCDsendCommand(0x01);
}


//LCD cursor home
void LCDhome(void)			
{
	LCDsendCommand(0x02);
}


//Outputs string to LCD
void LCDstring(unsigned char* data, unsigned char nBytes) 
{
unsigned char i;

	// check to make sure we have a good pointer
	if (!data) return;

	// print data
	for(i=0; i<nBytes; i++)
	{
		LCDsendChar(data[i]);
	}

}


//Cursor to X Y position
void LCDGotoXY(unsigned char x, unsigned char y)	
{
#define LCD_DDRAM		7	//set DD RAM address

#define LCD_LINE0_DDRAMADDR		0x00
#define LCD_LINE1_DDRAMADDR		0x40
#define LCD_LINE2_DDRAMADDR		0x14
#define LCD_LINE3_DDRAMADDR		0x54

	int DDRAMAddr;
	// remap lines into proper order
	switch(y)
	{
	case 0:  DDRAMAddr = LCD_LINE0_DDRAMADDR + x; break;
	case 1:  DDRAMAddr = LCD_LINE1_DDRAMADDR + x; break;
	case 2:  DDRAMAddr = LCD_LINE2_DDRAMADDR + x; break;
	case 3:  DDRAMAddr = LCD_LINE3_DDRAMADDR + x; break;
	default: DDRAMAddr = LCD_LINE0_DDRAMADDR + x;
	}
	// set data address
	LCDsendCommand(1<<LCD_DDRAM | DDRAMAddr);	
} 


//Scrol n of characters Right
void LCDshiftLeft(unsigned char n)	
{
unsigned char i;

	for (i=0;i<n;i++)
	{
		LCDsendCommand(0x1E);
	}
}


//Scrol n of characters Left
void LCDshiftRight(unsigned char n)	
{
unsigned char i;

	for (i=0;i<n;i++)
	{
		LCDsendCommand(0x18);
	}
}


//displays LCD cursor
void LCDcursorOn(void) 
{
	LCDsendCommand(0x0E);
}


//displays LCD blinking cursor
void LCDcursorOnBlink(void)	
{
	LCDsendCommand(0x0F);
}


//Turns OFF cursor
void LCDcursorOFF(void) 
{
	LCDsendCommand(0x0C);
}


//Blanks LCD
void LCDblank(void)		
{
	LCDsendCommand(0x08);
}


//Shows LCD
void LCDvisible(void)		
{
	LCDsendCommand(0x0C);
}


//Moves cursor by n poisitions left
void LCDcursorLeft(unsigned char n)	
{
unsigned char i;

	for (i=0;i<n;i++)
	{
		LCDsendCommand(0x10);
	}
}


//Moves cursor by n poisitions right
void LCDcursorRight(unsigned char n)	
{
unsigned char i;

	for (i=0;i<n;i++)
	{
		LCDsendCommand(0x14);
	}
}
