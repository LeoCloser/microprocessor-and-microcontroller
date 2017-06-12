dosseg
.model small
.code
public _myputchar
public _getch
public _peek
public _poke 
public _printNumBase

_myputchar PROC 
           push bp
		   mov bp,sp 
		   mov al,[bp+4]
		   mov dl, al 
		   mov ah,2
		   int 21h
		   pop bp
		   ret
_myputchar ENDP

_getch PROC
         mov ah,8
		 int 21h		 
		 ret
_getch ENDP 

_peek PROC
      push bp 
	  mov bp, sp	  
	  mov ax, [bp+4]
      mov ds, ax	  
	  mov bx, [bp+6]
	  mov al, ds:[bx]
	  pop bp 
	  ret 
_peek ENDP

      ret 
_poke PROC 
      push bp 
	  mov bp, sp   
	  mov ax, [bp+4]
	  mov ds, ax  
	  mov bx, [bp+6]
	  mov al, [bp+8]
	  mov ds:[bx], al 
	  pop bp 
	  ret 
_poke ENDP

_printNumBase PROC 
              push ax 
			  push bx 
			  push cx 
			  push dx 
			  
			  mov bx, 16 
			  mov cx, 0 
			  
			  @@salto:
			  mov dx, 0 
			  div bx 
			  push dx 
			  inc cx 
			  cmp ax, 0 
			  jne @@salto
			  
			  @@nxt:
			  pop ax 
			  add ax, 30h
			  cmp ax, 39h 
			  jbe @@print 
			  add ax, 7h
              @@print:
              call putchar			  			  
              loop @@nxt			  
			  
			  pop dx 
			  pop cx 
			  pop bx 
			  pop ax 
			  ret
_printNumBase ENDP

putchar	proc
		push ax
		push dx
		mov dl,al
	    mov ah,2				; imprimir caracter DL
		int 21h					; usando servico 2 (ah=2)
		pop dx					; del int 21h
		pop ax
		ret
		endp
		
END