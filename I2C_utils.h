/*
 * I2C Utils
 * Biblioteca para I2C utilizando timers e interrupções.
 */

#ifndef I2C_UTILS_H_
#define I2C_UTILS_H_

typedef unsigned char byte;
typedef unsigned int  word;

/**
 * module: qual módulo I2C utilizar
 * - 0: UCB0
 * - 1: UCB1
 *
 * master:
 * - 0: slave
 * - 1: master
 *
 * internal_res:
 * - 0: desabilita resistor pull-up interno.
 * - 1: habilita resistor de pull-up interno.
 *
 * baud_rate:
 * - baud rate em bauds por segundo (bauds/s).
 * - Recomendado: 9600
 * */
void I2C_config(byte module, byte master, byte internal_res, unsigned int baud_rate);
int  I2C_txbyte(byte module, byte slave_address, byte data);
int  I2C_txword(byte module, byte slave_address, word data);

/**
 * Função que lê um byte no canal I2C.
 * Retorna -1 em caso de erro ou o byte lido em caso de sucesso.
 */
int  I2C_rxbyte(byte module, byte slave_address);
int  I2C_rxword(byte module, byte slave_address, word* dest);

#endif /* I2C_UTILS_H_ */
