#ifndef _DEFINE_H
#define _DEFINE_H

#define COMMENT_CHAR ';'
#define LABEL_CHAR ':'
#define STRING_CHAR '"'
#define TWO 2
#define MAX_LINE_LENGTH 80
#define MAX_MCRO_LENGTH 31

char* reg[8] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

typedef struct op_code{
    char* name;
    int input_num;
}op_code;

op_code op_codes[16] = {
    {"mov",2},{"cmp",2},{"add",2},{"sub",2},{"lea,2"},
    {"clr",1},{"not",1},{"inc",1},{"dec",1},{"jmp",1},{"bne",1},{"jsr",1},{"red",1},{"prn,1"},
    {"rts",0},{"stop",0}
};

char* insturctions[4] = {"data","string","entry","extern"};



#endif // _DEFINE_H