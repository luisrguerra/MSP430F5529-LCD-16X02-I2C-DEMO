#include <I2C_utils.h>
#include <msp430.h>

void __I2C_config_m0(byte master, byte internal_res, unsigned int baud_rate) {
    UCB0CTL1 |= UCSWRST;            // Reset no USCI
    UCB0CTL0 |= UCMODE_3            // Modo I2C
             |  UCSYNC              // Sync
             |  UCMST;              // Master
    UCB0CTL0 &= ~UCA10;             // 7 bit own address
    UCB0CTL0 &= ~UCSLA10;           // 7 bit slave address
    UCB0CTL1 &= ~UCSSEL_3;          // Clear clock selection
    UCB0CTL1 |= UCSSEL__SMCLK;      // Select SM clock (1MHz)
    UCB0BRW   =  100;               // Freq = 1M / 10 = 100k
    P3SEL    |= (BIT0 | BIT1);      // Usa os pinos P3.0 e P3.1 como periférico
    P3REN    |= (BIT0 | BIT1);      // Habilita resistores
    P3OUT    |= (BIT0 | BIT1);      // Pull-up
    UCB0CTL1 &= ~UCSWRST;           // Fim do reset no USCI
}

int __I2C_config_m1(byte master, byte internal_res, unsigned int baud_rate) {

}

int __I2C_txbyte_m0(byte slave_address, byte data) {
    UCB0I2CSA = slave_address;              // slave address
    UCB0CTL1 |= UCTR;                       // transmitter mode
    UCB0CTL1 |= UCTXSTT;                    // start condition
    while (!(UCB0IFG & UCTXIFG));           // wait start
    UCB0TXBUF = data;

    while(!(UCB0IFG & UCTXIFG) &&           // Aguarda ACK/NACK
          !(UCB0IFG & UCNACKIFG));

    if(UCB0IFG & UCNACKIFG) {
        UCB0CTLW0 |= UCTXSTP;               // Se o escravo nÆo respondeu
        while(UCB0CTLW0 & UCTXSTP);         // eu pe‡o um stop e espero o stop
        return 1;                           // ser enviado antes de retornar
    }

    while(!(UCB0IFG & UCTXIFG));            // Espera enviar o £ltimo byte para
    UCB0CTLW0 |= UCTXSTP;                   // pedir um stop

    while(UCB0CTLW0 & UCTXSTP);             // Espera o stop ser enviado para retornar

    return 0;                               // Retorno = 0 significa sucesso
}

int __I2C_txword_m0(byte slave_address, word data) {
    UCB0I2CSA = slave_address;              // slave address
    UCB0CTL1 |= UCTR;                       // transmitter mode
    UCB0CTL1 |= UCTXSTT;                    // start condition
    while (!(UCB0IFG & UCTXIFG));           // wait start
    UCB0TXBUF = data>>8;

    while(!(UCB0IFG & UCTXIFG) &&           // Aguarda ACK/NACK
          !(UCB0IFG & UCNACKIFG));

    if(UCB0IFG & UCNACKIFG) {
        UCB0CTLW0 |= UCTXSTP;               // Se o escravo nÆo respondeu
        while(UCB0CTLW0 & UCTXSTP);         // eu pe‡o um stop e espero o stop
        return 1;                           // ser enviado antes de retornar
    }

    while(!(UCB0IFG & UCTXIFG));            // Espera enviar o £ltimo bit para
    UCB0TXBUF = data&0xFF;                  // enviar o segundo byte

    while(!(UCB0IFG & UCTXIFG));            // Espera enviar o £ltimo bit para
    UCB0CTLW0 |= UCTXSTP;                   // pedir um stop

    while(UCB0CTLW0 & UCTXSTP);             // Espera o stop ser enviado para retornar

    return 0;                               // Retorno = 0 significa sucesso
}

int __I2C_rxbyte_m0(byte slave_address) {
    UCB0CTL1 &= ~UCTR;                      // receiver mode
    UCB0I2CSA = slave_address;              // slave address

    //Espero a linha estar desocupada.
    while (UCB1STAT & UCBBUSY);

    UCB0CTL1 |= UCTXSTT;                    // start condition

    while((UCB0CTL1 & UCTXSTT) &&           // Aguarda fim do START/NACK
          !(UCB0IFG & UCNACKIFG));

    if (UCB0IFG & UCNACKIFG) {
        UCB0CTLW0 |= UCTXSTP;               // pedir um stop
        while(UCB0CTLW0 & UCTXSTP);         // eu peço um stop e espero o stop
        return -1;                          // ser enviado antes de retornar
    }
    UCB0CTLW0 |= UCTXSTP;                   // pedir um stop
    while(!(UCB0IFG & UCRXIFG));            // Aguarda a leitura

    int data = UCB0RXBUF<<8;                // Lê dados do buffer de recepção

    while(UCB0CTLW0 & UCTXSTP);             // Espera o stop ser enviado para retornar

    return data;                            // Retorna os dados
}

int __I2C_rxword_m0(byte slave_address, word* dest) {
    UCB0CTL1 &= ~UCTR;                      // receiver mode
    UCB0I2CSA = slave_address;              // slave address

    //Espero a linha estar desocupada.
    while (UCB1STAT & UCBBUSY);

    UCB0CTL1 |= UCTXSTT;                    // start condition

    while((UCB0CTL1 & UCTXSTT) &&           // Aguarda fim do START/NACK
          !(UCB0IFG & UCNACKIFG));

    if (UCB0IFG & UCNACKIFG) {
        UCB0CTLW0 |= UCTXSTP;               // pedir um stop
        while(UCB0CTLW0 & UCTXSTP);         // eu peço um stop e espero o stop
        return -1;                          // ser enviado antes de retornar
    }

    while(!(UCB0IFG & UCRXIFG));            // Aguarda a leitura

    int data = UCB0RXBUF<<8;                // Lê dados do buffer de recepção
    UCB0CTLW0 |= UCTXSTP;                   // pedir um stop

    while(!(UCB0IFG & UCRXIFG));            // Aguarda a leitura
    data |= UCB0RXBUF;

    while(UCB0CTLW0 & UCTXSTP);             // Espera o stop ser enviado para retornar

    *dest = data;
    return 0;                               // Retorna os dados
}

int __I2C_txbyte_m1(byte slave_address, byte data) {
    return 1;
}

int __I2C_rxbyte_m1(byte slave_address) {
    return -1;
}

int __I2C_txword_m1(byte slave_address, word data) {
    return 1;
}

int __I2C_rxword_m1(byte slave_address, word* dest) {
    return -1;
}

void I2C_config(byte module, byte master, byte internal_res, unsigned int baud_rate) {
    if (module == 0) {
        __I2C_config_m0(master, internal_res, baud_rate);
    }
    if (module == 1) {
        __I2C_config_m1(master, internal_res, baud_rate);
    }
}

int  I2C_txword(byte module, byte slave_address, word data) {
    if (module == 0) {
        return __I2C_txword_m0(slave_address, data);
    }
    if (module == 1) {
        return __I2C_txword_m1(slave_address, data);
    }
}

int I2C_txbyte(byte module, byte slave_address, byte data) {
    if (module == 0) {
        return __I2C_txbyte_m0(slave_address, data);
    }
    if (module == 1) {
        return __I2C_txbyte_m1(slave_address, data);
    }
}

int I2C_rxbyte(byte module, byte slave_address) {
    if (module == 0) {
        return __I2C_rxbyte_m0(slave_address);
    }
    if (module == 1) {
        return __I2C_rxbyte_m1(slave_address);
    }
}

int I2C_rxword(byte module, byte slave_address, word* dest) {
    if (module == 0) {
        return __I2C_rxword_m0(slave_address, dest);
    }
    if (module == 1) {
        return __I2C_rxword_m1(slave_address, dest);
    }
}
