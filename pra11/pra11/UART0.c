#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_SIZE 64
#define MOD(n) n&(BUFFER_SIZE-1)
#define IS_BUFFER_EMPTY(buffer) buffer.in_idx == buffer.out_idx
#define IS_BUFFER_FULL(buffer) buffer.in_idx == MOD(buffer.out_idx - 1)

typedef struct
{
	char buffer[BUFFER_SIZE];				/*	espacio reservado			*/
	volatile unsigned char in_idx :6;		/*	indice entrada (Head)		*/
	volatile unsigned char out_idx :6;		/*	indice entrada (tail)		*/
} ring_buffer_t;

ring_buffer_t bufferRx;
ring_buffer_t bufferTx;

void UART0_Ini( void )
{
	UCSR0A = 1<<U2X0;					//doble velocidad asincrona UART0
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);		/*		transmision, recepcion     */
	UCSR0C = (3<<UCSZ00);
	sei();								/*		Ativa las interrupciones glovales						*/
	
	UCSR0B|=(1<<RXCIE0);		/*		Activa interrupcion de recepcion completa				*/
	bufferRx.in_idx = 0;
	bufferRx.out_idx = 0;
	bufferTx.in_idx = 0;
	bufferTx.out_idx = 0;
}

void UART0_putchar(char data)
{
	while((IS_BUFFER_FULL(bufferTx)));				/* espera si la cola esta llena*/
	
	if(IS_BUFFER_EMPTY(bufferTx))					/* si la cola esta vacia */
	{
		bufferTx.buffer[bufferTx.in_idx++] = data;  /* ingresa el dato en la cabeza y aumenta*/
		UCSR0B = UCSR0B|(1<<UDRIE0);				/* Activar interrupcion Transmision completa*/
	}
	else
	{
		bufferTx.buffer[bufferTx.in_idx++] = data;  /*ingresa el dato en la cabeza y aumenta*/
	}
}

ISR( _VECTOR(26) )									/* Registro de transmision vacio */
{
	if(IS_BUFFER_EMPTY(bufferTx))					/* si la cola esta vacia */
		UCSR0B = UCSR0B&(~(1<<UDRIE0));				/*desactivar la interrupcion*/
	else
		UDR0 = bufferTx.buffer[bufferTx.out_idx++];	/* Manda el dato de la cola al UDR0*/

}

uint8_t UART0_available( void )				/* Retorna 1 si existe datos en la cola */
{
	return IS_BUFFER_EMPTY(bufferRx) ? 0:1;
}

char UART0_getchar( void )
{
	while((IS_BUFFER_EMPTY(bufferRx)));					/*	espera si no hay datos en la cola */
	
	return bufferRx.buffer[bufferRx.out_idx++];			/*	Regrasa el dato				*/
}

ISR( _VECTOR(25) )										/* Recepcion completa */
{
	while(IS_BUFFER_FULL(bufferRx));					/* espera si esta llena la cola */
	
	bufferRx.buffer[bufferRx.in_idx++] = UDR0;
}

void UART0_puts(char *str)
{
	while(*str)
	UART0_putchar(*str++);
}


void itoa(char* str, uint16_t number, uint8_t base)
{
	char aux[20];
	int i=0;
	if(number == 0)
	*str++ = 48;
	aux[i] = number%base;
	i++;
	while(number)
	{
		aux[i] = number%base;
		number =number/base;
		i++;
	}
	i--;
	for(i; i!=0; i--)
	{
		if(aux[i] > 9)
		{
			aux[i] += 7;
		}
		*(str++) = aux[i]+48;
	}
	*(str++) = 0;  //cierre de cadena para evitar basura
}



