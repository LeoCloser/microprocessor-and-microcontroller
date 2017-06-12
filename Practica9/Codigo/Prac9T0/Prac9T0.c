#include <avr/io.h>
#include "Timer0.h"
#include "C:\microprocesadores\practica8\UART.h"

/* incluir lo necesario para usar UART0 */
void port_Init(void);
void Clock_Ini(uint8_t horas, uint8_t minutos, uint8_t segundos);
void Clock_Update(void);
void Clock_Display(void);
void PrintDec2d( uint8_t dato); 

uint8_t hrs = 0; 
uint8_t min = 0; 
uint8_t seg = 0;

int main(){
       
	   /* llamar a función para inicializar puertos E/S */
       port_Init();
	   /* llamar a función para inicializar UART0 */
       UART0_init(0);
	   Timer0_Ini(); /* Inicializar Timer0 para 1 sec.*/
	   
       Clock_Ini(23,59,50);
	   while(1){ /* == main loop == */
           
		   if( Timer0_SecFlag() ){ /* ¿ha pasado 1 Segundo? */
                  /* instrucciones para encender LED */
				  
				  PORTB |= (1<<PB7);
                  /*UART0_puts("1 segundo\n\r");*/
                  Clock_Update();
                  Clock_Display();

                  /* instrucciones para apagar LED */
				  PORTB &= ~(1<<PB7);
           }

       } /* fin del loop principal */
 
 return 0; /* <-- no se llega aqui */ 
}

void port_Init(void){
     DDRB |= (1 << DDB7);   /* Salida*/
     PORTB &= ~(1<<PORTB7); /* No pull-up */
}

void Clock_Ini(uint8_t horas, uint8_t minutos, uint8_t segundos){
     hrs = horas;
	 min = minutos; 
	 seg = segundos;
}

void Clock_Update(void){
    if(seg < 59){
	   seg++; 
	} else if(min < 59){
	   seg=0; 
	   min++;
	} else if(hrs < 23){
	   min=0;
	   seg=0; 
	   hrs++;
	} else {
	   hrs=0;
	   min=0; 
	   seg=0;
	}
}

void Clock_Display(void){
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
