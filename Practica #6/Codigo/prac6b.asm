;------------- definiciones e includes ------------------------------
.INCLUDE "m1280def.inc" ; Incluir definiciones de Registros para 1280
.equ INIT_VALUE = 0xf4 ; Valor inicial R24
.equ INIT_VALUE2 = 85 ; Valor inicial R25
;------------- inicializar ------------------------------------------

ldi R31, 0xB0 ;  ZH
ldi R30, 0xB0 ;  ZL 
ldi R29, 0xC0 ;  YH 
ldi R28, 0xCA ;  YL
ldi R24, 1
;------------- ciclo principal --;
  infinito:
  nohacer: 
  cpi R24, 0      ; Si R24 es 0 COMPARAR Z
  breq tagRZ
  
  cpi r24, 1
  breq mover_rY

  jmp nohacer
  
  tagRZ:          ; GUARDO LOS REGISTROS R31, R30 el cual es X
  mov r2, r30
  mov r1, r31
  jmp nxt
  nextRequister: 
  ;mov r2, r30
  jmp nxt2 
  mover_rY:       ; GUARDO LOS REGISTROS R29, R28 el cual es Y
  mov r2, r28
  mov r1, r29
  jmp nxt
  ; CAMBIAR SI NO ME FUNCIONA 
  jmp nxt2 
  nxt:   
  ldi r17, 7   ; Load constant in r17 Loop 
  ldi r18, 0   ; BL, 0 
  ldi r20, 128 ; DL = Posicion 8 = Mascara 0x80
  loop:
  or r18, r20  ; Hago 1 ultima posicion R18
  lsr r1      ; ver si hay acarreo AL --> C == 1
  brcc salto   ; Si no hay acarreo saltar
  mov r21, r20
  com r21
  and r18, r21
  salto: 
  dec r17    ; Decrement r17
  lsr r20
  brne loop  ; Branch if r17?0
  com r18
  jmp resultado30
  resultado30:
  cpi r24,1
  breq guardarY 
  mov r30, r18
  jmp siguiente
  guardarY:
  mov r28, r18
  siguiente:
  jmp nextRequister
  nxt2:   
  ldi r17, 7   ; Load constant in r17 Loop 
  ldi r18, 0   ; BL, 0 
  ldi r20, 0x80 ; DL = Posicion 8 = Mascara 0x80
  ldi r21, 0
  loop2:
  or r18, r20  ; Hago 1 ultima posicion R18
  lsr r2      ; ver si hay acarreo AL --> C == 1
  brcc salto2   ; Si no hay acarreo saltar
  mov r21, r20
  com r21
  and r18, r21
  salto2: 
  dec r17    ; Decrement r17
  lsr r20
  brne loop2  ; Branch if r17?0
  com r18
  ; Resultado R31
  cpi r24,1
  breq guardarY2 
  mov r31, r18
  jmp siguiente2
  guardarY2:
  mov r29, r18
  siguiente2:

  jmp fin
  fin:
  
  jmp infinito
