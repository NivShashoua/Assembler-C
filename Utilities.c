#include "Utilities.h"


const char* const forbiden_symbols_string[FORBIDEN_SYMBOLS_SIZE] =
{
	"r0",
	"r1",
	"r2",
	"r3",
	"r4",
	"r5",
	"r6",
	"r7",
	"data",
	"string",
	"entry",
	"extern",
	"mov",
	"cmp",
	"add",
	"sub",
	"not",
	"clr",
	"lea",
	"inc",
	"dec",
	"jmp",
	"bne",
	"red",
	"prn",
	"jsr",
	"rts",
	"stop"
};


/**
* check if a string is one of the fobiden symbols.
* input: string.
* output: if it is fobiden return false, else true.
*/
bool check_if_forbiden_symbol(char *str)
{
	int i;
	
	if (str == NULL)
	{
		printf("\nERROR: problem in function 'check_if_forbiden_symbol', given NULL, not suppost to happen.");
		exit(1);
	}

	for (i = 0; i < FORBIDEN_SYMBOLS_SIZE; i++)         /* check if it doesn't use forbiden symbols */
	{
		if (!strcmp(str, forbiden_symbols_string[i]))
		{
			return false;
		}
	}
	return true;
}

/* check if the label is valied */
bool check_if_its_label(symbol_table_list_t *list, char *name, int line_num)
{
	int i;
	if (name[0] >= '0' && name[0] <= '9')                   /* if the first letter of the name is a digit */
	{
		fprintf(stderr, "\nERROR: the first letter in a label can't be a number, in line: %d", line_num);
		return false;
	}


	if (strlen(name) > MAX_LABEL_NAME_LENGTH)               /* if the name of the label is more than the maximum(31) */
	{
		fprintf(stderr, "\nERROR: the label name can't be more than: %d characters, in line: %d", MAX_LABEL_NAME_LENGTH, line_num);
		return false;
	}


	for (i = 0; i < FORBIDEN_SYMBOLS_SIZE; i++)             /* check if it doesn't use forbiden symbols */
	{
		if (!strcmp(name, forbiden_symbols_string[i]))
		{
			fprintf(stderr, "\nERROR: used a forbiden symbol: %s, for a label, in line: %d", name, line_num);
			return false;
		}
	}


	if (string_only_letters_or_numbers(name) == NOT_ONLY_LETTERS_AND_NUMBERS)/* check that the name is only letters or numbers */
	{
		fprintf(stderr, "\nERROR: label name: %s can only be numbers or letters, in line: %d", name, line_num);
		return false;
	}

	if (check_if_macro_inti(list, name))  /* if label with the same name already exist */
	{
		fprintf(stderr, "\nERROR: this label name: %s, is already initialized as a macro, in line: %d", name, line_num);
		return false;
	}

	return true;
}



/* check if the macro already initialize in the symbol table, use this when the macro used as operand */
bool check_if_macro_inti(symbol_table_list_t *list, char *str)
{
	symbol_table_entry_t *node = NULL;
	if (symbol_table_find(list, str, &node) == SYMBOL_TABLE_ERROR_SUCCESS)
	{
		if (node->kind == SYMBOL_TYPE_MACRO)
			return true;
	}
	return false;
}

/**
* check if the string is integer.
* first charcters can be a number,+,-.
* no spaces in the string!
* not an empty string!
*/
bool string_is_integer(char *str)
{
	int i;
	int len = 0; 	/* length of the word not including spaces */

	for (i = 0; str[i] != '\0'; i++)
	{
		/* if the value isn't an integer, first char can be: number,-,+ */
		if (i == 0 && !(str[i] >= '0' && str[i] <= '9'))
		{
			if (str[i] != '-' && str[i] != '+')
				return false;
		
		}
		/* if the value isn't an integer */
		else if (!(str[i] >= '0' && str[i] <= '9'))
		{
			return false;
		}
		len++;
	}
	/* if the integer is only 1 word and its + or -(error) */
	if(len == 1 && (str[0] == '-' || str[0] == '+')  )
		return false;

	return true;
}


/** 
* check if the string only have letters.
* no spaces in the string!
* return case for: not only letters and numbers, only numbers, only capital letters,
* only small letters and both(samll and capital).
*/
STRING_ONLY_LETTER string_only_letters_or_numbers(char *str)
{
	int i;
	bool small_letter_on = false;
	bool capital_letter_on = false;
	bool num_on = false;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			small_letter_on = true;

		else if (str[i] >= 'A' && str[i] <= 'Z')
			capital_letter_on = true;

		else if (str[i] >= '0' && str[i] <= '9')
			num_on = true;

		else	/* if its not only letters */
			return NOT_ONLY_LETTERS_AND_NUMBERS;
	}
	if (small_letter_on && capital_letter_on && num_on)
		return SMALL_AND_CAPITAL_AND_NUM;

	else if (small_letter_on && capital_letter_on)
		return BOTH_SMALL_AND_CAPITAL;

	else if (small_letter_on && num_on)
		return BOTH_SMALL_AND_NUM;

	else if (capital_letter_on && num_on)
		return BOTH_CAPITAL_AND_NUM;

	else if (small_letter_on)
		return ONLY_SMALL_LETTERS;

	else if (capital_letter_on)
		return ONLY_CAPITAL_LETTERS;

	else if (num_on)
		return ONLY_NUMBERS;
	else 	/* not supposed to function */
		 return NOT_ONLY_LETTERS_AND_NUMBERS;
}


/** 
* ver.2
* check if the string only have letters.
* no spaces in the string!
* return case for: not only letters and numbers and other symbols, only numbers, only capital letters,
* only small letters and both(samll and capital).
*/
STRING_ONLY_LETTER string_only_vaild_char(char *str){
	int i;
	bool small_letter_on = false;
	bool capital_letter_on = false;
	bool num_on = false;
	bool opr = false; 
	bool opr_non_vaild =true;
	bool vaild_sign =true;
	
	for (i = 0; str[i] != '\0'; i++)
	{
		
		 if (str[i] >= 'a' && str[i] <= 'z')
			small_letter_on = true;
		 if (str[i] == '+' || str[i] == '-' || str[i] == '[' || str[i] == ']')
			opr = true;
		 if (str[i] >= 'A' && str[i] <= 'Z')
			capital_letter_on = true;
		 if (str[i] >= '0' && str[i] <= '9')
			num_on = true;
		 if (str[i] >= '!' && str[i] <= '*')
			opr_non_vaild = false;
		 if (str[i] == ',' || str[i] == '.' || str[i] == '/')
			opr_non_vaild = false;
		 if (str[i] >= ':' && str[i] <= '@')
			opr_non_vaild = false;
		 if(((str[i] >= '!' && str[i] <= 'Z') || (str[i] >= '^' && str[i] <= '~')) && (str[i+1] == '-' || str[i+1] == '+'))
			vaild_sign = false;
		 if ((str[i] >= '^' && str[i] <= '`') || (str[i] >= '{' && str[i] <= '~'))
			opr_non_vaild = false;
		 if (str[i] == ']' && (str[i+1] >= '!' && str[i+1] <= '~'))
			vaild_sign = false;
		 if (str[i] == '[' && str[i+1] == ']' )
			vaild_sign = false;
		 if (strchr(str, '[') && !strchr(str, ']') )
			vaild_sign = false;
		 if (!strchr(str, '[') && strchr(str, ']') )
			vaild_sign = false;

		
	}
	

	if(string_only_letters_or_numbers(str)!=NOT_ONLY_LETTERS_AND_NUMBERS)
		opr = true;
	if(vaild_sign  && opr && opr_non_vaild)
		return VAILD;
	if((vaild_sign && opr && opr_non_vaild) && (small_letter_on || capital_letter_on || num_on))
		return VAILD;
	else
		return NOT_VAILD;

}



/* remove spaces from a string */
void remove_spaces(char *str)	
{
	char tmp[MAX_LINE_LENGTH] = { 0 };
	int i,j;
	for (i=0, j=0; i < MAX_LINE_LENGTH; i++)
	{
		if (str[i] == '\0') /* if we got to the end of the string */
			break;

		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r')
		{
			tmp[j] = str[i];
			j++;
		}
	}
	tmp[j] = '\0';	/* put null in the end of the string */
	strcpy(str, tmp);
}


/* count how many words in a given string */
unsigned int count_words(char *str)
{
	int i, size, count;
	bool char_on = false;
	
	size = strlen(str);
	count = 0;
	
	for(i=0; i < size; i++)
	{
		if(str[i] != ' ' && str[i] != '\t') /* if its a charcter */
		{
			if(!char_on)		/* if it's a new word */
				count++;
			
			char_on = true;
		}
		else		/* if its a space or a tab */
			char_on = false;
	}
	return count;
}


/* Description: Checks if string is an operation name*/
bool is_operation_name(char* str) {
	return (strcmp(str, MOV_OPERATION) == 0) || (strcmp(str, CMP_OPERATION) == 0) ||
			(strcmp(str, ADD_OPERATION) == 0) || (strcmp(str, SUB_OPERATION) == 0) ||
			(strcmp(str, NOT_OPERATION) == 0) || (strcmp(str, CLR_OPERATION) == 0) ||
			(strcmp(str, LEA_OPERATION) == 0) || (strcmp(str, INC_OPERATION) == 0) ||
			(strcmp(str, DEC_OPERATION) == 0) || (strcmp(str, JMP_OPERATION) == 0) ||
			(strcmp(str, BNE_OPERATION) == 0) || (strcmp(str, RED_OPERATION) == 0) ||
			(strcmp(str, PRN_OPERATION) == 0) || (strcmp(str, JSR_OPERATION) == 0) ||
			(strcmp(str, RTS_OPERATION) == 0) || (strcmp(str, STOP_OPERATION) == 0);
}


/*  Description: Checks if the string is a valid label*/
bool is_valid_label(char* str) {
	int i;
	int len = strlen(str);

	if (!isalpha(str[0]) || len > MAX_LINE_LENGTH ) {  /*string is shorter then maximum */
	     return false;                                 /*length string start's with a letter*/
	}

	
	for (i = 1; i < len; i++) {
		if (!isalnum(str[i])) {            
	            return false;           /* We checked the first and last char's, we check the string length,*/
	                                    /* Now we will make sure that the rest are alpha numeric*/	
		}
	}
	return !is_operation_name(str) && !is_register(str);  /* All was fine */
}


int digits_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }

    return 1;
}


bool is_register(char* operand) {
	int digit;
	int length;
	bool flag;
	if(operand == NULL)
	{
		printf("\nERROR: in function 'is_register' given null, not suppost to happen, abort");
		exit(1);
	}
	length = strlen(operand);

	if ((length != 2) || (operand[0] != REGISTER_FIRST_TOKEN) || (!isdigit(operand[1])))
	{
		return false;
	}

	digit = atoi(operand + 1);

    flag = (digit < REGISTERS_COUNT) && (digit >= 0);
	
	return flag;
}



/* For the ecode operations */
/***************************/

/* turn all the cells in the word cell back to 0, stop at null */
void clean_word(char *word)
{
	int i = 0;
	while (word[i] != '\0')
	{
		word[i] = '0';
		i++;
	}
}


/* translate a decimal integer to binary string */
void dec_to_binary(char *word, int num, int word_size)
{
	int i, j;
	int number = num;
	/* put the binary number to a gicen string */
	for (i = word_size - 1, j = 0; i >= 0; i--)
	{
		int mask;
		if (word[j] == '\0')
		{
			printf("\nERROR: in function 'dec_to_binary' write on null");
			break;
		}
		mask = number >> i;
		if (mask & 1)
			word[j] = '1';
		else
			word[j] = '0';
		j++;
	}
}

