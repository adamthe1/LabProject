#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../headers/define.h"
#include "../headers/first_pass.h"
#include "../headers/label_data_struct.h"
#include "../headers/errors.h"
#include "../headers/helper.h"
#include "../headers/analyze.h"

/* Helper function to get opcode by name */

void test_process_operation() {
    printf("Testing process_operation function...\n");
    
    char test_line1[] = "mov r1, r2";
    char test_line2[] = "add #5, r3";
    char test_line3[] = "lea LABEL, r4";
    char test_line4[] = "jmp &LABEL";
    char test_line5[] = "stop";
    char test_line6[] = "mov r1"; /* Missing operand */
    char test_line7[] = "mov r1, r2, r3"; /* Too many operands */
    char test_line8[] = "mov r9, r2"; /* Invalid register */
    char test_line9[] = "unknown r1, r2"; /* Unknown operation */
    char test_line10[] = "lea #5, r1"; /* Illegal addressing mode */
    char test_line11[] = "cmp #5, #10"; /* Immediate to immediate comparison */
    char test_line12[] = "cmp r1, #5"; /* Register to immediate comparison */
    char test_line13[] = "sub #15, r2"; /* Immediate to register subtraction */
    char test_line14[] = "not r3"; /* Not operation on register */
    char test_line15[] = "clr LABEL"; /* Clear operation on direct addressing */
    char test_line16[] = "inc r4"; /* Increment register */
    char test_line17[] = "dec LABEL"; /* Decrement memory location */
    char test_line18[] = "jmp LABEL"; /* Jump to label */
    char test_line19[] = "bne &LABEL"; /* Branch if not equal with relative addressing */
    char test_line20[] = "jsr LABEL"; /* Jump to subroutine */
    char test_line21[] = "red r5"; /* Read to register */
    char test_line22[] = "prn #42"; /* Print immediate value */
    char test_line23[] = "prn LABEL"; /* Print from label */
    char test_line24[] = "rts"; /* Return from subroutine */
    char test_line25[] = "prn r3"; /* Print register value */
    char test_line26[] = "red LABEL"; /* Read to memory location */
    char test_line27[] = "mov LABEL, r7"; /* Move from memory to register */
    char test_line28[] = "mov #100, LABEL"; /* Invalid: Move immediate to memory */
    
    int ic;
    int result;
    
    /* Create a test label for relative and direct addressing tests */
    create_label(copy_string("LABEL"), CODE_TYPE, 120);
    
    ic = 100;
    printf("Test line: %s\n", test_line1);
    result = process_operation(test_line1, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* Only one word for register-register */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line2);
    result = process_operation(test_line2, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: one for instruction, one for immediate value */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line3);
    result = process_operation(test_line3, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: one for instruction, one for label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line4);
    result = process_operation(test_line4, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: one for instruction, one for relative offset */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line5);
    result = process_operation(test_line5, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word for stop instruction */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line6);
    result = process_operation(test_line6, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 0); /* Should fail for missing operand */
    
    ic = 100;
    printf("\nTest line: %s\n", test_line7);
    result = process_operation(test_line7, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 0); /* Should fail for too many operands */
    
    ic = 100;
    printf("\nTest line: %s\n", test_line8);
    result = process_operation(test_line8, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 0); /* Should fail for invalid register */
    
    ic = 100;
    printf("\nTest line: %s\n", test_line9);
    result = process_operation(test_line9, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 0); /* Should fail for unknown operation */
    
    ic = 100;
    printf("\nTest line: %s\n", test_line10);
    result = process_operation(test_line10, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 0); /* Should fail for illegal addressing mode */

    ic = 100;
    printf("\nTest line: %s\n", test_line11);
    result = process_operation(test_line11, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 103); /* Three words: instruction, first immediate, second immediate */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line12);
    result = process_operation(test_line12, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, immediate value */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line13);
    result = process_operation(test_line13, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, immediate value */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line14);
    result = process_operation(test_line14, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word: instruction only */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line15);
    result = process_operation(test_line15, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();

    ic = 100;
    printf("\nTest line: %s\n", test_line16);
    result = process_operation(test_line16, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word: instruction with register */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line17);
    result = process_operation(test_line17, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line18);
    result = process_operation(test_line18, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line19);
    result = process_operation(test_line19, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, relative address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line20);
    result = process_operation(test_line20, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line21);
    result = process_operation(test_line21, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word: instruction with register */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line22);
    result = process_operation(test_line22, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, immediate value */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line23);
    result = process_operation(test_line23, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line24);
    result = process_operation(test_line24, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word: instruction only */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line25);
    result = process_operation(test_line25, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 101); /* One word: instruction with register */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line26);
    result = process_operation(test_line26, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line27);
    result = process_operation(test_line27, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1);
    assert(ic == 102); /* Two words: instruction, label address */
    print_tables();
    
    ic = 100;
    printf("\nTest line: %s\n", test_line28);
    result = process_operation(test_line28, &ic);
    printf("Result: %d, IC after: %d\n", result, ic);
    assert(result == 1); 
    
    printf("process_operation tests passed!\n\n");
}
/* Test functions declarations */
void test_parse_label();
void test_identify_instruction();
void test_process_data_directive();
void test_process_string_directive();
void test_process_extern_directive();
void test_first_pass();
void print_tables();

/* Helper functions */
void reset_label_table();

int main() {
    printf("=== Running First Pass Tests ===\n\n");
    /*
    test_parse_label();
    test_identify_instruction();
    test_process_data_directive();
    test_process_string_directive();
    test_process_extern_directive();
    
    test_process_operation();*/
    test_first_pass();
    
    printf("\n=== All tests completed ===\n");
    return 0;
}

void reset_label_table() {
    free_labels();
    free_binary_data();
    free_binary_code();
}

void print_tables() {
    printf("\n--- Current lable Table ---\n");
    /* If you have a function to print all labels, call it here */
    print_label_table();
    /* Otherwise you'll need to implement one that iterates through your labels */
    printf("--------------------------\n");

    printf("\n--- Current data Table ---\n");
    /* If you have a function to print all data, call it here */
    print_data_table();
    /* Otherwise you'll need to implement one that iterates through your data */
    printf("--------------------------\n");

    printf("\n--- Current code Table ---\n");
    /* If you have a function to print all code, call it here */
    print_code_table();
    /* Otherwise you'll need to implement one that iterates through your code */
    printf("--------------------------\n");
}

void test_parse_label() {
    printf("Testing parse_label function...\n");
    
    char test_line1[] = "LABEL: .data 100";
    char test_line2[] = "   LABEL2: .string \"test\"";
    char test_line3[] = ".data 100";
    char test_line4[] = "1INVALID: .data 100";
    char label_name[MAX_LABEL_LEN];
    int result;
    
    printf("Test line: %s\n", test_line1);
    result = parse_label(test_line1, label_name);
    printf("Result: %d, Label name: %s\n", result, (result ? label_name : "No label found"));
    assert(result == 1);
    assert(strcmp(label_name, "LABEL") == 0);
    
    printf("\nTest line: %s\n", test_line2);
    result = parse_label(test_line2, label_name);
    printf("Result: %d, Label name: %s\n", result, (result ? label_name : "No label found"));
    assert(result == 1);
    assert(strcmp(label_name, "LABEL2") == 0);
    
    printf("\nTest line: %s\n", test_line3);
    result = parse_label(test_line3, label_name);
    printf("Result: %d, Label name: %s\n", result, (result ? label_name : "No label found"));
    assert(result == 0);
    
    printf("\nTest line: %s\n", test_line4);
    result = parse_label(test_line4, label_name);
    printf("Result: %d, Label name: %s\n", result, (result ? label_name : "No label found"));
    assert(result == 1); /* Will parse but should be invalid */
    
    printf("parse_label tests passed!\n\n");
}

void test_identify_instruction() {
    printf("Testing identify_instruction function...\n");
    
    char test_line1[] = ".data 100, 200";
    char test_line2[] = ".string \"Hello\"";
    char test_line3[] = ".entry START";
    char test_line4[] = ".extern EXTERNAL";
    char test_line5[] = "LABEL: .data 100";
    char test_line6[] = "mov r1, r2";
    
    printf("Test line: %s\n", test_line1);
    int result = identify_instruction(test_line1);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_DATA);
    
    printf("\nTest line: %s\n", test_line2);
    result = identify_instruction(test_line2);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_STRING);
    
    printf("\nTest line: %s\n", test_line3);
    result = identify_instruction(test_line3);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_ENTRY);
    
    printf("\nTest line: %s\n", test_line4);
    result = identify_instruction(test_line4);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_EXTERN);
    
    printf("\nTest line: %s\n", test_line5);
    result = identify_instruction(test_line5);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_DATA);
    
    printf("\nTest line: %s\n", test_line6);
    result = identify_instruction(test_line6);
    printf("Result: %d (%s)\n", result, result == INST_TYPE_DATA ? "DATA" : 
                                       (result == INST_TYPE_STRING ? "STRING" : 
                                       (result == INST_TYPE_ENTRY ? "ENTRY" : 
                                       (result == INST_TYPE_EXTERN ? "EXTERN" : "OPERATION"))));
    assert(result == INST_TYPE_OPERATION);
    
    printf("identify_instruction tests passed!\n\n");
}

void test_process_data_directive() {
    printf("Testing process_data_directive function...\n");
        
    char test_line1[] = ".data 100, 200, 300";
    char test_line2[] = ".data -128, +45";
    char test_line3[] = ".data 2047, -2048"; /* Max/min values */
    char test_line4[] = ".data"; /* Empty data */
    char test_line5[] = ".data 100,,200"; /* Invalid format */
    char test_line6[] = ".data 100, abc"; /* Invalid number */
    char test_line7[] = ".data 4096"; /* Out of range */
    
    int dc = 0;
    int result;
    
    printf("Test line: %s\n", test_line1);
    result = process_data_directive(test_line1, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 1);
    assert(dc == 3);
    print_tables();
    
    dc = 0;
    printf("\nTest line: %s\n", test_line2);
    result = process_data_directive(test_line2, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 1);
    assert(dc == 2);
    print_tables();
    
    dc = 0;
    printf("\nTest line: %s\n", test_line3);
    result = process_data_directive(test_line3, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 1);
    assert(dc == 2);
    print_tables();
    
    dc = 0;
    printf("\nTest line: %s\n", test_line4);
    result = process_data_directive(test_line4, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for empty data */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line5);
    result = process_data_directive(test_line5, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for invalid format */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line6);
    result = process_data_directive(test_line6, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for invalid number */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line7);
    result = process_data_directive(test_line7, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for out of range */
    
    printf("process_data_directive tests passed!\n\n");
}

void test_process_string_directive() {
    printf("Testing process_string_directive function...\n");
    
    
    char test_line1[] = ".string \"Hello, World!\"";
    char test_line2[] = ".string \"\""; /* Empty string */
    char test_line3[] = ".string"; /* Missing string */
    char test_line4[] = ".string Hello"; /* Missing quotes */
    char test_line5[] = ".string \"Unclosed"; /* Missing closing quote */
    char test_line6[] = ".string \"Hello\" extra"; /* Extra content */
    
    int dc = 0;
    int result;
    
    printf("Test line: %s\n", test_line1);
    result = process_string_directive(test_line1, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 1);
    assert(dc == 14); /* 13 chars + null terminator */
    print_tables();
    
    dc = 0;
    printf("\nTest line: %s\n", test_line2);
    result = process_string_directive(test_line2, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 1);
    assert(dc == 1); /* Just null terminator */
    print_tables();
    
    dc = 0;
    printf("\nTest line: %s\n", test_line3);
    result = process_string_directive(test_line3, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for missing string */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line4);
    result = process_string_directive(test_line4, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for missing quotes */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line5);
    result = process_string_directive(test_line5, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for unclosed quote */
    
    dc = 0;
    printf("\nTest line: %s\n", test_line6);
    result = process_string_directive(test_line6, &dc);
    printf("Result: %d, DC after: %d\n", result, dc);
    assert(result == 0); /* Should fail for extra content */
    
    printf("process_string_directive tests passed!\n\n");
}

void test_process_extern_directive() {
    printf("Testing process_extern_directive function...\n");
    
    char test_line1[] = ".extern EXTERNAL";
    char test_line2[] = ".extern"; /* Missing label */
    char test_line3[] = ".extern 1INVALID"; /* Invalid label */
    char test_line4[] = ".extern EXTERNAL EXTRA"; /* Extra content */
    char test_line5[] = ".extern EXTERNAL"; /* Duplicate extern - should pass */
    
    int result;
    
    printf("Test line: %s\n", test_line1);
    result = process_extern_directive(test_line1);
    printf("Result: %d\n", result);
    assert(result == 1);
    print_tables();
    
    printf("\nTest line: %s\n", test_line2);
    result = process_extern_directive(test_line2);
    printf("Result: %d\n", result);
    assert(result == 0); /* Should fail for missing label */
    
    printf("\nTest line: %s\n", test_line3);
    result = process_extern_directive(test_line3);
    printf("Result: %d\n", result);
    assert(result == 0); /* Should fail for invalid label */
    
    printf("\nTest line: %s\n", test_line4);
    result = process_extern_directive(test_line4);
    printf("Result: %d\n", result);
    assert(result == 0); /* Should fail for extra content */
    
    printf("\nTest line: %s\n", test_line5);
    result = process_extern_directive(test_line5);
    printf("Result: %d\n", result);
    assert(result == 1); /* Should pass for duplicate extern */
    print_tables();
    
    printf("process_extern_directive tests passed!\n\n");
}

void test_first_pass() {
    printf("Testing first_pass function...\n");
    
    /* Create a temporary assembly file for testing */
    FILE *temp = fopen("test_first_pass.am", "w");
    if (temp == NULL) {
        printf("Failed to create test file\n");
        return;
    }
    
    /* Write test assembly code */
    fprintf(temp, "; Test assembly code\n");
    fprintf(temp, ".extern EXTERNAL\n");
    fprintf(temp, "DATA_LABEL: .data 100, 200, 300\n");
    fprintf(temp, "STRING_LABEL: .string \"Hello\"\n");
    fprintf(temp, "\n"); /* Empty line test */
    fprintf(temp, "; Comment line test\n");
    fprintf(temp, "CODE_LABEL: mov r1, r2\n");
    
    fclose(temp);
    
    /* Open file for reading */
    temp = fopen("example.am", "r");
    if (temp == NULL) {
        printf("Failed to open test file\n");
        return;
    }
    
    /* Run first pass */
    int result = first_pass(temp);
    fclose(temp);
    
    printf("First pass result: %d (0=errors found, 1=success)\n", result);
    print_tables(); 
    
    /* Clean up the temporary file */
    remove("test_first_pass.as");
    
    printf("first_pass test completed\n");
}