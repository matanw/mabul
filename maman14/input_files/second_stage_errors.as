;not exist label
jmp GOD
;duplicate label
DUPLICATE: mov @r1,@r2
DUPLICATE: mov @r1,@r2
; extern and label
.extern ABC
ABC: mov @r1,@r2
; .entry that not exists
.entry ENTRY