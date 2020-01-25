#ifndef ATCOMMAND_H_
#define ATCOMMAND_H_

//KOMENDY AT//

//LED//
const char cmd_ledon_all[] PROGMEM = {"LEDON"};
const char cmd_ledoff_all[] PROGMEM = {"LEDOFF"};
const char cmd_led[] PROGMEM = {"LED"};
//WYSWIETLACZ 7SEG//
const char cmd_7seg_0[] PROGMEM = {"7SEG0"};
const char cmd_7seg_1[] PROGMEM = {"7SEG1"};
const char cmd_7seg_2[] PROGMEM = {"7SEG2"};
const char cmd_7seg_3[] PROGMEM = {"7SEG3"};
const char cmd_7seg_4[] PROGMEM = {"7SEG4"};
const char cmd_7seg_5[] PROGMEM = {"7SEG5"};
const char cmd_7seg_6[] PROGMEM = {"7SEG6"};
const char cmd_7seg_7[] PROGMEM = {"7SEG7"};
const char cmd_7seg_8[] PROGMEM = {"7SEG8"};
const char cmd_7seg_9[] PROGMEM = {"7SEG9"};
const char cmd_7seg_A[] PROGMEM = {"7SEGA"};
const char cmd_7seg_B[] PROGMEM = {"7SEGB"};
const char cmd_7seg_C[] PROGMEM = {"7SEGC"};
const char cmd_7seg_D[] PROGMEM = {"7SEGD"};
const char cmd_7seg_E[] PROGMEM = {"7SEGE"};
const char cmd_7seg_F[] PROGMEM = {"7SEGF"};
const char cmd_7seg_clear[] PROGMEM = {"7SEGR"};
//LCD//
const char cmd_text[] PROGMEM = {"TEXT"};
const char cmd_clearLCD[] PROGMEM = {"CLEARLCD"};
//INNE/
const char cmd_dip_switch[] PROGMEM = {"DIP"};
const char cmd_potentiometer1[] PROGMEM = {"POT1"};
const char cmd_potentiometer2[] PROGMEM = {"POT2"};

const char * const cmds[] PROGMEM = {cmd_ledon_all, cmd_ledoff_all, cmd_led,
									cmd_7seg_0, cmd_7seg_1, cmd_7seg_2, cmd_7seg_3, cmd_7seg_4, cmd_7seg_5, cmd_7seg_6, cmd_7seg_7, cmd_7seg_8,
									cmd_7seg_9, cmd_7seg_A, cmd_7seg_B, cmd_7seg_C, cmd_7seg_D, cmd_7seg_E, cmd_7seg_F, cmd_7seg_clear,
									cmd_text, cmd_clearLCD, cmd_dip_switch, cmd_potentiometer1, cmd_potentiometer2 };

#endif