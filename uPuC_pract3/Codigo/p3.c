#define WORD unsigned int 
#define BYTE unsigned char 

extern char getch(void);
extern char myputchar(BYTE dato); 
extern void poke(WORD segment, WORD offset, BYTE dato);
extern BYTE peek(WORD segment, WORD offset);
extern void printNumBase(WORD numero);
int mapeoMemoria();

void main(void)
{
	if(mapeoMemoria() == 0)
	{
		myputchar('E');
	} else {
		myputchar('S');
	}
	while(1); 
}

int mapeoMemoria()
{
	WORD segment = 0x220;
	WORD offset = 0x0;
	BYTE dato;
	
	do{
		poke(segment,offset,0xaa);
		dato = peek(segment,offset);
		if(dato != 0xaa)
		{
		    printNumBase(offset+0x2200);
		    myputchar(':');myputchar(' ');
			printNumBase(dato);
			myputchar(10); myputchar(13);
		    return 0;
	    }
	    printNumBase(offset+0x2200);
		myputchar(':');myputchar(' ');
		printNumBase(dato);
		myputchar(10); 
		myputchar(13);
		poke(segment,offset,0x55);
	    dato = peek(segment,offset);
		if(dato != 0x55)
		{
			printNumBase(offset+0x2200);
			myputchar(':');myputchar(' ');
			printNumBase(dato);
			myputchar(10); myputchar(13);
			return 0;
		}
	    printNumBase(offset+0x2200);
		myputchar(':');myputchar(' ');
		printNumBase(dato);
		myputchar(10); 
		myputchar(13);
		offset++;
	}while(offset < 0xde00);
	return 1;
}