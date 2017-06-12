#include <avr/io.h>
void UART0_Ini( void );
void UART0_putchar(char data);
char UART0_getchar( void );
void UART0_puts(char *str);
void itoa(char* str, uint16_t number, uint8_t base);
uint8_t UART0_available( void );	