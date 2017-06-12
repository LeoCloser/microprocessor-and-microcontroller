dosseg
.model small
.code
public _outportb
public _inportb

_outportb PROC
          push bp 
		  mov bp, sp 
		  
		  mov al, [bp+6]
		  mov dx, [bp+4]
		  out dx, al 
		  
		  pop bp 
		  ret 
_outportb ENDP 

_inportb PROC 
         push bp 
		 mov bp, sp 
		 
		 mov dx, [bp+4]
		 in al, dx 
		 
		 pop bp 
		 ret 
_inportb ENDP

END