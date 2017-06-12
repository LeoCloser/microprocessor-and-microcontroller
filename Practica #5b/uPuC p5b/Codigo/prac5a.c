#define BYTE unsigned char 
#define WORD unsigned int 

#define PA      0x40
#define PB      0x41
#define PC      0x42
#define RCtr    0x43
#define PTOs_Cin_ABout 0x81 

extern void outportb( WORD port, BYTE dato);
extern char inportb( WORD port );

void SetBitPort( WORD puerto, BYTE num_bit );
void NotBitPort( WORD puerto, BYTE num_bit );
BYTE TstBitPort ( WORD puerto, BYTE num_bit );
void ClrBitPort( WORD puerto, BYTE num_bit );
BYTE ReverseBitsPort ( WORD puerto);

void putchar( char dato );
char getch( void );
void puts( char *str );
void printBin( BYTE dato );

BYTE dato; 

void main( void ){

     puts("Practica 5a\n\r");           /* imprimir mensaje*/
	 outportb(RCtr,PTOs_Cin_ABout);      /* inicializar 8255 */
     while(1) {
	    getch();
		dato = inportb(PC);            
		printBin(dato);
		puts("\n\r");
		outportb(PA,dato); 
		getch();
		NotBitPort(PA,1);
		printBin(dato);
		getch();
		if(TstBitPort(PA,6)){
		  ClrBitPort(PA,6);
		  puts("Limpiar bit\n");
		  printBin(dato);
		} else {
		  SetBitPort(PA,6);
		  puts("Activar bit\n");
		  printBin(dato);
		}
		getch();
		puts("ReverseBitsPort: ");
	    ReverseBitsPort(PA);
		puts("\n\r");
	 }
}

void printBin( BYTE dato ){
     BYTE mask =0x80;
	 do{
	    putchar((dato & mask) ? '1':'0');
		mask >>= 1;
	 }while( mask );
}

void putchar(char dato){
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

void puts ( char *str )
{
    while( *str )
      putchar( *str++ );
}

void SetBitPort(WORD puerto, BYTE num_bit){
     outportb(puerto, inportb(puerto)|(1 << num_bit));
}

void NotBitPort( WORD puerto, BYTE num_bit){
     outportb(puerto, inportb(puerto)^(1 << num_bit));
}

void ClrBitPort( WORD puerto, BYTE num_bit ){
     outportb(puerto, inportb(puerto)& ~(1 << num_bit));
}

BYTE TstBitPort ( WORD puerto, BYTE num_bit ){
     BYTE mask = 0x01 << num_bit; 
	 BYTE dato = inportb(puerto);
	 if(dato & mask){
	    return 1; 
	 } else {
	    return 0;
	 }
}

BYTE ReverseBitsPort ( WORD puerto){
     BYTE mask = 0x01;
	 BYTE x=7;
	 BYTE i=0, temp;
     BYTE datot	= inportb(puerto); 
	 for(i=0; i<8; i++){
	     if( (mask << i) & datot )
		    temp |= (1 << x-i);
		 else 
		    temp &= ~(1 << x-i);
	 }
	 outportb(puerto,temp);
}