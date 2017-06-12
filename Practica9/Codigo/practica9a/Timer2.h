#ifndef _TIMER2_H
#define _TIMER2_H
#include <inttypes.h>
#define BYTE unsigned char

/* Funcion para inicializar el Timer2 para generar N seg. */
void Timer2_Ini( void );
/* Funcion para verificar bandera del segundo */
uint8_t Timer2_Flag ( void );

#endif /* _TIMER2_H */
