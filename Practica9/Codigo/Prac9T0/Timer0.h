#ifndef _TIMER0_H
#define _TIMER0_H
#include <inttypes.h>

/* Función para inicializar el Timer0 y generar */
/* la temporización de 1 Sec. */
void Timer0_Ini ( void );

/* Función para verificar bandera del segundo */
uint8_t Timer0_SecFlag ( void );


#endif /* _TIMER0_H */
