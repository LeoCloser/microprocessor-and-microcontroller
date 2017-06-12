#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "Timer2.h"
#include "UART.h"

/* incluir lo necesario para usar UART0 */
void inicializar_puertos(void);
void Timer0_Ini ( void );
void Clock_Update(void);
void Clock_Display( void );
void PrintDec2d( uint8_t );
void Clock_ini( uint8_t hrs, uint8_t min, uint8_t seg );

uint8_t seg = 0; /* para el manejo de los segundos */
uint8_t min = 0;
uint8_t hrs = 0;

int main(){

	/* llamar a función para inicializar puertos E/S */
    inicializar_puertos();
    /* llamar a función para inicializar UART0 */
    UART0_Init(0);   
    UART0_puts("\n\r"); 
    
	Timer2_Ini(); /* Inicializar Timer2 para 1 sec.*/
    Clock_Ini(23,22,30);
     while(1)
     { /* == main loop == */
          
		  if( Timer2_Flag() )
		  { /* ¿ha pasado 1 Segundo? */
		      PORTB |= (1<<PB7);
			  
			  Clock_Update();
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
}

void Clock_Ini( uint8_t horas, uint8_t minutos, uint8_t segundos ){
     hrs = horas;
	 min = minutos; 
	 seg = segundos;
}

void Clock_Update( void ){
	if(seg < 59){
	   seg++;
	} else if(min < 59){
	   seg=0;
	   min++;
	} else if(hrs < 23){
	   min=0;
	   hrs++;
	} else {
	   hrs=0;
	   min=0; 
	   seg=0;
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


void PrintDec2d( uint8_t dato){
    UART0_putchar( dato/10 +'0'); /* Imprime decenas */
    UART0_putchar( dato%10 +'0'); /* Imprime unidades */
}
