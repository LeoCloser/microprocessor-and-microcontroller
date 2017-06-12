#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "Timer2.h"
#include "UART.h"

/* incluir lo necesario para usar UART0 */
void UART0_AutoBaudRate(void);
void inicializar_puertos(void);
void Timer0_Ini ( void );
void Clock_Update(uint8_t base);
void Clock_Display( void );
void PrintDec2d( uint8_t );
void Clock_ini( uint8_t hrs, uint8_t min, uint8_t seg );

uint8_t seg = 0; /* para el manejo de los segundos */
uint8_t min = 0;
uint8_t hrs = 0;

int main(){
    char str[20];
    uint8_t base = 7;
	/* llamar a función para inicializar puertos E/S */
    inicializar_puertos();
    /* llamar a función para inicializar UART0 */
    UART0_Init();
    UART0_AutoBaudRate();    
    UART0_puts("\n\rAutobauding done. UBRR0=");     
	itoa(str,UBRR0,10);     
    UART0_puts(str);     
    UART0_puts("\n\r"); 
    
	Timer2_Ini(base); /* Inicializar Timer2 para 7 sec.*/
    Clock_Ini(17,20,00);
     while(1)
     { /* == main loop == */
          
		  if( Timer2_Flag() )
		  { /* ¿ha pasado 1 Segundo? */
		      PORTB |= (1<<PB7);
			  
			  Clock_Update(base);
			  Clock_Display();
			  
              PORTB &= ~(1<<PB7);

          }
     } /* fin del loop principal */
 
   return 0; /* <-- no se llega aquí */
} 

void inicializar_puertos()
{
    DDRB |= (1<<DDB7);
	PORTB |= (1<<PB7);

	DDRE &= ~(1<<DDE0);
	PORTE |= (1<<PE0);
}

void UART0_AutoBaudRate(void)
{
	TCNT0 = 0x00;
	TCCR0A = 0x0;
	OCR0A = 0x00;
	
	while((PINE & (1 << PINE0)) == 1); //esperamos a que comienze el start bit, para comenzar a contat ticks
	TCCR0B = 0x02; // (1 << CS01)

	while((PINE & (1 << PINE0)) == 0); //Termina el conteo de ticks
	TCCR0B = 0x00;
	
	//temp = (16000000/(8*TCNT0))-1; 
	/*
	   Formula alterna: 
	   UBRR0 = TCNT0( FOSC / 8(FOSC/8) )-1
	*/	
	UBRR0 = TCNT0-1;
    sei();
}

void Clock_Ini(uint8_t horas, uint8_t minutos, uint8_t segundos){
     hrs = horas;
	 min = minutos; 
	 seg = segundos;
}

void Clock_Update(uint8_t base){
    seg+=base;
	if(seg < 59){
	   //seg++; 
	} else if(min < 59){
	   seg = seg%60; 
	   min++;
	} else if(hrs < 23){
	   min=0;
	   seg=seg%60; 
	   hrs++;
	} else {
	   hrs=0;
	   min=0; 
	   seg=seg%60;
	}
}

void Clock_Display( void ){ /* imprimir reloj en formato "hh:mm:ss" */;
     PrintDec2d(hrs);
	 UART0_putchar(':');
	 PrintDec2d(min);
     UART0_putchar(':');
	 PrintDec2d(seg); /* solo se esta desplegando segundos */
	 UART0_putchar(13);
}


void PrintDec2d( BYTE dato){
    UART0_putchar( dato/10 +'0'); /* Imprime decenas */
    UART0_putchar( dato%10 +'0'); /* Imprime unidades */
}
