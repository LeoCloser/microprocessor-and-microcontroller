#define WORD unsigned int 
#define BYTE unsigned char
#define DWORD unsigned long

void puts( char *str );
char getch( void );
void putchar ( char dato );

DWORD potencia(int base,int p);
void prac1(int M, int N);
void imprimirBinario(DWORD dato);

BYTE i=0;
WORD pot=8;
DWORD mascaras[5] = {0xaaaaaaaa,0x22222222,0x20202020,0x00200020,0x00200000};  
DWORD mascaras2[5] = {0xaaaaaaaa,0x88888888,0x80808080,0x00800080,0x00800000};

void main ( void ){
    while(1){
    	prac1(3,pot);
    	getch();
   }
}

void puts ( char *str )
{
    while( *str )
        putchar( *str++ );
}

void putchar ( char dato )
{
     asm mov dl,dato
     asm mov ah,2
     asm int 21h
}

char getch( void )
{
    char dato;
    asm mov ah,8
    asm int 21h
    asm mov dato,al
    return dato;
}

void printdec ( unsigned char dato )
{
     putchar( dato/100 + 0x30 );
     dato%=100;
     putchar( dato/10 + 0x30 );
     putchar( dato%10 + 0x30 );
}

DWORD potencia(int base, int p)
{
	int i=0, aux=base;
	for(i=p; i > 1; i--){
		aux = base*aux;
	}
	return aux;
}
/*
N = numero de bits a poner 1's
M = numero de iteraciones
X = numero final a imprimir
*/

void prac1(int M, int N){
	/*unsigned long int mask = 0xaaaaaaaa;*/
	DWORD valor = potencia(2,N)-1; /*Lleno de 1's valor */
	DWORD X=0x00000000; /*Valor a imprimir*/
	BYTE i=0, j=0, recorrido;
	DWORD aux;
	if(N%4){
		recorrido=32-N-3;
	}
	
	if(M < N/2 && N <= 32)
	{		
		for(i=0; i<M; i++){
			aux = mascaras[i];
			do{
				aux = aux >> 1; /*Hago el recorrido uno x uno por que me sale en error de tipo de dato*/
			}while(j < recorrido);
			valor = valor & aux;
		}
	} else {
		puts("No se puede aqui :s");
	}
 	X = valor;
 	putchar(13);
 	putchar(10);
	imprimirBinario(X);
}

/*Imprimir cada bie*/

void imprimirBinario( DWORD dato ){
	DWORD mask=0x80000000;
		do{
			putchar( (dato & mask) ? '1' : '0' );
			mask = mask >>1;
		}while( mask );
}
