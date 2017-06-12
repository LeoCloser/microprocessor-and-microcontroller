#ifndef _UART_H
#define _UART_H
#include <inttypes.h>

void UART0_Init(uint16_t mode);
uint8_t UART_available(void);
/*char UART0_getchar(void);
void UART0_putchar(char data);*/
char uart0_getchar(void);
void uart0_putchar(unsigned char dato);
void UART0_puts(char *str);
void UART0_gets(char *str);
void itoa(char *str, uint16_t dato, uint8_t base);
unsigned int atoi(char *str);

#endif /* _UART_H */
