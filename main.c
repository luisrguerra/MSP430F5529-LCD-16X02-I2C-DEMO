#include <msp430.h>
#include <LCD_utils.h>

//MSP-430-EXP430F5529LP
//UTILIZADO: DISPLAY CRISTAL LIQUIDO (LCD 16X02 - AZ/BR) COM MODULO I2C
//https://www.huinfinito.com.br/modulos/1182-display-cristal-liquido-lcd-16x02-azbr-com-modulo-i2c.html
//Necessario fazer o ajuste de contraste para o texto aparecer
//Chip I2C PCF8574T
//SDA -> P3.0
//SCL -> P3.1
//GND -> GND (Ground)
//VCC -> 5V

void esperar(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}

int main(void){

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	I2C_config(0, 1, 1, 9600);
	LCD_config();
	char texto[] = "oi tudo bom?\ncomo vai voce?";
	LCD_print(texto);
	char texto2[] = "Vou bem e voce?";
	esperar(250000);
    LCD_clear();
	LCD_print(texto2);
	return 0;
}
