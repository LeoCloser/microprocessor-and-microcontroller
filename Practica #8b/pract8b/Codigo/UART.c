#include <avr/interrupt.h>
#include <inttypes.h>
#define BUFFER_SIZE 64
#define MOD(n) n&(BUFFER_SIZE - 1)
#define IS_BUFFER_EMPTY(buffer) buffer.in_idx == buffer.out_idx
#define IS_BUFFER_FULL(buffer) MOD(buffer.out_idx) == MOD(buffer.in_idx + 1)

typedef struct {
   char buffer[BUFFER_SIZE];    /*Espacio reservador*/
   volatile unsigned char in_idx;  /*indice entrada (Head)*/
   volatile unsigned char out_idx; /*indice entrada (tail)*/
}ring_buffer_t;

ring_buffer_t ring_tx;
ring_buffer_t ring_rx;

void UART0_Init(uint16_t mode)
{
   if(mode == 0)
   {
       UBRR0L = 0x67 ; // 9600,8,N,1 
   } else {
       UBRR0L = 0x33; // 19200,8,N,1 
	   UCSR0A |= (mode<<U2X0);
   }
   
   UCSR0B = (1 << RXEN0) | (1 << TXEN0); /* Habilitar Emisor y Transmicion */
   UCSR0B |=(1 << RXCIE0);
   /* Formato de trasmision: 8datos, 1 stops bits */
   UCSR0C |= (3 << UCSZ00); 
  
} 

uint8_t UART_available(void)
{
   if(ring_rx.in_idx == ring_rx.out_idx)
		return 0;
   else 
		return 1;
}
/*
char UART0_getchar(void)
{
    
	while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
	
}

void UART0_putchar(char data)
{

	while( !( UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}*/


char uart0_getchar(void)
{
     char dato;
	 while( !UART_available() )
	 ;
	  
	 if( UART_available() ){
	      dato = ring_rx.buffer[ring_rx.out_idx];
		  ring_rx.out_idx = MOD(ring_rx.out_idx+1);
	 }

	 return dato;
}

void uart0_putchar(unsigned char data)
{
    if( (MOD(ring_tx.in_idx+1))==(MOD(ring_tx.out_idx)) ){
	    return;
    } else { 
	   ring_tx.buffer[ring_tx.in_idx] = data;
       ring_tx.in_idx = (MOD(ring_tx.in_idx+1));
       UCSR0B |= (1<<UDRIE0); /* activar interrupcion de transmision */
	}
}

void UART0_puts(char *str){
	while( *str )
	    uart0_putchar( *str++ );	

}

ISR( USART0_UDRE_vect ){
   if( ring_tx.in_idx == ring_tx.out_idx )
   {
      UCSR0B &= ~((1 << UDRIE0)); 
	  return;
   } else {
      UDR0 = ring_tx.buffer[ring_tx.out_idx]; /*lo uso para transmitir*/
	  ring_tx.out_idx = (MOD(ring_tx.out_idx+1));
   }
}

ISR( USART0_RX_vect ){ 
   ring_rx.buffer[ring_rx.in_idx] = UDR0;
   ring_rx.in_idx = (MOD(ring_rx.in_idx+1));
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
		tecla = uart0_getchar();
		if(tecla == 0x08 && i>0)
		{
		    str[i] = '\0';
			uart0_putchar(8);
			uart0_putchar(' ');
			uart0_putchar(8);
			i--;
		} else{
		     if(i < 20 && tecla != 0x08)
		     {
			   uart0_putchar(tecla);
			   str[i] = tecla;
			   i++;
			   str[i] = '\0';
		     }
		 } 
	}while(tecla != 13);
}
