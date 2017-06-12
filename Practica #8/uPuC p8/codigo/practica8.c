#include <avr/io.h>
#include "UART.h"

int main( void ) { 

    char cad[20]; 
	uint16_t num; 
	UART0_init(1); 
	
	while(1) 
	{ 
	  
	  UART0_getchar();
	  UART0_puts("\n\rIntroduce un número:\n\r"); 
	  
	  UART0_gets(cad); 
	  num = atoi(cad); 
	  itoa(cad,num,16); 
	  UART0_puts("\n\rHex:"); 
	  UART0_puts(cad); 
	  itoa(cad,num,2); 
	  UART0_puts("\n\rBin:"); 
	  UART0_puts(cad);
	
	}
}
