/*	Definir el macro que calcula los ticks en base
	a al parametro de frecuencia (f). */
#include <inttypes.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "UART.h"

#define FOSC 16000000
#define Ft FOSC/256
#define TICKS(f) Ft/f

volatile static unsigned char segFlag;
volatile static unsigned int seg;  
volatile static unsigned int miliseconds;
volatile static unsigned int miliSilence;

const struct note *estructura=0;

volatile static unsigned int i;
volatile static unsigned int songSZ; 
volatile static unsigned int volumen; /* Variable global especial para volumen*/
volatile static unsigned int silencio; 

uint8_t Timer0_SecFlag ( void ){

    if( segFlag )
	{        
		segFlag=0;
        return 1;
    } else{
        return 0;
    }
}

void Timer0_Ini ( void ){
	/* 	Permanece igual, ocasionando una interrupción 
		cada 1 ms en modo CTC. */
    TCNT0 = 0x00;
	TCCR0A = (1<<WGM01); /* MODO CTC */  
	OCR0A = 250 - 1; // 250-1 249
	TCCR0B = (1<<CS01) | (1<<CS00); /* Prescalador 64*/
    TIMSK0 = (1<<OCIE0A);
}

ISR( TIMER0_COMPA_vect ){ 
	/* 	Código para actualizar bandera de segundos */
    miliseconds++;
	if(miliseconds == 1000){
	   miliseconds = 0;
	   seg++;
	}
	miliSilence++;    
	/*	Agregar las instrucciones necesarias para reproducir
		la siguiente nota en el arreglo dependiendo de la duración, 
		e insertar los silencios entre cada nota. */
    if(i < songSZ){
	     if(miliSilence == estructura[i].delay && silencio == 0){
		    miliSilence = 0;
			silencio = 1;
			Timer2_Freq_Gen(0);
		 } 
		 if (silencio == 1 && miliSilence == SILENCE) {
			  i++;
              silencio = 0;
              miliSilence = 0; 
			  Timer2_Freq_Gen( TICKS(estructura[i].freq) );
		 }
	     if ( i >= songSZ ){
	          TCCR2B = 0x00;
	     }       
	}
	 
}

void Timer2_Freq_Gen(uint8_t ticks){
	/* 	Si "ticks" es mayor que 0 entonces, inicializa y habilita el Generador 
		de Frecuencia del Timer2 con el tope dado por "ticks".
		De lo contrario se requiere deshabilitar el Generador, generando de 
		esta forma el silencio (0 lógico). */
	
    if(ticks > 0){
	   TCCR2A = (3>>WGM20)|(2<<COM2B0);			/*Fast PWM & OC2B*/
	   TCCR2B = (6<<CS20)|(1<<WGM22);				/*Preescalador:256 */
	   //TCCR2A |= (1<<WGM21) |(1<<WGM20); /* fast PWM */
	   //TCCR2B |= (1<<WGM22);
	   //TCCR2A |= (1<<COM2B1) ; /* Set OC2B */
	   //TCCR2B |= (1<<CS22) | (1<<CS21); /* 256 prescaler */
       OCR2A = ticks - 1;
	   OCR2B = (OCR2A*volumen)/100;
	    
	} else {
	  TCCR2B = 0x00; /* Clear OC2B */
	}
	
	 
}

void Timer2_Play(const struct note song[],unsigned int len)
{
	/*	Función que establece las condiciones necesarias para que
		el generador recorra el arreglo de notas. */
    i=0;
    estructura = song; /* pointer of song[] &*/
    songSZ = len; 
    silencio = 0;
    miliSilence=0;

	DDRH |= ( 1<<DDH6 ); /* Salida PH6 pwm OC2B */
    /*Init Timer2 PWM Fast Mode */
    //TCCR2B &= ~( 1<< COM2B1) & ~( 1<< COM2B0); /* no hace nada*/
	TCNT2 = 0;
    Timer2_Freq_Gen( TICKS(estructura[i].freq) );
}

void Timer2_Volume(uint8_t direction){
	/* 	Ajusta el ciclo de trabajo para incrementar o decrementar  el volumen
	   	de las notas que se estan generando. */
	char cad[20];
	
	if( (direction == 2) & (volumen>0) ){
		
		volumen -=5;
		itoa(cad,volumen,10);
		UART_puts("\n\rBajando:");
		UART_puts(cad);
		UART_putchar('v');
	}
	
	if(direction & (volumen<75)){
		
		volumen +=5;
		itoa(cad,volumen,10);
		UART_puts("\n\rSubiendo:");
		UART_puts(cad);
		UART_putchar('V');
	}
	UART_putchar(13);
	   	    
}
