#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include <stdbool.h>
#include "RingBuffer.h"
#include "SendCommands.h"
#include "lcd_lib.h"


//ZMIENNE GLOBALNE//
#define MAX_CMD_LEN 40      //Maksymalna d³ugoœæ polecenia w bajtach
bool TxFlag=false;
uint8_t cmdrec=false;
CircBuffer recBuf, sendBuf;
unsigned int v_res;
volatile unsigned char danaADC0, danaADC1;
unsigned char read_DIP_switch_data, result, end_transmit;

/* *** Tablica znaków dla wyswietlacza 7-segmentowego *** */
//  '0' , '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
const unsigned char buf[16]={0x21,0xF5,0x13,0x91,0xC5,0x89,0x09,0xF1,0x01,0x81,0x41,0x0D,0x2B,0x15,0x0B,0x4B};
	
//DEKLARACJE FUNCKJI//
void avr_init(void);
void serviceSW3(void);
void serviceSW2(void);
void SendDataLCD(void);
void commandFunction(void);
void SendPotentiometer1(void);
void SendPotentiometer2(void);
void send8LED(unsigned char bajt);
void send7SEG(unsigned char bajt);
void USART_putchar(USART_t * const usart, char ch);
void USART_send_buf_F(CircBuffer *buf, const char *txt);
unsigned char ReadDip8(void);
unsigned char receiveLEDByte(void);
uint8_t TranslateCommand(void);
uint8_t read_calibration_row(uint16_t addr);
bool GetToken(char txt[MAX_CMD_LEN], uint8_t len);

//FUNKCJA GLOWNA//
int main(void)
{
	avr_init();

	while(1)
	{
		if(cmdrec)		//czy coœ jest w buforze pierœcieniowym
		{
			commandFunction();			//ROZPOZNAJ KOMENDE AT//
		}
		serviceSW2();			    // sprawdzenie stanu przycisku SW2
		serviceSW3();			    // sprawdzenie stanu przycisku SW3
	}
}

//PRZERWANIA//
ISR(USARTD1_RXC_vect)
{
	uint8_t ch=USARTD1_DATA;
	if(ch=='\n')
	{
		ch=0;
		++cmdrec;
	}
	cb_Add(&recBuf, ch);
}

ISR(USARTD1_TXC_vect)
{
	if(!cb_IsEmpty(&sendBuf))
	{
		USARTD1_DATA=cb_Read(&sendBuf);
		TxFlag=true;
	} else TxFlag=false;
}

ISR(PORTD_INT0_vect)
{
	loop_until_bit_is_set(PORTD_IN,0);      // czekaj na puszczenie klawisza SW0
	USART_send_buf_F(&sendBuf, PSTR("0Wybrany przycisk SW0\r\n"));
}

ISR(PORTD_INT1_vect)
{
	loop_until_bit_is_set(PORTD_IN,1);      // czekaj na puszczenie klawisza SW1
	USART_send_buf_F(&sendBuf, PSTR("1Wybrany przycisk SW1\r\n"));
}

// ADC A CH0 interrupt - odczytanie wyniku
ISR(ADCA_CH0_vect)
{
	v_res = ADCA_CH0RES;		    //	odczyt wyniku pomiaru napiêcia na kanale CH0
	danaADC0 = (unsigned char)(v_res = v_res>> 8);     // konwersja na bajt
}

// ADC A CH1 interrupt - odczytanie wyniku 
ISR(ADCA_CH1_vect)
{
	v_res = ADCA_CH1RES;		   //  odczyt wyniku pomiaru napiêcia na kanale CH1
	danaADC1 = (unsigned char)(v_res = v_res>> 8);     // konwersja na nibl
}

//DEFINICJE FUNKCJI//
void commandFunction(void)						// FUNKCJA ROZPOZNAJACA KOMENDY AT//
{
	switch(TranslateCommand())
	{	//LED//
		case 0: USART_send_buf_F(&sendBuf, PSTR("ALL LED ON\r\n"));
		send8LED(0x00);	break;
		case 1:  USART_send_buf_F(&sendBuf, PSTR("ALL LED OFF\r\n"));
		send8LED(0xFF);	break;
		case 2: USART_send_buf_F(&sendBuf, PSTR("Ustawiono bajt na LED\r\n"));
		send8LED(receiveLEDByte()); break;
		//7SEG//
		case 3: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 0\r\n"));
		send7SEG(0x00); break;
		case 4: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 1\r\n"));
		send7SEG(0x01); break;
		case 5: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 2\r\n"));
		send7SEG(0x02); break;
		case 6: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 3\r\n"));
		send7SEG(0x03); break;
		case 7: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 4\r\n"));
		send7SEG(0x04); break;
		case 8: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 5\r\n"));
		send7SEG(0x05); break;
		case 9: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 6\r\n"));
		send7SEG(0x06); break;
		case 10: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 7\r\n"));
		send7SEG(0x07); break;
		case 11: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 8\r\n"));
		send7SEG(0x08); break;
		case 12: USART_send_buf_F(&sendBuf, PSTR("Ustawiono 9\r\n"));
		send7SEG(0x09); break;
		case 13: USART_send_buf_F(&sendBuf, PSTR("Ustawiono A\r\n"));
		send7SEG(0x0A); break;
		case 14: USART_send_buf_F(&sendBuf, PSTR("Ustawiono B\r\n"));
		send7SEG(0x0B); break;
		case 15: USART_send_buf_F(&sendBuf, PSTR("Ustawiono C\r\n"));
		send7SEG(0x0C); break;
		case 16: USART_send_buf_F(&sendBuf, PSTR("Ustawiono D\r\n"));
		send7SEG(0x0D); break;
		case 17: USART_send_buf_F(&sendBuf, PSTR("Ustawiono E\r\n"));
		send7SEG(0x0E); break;
		case 18: USART_send_buf_F(&sendBuf, PSTR("Ustawiono F\r\n"));
		send7SEG(0x0F); break;
		case 19: USART_send_buf_F(&sendBuf, PSTR("Wyswietlacz wylaczony\r\n"));
		PORTC_OUT = 0xFF;		   // wystawienie zmiennej na port C
		PORTA_OUTSET = 0x20; 	   // aktywacja uk³adu IC2 (od wysw. 7-seg.)
		PORTA_OUTCLR = 0x20; 	   // zatrzaœniêcie stanu na wyjœciu IC2 break;
		break;
		//LCD//
		case 20: USART_send_buf_F(&sendBuf, PSTR("Tekst wyslany\r\n"));
		LCDclr(); SendDataLCD(); break;
		case 21: USART_send_buf_F(&sendBuf, PSTR("Ekran wyczyszczony\r\n"));
		LCDclr(); break;
		//INNE//
		case 22: read_DIP_switch_data = ReadDip8();
		USART_putchar(&USARTD1,read_DIP_switch_data); break;
		case 23: SendPotentiometer1(); break;
		case 24: SendPotentiometer2(); break;
		default: break;
	}
}
void USART_send_buf_F(CircBuffer *buf, const char *txt)			//FUNCJA WYSYLAJACA TEKST//
{
	uint8_t ch;
	while ((ch=pgm_read_byte(txt++))!=0)
	cb_Add(&sendBuf, ch);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if(!TxFlag) USARTD1_TXC_vect();
	}
}

void USART_putchar(USART_t * const usart, char ch)				//FUNCJA WYSYLAJACA BAJT//
{
	while(!(usart->STATUS & USART_DREIF_bm));
	usart->DATA=ch;
}

uint8_t TranslateCommand(void)									//FUNKCJA KONWERTUJACA ODEBRANE BAJTY//
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)   //Licznik odebranych komend
	{
		if(cmdrec) --cmdrec;
	}
	
	uint8_t indeks;
	char txt[MAX_CMD_LEN];
	if(GetToken(txt, MAX_CMD_LEN)==false) return -1;  //Wyst¹pi³ b³¹d
	
	for(indeks=0; indeks<sizeof(cmds)/sizeof(cmds[0]); indeks++)  //Porównaj token z list¹
			if(strcmp_P(txt, (char*)pgm_read_word(&cmds[indeks]))==0) break;
	
	return indeks;
}

bool GetToken(char txt[MAX_CMD_LEN], uint8_t len)
{
	char ch;
	for(uint8_t i=0; i<15; i++)        //Przepisz token do zmiennej txt w formacie NULLZ
	{
		if(cb_IsEmpty(&recBuf)) return false; //B³¹d
		ch=cb_Read(&recBuf);
		if(ch==' ') ch=0;
		txt[i]=ch;
		if(ch==0) break;
	}
	return true;
}

uint8_t read_calibration_row(uint16_t addr)					//FUNCKJA KALIBRUJACA, POTRZEBNA DO ODCZYTU POTENCJOMETROW//
{
	uint8_t result;
	asm(
	"ldi r19,0x02 \n"
	"ldi r27,0x01 \n"
	"ldi r26,0xCA \n"
	"st x,r19 \n"

	"lpm %0, Z" "\n\t"

	"ldi r19,0x00 \n"
	"st x,r19 \n"

	: "=r" (result) : "z" (addr) );

	return(result);
}

unsigned char ReadDip8(void)				//FUNKCJA ODCZYTUJACA DIPSWITCH//
{
	uint8_t bajt;
	cli();			   // blokada przerwan
	PORTC_DIR = 0x00;		   // ustawienie portu C jako wejœciowy
	PORTA_OUTCLR = 0x80; 	   // aktywacja uk³adu IC4 (od DIP-8)
	bajt = PORTC_IN;		   // odczyt z pinów stanu prze³¹czników DIP SWITCH
	bajt = PORTC_IN;		   // odczyt z pinów stanu prze³¹czników DIP SWITCH - dodatkowy
	PORTA_OUTSET = 0x80; 	   // dezaktywacja uk³adu IC4
	PORTC_DIR = 0xFF;		   // ustawienie portu C na wyjœciowy
	sei();			   // globalne odblokowanie przerwañ
	return bajt;
}

void send8LED(unsigned char bajt)					//FUNKCJA USTAWIAJACA LEDY//
{
	PORTC_OUT = bajt;		   // wystawienie zmiennej na port C
	PORTA_OUTSET = 0x40; 	   // aktywacja uk³adu IC3 (od linijki LED)
	PORTA_OUTCLR = 0x40; 	   // zatrzaœniêcie stanu na wyjœciu IC3
	return;
}

unsigned char receiveLEDByte(void)							//FUNKCJA ODBIERAJACA BAJT LED//
{
	unsigned char receivedByte[2];
	unsigned char x=0;
	while((!cb_IsEmpty(&recBuf)))
	{
		receivedByte[x] = cb_Read(&recBuf);
		x++;
	}
	return receivedByte[0];
	
}

void send7SEG(unsigned char bajt)				//FUNKCJA USTAWIAJACA 7SEGMENT//
{
	bajt &= 0x0F;		   // u¿ycie maski 0x0F - zerowanie czterech najstarszych bitów
	result = buf[bajt];		   // konwersja na kod wys. 7-seg.
	PORTC_OUT = result;		   // wystawienie zmiennej na port C
	PORTA_OUTSET = 0x20; 	   // aktywacja uk³adu IC2 (od wysw. 7-seg.)
	PORTA_OUTCLR = 0x20; 	   // zatrzaœniêcie stanu na wyjœciu IC2
	return;
}

void SendDataLCD(void)							//FUNKCJA USTAWIAJACA EKRAN LCD//
{
	unsigned char x=0;
	char txt[32];
	while((!cb_IsEmpty(&recBuf)))
	{
		txt[x] = cb_Read(&recBuf);
		if(txt[x] == '\n') return;		//jesli wykryje 0, to zakoncz wysylanie
		if(x<=16)
		{
			LCDGotoXY(x-1,0);
			LCDsendChar(txt[x-1]);
		}
		else if(x>16 && x<=32)
		{
			LCDGotoXY(x-17,1);
			LCDsendChar(txt[x-1]);
		}
		else return;
		x++;
	}
}

void SendPotentiometer1(void)				//FUNKCJA ODCZYTUJACA POTENCJOMETR 1//
{
	ADCA_CH0_CTRL = 0x81;		    // start konwersji, wejœcie dodatnie na kanale CH0
	USART_putchar(&USARTD1, danaADC0);
}

void SendPotentiometer2(void)				//FUNKCJA ODCZYTUJACA POTENCJOMETR 2//
{
	ADCA_CH1_CTRL = 0x81;		    // start konwersji, wejœcie dodatnie na kanale CH1
	USART_putchar(&USARTD1, danaADC1);
}

void serviceSW2(void)					//FUNKCJA REAGUJACA NA WCISNIECIE SW2//
{
	if bit_is_clear(PORTD_IN,2)		 // testowanie klawisza SW2 - gdy naciœniêty to jest stan niski
	{
		loop_until_bit_is_set(PORTD_IN,2);  // oczekiwanie na puszczenie klawisza SW2
		USART_send_buf_F(&sendBuf, PSTR("2Wybrany przycisk SW2\r\n"));
		return;
	}
}

void serviceSW3(void)					//FUNKCJA REAGUJACA NA WCISNIECIE SW3//
{
	if bit_is_clear(PORTD_IN,3)		 // testowanie klawisza SW3 - gdy naciœniêty to jest stan niski
	{
		loop_until_bit_is_set(PORTD_IN,3);  // oczekiwanie na puszczenie klawisza SW3
		USART_send_buf_F(&sendBuf, PSTR("3Wybrany przycisk SW3\r\n"));
		return;
	}
}

//FUNCKJA INICJALIZUJACA//
void avr_init(void)
{
	// Ustawienie zegara systemowego
	OSC_XOSCCTRL = 0xCB;			    // zakres 12 - 16 MHz, 16K clk start-up time
	OSC_CTRL |= 0x08;				    // External Oscillator (EO) mo¿liwy do wykorzystania
	do {} while((OSC_STATUS & 0x08) == 0);	    // czekaj a¿ EO bêdzie gotowy

	//CPU_CCP = 0xD8;				    // odblokuj rejest przed zapisem
	//CLK_CTRL = 0x03;				    // prze³¹cz system zegarowy na EO
	asm ("ldi r16,0xD8 \n"
	"ldi r19,0x03 \n"
	"ldi r27,0x00 \n"
	"ldi r26,0x40 \n"
	"out 0x34,r16 \n"
	"st x,r19 \n"
	: : );

	// Porty: A, B, C, D, E - kierunek linii: 0-wejœcie 1-wyjœcie
	PORTA_DIR = 0xF2;		     // linie PA7-PA4, PA1 wyjœciami, PA3,PA2 i PA0 wejœciami (analogowymi)
	PORTA_OUTSET = 0x80; 	     // sterowanie lini¹ PA7 - dezaktywacja uk³adu IC4
	PORTB_DIR = 0x0C;		     // Port B - wejœcie
	PORTC_DIR = 0xFF;		     // port C - wyjœciowy
	PORTD_DIR = 0x80;		     //
	PORTE_DIR = 0x00;		     //

	// wygaszenie diod po resecie /
	PORTC_OUT  = 0xFF;		    // wystawienie stanu wysokiego na diody LED
	PORTA_OUTSET = 0x40; 	    // aktywacja uk³adu IC3 (od linijki LED)
	PORTA_OUTCLR = 0x40; 	    // zatrzaœniêcie stanu na wyjœciu IC3

	// konfiguracja interfejsu szeregowego UART na porcie D1
	USARTD1_BAUDCTRLA = 0x67;	    // 103 - daje 9600 b/s przy 16 MHz sygna³u taktuj¹cego CPU
	USARTD1_BAUDCTRLB = 0x00;
	USARTD1_CTRLA = 0x14;	    // Low level interrupt Rx i TX odblokowane
	USARTD1_CTRLC = 0x03;	    // Tryb asynchroniczny, 8-bit dana
	USARTD1_CTRLB = 0x18;	    // W³¹czenie uk³adów odbiornika RX i nadajnika TX

	// przerwania zewnêtrzne INT0 i INT1 na PORT D /
	PORTD_INTCTRL = 0x05;	    // niski poziom przerwañ INT0 i INT1
	PORTD_INT0MASK = 0x01;	    // PD0 - INT0
	PORTD_INT1MASK = 0x02;	    // PD1 - INT1
	PORTD_PIN0CTRL = 0x01;	    // INT0 na zbocze narastaj¹ce
	PORTD_PIN1CTRL = 0x01;	    // INT1 na zbocze narastaj¹ce
	PMIC_CTRL = 0x01;		    // Low level interrupt odblokowane

	// Kalibracja przetwornika ADCA    // ADACAL0 - 0x20; ADACAL1 - 0x21;

	danaADC0 = (read_calibration_row(0x21)<<8) |  read_calibration_row(0x20);
	danaADC1 = (read_calibration_row(0x21)<<8) |  read_calibration_row(0x20);
	ADCA_CAL = danaADC0; 	    // ADCA_CAL = 0x0444;
	
	// ADC A
	
	ADCA_CTRLB = 0x06;		    // wynik bez znaku, 12-bitowa rozdzielczoœæ, justowanie w lewo
	ADCA_REFCTRL = 0x20; 	    // zewnêtrzne napiêcie referencyjne z pinu AREF na PORT A
	ADCA_PRESCALER = 0x02;	    // podzia³ sygna³u zegarowego DIV16 (1 MHz)

	ADCA_CH0_CTRL = 0x01;	    // wejœcie dodatnie na kanale CH0
	ADCA_CH1_CTRL = 0x01;	    // wejœcie dodatnie na kanale CH1

	ADCA_CH0_MUXCTRL = 0x10;	    // kana³ CH0 na pinie ADC2
	ADCA_CH1_MUXCTRL = 0x18;	    // kana³ CH1 na pinie ADC3

	ADCA_CH0_INTCTRL = 0x01;	    // Low level interrupt na kanale CH0
	ADCA_CH1_INTCTRL = 0x01;	    // Low level interrupt na kanale CH1

	ADCA_CTRLA = 0x01;		    // w³¹czenie ADC A
	
	LCDinit();
	LCDclr();
	LCDcursorOFF();
	sei();			    // odblokowanie przerwañ
	
	return;
}