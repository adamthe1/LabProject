mcro macroAlpha
    add r2, r3
    cmp r1, #20
    bne routineOne
mcroend

mcro macroBeta                      
    sub r5, r6
    jsr routineTwo
    dec r4


mcro macroGamma
    mov r3, r4
    prn r5
    clr r2
mcroend

mcro bigMacroNew
    mov r2, r4
    add r3, r5
    sub r1, r6
    cmp r7, r0
    bne routineThree
    jsr routineFour
    prn r7
mcroend

.extern routineOne

start:      mov r2, r3
            prn textOne
start2:            lea addrOne, r4


.entry loopStart

            sub r4, dataTwo
            jsr routineOne

.extern externalAddr1

    macroAlpha
            prn r3
            clr r7
            mov r0, r5
            sub textTwo, addrTwo
            add r2, r4
            mov dataTwo, r6
            jsr routineFive
            stop

textOne:    .string "Starting the testing sequence."
dataTwo:    .data 2, -2

.extern routineTwo

routineOneCall: lea textTwo, r7
                inc r7

.entry routineOneCall

                mov r7, dataTwo
                sub r2, r5
                cmp r4, #-10
                bne loopStart
                add r0, r7

.extern externalAddr2

                clr mVar
                sub addrThree, r3
                jmp endPoint

textTwo:    .string "Further testing with a new string."
mVar:       .data 30, 40, -50, 60, 70

loopStart:  prn #55
            lea textThree, r6
macroBeta
            inc r2
            cmp r3, #-25
            bne routineOne
            jsr routineTwo
            dec r4
            jmp loopStart

.extern routineThree

routineAlpha: clr r3
              lea textOne, r4
              add r5, r4
              jsr routineFive
              prn r2
macroAlpha

.entry routineAlpha

routineFive: clr r0
             mov r1, r2
             add r3, r4
             prn r5
             jsr routineAlpha
             stop
addrFour:   .data 200, 300, 400

              mov r7, addrFour
              stop

.extern externalAddr3

routineBeta: clr r6
             mov r2, r7
             add r3, r4
             sub r5, r6
             jsr routineAlpha
macroBeta

.extern externalAddr4

             mov r0, r7

.entry routineBeta

             prn addrFive
             stop

addrOne:    .data 1024, -2048
            prn textThree

textThree:  .string "Data string within the sequence."

             mov r3, r4
             prn r5

routineGamma: jsr routineAlpha
              prn r7
              clr r3
              mov r4, addrSix
              sub r2, r3
              cmp r7, #-40
              bne routineBeta
macroGamma
              stop

addrSix:    .data 700, 800, 900

bigMacroNew

.extern externalAddr5

             mov r3, addrTwo
             cmp r4, r5
             add r6, r7
             sub r0, r2
             jsr routineAlpha
             dec r3
             prn r4
             mov addrOne, r6
             jsr routineGamma
             jmp loopStart
             prn r7

addrTwo:    .data 1000
macroGamma

.extern externalAddr6

mov r3, textFour

textFour:   .string "A different string within the code."

             prn addrTwo
             add r4, r5

addrFive:   .data 900, -1000, 1100, -1200, 1300, -1400
             mov r3, r6
             sub r7, r0
             prn r7

.extern externalAddr7

macroAlpha
             mov r6, addrFive
             add r3, r7
             prn textFive
             sub r6, r4
             jmp routineAlpha

addrThree:  .data 1500, 1600, 1700
             prn r2
             mov r3, r4
             clr r6

.extern routineFour

             mov addrFour, r5
             prn textThree
             sub r7, r0
             stop

textFive:   .string "Ending sequence with this string."

.entry endPoint

.extern finalRoutine

endPoint:   stop
             prn r7  