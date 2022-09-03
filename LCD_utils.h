#ifndef LCD_UTILS_H_
#define LCD_UTILS_H_
#include "I2C_utils.h"

byte LCD_findAddress();
void LCD_txnibble(byte nibble, byte rs);
void LCD_txbyte(byte data, byte rs);
void LCD_txchar(char c);
void LCD_goto(byte pos);
void LCD_goto_line(byte line);
void LCD_clear();
void LCD_print(char* data);
void LCD_config();

#endif /* LCD_UTILS_H_ */
