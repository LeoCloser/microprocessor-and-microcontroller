#include <avr/interrupt.h>
#include <inttypes.h>
#define BUFFER_SIZE 64
#define MOD(n) n&(BUFFER_SIZE-1)
#define BUFFER_EMPTY(buffer) (buffer.in_idx == buffer.out_idx)
#define BUFFER_FULL(buffer) (MOD(buffer.in_idx+1) == MOD(buffer.out_idx))

typedef struct
{
	char buffer[BUFFER_SIZE];
	volatile unsigned char in_idx;
	volatile unsigned char out_idx;
		
}ring_buffer_t;

ring_buffer_t buffer_tx,buffer_rx;

void UART0_Ini(void){
/* Set baud rate */

	//unsigned char ubrr = 103;
	
	/*UBRR0L = (unsigned char)ubrr;*/
	UCSR0A |= (1<<U2X0);//doble velocidad (menos muestras)
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0); //recepcion y transmicion activadas, interrupcion registro de datos vacio
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
	UCSR0B |= (1 << RXCIE0); 
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); 
	//UCSR0C &= ~((1<<USBS0)|(1<<UPM00)|(1<<UPM01));
	//UBRR0H = 0;
	//sei();//interrupciones globales activadas
} // UART_Init

void UART0_AutoBaudRate(void)
{
	
	TCNT0 = 0;
	TCCR0A = 0;
	OCR0A = 0;
	
	while( ( PINE & (1 << PE0) ) == 1);
	    TCCR0B = (1 << CS01);
	while( ( PINE & (1 << PE0) ) == 0 );
	    TCCR0B = 0x00;
	 
	UBRR0 = TCNT0-1;
	
	//sei(); 

}

void UART_putchar(unsigned char data)
{
	if(BUFFER_FULL(buffer_tx))//(MOD(buffer_tx.in_idx+1) == MOD(buffer_tx.out_idx))
	{
		return;
	}
	else
	{
		buffer_tx.buffer[buffer_tx.in_idx] = data; //dato en cola
		buffer_tx.in_idx = MOD(buffer_tx.in_idx+1); //siguiente posicion
		UCSR0B|=(1<<UDRIE0); //activar interrupcion de transmision	
	}
}

//USART0_TX_vect transmition complete vector

ISR(USART0_UDRE_vect) //USART data register empty rutine
{
	if(BUFFER_EMPTY(buffer_tx))//(buffer_tx.in_idx == buffer_tx.out_idx)
	{//si esta vacio
		UCSR0B&=~((1<<UDRIE0));
		return;
	}
	else
	{
		UDR0 = buffer_tx.buffer[buffer_tx.out_idx];
		buffer_tx.out_idx = MOD(buffer_tx.out_idx+1); //sacamos dato del buffer
	}
}

unsigned char UART0_available(void)
{
	if(buffer_rx.in_idx == buffer_rx.out_idx)
		return 0;
	else
		return 1;
}

ISR(USART0_RX_vect)
{
	buffer_rx.buffer[buffer_rx.in_idx] = UDR0;
	buffer_rx.in_idx = MOD(buffer_rx.in_idx+1);
}

unsigned char UART_getchar()
{
	unsigned char dato = 0;
	while(!UART0_available())
	;
	if(UART0_available())
	{
		dato = buffer_rx.buffer[buffer_rx.out_idx];
		buffer_rx.out_idx = MOD(buffer_rx.out_idx+1);
	}
	return dato;
}

void UART_puts( char *str)
{
	while(*str)
		UART_putchar(*str++);
	
}

void UART_gets(char *str)
{
	unsigned char dato,i=0;
	do
	{
		dato = UART_getchar();
		if(dato == 8 && i != 0)
		{
			UART_putchar(8);
			UART_putchar(' ');
			UART_putchar(8);
			str[i] = '\0';
			i--;
		}
		else
		{
			if(i<=9)
			{
				str[i] = dato;
				UART_putchar(dato);
				i++;
			}	
		}
	}while(dato!=13);
	str[i]= '\x0';
}

unsigned int atoi(char *str)
{
	unsigned char temp,i=0;
	unsigned int acum=0;
	while(str[i]!='\0')
	{
		if(str[i] >= '0' && str[i] <= '9')
		{
			temp=str[i]-48;
			acum = (acum*10)+temp;
			i++;
		}
		else
		{
			return acum;
		}
	};
	return acum;
}

void itoa(char *str,unsigned int num,unsigned int base)
{
	unsigned char temp,i=0,j=0;
	
	do
	{
		temp=num%base;
		temp+=0x30;
		if(temp>0x39)
		{
			temp+=7;
		}
		str[i]=temp;
		num/=base;
		i++;
	}while(num!=0);
	str[i]='\x0';
	do
	{
		i--;
		temp=str[j];
		str[j]=str[i];
		str[i]=temp;
		j++;
	}while(j<i);

}
