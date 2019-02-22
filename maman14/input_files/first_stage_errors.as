;not exist command
comm

; few argument
mov 2

; too many argument
mov @r1 ,@r2, @r3, @r4

; not valid argument type
inc 4
;valid argument type
prn 4
; wrong source 
lea 1,@r3
;wrong target
lea LABEL, 1
;wrong source an target
lea 1,3
;label start with numbeer
34e: mov @r1,@r2
;lebal that is saved word
data: mov @r1,@r2