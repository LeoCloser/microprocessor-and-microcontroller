.model tiny
;----- Insert INCLUDE "filename" directives here
;----- Insert EQU and = equates here
locals
.data
new_line db 13,10,0 
.code
org 100h
;*************************************************************
; Procedimiento principal
;*************************************************************
principal proc
          mov sp,0fffh ; inicializar SP (Stack Pointer)
          
		  mov bx, 0200h
		  @@nxt: 
		  mov byte ptr[bx], 55h	
          cmp byte ptr[bx], 55h
          jne @@error
		
		  mov byte ptr[bx], 0aah  
		  cmp byte ptr[bx], 0aah
		  jne @@error 
		  inc bx
          cmp bx, 0fffh 
          je @@succes		  
		  jmp @@nxt 
		  
		  @@error:
		  mov al, 'E'
		  mov dl,al
          mov ah,2 ; imprimir caracter DL
          int 21h ; usando servicio 2 (ah=2)
		  jmp @@fin 
		  
		  @@succes:
		  mov al, 'S'
		  mov dl,al
          mov ah,2 ; imprimir caracter DL
          int 21h ; usando servicio 2 (ah=2)
		  
		  @@fin: 
		  
		  @@ini0: 		  
          jmp @@ini0
          ret ; nunca se llega aquí
endp
end principal