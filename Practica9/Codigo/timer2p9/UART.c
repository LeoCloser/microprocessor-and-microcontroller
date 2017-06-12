#include <avr/interrupt.h>
#include <inttypes.h>

void UART0_Init()
{ 
   //UCSR0A |= (mode<<U2X0);
   //UCSR0B = (1 << RXEN0) | (1 << TXEN0); /* Habilitar Emisor y Transmicion */
   /* Formato de trasmision: 8datos, 2 stops bits */
   //UCSR0C = UCSR0C |  (3 << UCSZ00);
    UCSR0A |= (1<<U2X0);

	UCSR0B |= (1<<TXEN0)|(1<<RXEN0); 
	UCSR0B &= ~(1<< UCSZ02);		
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); 
	UCSR0C &= ~((1<<USBS0)|(1<<UPM00)|(1<<UPM01));
} 

char UART0_getchar(void)
{
    
	while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
	
}

void UART0_putchar(char data)
{

	while( !( UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void UART0_puts(char *str){
	while( *str != '\0'){
	    UART0_putchar( *str++ );
    }	

}

/* CAMBIAR ESTOS 3 al formato de la practica */
void itoa(char *str, uint16_t dato, uint8_t base){
	int aux = 0,aux2=0,i;
	do{
		aux = dato%base;
		aux +=0x30;
		if(aux>'9'){
			aux+=0x07;
		}
		str[0] = aux;
		aux2++;
		dato = dato/base;
		if(dato !=0){
			for(i = aux2; i >0; i--){
				str[i] = str[i-1];
			}
		}
	}while(dato!=0);
	str[aux2] = '\0';
}

unsigned int atoi(char *str){
	unsigned char temporal, i=0;
	unsigned int resultado=0;

	while(str[i] != '\x0'){
	   
	   if(str[i] >= '0' && str[i] <= '9'){
	      temporal = str[i] - '0'; /* 0 ASCII = 0*/
		  resultado = (resultado*10) + temporal;
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
		if(tecla == 0x08 && i>0)
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
