#ifndef LCD_LIB_H_
#define LCD_LIB_H_

void delay_loop(uint16_t);					     // For ATxmega32A4
void  _delay_us(uint16_t);					     // For fosc = 16 MHz
void  _delay_ms(uint16_t);					     // For fosc = 16 MHz

void LCDsendChar(unsigned char);			     //forms data ready to send to hd44780
void LCDsendCommand(unsigned char);			 	 //forms data ready to send to hd44780
void LCDinit(void);						         //Initializes LCD
void LCDclr(void);					             //Clears LCD
void LCDhome(void);					             //LCD cursor home
void LCDstring(unsigned char*,unsigned char);	 //Outputs string to LCD
void LCDGotoXY(unsigned char, unsigned char);	 //Cursor to X Y position
void LCDshiftRight(unsigned char);				 //shift by n characters Right
void LCDshiftLeft(unsigned char); 				 //shift by n characters Left
void LCDcursorOn(void); 				         //Underline cursor ON
void LCDcursorOnBlink(void);				     //Underline blinking cursor ON
void LCDcursorOFF(void);				         //Cursor OFF
void LCDblank(void);					         //LCD blank but not cleared
void LCDvisible(void);					         //LCD visible
void LCDcursorLeft(unsigned char);				 //Shift cursor left by n
void LCDcursorRight(unsigned char);				 //shif cursor right by n




#endif /* LCD_LIB_H_ */