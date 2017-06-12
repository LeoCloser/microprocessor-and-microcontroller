#include "Timer.h"
#define BYTE unsigned char

extern void putchar( unsigned char dato );
extern void pokeb(unsigned int , unsigned int , unsigned char );
/*extern void clrscr( void );*/
void UpdateClock( void );
void DisplayClock( void );
void PrintDec2d( BYTE );
void Delay( BYTE );
void setClock( BYTE , BYTE , BYTE  );
void puts ( char *str );

BYTE seg=0; /* para el manejo de los segundos */
BYTE min=0;
BYTE hrs=0;

void main(){
	 Timer_Ini(); /* inicialzaTimer de 1 segundo */
	
	 setClock(23,59,40);
     while(1){    /* Verificación para actualizar el reloj */
        if(TimerSecFlag() ){ /* Ha pasado un segundo? */
           UpdateClock(); /* actualiza y muestra el reloj */
           DisplayClock(); /* desplegar reloj en 0,0 */
           /* indicar en puerto */
           pokeb(0x0,0xffff,seg); /*Puerto 0x01*/
        }
        /* otras cosas por hacer */
        Delay(100);
        
        /*putchar('.');*/  /* Imprime */
    }
} 

void UpdateClock( void ){
        /*falta minutos , horas etc. etc. */
    if(seg < 59){
		seg++;		
	} else {
		seg = 0;
	    if(min < 59){
		  min++;
	    } else{
	       min = 0;
	       if(hrs < 23){
	 	      hrs++;
	       } else {
		   hrs = 0;
	       }
	 }
	}
}

void DisplayClock( void ){ /* imprimir reloj en formato "hh:mm:ss" */;
     PrintDec2d(hrs);
	 putchar(':');
	 PrintDec2d(min);
     putchar(':');
	 PrintDec2d(seg); /* solo se esta desplegando segundos */
	 myputchar(13);
	 /* clrscr(); limpiar pantalla*/
}

void SetClock(BYTE hora, BYTE minutos, BYTE segundos)
{
	hrs = (BYTE)hora;
	min = (BYTE)minutos;
	seg = (BYTE)segundos;
}

void Delay( BYTE i){
     while(--i);
}

void PrintDec2d( BYTE dato){
    putchar( dato/10 +'0'); /* Imprime decenas */
    putchar( dato%10 +'0'); /* Imprime unidades */
}

void puts ( char *str )
{
while( *str )
putchar( *str++ );
}
