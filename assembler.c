#include "assembler.h"
#include "opcode.h"
#include "Utilities.h"
#include "symbol_table.h"

#define MAX_LINE_LENGTH_ALLOWED (80)
#define MAX_WORD_SIZE (15)
#define Max_4from_word (8)


/* function declaration */
bool check_label_or_macro_name_taken(symbol_table_list_t *list, char *str);
int handle_data(symbol_table_list_t *list, char* str, int line_num);
bool handle_string(char *string_body, char *output_value, int line_num);
bool handle_macro(symbol_table_list_t *list, char *macro_body, int line_num);
bool handle_label(symbol_table_list_t *list, opcode_list_t *opcode_list, char *line, unsigned int *dc, unsigned int *ic, int line_num);
bool update_entry(FILE *input, symbol_table_list_t *list);
bool phase_1(FILE *input, FILE *output_code_tmp, symbol_table_list_t *list, opcode_list_t *opcode_list);
bool phase_2(symbol_table_list_t *symbol_table, opcode_list_t *opcode_list, FILE *output_code_tmp, FILE *output_code, FILE *output_entry, FILE *output_extern);
void encode_data(symbol_table_list_t *symbol_table, symbol_table_entry_t *symbol_table_node, int *dc, FILE *output_code);
void extract_entry_to_file(symbol_table_list_t *symbol_table, FILE *output_entry);
void Base4_code(FILE * input, FILE * output);
void get4FromDigit(char *two_digit, char *resultString, int put_in);


/**
 * check if the macro or label name is already taken.
 * input:	symbol table list, string(the name to check).
 * output:	already taken: true, else false.
 */
bool check_label_or_macro_name_taken(symbol_table_list_t *list, char *str)
{
        symbol_table_entry_t *node = NULL;
        if (symbol_table_find(list, str, &node) == SYMBOL_TABLE_ERROR_SUCCESS)
        {
                return true;
        }
        return false;
}


/* check if there is a syntax problem with the data instruction.
* input: string of data
* output: -1 if syntax error, number of arguments if syntax ok.
*/
int handle_data(symbol_table_list_t *list, char* str, int line_num)
{
	int count = 0;
	char copy[MAX_LINE_LENGTH] = { 0 }; /* copy of the orignal string */
	char *first_comma = NULL;
	char *second_comma = NULL;
	
	strcpy(copy,str);
	first_comma = copy;
        second_comma = strstr(first_comma,",");


	if(first_comma[0] != ' ' && first_comma[0] != '\t' ) 	/* the first letter after ".data" have to be a space or a tab */
	{
		fprintf(stderr, "\nERROR: data definition, after \".data\" there has to be a space or a tab. In line %d", line_num);
       		return (-1);
	}

	/* if there is more than 1 word */
	while(second_comma != NULL)
	{
		*second_comma = '\0';
		if(count_words(first_comma) != 1)	/* if there is more than word between commas(error) */
		{
			fprintf(stderr, "\nERROR: data definition, wrong syntax at the data initialization. in line: %d", line_num);
               		return (-1);
	        }
	
		remove_spaces(first_comma);			
		if(!string_is_integer(first_comma))			/* check if thw word is an integer */
		{
			if( !check_if_macro_inti(list, first_comma) ) /* if it's not an integer it have to be macro */
			{
				fprintf(stderr, "\nERROR: data definition, use undefined macro name: %s. In line %d", first_comma, line_num);
                		return (-1);
			}

		}
		
		first_comma = second_comma + 1;			/* put the pointer of the first comma in the place of the second comma+1 */
		second_comma = strstr(first_comma,",");		/* search for a new comma */
		count++;
	}


	if(count_words(first_comma) == 1)		/*check the last word or if there is only 1 word check it */
	{
		remove_spaces(first_comma);
		if(!string_is_integer(first_comma)) 	/* if the word is not an integer */
		{
			if( !check_if_macro_inti(list, first_comma) ) /* if it's not an integer it have to be macro */
                	{
				fprintf(stderr, "\nERROR: data definition, use undefined macro name: %s. In line %d", first_comma, line_num);
				return (-1);
                 	}

	    	}

		count++;
	}
	else	/* if there are no words at all(error) */
	{
		fprintf(stderr, "\nERROR: data definition, wrong syntax at the data initialization. in line: %d", line_num);
         	return (-1);

	}
	return(count);
}


/* check if there is a syntax problem with string definition */
bool handle_string(char *string_body, char *output_value, int line_num)
{

        char *quote = strstr(string_body, "\"");
        char *value = NULL;
        char *end_quote = NULL;
        char *after_end_quote = NULL;
	char *after_last_qoute = NULL;

	if(string_body[0] != ' ' && string_body[0] != '\t' )    /* the first letter after ".string" have to be a space or a tab */
        {
		fprintf(stderr, "\nERROR: data definition, after \".string\" there has to be a space or a tab. In line %d", line_num);
       		return false;
        }


        if (!quote)             /* if the first quote is missing */
        {
		fprintf(stderr,	"\nERROR: string definition, missing a quote. in line: %d", line_num);
                return false;
        }

        value = quote + 1;


        end_quote = strstr(value, "\"");
        if (!end_quote)         /* if the first quote is missing */
        {
		fprintf(stderr, "\nERROR: string definition, missing a quote. in line: %d", line_num);
                return false;
        }

	after_last_qoute = strstr(end_quote+1, "\"");
        while(after_last_qoute != NULL)        /* find the last qoute if there is more than 2 qoutes */
        {
		end_quote = after_last_qoute;
		after_last_qoute = strstr(end_quote+1, "\"");
        }


        *quote = '\0';
        if (count_words(string_body) != 0)      /* if there is a charcter before the first quote */
        {
		fprintf(stderr, "\nERROR: string definition, can't be letters before the first quote. in line: %d", line_num);
                return false;
        }

        after_end_quote = end_quote + 1;
        if (count_words(after_end_quote) != 0)  /* if there is a charcter after the second quote */
        {
		fprintf(stderr, "\nERROR: string definition, can't be letters after the second quote. in line: %d", line_num);
                return false;
        }

        *end_quote = '\0';      /* value need to be the phrase between the qoutes */

        strcpy(output_value, value);
        return true;
}



/* check if there is a syntax problem with macro definition, and insert data to the symbol table */
bool handle_macro(symbol_table_list_t *list, char *macro_body, int line_num)
{
        char *equals_sign = strstr(macro_body, "=");
        char *value_str = equals_sign + 1;
        int value_int = 0;

        if (!equals_sign)               /* the macro sentence doesn't have equal sign */
        {
		fprintf(stderr, "\nERROR: illegal macro sentence, missing equal sign. In line %d", line_num);
		return false;
        }

        *equals_sign = '\0';    /* split the string and check syntax */

        if (count_words(macro_body) != 1 || count_words(value_str) != 1) /* check that the name and the value are only one word */
        {
		fprintf(stderr, "\nERROR: illegal macro sentence, in line %d", line_num);
		return false;
        }

        remove_spaces(macro_body);              /* remove all the spaces from the macro name */
        remove_spaces(value_str);               /* remove all the spaces from the macro value */

        if ( !(string_only_letters_or_numbers(macro_body) == ONLY_SMALL_LETTERS) )  /* check if the name is a small letter */
        {
		fprintf(stderr, "\nERROR: macro name : %s, need to have only have small letters, at line %d", macro_body, line_num);
		return false;
        }
        if ( !string_is_integer(value_str) )  /* check if the value is integer */
        {
		fprintf(stderr, "\nERROR: macro value: %s, have to be an integer, in line %d", value_str, line_num);
		return false;
        }

	if (!check_if_forbiden_symbol(macro_body))
	{
		fprintf(stderr, "\nERROR: used a forbiden symbol: %s, for a macro. in line: %d", macro_body, line_num);
		return false;
	}

        if (check_label_or_macro_name_taken(list, macro_body))  /* check if the macro name is already taken */
        {
		fprintf(stderr, "\nERROR: the macro name: %s is already taken, at line %d", macro_body, line_num);
		return false;
        }
 	value_int = atoi(value_str);	/* turn the string to an integer */

        /* put the macro inside the symbol table */
        symbol_table_add(list, macro_body, value_int, SYMBOL_TYPE_MACRO, false, NULL);
        return true;
}


/*
* check the syntax of labels, and insert data in the symboltable if needed.
* output: 0 not a label
*/
bool handle_label(symbol_table_list_t *list, opcode_list_t *opcode_list, char *line, unsigned int *dc, unsigned int *ic, int line_num)
{
        char *name = line;
        char *colon = strstr(line, ":");

        char *string_def = strstr(line, ".string");     /* check the case that it is string */
        char *data_def = strstr(line, ".data");         /* check the case that it is data */
	char *entry_def = strstr(line, ".entry");
	char *extern_def = strstr(line, ".extern");


        /********************************************/
        /* check if the name of the label is legal */
        /******************************************/
       	if(colon != NULL) 
		*colon = '\0';
	else
	{
		fprintf(stderr, "\nERROR: UNACCEPTED error in label defination. In line %d", line_num);
                return false;
	}

        if (count_words(name) != 1)             /* if there is more than on word in the label name */
        {
		fprintf(stderr, "\nERROR: label name need to be only one word, in line: %d", line_num);
                return false;
        }
		
	if( *(colon-1) == ' ' || *(colon-1) == '\t' )		/* the colon have to be attached to the label name */
	{
		fprintf(stderr, "\nERROR: label name have to end with a colon(:), can't be spaces between. in line: %d", line_num);
		return false;
	}

	remove_spaces(name);

	if (!check_if_its_label(list, name, line_num)) /* check if the labal name is valied */
		return false;

	if( check_label_or_macro_name_taken(list, name) ) /* check if the label name already initialized */
	{
		fprintf(stderr, "\nERROR: the label name: %s is already taken, at line %d", name, line_num);
		return false;
	}

        /*********************************/
        /* check the syntax of the body */
        /*******************************/


        /***************************/
        /* deal with string label */
        if (string_def)
        {
                char *string_body = string_def + 7;             /* move the pointer after the 7 chars of ".string" */
                char value[MAX_LINE_LENGTH] = { 0 };

                *string_def = '\0';
                if (count_words(colon + 1) != 0)
                {
			fprintf(stderr, "\nERROR: string definition, there can't be any letters before the dot(.). in line: %d", line_num);
                        return false;
                }

                if (!handle_string(string_body, value, line_num))
                        return false;

                symbol_table_add(list, name, *dc, SYMBOL_TYPE_LABEL_STRING, false, value); /*remeber! the dc change acording to the ic*/

                *dc += strlen(value) + 1; /* advance dc by the number of letter in the string +1 for the null*/
                                                                                                                           
		 return true;
        }
	/*************************/
        /* deal with data label */
        else if (data_def)
        {
                char *value = data_def + 5;             /* move the pointer after the 5 chars of ".data" */
                int number_of_args = handle_data(list, value, line_num); /* check the syntax, and return the number of arguments*/

                *data_def = '\0';
                if (count_words(colon + 1) != 0)
                {
			fprintf(stderr, "\nERROR: data definition, there can't be any letters before the dot(.). in line: %d", line_num);
                        return false;
                }

                if (number_of_args == EOF) /* check the syntax of data defintion */
                {
                        return false;
                }
                remove_spaces(value);

                symbol_table_add(list, name, *dc, SYMBOL_TYPE_LABEL_DATA, false, value); /*remeber! the dc change acording to the ic*/

                *dc += number_of_args ;    /* advance dc by the number of integers that had been scaned */

                return true;
        }
	else if (entry_def)
	{
		fprintf(stderr, "\nERROR: you used entry defintion in a label. in line: %d", line_num);
		return false;

	}
	else if (extern_def)
	{
		fprintf(stderr, "\nERROR: you used extern defintion in a label. in line: %d", line_num);
		return false;
	}
	/**************************/
        /* deal with opcode label*/
        else
        {
                char *opcode_body = colon + 1;
                if (!opcode_parse(list, opcode_list, opcode_body , line_num))
                        return false;

		symbol_table_add(list, name, *ic, SYMBOL_TYPE_LABEL_CODE, false, NULL);
                *ic = *ic + opcode_list->last->size;      /* advance dc by the number of integers that had been scaned */

                return true;
        }

}


/**
* This function read the file again and update the is_entry field in the symbol table.
* If the label name ,which defined after the entry definition, doesn't exist it retuen an error. 
*/
bool update_entry(FILE *input, symbol_table_list_t *list)
{
	char line[MAX_LINE_LENGTH] = { 0 };
	int line_num = 1;
	bool flag = true; /* when it is false there is an error */
	fseek(input, 0, SEEK_SET);
	while (fgets(line, sizeof(line) - 1, input))
	{
		
		char *entry_definiton = strstr(line, ".entry");

		/********************************/
		/* check if its entry sentence */
		if (entry_definiton)
		{
			char *entry_body = entry_definiton + 6;       /* +6 its the name of the entry label */
			symbol_table_entry_t *node = NULL;

			*entry_definiton = '\0';

			if (count_words(line) != 0)     /* if there is a charchter before the dot . */
				break;
			
			*entry_definiton = '.';
			if (count_words(line) != 2) /*check that there are only 2 words in total in the sentence*/
				break;

			remove_spaces(entry_body);
			if (!check_if_forbiden_symbol(entry_body))
				break;

			if (symbol_table_find(list, entry_body, &node) == SYMBOL_TABLE_ERROR_SUCCESS)
			{
				node->is_entry = true; /* if the label exist in the symbol table, turn the is_entry field to true */
			}
			else
			{
				fprintf(stderr, "\nERROR: entry definition, this label name: %s, didn't initialize in the file .In line: %d", entry_body, line_num);
				flag = false;
			}
		}


		++line_num;
	}

	return flag;
}



/*
* Do phase 1. check that all the syntax is fine ,and fill the symbol tabel.
* input:        the assembly file, the symbol tabel list.
* output:       error or sucsses.
*/
bool phase_1(FILE *input, FILE *output_code_tmp, symbol_table_list_t *list, opcode_list_t *opcode_list)
{
        char line[MAX_LINE_LENGTH] = { 0 };
        unsigned int ic = 100;                /* counter for the code lines */
        unsigned int dc = 0;                  /* counter for the data lines */
        int line_num = 1;               /* counter for the lines in the assembly file */
        bool flag = false;              /* if there is a prase(syntax) problem turned to true */
        fseek(input, 0, SEEK_SET);

        while (fgets(line, sizeof(line) - 1, input))
        {
		
		char *end_of_line = strstr(line, "\n");
		char *carriage_return = strstr(line, "\r"); /* for microsoft files */
                char *macro_definiton = strstr(line, ".define");
                char *entry_definiton = strstr(line, ".entry");
                char *extern_defintion = strstr(line, ".extern");
                char *data_defintion = strstr(line, ".data");
                char *string_defintion = strstr(line, ".string");
                char *comment_defintion = strstr(line, ";");
                char *label_defintion = strstr(line, ":");
	
		if(end_of_line != NULL)
		{
			*end_of_line = '\0';	 /*put a null in the new line(\n) if its exist */
		}
		if(carriage_return != NULL)	
		{
			*carriage_return = '\0';  /* put null in carriage return, if it exist */
		}	
	
		/* check that the maximum of characters in a line is no more than 80 */
		if(strlen(line) > MAX_LINE_LENGTH_ALLOWED )
		{
			fprintf(stderr, "\nERROR: the maximum length allowed for a line is %d characters + new line. In line %d", MAX_LINE_LENGTH_ALLOWED, line_num);
			flag = true;
			goto next_line;
		}	
		
		
        /***********************************/
        /* check if its an empty sentence */
        if (count_words(line) == 0)
        {
			goto next_line;
        }
        /************************************/
        /* check if its a comment sentence */
        if (comment_defintion)
        {
		*comment_defintion = '\0';

		if (strstr(line, "\"")) /* if there is a qoute before the semicolon(;) (maybe part of a string) */
            	{
			*comment_defintion = ';';       /* maybe it's part of a string, put the semicolon back */
		}
		else	/* if its not part of a string */
		{
			remove_spaces(line);

			if (line[0] != '\0')            /* if there are a charchters before semicolon (;) */
			{
				fprintf(stderr, "\nERROR: used ; not at the strat of the line, in line %d", line_num);
                    		flag = true;
                	}
                	else
				goto next_line;         /* if it realy a comment and not a part of a string, go to the next line */
		}
	}
        /********************************************/
        /* if its a sentence with label, handel it */
        if (label_defintion)
        {
		*label_defintion = '\0';

		if (strstr(line, "\"")) /* if there is a qoute before the colon (:) (maybe part of a string) */
		{
			*label_defintion = ':';      /* maybe it's part of a string, put the colon back */
            	}
            	else            /* if its not part of a string */
		{
			*label_defintion = ':';  /* return the colon to the string */
			if (!(handle_label(list, opcode_list, line, &dc, &ic, line_num)))/* if the is an error */
				flag = true;

			goto next_line;
		}
	}
        /*********************************/
	/* check if its string sentence */
        if (string_defintion)
        {
		char *string_body = string_defintion + 7;       /* move the pointer after the 7 chars of ".string" */
		char value[MAX_LINE_LENGTH] = { 0 };

		*string_defintion = '\0';
		if (count_words(line) != 0)
		{
			fprintf(stderr, "\nERROR: used string definition not at the start of the line. in line: %d", line_num);
			flag = true;
			goto next_line;
		}

		if (!handle_string(string_body, value, line_num))       /* handle the string syntax */
            	{
			flag = true;
			goto next_line;
            	}

		symbol_table_add(list, NULL, dc, SYMBOL_TYPE_LABEL_STRING, false, value); /*remeber! the dc change acording to the ic*/

            	dc += strlen(value) + 1; /* advance dc by the number of letter in the string +1 for the null */

		goto next_line;;

	}
        /*******************************/
        /* check if its data sentence */
	if (data_defintion)
	{
		char *value = data_defintion + 5;               /* move the pointer after the 5 chars of ".data" */
		int number_of_args = handle_data(list, value, line_num); /* check the syntax, and return the number of arguments*/
		*data_defintion = '\0';
		if (count_words(line) != 0)
		{
			fprintf(stderr, "\nERROR: used data definition not at the start of the line. in line: %d", line_num);
			flag = true;
			goto next_line;
		}
		if (number_of_args == EOF) /* check the syntax of data defintion */
		{
			flag = true;
			goto next_line;
		}
		remove_spaces(value);

		symbol_table_add(list, NULL, dc, SYMBOL_TYPE_LABEL_DATA, false, value); /*remeber! the dc change acording to the ic*/

		dc += number_of_args;     /* advance dc by the number of integers that had been scaned */

		goto next_line;;
	}
        /********************************/
        /* check if its macro sentence */
        if (macro_definiton)
	{
		char *macro_body = macro_definiton +7;  /* +7 becouse we want to check the body of the sentence  */

		*macro_definiton = '\0';
		if (count_words(line) != 0)     /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: used macro definition not at the start of the line, in line %d", line_num);
			flag = true;
			goto next_line;
		}
		/* handle macro syntax, and insert data inside the symbol table */
		if (!handle_macro(list, macro_body, line_num))
		{
			flag = true;
			goto next_line;
		}
			
		goto next_line;
	}
	/********************************/
	/* check if its entry sentence */
	if (entry_definiton)
	{
		char *entry_body = entry_definiton + 6;       /* +6 its the name of the entry label */
		*entry_definiton = '\0';

		if (count_words(line) != 0)     /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: used entry definition not at the start of the line, in line %d", line_num);
			flag = true;
			goto next_line;
		}

		*entry_definiton = '.';
		if (count_words(line) != 2) /*check that there are only 2 words in total in the sentence*/
		{
			fprintf(stderr, "\nERROR: used entry definition , can only accept one label. in line %d", line_num);
			flag = true;
			goto next_line;
		}
			
		remove_spaces(entry_body);

		if ( !check_if_forbiden_symbol(entry_body) )
		{
			fprintf(stderr, "\nERROR: used a forbiden symbol: %s, for a label. in line: %d", entry_body, line_num);
        		flag = true;
			goto next_line;
		}
        	

		goto next_line;
	}

        /*********************************/
	/* check if its extern sentence */
	if (extern_defintion)
	{
		char *extern_body = extern_defintion + 7;	/* +7 its the name of the extern label */
		*extern_defintion = '\0';
		if (count_words(line) != 0)     /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: used extern definition not at the start of the line, in line %d", line_num);
                	flag = true;
			goto next_line;
            	}

		*extern_defintion = '.';
		if (count_words(line) != 2) /*check that there are only 2 words in total in the sentence*/
		{
			fprintf(stderr, "\nERROR: used extern definition , can only accept one label. in line %d", line_num);
			flag = true;
			goto next_line;
		}

		remove_spaces(extern_body);	
			
		if (!check_if_its_label(list, extern_body, line_num))
		{
        		flag = true;
			goto next_line;
		}
       		
		if (check_label_or_macro_name_taken(list, extern_body))  /* check if the label name is already taken */
       		{
			fprintf(stderr, "\nERROR: the label name: %s, is already taken, at line %d", extern_body, line_num);
			flag = true;
			goto next_line;
       		}
			
		symbol_table_add(list, extern_body, 0, SYMBOL_TYPE_LABEL_EXTERN, true, NULL);	
		goto next_line;
	}
        /************************************/
        /* check if its an opcode sentence */
         	
	if (!opcode_parse(list, opcode_list, line, line_num))
		flag = true;
	if(opcode_list->first != NULL )
       		ic = ic + opcode_list->last->size;      /* advance dc by the number of integers that had been scaned */

	

        


next_line:
     	++line_num;
	}

	if (!update_entry(input, list))
		flag = true;

	if(!flag)	/* update the adress of all the data labels according to the final ic */
		symbol_table_update_data_label_address(list,ic);	
	
	if (!flag)
	{
		ic = ic - 100;
		if(ic != 0 || dc != 0)  /* if the file is empty dont wite anything */
		fprintf(output_code_tmp, "\t%u %u\n", ic, dc);	/*write the number of code lines and the number of data line*/
	}

    return flag;
}



/**
* This function translate the data to binary code and put it into the tmp file.
* input:	symbol_table_list,
*			symbol_table_entry,
*			dc(data line number),
*			output_code file.
*/
void encode_data(symbol_table_list_t *symbol_table, symbol_table_entry_t *symbol_table_node, int *dc, FILE *output_code)
{

	char word[MAX_WORD_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };	/* word size, dont forget the null in the end */

	/* clean the array */
	word[MAX_WORD_SIZE - 1] = '\0';
	clean_word(word);

	/* if the data is string, insert it to the output_code file */
	/***********************************************************/
	if (symbol_table_node->kind == SYMBOL_TYPE_LABEL_STRING)
	{
		int i;
		char *string = symbol_table_node->data;
		for (i = 0; string[i] != '\0'; i++) /* insert every char, according to the ascii table */
		{
			dec_to_binary(word, string[i], MAX_WORD_SIZE - 1);

			fprintf(output_code, "%d\t%s\n", *dc, word); /* print the word to a tmp file */
			*dc = *dc + 1;	/* move forword the line counter */
			clean_word(word);	/* turn all the cell in the array back to zero */
		}
		dec_to_binary(word, 0, MAX_WORD_SIZE - 1); /* insert the null at the end of the string */

		fprintf(output_code, "%d\t%s\n", *dc, word); /* print the word to a tmp file */
		*dc = *dc + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */
	}

	/* if the data is numeric(.data), insert it to the output_code file*/
	/******************************************************************/
	else if (symbol_table_node->kind == SYMBOL_TYPE_LABEL_DATA)
	{
		char *num_str = symbol_table_node->data; /* the str */
		int num_int = 0;
		char *comma = strstr(num_str, ",");
		symbol_table_entry_t *macro_node = NULL;

		while (comma != NULL)
		{
			*comma = '\0';
			if(string_is_integer(num_str)) /* if its a number  */
				num_int = atoi(num_str);
			else /* if its macro */
			{
				symbol_table_find(symbol_table, num_str, &macro_node);
				num_int = macro_node->address_value;
			}

			dec_to_binary(word, num_int, MAX_WORD_SIZE - 1);

			fprintf(output_code, "%d\t%s\n", *dc, word); /* print the word to a tmp file */
			*dc = *dc + 1;	/* move forword the line counter */
			clean_word(word);	/* turn all the cell in the array back to zero */


			*comma = ',';
			num_str = comma + 1;
			comma = strstr(num_str, ",");
		}

		/* the last word, or the first word if it's not an array */
		if(string_is_integer(num_str)) /* if its a number  */
                	num_int = atoi(num_str);
                else /* if its macro */
               	{
             		symbol_table_find(symbol_table, num_str, &macro_node);
               		num_int = macro_node->address_value;
              	}

		dec_to_binary(word, num_int, MAX_WORD_SIZE - 1);

		fprintf(output_code, "%d\t%s\n", *dc, word); /* print the word to a tmp file */
		*dc = *dc + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */


	}

}


/**
* This function write the entry file.
* input:	symbol_table_list,
*			output_entry file.
*/
void extract_entry_to_file(symbol_table_list_t *symbol_table, FILE *output_entry)
{

	symbol_table_entry_t *symbol_table_node = symbol_table->first;

	while (symbol_table_node != NULL)
	{
		if (symbol_table_node->is_entry)
		{
			/* print the word to a entry file */
			if (symbol_table_node->address_value < 1000)
				fprintf(output_entry, "%s\t0%d\n", symbol_table_node->name, symbol_table_node->address_value);
			else
				fprintf(output_entry, "%s\t%d\n", symbol_table_node->name, symbol_table_node->address_value);
		}
		symbol_table_node = symbol_table_node->next;
	}

}



/* convert the binary base to special 4 base  */
void Base4_code(FILE * input, FILE * output)
{

	int count_line = 1;
	char line[MAX_LINE_LENGTH] = { 0 };
	char resultString[Max_4from_word] = { '1', '1', '1', '1', '1', '1', '1', '1' };
	
	resultString[Max_4from_word - 1] = '\0'; 

	fseek(input, 0, SEEK_SET);
	
	while (fgets(line, sizeof(line) - 1, input))
	{

		
		char *end_line_def = strstr(line, "\n");
		if(end_line_def != NULL)
			*end_line_def = '\0';
	
		/* if its the first line  */
		if(count_line == 1) 
		{
			 fprintf(output, "%s\n",line);
		}
		/* all the other lines */
		else 
		{	
			char *tab = strstr(line, "\t");
			char tmp; 
			int i, j;	
			char* first_word = line;
			int first_word_int = atoi(first_word);
			char* second_word = tab + 1;
			if(tab == NULL)
			{
				printf("\nERROR, in function 'Base4_code' didn't fint tab, problem unexepted");
				exit(1);
			}
			*tab = '\0';
			
			/* deal with the first word */
			if(first_word_int < 1000)
				fprintf(output, "0%s\t",first_word);		
			else
				fprintf(output, "%s\t",first_word);


			/* deal with the second word, the binary one */
			for(i = 2, j = 0; i < MAX_WORD_SIZE ; i = i + 2)
			{
				tmp = second_word[i]; /* save a char we will earse for a moment */

				if(second_word[i] != '\0') /* the last letter is null */
					second_word[i] = '\0';
				
				get4FromDigit(second_word + (i-2), resultString, j); /* put the special letter in the resultString*/
				
				second_word[i] = tmp; /* put back the char we saved  */

				if(j >= MAX_LINE_LENGTH)
				{
					printf("\nERROR, in function 'Base4_code' overwrite, problem unexepted");
					exit(1);
				}

				j++;
			}
			resultString[Max_4from_word - 1] = '\0';
			fprintf(output, "%s\n",resultString);
			

		}
		
		count_line++;
	}





}


/*convert decimal to base4
* get: integer
* return
*/
void get4FromDigit(char *two_digit, char *resultString, int put_in)
{
	
	char returnVal = '0';

	if(strcmp(two_digit,"00") == 0)
		returnVal = '*';
	
	else if(strcmp(two_digit,"01") == 0)
		returnVal = '#';
	
	else if(strcmp(two_digit,"10") == 0)
		returnVal = '%';
		
	else if(strcmp(two_digit,"11") == 0)
		returnVal = '!';

	else
	{
		printf("\nERROR, in function 'get4FromDigit' didn't found a symbol, problem unexepted");
	}
	
	resultString[put_in] = returnVal;
}





bool phase_2(symbol_table_list_t *symbol_table, opcode_list_t *opcode_list, FILE *output_code_tmp, FILE *output_code, FILE *output_entry, FILE *output_extern)
{
	int ic = 100;
	int dc = 0;
	bool flag = false; /* if there are error in the compilation, turn to true */
	opcode_t *opcode_node = opcode_list->first;
	symbol_table_entry_t *symbol_table_node = symbol_table->first;

	while(opcode_node != NULL)
	{
		if (!opcode_encode(opcode_node, symbol_table, &ic, output_code_tmp, output_extern)) /*dont forget to put if*/
			flag = true;
		opcode_node = opcode_node->next;
	}

	dc = ic;
	while (symbol_table_node != NULL)
	{
		encode_data(symbol_table, symbol_table_node, &dc, output_code_tmp);
		symbol_table_node = symbol_table_node->next;
	}
	
	extract_entry_to_file(symbol_table, output_entry);
	Base4_code(output_code_tmp, output_code);

	return flag;
}

bool assembler(FILE *input, FILE *output_code_tmp, FILE *output_code, FILE *output_entry, FILE *output_extern)
{
	bool flag = false; /* if there are problems, turn to true */
	symbol_table_list_t symbol_table;	/* initialize the symbol table list */
	opcode_list_t opcode_table;		/* initialize the opcode table list */
	if (symbol_table_init(&symbol_table)) 
	{
		printf("\nfailed init symbol table");
		exit(1);
	}

	if (opcode_list_init(&opcode_table))
	{
		printf("\nfailed init opcode table");
		exit(1);
	}

	if ( phase_1(input, output_code_tmp, &symbol_table, &opcode_table) )
	{
		flag = true;
		goto error;
	}	


	if( phase_2(&symbol_table, &opcode_table, output_code_tmp, output_code, output_entry, output_extern) )
	{
		flag = true;
		goto error;
	}

error:

	symbol_table_destroy(&symbol_table);
	opcode_list_destroy(&opcode_table);     

	return flag;
}


