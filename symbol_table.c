#include "symbol_table.h"
#include <stdio.h>


symbol_table_error_t symbol_table_init(symbol_table_list_t *symbol_table)
{
	symbol_table->first = NULL;
	symbol_table->last = NULL;
	symbol_table->size = 0;
	return SYMBOL_TABLE_ERROR_SUCCESS;
}

symbol_table_error_t symbol_table_add(symbol_table_list_t *symbol_table, char *name, int address_value, symbol_table_type_t kind, bool is_extern, char *data)
{
	symbol_table_entry_t *new_entry = NULL;
	if (symbol_table == NULL)
		return SYMBOL_TABLE_ERROR_GIVE_NULL;

	/* create a new entry */
	new_entry = (symbol_table_entry_t*)malloc(sizeof(symbol_table_entry_t));
	if (new_entry == NULL)
	{
		printf("\ncan't allocate memory\n");
		exit(1);
	}

	/* get all the prameters inside the entry */
	new_entry->address_value = address_value;
	new_entry->kind = kind;
	new_entry->is_extern = is_extern;
	new_entry->is_entry = false;
	new_entry->next = NULL;
	if (data != NULL)
	{
		/* allocate memory for the string(data) */
		char *new_data = (char*)malloc((sizeof(char)*strlen(data)) + 1);
		if (new_data == NULL)
		{
			printf("\ncan't allocate memory\n");
			exit(1);
		}
		strcpy(new_data, data);
		new_entry->data = new_data;
	}
	else	/* the data is NULL */
	{
		new_entry->data = NULL;
	}

	if (name != NULL)
	{
		/* allocate memory for the string(name) */
		char *new_name = (char*)malloc((sizeof(char)*strlen(name)) + 1);
		if (new_name == NULL)
		{
			printf("\ncan't allocate memory\n");
			exit(1);
		}
		strcpy(new_name, name);
		new_entry->name = new_name;
	}
	else	/* the name is NULL */
	{
		new_entry->name = NULL;
	}

	/* if the list is empty */
	if (symbol_table->first == NULL)
	{
		symbol_table->first = new_entry;
		symbol_table->last = symbol_table->first;
		symbol_table->size++;

		return SYMBOL_TABLE_ERROR_SUCCESS;
	}
	else
	{
		symbol_table->last->next = new_entry;
		symbol_table->last = symbol_table->last->next;
		symbol_table->size++;

		return SYMBOL_TABLE_ERROR_SUCCESS;
	}
}

symbol_table_error_t symbol_table_destroy(symbol_table_list_t *symbol_table)
{
	symbol_table_entry_t* current = symbol_table->first;
	symbol_table_entry_t* Pnext = NULL;
	if (symbol_table == NULL)
	{
		return SYMBOL_TABLE_ERROR_GIVE_NULL;
	}

	while (current != NULL)
	{
		Pnext = current->next;
		if (current->name != NULL)	/*  free the name, if its exist */
			free(current->name);
		if (current->data != NULL)	/* free the data, if its exist */
			free(current->data);

		free(current);		/* free the entery */
		current = Pnext;
	}
	return SYMBOL_TABLE_ERROR_SUCCESS;

}

symbol_table_error_t symbol_table_find(symbol_table_list_t *symbol_table, const char* name, symbol_table_entry_t **entry)
{

	symbol_table_entry_t *current = NULL;

	if (symbol_table == NULL)
		return SYMBOL_TABLE_ERROR_GIVE_NULL;
 	current  = symbol_table->first;

	while (current != NULL)
	{	
		if(current->name == NULL)	/* if the name of the entry is NULL(strcmp wont work) */
			current = current->next;	
		else if (strcmp(current->name, name) == 0)	/* if the name we looked for exist in the symbol table list */
		{
			*entry = current;
			return SYMBOL_TABLE_ERROR_SUCCESS;
		}
		else
			current = current->next;
	}
	return SYMBOL_TABLE_ERROR_DIDNT_FOUND_ENTRY;	/* didn't found the name we looked for in the symbol table list */
}


symbol_table_error_t symbol_table_update_data_label_address(symbol_table_list_t *symbol_table, int ic_size)
{
	symbol_table_entry_t *current = NULL;

	if (symbol_table == NULL)
        return SYMBOL_TABLE_ERROR_GIVE_NULL;
    current  = symbol_table->first;

	while (current != NULL)
	{
		if (current->kind == SYMBOL_TYPE_LABEL_DATA || current->kind == SYMBOL_TYPE_LABEL_STRING)
		{
			current->address_value += ic_size;
		}

		current = current->next;
	}
	return SYMBOL_TABLE_ERROR_SUCCESS;
}


/* for debug use */
void symbol_table_print_list(symbol_table_list_t *symbol_table)
{
	symbol_table_entry_t* current = symbol_table->first;
	int i = 1;

	while (current != NULL)
	{
		printf("\nnode %d", i);
		printf("\nname: %s", current->name);
		printf("\naddress_value: %d", current->address_value);
		if (current->kind == SYMBOL_TYPE_MACRO)
			printf("\nkind: SYMBOL_TYPE_MACRO");
		else if (current->kind == SYMBOL_TYPE_LABEL_DATA)
			printf("\nkind: SYMBOL_TYPE_LABEL_DATA");
		else if (current->kind == SYMBOL_TYPE_LABEL_CODE)
			printf("\nkind: SYMBOL_TYPE_LABEL_CODE");
		else if (current->kind == SYMBOL_TYPE_LABEL_STRING)
			printf("\nkind: SYMBOL_TYPE_LABEL_STRING");
		else if (current->kind == SYMBOL_TYPE_LABEL_EXTERN)
			printf("\nkind: SYMBOL_TYPE_LABEL_EXTERN");

		printf("\nis extern: %d", current->is_extern);
		printf("\nis entry: %d", current->is_entry);
		printf("\ndata: %s", current->data);
		printf("\n-------------------------------------------------\n");

		current = current->next;
		i++;
	}
}

