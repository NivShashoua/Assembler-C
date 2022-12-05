/*pragma once*/
#ifndef _OPCODE_H
#define _OPCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "symbol_table.h"
#include "Enums.h"
#include "bool.h"

#define OPERAION_MIN_WORD_SIZE 1



/** This data structre hold most of the information we need
*	to build the final commands senteces in the code file. we will use it in phase 2. 	
*/
typedef struct opcode_t
{
	int are : 2;				/* if its A(absolute), R(relocatable), E(external) */
	char *des_operand;	/* destination operand, as the string in the line */
	char *src_operand;	/* source operand, as the string in the line */
	unsigned int opcode : 4;				/* the command itself (opcode), as an int */
	int reserved : 4;			/* the 4 reserved digit at the start of the line */
	int line_num;				/* the line number this command in */
	int size;					/* number of lines this single command sentence will take */
        struct opcode_t *next;
} opcode_t;


/* the opcode list */
typedef struct opcode_list_t
{
	opcode_t *first;	/* the first node of the list */
	opcode_t *last;		/* the last node of the list */
	size_t size;		/* the size of the list */
} opcode_list_t;



typedef enum {
	OPCODE_MNEMONIC_MOV,
	OPCODE_MNEMONIC_CMP,
	OPCODE_MNEMONIC_ADD,
	OPCODE_MNEMONIC_SUB,
	OPCODE_MNEMONIC_NOT,
	OPCODE_MNEMONIC_CLR,
	OPCODE_MNEMONIC_LEA,
	OPCODE_MNEMONIC_INC,
	OPCODE_MNEMONIC_DEC,
	OPCODE_MNEMONIC_JMP,
	OPCODE_MNEMONIC_BNE,
	OPCODE_MNEMONIC_RED,
	OPCODE_MNEMONIC_PRN,
	OPCODE_MNEMONIC_JSR,
	OPCODE_MNEMONIC_RTS,
	OPCODE_MNEMONIC_STOP
	
} opcode_mnemonic_t; 


typedef enum
{
	OPCODE_TABLE_ERROR_SUCCESS,
	OPCODE_TABLE_ERROR_GIVE_NULL,
	OPCODE_TABLE_ERROR_DIDNT_FOUND_ENTRY

} opcode_list_error_t;


bool opcode_parse(symbol_table_list_t *table_list, opcode_list_t *opcode_list, char *opcode_str ,int line_num);


/**
* This function find if there are label that didn't initialized,
* translate the opcode into binary code and put it in a tmp file.
* input:	opcode_entry,
*			symbol_table_list,
*			ic(code line number),
*			output_code file,
*			output_extern file.
* output:	false, if there are errors, else true
*/
bool opcode_encode(opcode_t *opcode_entry, symbol_table_list_t *list, int *ic, FILE *output_code, FILE *output_extern);


void inside_in_Array(char * str);


/**
* This function handle register operand.
* if both opernad are registers it will write the information to the file.
* if src opernad is register it will write the information to the file.
* if des opernad is register it will write the information to the file.
* It return which operands are register: both, only src, only des and none.
*/
TAKE_CARE handle_rgister(char *word, char *src_operand_str, char *des_operand_str, int *ic, FILE *output_code);

/**
* This function handle the rest of the operands, NOT including register.
* It write to the file the operand extra information.
* The 3 option to deal with are: direct addressing (#), array, label(that is not array).
*/
bool handle_operand(symbol_table_list_t *list, char *word, char *operand_str, int line_num, int *ic, FILE *output_code, FILE *output_extern);

opcode_list_error_t opcode_list_init(opcode_list_t *opcode_list);
opcode_list_error_t opcode_list_add(opcode_list_t *opcode_list, int are, char *src_operand, char *des_operand, int opcode, int reserved, int line_num, int size);
opcode_list_error_t opcode_list_destroy(opcode_list_t *opcode_list);
void opcode_table_print_list(opcode_list_t *opcode_list);


#endif /* _OPCODE_H */