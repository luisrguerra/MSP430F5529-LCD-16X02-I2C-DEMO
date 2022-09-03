#include "LCD_utils.h"
#include <msp430.h>

#define BT  BIT3
#define EN  BIT2
#define RW  BIT1
#define LCD_I2C_PORT 0

byte LCD_addressList[] = {0x27, 0x3F};
byte LCD_address = 0;

byte LCD_findAddress() {
    byte b = 0;
    for (;b<2;b++)
        if (!I2C_txbyte(LCD_I2C_PORT, LCD_addressList[b], 0xFF))
            return LCD_addressList[b];
    return 0;
}

void LCD_txnibble(byte nibble, byte rs) {
    I2C_txbyte(LCD_I2C_PORT, LCD_address, nibble << 4 | BT | 0  | 0 | rs );
    I2C_txbyte(LCD_I2C_PORT, LCD_address, nibble << 4 | BT | EN | 0 | rs );
    I2C_txbyte(LCD_I2C_PORT, LCD_address, nibble << 4 | BT | 0  | 0 | rs );
}

void LCD_txbyte(byte data, byte rs) {
    LCD_txnibble(data >> 4  , rs);
    LCD_txnibble(data & 0x0F, rs);
}

void LCD_config() {
    while(!LCD_address) {
        LCD_address = LCD_findAddress();
    }

    // Entra em modo 8 bits
    LCD_txnibble(0x3, 0);
    LCD_txnibble(0x3, 0);
    LCD_txnibble(0x3, 0);

    // Entra em modo 4-bits
    LCD_txnibble(0x2, 0);

    // Liga o cursor piscando e liga o display
    LCD_txbyte(0x0C, 0);

    // Return home and clear display
    LCD_txbyte(0x02, 0);
    LCD_txbyte(0x01, 0);
}

void LCD_txchar(char c) {
    LCD_txbyte(c, 1);
}

void LCD_goto(byte pos) {
    LCD_txbyte(pos | 0x80, 0);
}

void LCD_goto_line(byte line) {
    LCD_goto(line ? 0x40 : 0x00);
}

void LCD_clear() {
    LCD_txbyte(0x01, 0);
}

void LCD_print(char* data) {
    char c;
    LCD_goto(0x00);
    while ((c =*data) != 0) {
        if (c == '\n')
            LCD_goto_line(1);
        else LCD_txchar(c);
        data++;
    }
}

