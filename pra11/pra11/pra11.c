#include "UART0.h"
#include "Timer.h"
#include<avr/io.h>

#define PHOTORESISTOR PORTA0
#define ADC_Normalize( value ) ((value-menor)*100)/(mayor-menor)
uint8_t offset, var, menor, mayor;

void ADC_Ini ( void );
uint8_t ADC_Read( uint8_t channel);
void ADC_MinMax( uint8_t channel );
void Timer2_Set_Volume(uint8_t volume);
UART0_AutoBaudRate( void );

const struct note ImperialMarch[]=
{
	//for the sheet music see:
	//http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0016254
	//this is just a translation of said sheet music to frequencies / time in ms
	//used TEMPO ms for a quart note
	{a, TEMPO},
	{a, TEMPO},
	{a, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{a, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{a, TEMPO*2},
	//first bit

	{eH, TEMPO},
	{eH, TEMPO},
	{eH, TEMPO},
	{fH, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{gS, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{a, TEMPO*2},
	//second bit...

	{aH, TEMPO},
	{a, TEMPO*3/4},
	{a, TEMPO*1/4},
	{aH, TEMPO},
	{gSH, TEMPO/2},
	{gH, TEMPO/2},

	{fSH, TEMPO*1/4},
	{fH, TEMPO*1/4},
	{fSH, TEMPO/2},
	{0,TEMPO/2},
	{aS, TEMPO/2},
	{dSH, TEMPO},
	{dH, TEMPO/2},
	{cSH, TEMPO/2},
	//start of the interesting bit

	{cH, TEMPO*1/4},
	{b, TEMPO*1/4},
	{cH, TEMPO/2},
	{0,TEMPO/2},
	{f, TEMPO*1/4},
	{gS, TEMPO},
	{f, TEMPO*3/4},
	{a, TEMPO*1/4},

	{cH, TEMPO},
	{a, TEMPO*3/4},
	{cH, TEMPO*1/4},
	{eH, TEMPO*2},
	//more interesting stuff (this doesn't quite get it right somehow)

	{aH, TEMPO},
	{a, TEMPO*3/4},
	{a, TEMPO*1/4},
	{aH, TEMPO},
	{gSH, TEMPO/2},
	{gH, TEMPO/2},

	{fSH, TEMPO*1/4},
	{fH, TEMPO*1/4},
	{fSH, TEMPO/2},
	{0,TEMPO/2},
	{aS, TEMPO/2},
	{dSH, TEMPO},
	{dH, TEMPO/2},
	{cSH, TEMPO/2},
	//repeat... repeat

	{cH, TEMPO*1/4},
	{b, TEMPO*1/4},
	{cH, TEMPO/2},
	{0,TEMPO/2},
	{f, TEMPO/2},
	{gS, TEMPO},
	{f, TEMPO*3/4},
	{cH, TEMPO*1/4},

	{a, TEMPO},
	{f, TEMPO*3/4},
	{c, TEMPO*1/4},
	{a, TEMPO*2}
	//and we're done
};


int main(void)
{
	UART0_Ini();
	UART0_AutoBaudRate();
	Timer0_Ini();
	ADC_Ini();
	ADC_MinMax(PHOTORESISTOR);
	while(1){
		if (UART0_available()){
			if(UART0_getchar() == 'p'){
				Timer2_Play(ImperialMarch,sizeof(ImperialMarch)/sizeof(struct note));
			}
		}
		Timer2_Set_Volume(ADC_Normalize(ADC_Read(PHOTORESISTOR)));
		
	}
	return 0;
}

/*Esta función inicializa para 8 bits de resolución y habilita el ADC del microcontrolador de
forma generica. Encontrar el desplazamiento (offset) de la medición y almacenarla.*/

void ADC_Ini ( void )
{
	char cad[10];
	ADMUX = (1<<REFS0)|(0x1f<<MUX0);  // 8 bits y a tierra la medicion
	ADCSRB &=~(1<<MUX5);				//parte de medicion a tierra
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(7<<ADPS0);  //activa ADC, inicia conversion, prescaler 128
	while(ADCSRA & (1<<ADSC));				//espera a que termine la conversion
	offset = ADCL;
	var = ADCH;			//lee la parte alta para ahbilitar la siguiente conversion
	itoa(cad,offset,10);
	UART0_puts("Offset\n\r");
	UART0_puts(cad);
	UART0_puts("\n\r");
}

/*Está función lo que realiza es una lectura del ADC usando el canal correcto y retornando el
valor de 8 bits acorde a la aplicación (ver figura 1), compensando el desplazamiento de la
medición.*/
uint8_t ADC_Read( uint8_t channel )
{
	uint8_t aux;
	DIDR0 |=1<<ADC0D;		//desactiva la parte digital 
	DDRA &= ~(1<<channel);  //pc0 como salida
	ADMUX = (1<<REFS0)|(channel<<MUX0);
	ADCSRB &=~(1<<MUX5);				//parte de medicion a tierra
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(7<<ADPS0);
	while(ADCSRA&(1<<ADSC));				//espera a que termine conversion
	aux = ADCL;
	var = ADCH;
	return aux-offset;
}

/*Función que captura el rango de valores, encuestando primero por el mínimo.*/
void ADC_MinMax( uint8_t channel )
{
	char cad[10];
	UART0_getchar();     //saca el valor del autobaudaje y esta vacia
	UART0_puts("Valor menor listo?\n\r");
	UART0_getchar();		//espera a que haya un dato y lo saca
	menor=ADC_Read(channel);
	itoa(cad,menor,10);
	UART0_puts(cad);
	UART0_puts("\n\r");
	UART0_puts("Valor mayor listo?\n\r");
	UART0_getchar();
	mayor=ADC_Read(channel);
	itoa(cad,mayor,10);
	UART0_puts(cad);
	UART0_puts("\n\r");
	UART0_puts("Termino min_Max\n\r");
}

/*Ajusta el ancho de pulso que es producido sobre la terminal OC2B. El rango del valor de
entrada sera de 0 a 100.*/
void Timer2_Set_Volume(uint8_t volume)
{
	/*char cad[10];
	itoa(cad,volume,10);
	UART0_puts(cad);
	UART0_puts("\n\r");*/
	Timer2_Volume(volume);
	
}

UART0_AutoBaudRate( void )
{
	DDRE = ~(1<<PE0);				//PE0 como entrada
	UCSR0C = 3<<UCSZ00;             //8 bits timer 0
	TCCR0A = 0;						//modo normal timer 0
	TCCR0B = 2<<CS00;				//preescalador en 8 timer 0
	while( PINE & 1<<PE0 );			//mientras no se active el startbit esperar
	TCNT0 = 0;						//inicializa en 0 para el conteo
	while(!(PINE &1<<PE0));
	TCCR0B =0;						//salva el conteo
	
	UBRR0 = TCNT0 - 1;				//1/Time;
}
