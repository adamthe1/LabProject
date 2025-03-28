val: .string "asfas   %%dfjk"
    .extern LABEL
    .entry MAIN

KINITIALVALUE: sub r4   ,    r3

MAIN:   mov r3, LENGTH

LOOP:   jmp L1

mcro M1
sub r1, r3
bne END
mcroend

    .entry LOOP
prn #4
bne LOOP

XYZ: mov r4, r2

M1

L1:     inc K
bne LOOP

END: stop

STR:    .string "666"abcdef"

LENGTH: .data 6, -9, 15

K:      .data 4      ,  -55,4,4,4,6

mcro M2
mov reg1, val
add reg2, reg1
mcroend

M2
ABC: mov XYZ, STR
reg1: .data 6,5,-555,66
reg2: .data 6,5,-555,66
