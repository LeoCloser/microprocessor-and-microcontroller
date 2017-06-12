#include <avr/interrupt.h>
#include <inttypes.h>

void UART0_Init(uint16_t mode)
{
   if(mode == 0)
   {
       UBRR0L = 0x67 ; /* 9600,8,N,1 103val decimal*/
   } else {
       UBRR0L = 0x33;  /* 19200,8,N,1 51val decimal*/
   }
   //UCSR0A |= (mode<<U2X0);
   UCSR0B = (1 << RXEN0) | (1 << TXEN0); /* Habilitar Emisor y Transmicion */
   /* Formato de trasmision: 8datos, 1 stops bits */
   UCSR0C |= (3 << UCSZ00);
} 

char UART0_getchar(void)
{
    /* UCSR0A se hace 1 verificando a RXC0*/
	while(!(UCSR0A & (1<<RXC0)));  /* Espera a que termine de recibir el dato completo*/
    return UDR0;
	
}

void UART0_putchar(char data)
{
    /* UCSR0A se hace 1 verificando a UDRE0*/
	while( !( UCSR0A & (1<<UDRE0))); /*Espera a que este vacio el buffer*/
	UDR0 = data;
}

void UART0_puts(char *str){
	while( *str )
	    UART0_putchar( *str++ );	

}

void itoa(char *str, uint16_t dato, uint8_t base){
	unsigned char i=0, j=0;
    unsigned char aux;
	unsigned char temp;

	 do{ 
	 	aux = dato % base;
		if(aux > 9)
		   str[i] = aux + '7'; 
		else 
		   str[i] = aux + '0';
		i++;
		dato = dato/base;
	 }while( dato != 0 );
	
	str[i] = '\x0';
    
	/* SWAP array*/
    do{
	   i--;
	   temp = str[j];
	   str[j] = str[i];
	   str[i] = temp;
	   j++;
	}while(j < i);
}

unsigned int atoi(char *str){
	unsigned char temp, i=0;
	unsigned int resultado=0;

	while(str[i] != '\x0')
	{ 
	   if(str[i] >= '0' && str[i] <= '9')
	   {
	      temp = str[i] - '0'; /* 0 ASCII = 0*/
		  resultado = (resultado*10) + temp;
		  i++;
	   } else {
	      return resultado;
	   }

	}
	return resultado;
} 

void UART0_gets(char *str)
{
	int i=0;
	unsigned char tecla;
	do{
		tecla = UART0_getchar();
		if(tecla == 8 && i>0)
		{
		    str[i] = '\0';
			UART0_putchar(8);
			UART0_putchar(' ');
			UART0_putchar(8);
			i--;
		} else{
		     if(i < 20 && tecla != 0x08)
		     {
			   UART0_putchar(tecla);
			   str[i] = tecla;
			   i++;
			   str[i] = '\0';
		     }
		 } 
	}while(tecla != 13);
}

int _strlen(char *str){
	int i=0;
	while(str[i]){
		i++;
	}
	return i;
}
