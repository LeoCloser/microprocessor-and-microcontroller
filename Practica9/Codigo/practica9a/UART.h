#ifndef _UART_H
#define _UART_H
#include <inttypes.h>

void UART0_Init(uint16_t mode);
char UART0_getchar(void);
void UART0_putchar(char data);
void UART0_puts(char *str);
void UART0_gets(char *str);
void itoa(char *str, uint16_t dato, uint8_t base);
unsigned int atoi(char *str);
int _strlen(char *str);

#endif /* _UART_H */
