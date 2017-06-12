.model tiny
;----- Insert INCLUDE "filename" directives here
;----- Insert EQU and = equates here
locals
.data
Mens DB 'Hola Mundo',10,13,0
mensaje1 db 13,10,'Numero 1: ',0
mensaje2 db 13,10,'Numero 2: ',0
mensaje3 db 13,10,'Numero de hammingDistance: ',0
new_line db 13,10,0 
.code
org 100h
;*************************************************************
; Procedimiento principal
;*************************************************************
principal proc
          mov sp,0fffh ; inicializar SP (Stack Pointer)
          @@ini0: 
          mov bx, 08762h
		  push bx 
		  mov ax, bx 
		  mov dx, offset mensaje1
		  call puts 
		  call printBinAX   ; imprimir BX
		  
		  call getch 
          mov dx, 7412h 
		  push dx 
		  mov ax, dx 
		  mov dx, offset mensaje2 
		  call puts 
		  call printBinAX   ; imprimir DX 
		  call getch 
		  mov dx, offset new_line
		  call puts 
		  pop dx 
		  pop bx 
          call hammingDistance 
          call getch 		  
          jmp @@ini0
          ret ; nunca se llega aquí
endp
;***************************************************************
; procedimientos
;***************************************************************
getch	proc
			mov ah,8				; no echo
			int 21h				
			ret
			endp
putchar proc
        push ax
        push dx
        
		mov dl,al
        mov ah,2 ; imprimir caracter DL
        int 21h ; usando servicio 2 (ah=2)
        pop dx ; del int 21h
        pop ax
        ret
        endp
puts proc 
     push ax 
	 push bx 
	 push cx 
	 
	 mov si, 0 
	 mov bx, dx 
	 @@siguiente:
	 mov al, [bx+si]
	 call putchar
	 cmp byte ptr [bx+si], 0 
	 je @@fin 
	 inc bx 
	 jmp @@siguiente 
	 @@fin:
	 pop cx 
	 pop bx 
	 pop ax 
	 ret
     endp 
printBinAX proc 
         push ax 
		 push bx 
		 
		 mov cx, 16 
		 @@recorrer:
		 shl ax, 1
		 mov bx, ax 
		 jc @@es_uno
		 mov al, '0'
		 call putchar
		 jmp @@jump 
		 @@es_uno:
		 mov al, '1'
		 call putchar
		 @@jump:
		 mov ax, bx 
		 loop @@recorrer
		 
		 pop bx 
		 pop ax 
		 ret
         endp
HammingDistance proc 
                push cx 
             	push bx 
                push dx				
                push ax 
				
				mov cx, 16 ; numero de bits
				mov si, 0 ; contador
                xor dx, bx				
				mov ax, dx 
				
				@@nxt:
				shr ax, 1
				jc @@carry
				jmp @@seguir
				@@carry:
				inc si
				@@seguir:
				loop @@nxt
				
				mov ax, si
				mov bx, 10
                call printHex 			
				
				pop ax 
				pop dx 
				pop bx 
                pop cx
                ret				
                endp
printHex PROC ; < -- Indica a TASM el inicio del un procedimiento
             push dx
			 push ax
			 push bx 
			 push cx 
			 
			 
             mov cx, 0
             @@continue:
             mov dx, 0       ; Aseguramos el residuo en 0 
             div bx          ; AX= AX/BX DX=Residuo BX = Base a dividir
             push dx         ; Guardar el residuo en la pila
             inc cx          ; Incrementa cada vez que se divide
             cmp ax, 0       ; cmp con AX si no es = 0 
             jne @@continue  ; Continuara dividiento 
			 
			 @@nxt:        
			 pop ax        ; Saco los valores de la pila (residuos)
			 cmp al, 9     ; Comparo con 9 Decimal para hacer el ajuste
			 je @@print
			 cmp al, 48    ; 0
			 jle @@print 
			 add al, 55    ; 7
             @@print:
			 add al, 48    ; 0 Le sumo Ascii 0
			 call putchar
			 loop @@nxt
			 
			 pop cx
             pop bx	
             pop ax 
             pop dx 			 
           ret
           ENDP; < -- Indica a TASM el fin del procedimiento
end principal ; fin de programa (file