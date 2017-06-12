;------------- definiciones e includes ------------------------------
.INCLUDE "m1280def.inc"
.equ INIT_VALUE = 0x01 ; Port C
.equ CONFIG_PORT = 0xff ; Direction register 123 Port is DDRC
;------------- init PORTS -------------------------------------------
;ldi R24,CONFIG_PORT
;out DDRC,R24
;ldi R24,INIT_VALUE

;------------- main loop -------------------------------------------
;next:
 ;**** Retardo ****
;out PORTC,R24
; rol R24
;sjmp next

        next:
		call delay666ms
;		out PINB, R24	
		jmp next
;-------------------------------------------------------------------- 

;delay5us:
;call   ; 5
;ldi r16, 23 ;1
;nxt1:
;dec r16    ; n
;brne nxt1  ; (n-1)2 + 1
;nop       ; 1
;ret    ; 5

; 5 + 1 + n + 2n-2 + 1 + 1 + 5 
; 11 + 3n 
; 11 + 3(23) = 80 ciclos

;delay234us:
;ldi r16, 233 ; 1   M
;nxt2:
;ldi r17, 4 ; 1     N   | m  
;nxt:	
;dec r17     ; n      |  
;brne nxt    ; 2(n-1)  |
;dec r16     ; m        |
;nop         ; m
;brne nxt2	; 2(m-1)  |
;nop         ;1
;nop         ;1
;nop  
;nop
;nop 
;nop 
;ret         ; 5

; ECUACION: 
; 1 + 1 + m(n + 2(n-1)) + 1m + 1m + 2(m-1) + 1 + 1 
; 2 + m(3n-2) + 2m + 2m - 2 + 2
; 2 + 3mn + 4m 
; 2 + m(3n) + 4m
; 5+ 4m + m (3n) + 6 + 5
; M = 233 N = 4

;delay5ms:  ; 5000us M=158 N=134 para 4ms

;ldi r16, 197 ; 1
;nxt2:	
;ldi r17, 80 ; 1  ;M
;nxt:    
;dec r17      ; n
;nop
;nop
;brne nxt     ; (n-1)2
;dec r16      ; m
;nop
;nop
;nop
;brne nxt2	 ; (m-1)2
;ret          ; 5

; ECUACION: 
; 5 + 1 + 1 + m(3n + 2(n-1)) + (1m + 1m + 1m + 2(m-1)) + 5
; 7 + m(3n + 2n - 2) + 3m + 2m - 2 + 5
; 7 + m(5n - 2) + 5m + 3 
; 10 + 5mn + 3 
; 13 + 5mn + 5m  

delay666ms: ; call 5   666000us
ldi r19, 232     ; 1 y 
nxt4:
ldi r18, 8       ; z      ;todo x Y
 nxt3: 
  ldi r16, 151   ; m      ;todo x Z
    nxt2:	
      ldi r17, 7 ; n      ;M9  todo x M
      nxt:
	  dec r17    ; n
	  nop        ; n
	  nop        ; n 
	  brne nxt   ; (n-1)2
    dec r16      ; m
    brne nxt2	 ; (m-1)2
  dec r18      ; z
  brne nxt3    ; (z-1)2
 dec r19       ; y
 brne nxt4     ; (y-1)2

ret          ; 5

