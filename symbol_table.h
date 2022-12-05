/*pragma once*/
#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>
#include "bool.h"

/*
* All the types of data that can co stored inside the symbol table list
*/
typedef enum
{
	SYMBOL_TYPE_MACRO,
	SYMBOL_TYPE_LABEL_DATA,
	SYMBOL_TYPE_LABEL_STRING,
	SYMBOL_TYPE_LABEL_CODE,
	SYMBOL_TYPE_LABEL_EXTERN
} symbol_table_type_t;


/*
* A structer for the symbol list.
* A single node of the list.
*/
typedef	struct symbol_table_entry_t
{
	char *name;						/* name of the label or macro */
	int  address_value;				/* address for labal or value for macro */
	symbol_table_type_t kind;		/* what type this symbol is: macro, labal_data or labal_code */
	bool is_extern;					/* is extern or not */
	bool is_entry;					/* is it entry or not */
	char *data;						/* (only for data labels) if the label contains data write the data as string*/
	struct symbol_table_entry_t *next;
} symbol_table_entry_t;


/*
* A structer for the list
*/
typedef struct
{
	symbol_table_entry_t *first;	/* the first node of the list */
	symbol_table_entry_t *last;		/* the last node of the list */
	size_t size;					/* the size of the list */
} symbol_table_list_t;

/*
* All the errors types that can be return from creating a table
*/
typedef enum
{
	SYMBOL_TABLE_ERROR_SUCCESS,
	SYMBOL_TABLE_ERROR_GIVE_NULL,
	SYMBOL_TABLE_ERROR_DIDNT_FOUND_ENTRY

} symbol_table_error_t;

/*
* Initialize the list.
* input:	symbol_table list.
* output:	kind of error or sucess.
*/
symbol_table_error_t symbol_table_init(symbol_table_list_t *symbol_table);

/*
* Add the parameter to the tabel.
* input:	symbol_table list,
*			name of label\macro,
*			addrss of label or value of macro,
*			type of symbol: macro\ label_code\ label_data,
*			boolian if it's extern(true),
*			(for data label) write the data of the label.
* output:	kind of error or sucess.
*/
symbol_table_error_t symbol_table_add(symbol_table_list_t *symbol_table, char *name, int address_value, symbol_table_type_t kind, bool is_extern, char *data);

/*
* Destroy the list.
* input:	symbol_table list.
* output:	kind of error or sucess.
*/
symbol_table_error_t symbol_table_destroy(symbol_table_list_t *symbol_table);

/*
* At phase 2, when we want to extract data from the tabel.
* input:	symbol_table list,
* 		the name of label/macro we want to find,
* 		the address of a node.
* output:	kind of error or sucess.
*/
symbol_table_error_t symbol_table_find(symbol_table_list_t *symbol_table, const char* name, symbol_table_entry_t **entry);

/**
 * after the first run on the file we need to change the
 * address of all the data labels, according to the final ic.
 * input:	symbol_table list,
 *		the size of the final ic.
 * output:      kind of error or sucess.
 */
symbol_table_error_t symbol_table_update_data_label_address(symbol_table_list_t *symbol_table, int ic_size);

/* print the symbol table list,for debug use */
void symbol_table_print_list(symbol_table_list_t *symbol_table);


#endif /* _SYMBOL_TABLE_H */