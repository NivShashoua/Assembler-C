/* pragma once */
#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Enums.h"
#include "symbol_table.h"
#include "bool.h"

#define REGISTER_FIRST_TOKEN 'r'
#define REGISTERS_COUNT 8
#define MOV_OPERATION "mov"
#define CMP_OPERATION "cmp"
#define ADD_OPERATION "add"
#define SUB_OPERATION "sub"
#define NOT_OPERATION "not"
#define CLR_OPERATION "clr"
#define LEA_OPERATION "lea"
#define INC_OPERATION "inc"
#define DEC_OPERATION "dec"
#define JMP_OPERATION "jmp"
#define BNE_OPERATION "bne"
#define RED_OPERATION "red"
#define PRN_OPERATION "prn"
#define JSR_OPERATION "jsr"
#define RTS_OPERATION "rts"
#define STOP_OPERATION "stop"
#define NUM_OF_INSTRUCTS 14


#define MAX_LINE_LENGTH (100)
#define MAX_LABEL_NAME_LENGTH (31)
#define FORBIDEN_SYMBOLS_SIZE (28)


extern char* instructions[NUM_OF_INSTRUCTS];

/**
* check if a string is one of the fobiden symbols.
* input: string.
* output: if it is fobiden return false, else true.
*/
bool check_if_forbiden_symbol(char *str);

/* check if the label is valied */
bool check_if_its_label(symbol_table_list_t *list, char *name, int line_num);


/* check if the macro already initialize in the symbol table, use this when the macro used as operand */
bool check_if_macro_inti(symbol_table_list_t *list, char *str);

/* check if the string is integer */
bool string_is_integer(char *str);

/**
* check if the string only have letters.
* return case for: not only letters, only capital letter, only small letters and bouth(samll and capital).
*/
STRING_ONLY_LETTER string_only_letters_or_numbers(char *str);

/*
* check if the string only have vaild char.
* no spaces in the string!
* return case for: vaild, not vaild.
*/
STRING_ONLY_LETTER string_only_vaild_char(char *str);
 
/* remove spaces from a string */
void remove_spaces(char *str);

/* count how many words in a given string */
unsigned int count_words(char *str);

/* Description: Checks if operand is a register
 * Input:		  1. Operand
 * Output:		True if register, otherwise false */
bool is_register(char* operand);

/* Description: Checks if string is a digits only
 * Input:		  1. string
 * Output:		1-true , 0-flase */
int digits_only(const char *s);


/*  Description: Checks if the string is a valid label
 * Input:		  String to check
 * Output:		True if label, otherwise false */
bool is_valid_label(char* str);


 /* Description: Checks if string is an operation name
 * Input:		String to check
 * Output:		True if string is an operation, otherwise false */
bool is_operation_name(char* str);

/* turn all the cells in the word cell back to 0, stop at null */
void clean_word(char *word);

/* translate a decimal integer to binary string */
void dec_to_binary(char *word, int num, int word_size);


#endif /* _UTILITIES_H */