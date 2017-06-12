dosseg
.model small
.code
public _myputchar
public _pokeW
public _pokeb
public _getch
public _putchar 

_myputchar PROC
           push bp
		   mov  bp,sp
		   mov dl,[bp+4]
		   mov ah,2
		   int 21h
		   pop bp
		   ret
_myputchar ENDP

_pokeW PROC 
       push bp 
	   mov bp, sp 
	   ;/* Inicializarvector --faltapokeW(WORD segment, WORD offset, WORD value) -- */
       ;pokeW(0x0,INTR*4,/*offset de la ISR*/); /* Determinar offset */ 
	   push ds
	   mov ax,[bp+4]  ; parametro 1 WORD Segment
	   mov ds, ax 
	   mov bx,[bp+6]  
	   mov ax,[bp+8]   
	   mov word ptr[bx], ax 
	   pop ds
	   pop bp
       ret
_pokeW ENDP

_pokeb PROC 
      push bp 
	  mov bp, sp 
	  push ds
	  mov ax, [bp+4]    
	  mov ds, ax
	  mov bx, [bp+6]    
	  mov al, [bp+8]   
	  mov byte ptr[bx], al  
	  pop ds
	  pop bp
	  ret
_pokeb  ENDP

_getch PROC 
       mov ah, 8 
	   int 21h 
	   ret 
_getch ENDP

_putchar	PROC
			
	push bp
	mov bp,sp
	mov dl,[bp+4]
	mov ah,2
	int 21h
	pop bp
	ret
					
_putchar	ENDP

END
