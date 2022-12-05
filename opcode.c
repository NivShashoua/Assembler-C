
#include "opcode.h"
#include "Utilities.h" 


/* for the encode */
#define MAX_WORD_SIZE (15)
#define RESERVED_AREA_SIZE (4)
#define OPCODE_AREA_SIZE (4)
#define SRC_ADDRESSING_AREA_SIZE (2)
#define DES_ADDRESSING_AREA_SIZE (2)
#define ARE_AREA_SIZE (2)

/*all addresong mode */
#define IMMEDIATE_ADDRESSING (0)
#define DIRECT_ADDRESSING (1)
#define PERMANENT_INDEX_ADDRESSING (2)
#define DIRECT_REGISTER_ADDRESSING (3)

/*A R E*/
#define ABSOLUTE (0)
#define RELOCATABLE (2)
#define EXTERNAL (1)


/***********************mov 100% complte**********************/
bool opcode_parse(symbol_table_list_t *table_list, opcode_list_t *opcode_list, char *opcode_str ,int line_num){
 	
  	
    char *mov_definiton = strstr(opcode_str, "mov");
    char *cmp_definiton = strstr(opcode_str, "cmp");
    char *add_definiton = strstr(opcode_str, "add");
    char *sub_definiton = strstr(opcode_str, "sub");
    char *not_definiton = strstr(opcode_str, "not");
    char *clr_definiton = strstr(opcode_str, "clr");
    char *lea_definiton = strstr(opcode_str, "lea");
    char *inc_definiton = strstr(opcode_str, "inc");
    char *dec_definiton = strstr(opcode_str, "dec");
    char *jmp_definiton = strstr(opcode_str, "jmp");
    char *bne_definiton = strstr(opcode_str, "bne");
    char *red_definiton = strstr(opcode_str, "red");
    char *prn_definiton = strstr(opcode_str, "prn");
    char *jsr_definiton = strstr(opcode_str, "jsr");
    char *rts_definiton = strstr(opcode_str, "rts");
    char *stop_definiton = strstr(opcode_str,"stop");
    int op1;
    int op2;
    char inside[MAX_LINE_LENGTH];
    char insideOP1[MAX_LINE_LENGTH] ={ 0 };
    char insideOP2[MAX_LINE_LENGTH] ={ 0 };
    char *first_operand = NULL;
    char *definition_comma =NULL;
    char *second_operand = NULL;
    int count_comma = 0;
    int i = 0;
		for(i = 0; i <=strlen(opcode_str); i++) /*clac how mutch comma heva in line */
		{
			if(opcode_str[i] == ',')
				count_comma++;
		}

if(mov_definiton) 
{
		*mov_definiton = '\0' ;
		if(mov_definiton[3] != ' ' && mov_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}

		remove_spaces(opcode_str); /* remove_spaces from the string */
		mov_definiton +=1;

		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used mov definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}
			
		 first_operand = mov_definiton += 2; 		/*To advance to the beginning of the first operand */
		 definition_comma =strstr(mov_definiton, ","); /*Distribution of operands into two*/
		 if(definition_comma == NULL){
		 	fprintf(stderr, "\nERROR: Used mov definition must be with ',' Between two operands ,error in Line:%d", line_num);
			return false;
		 }
		 *definition_comma = '\0' ;  	/*Enter 0 where separating the operands*/
		 second_operand = definition_comma +1;  /*Progress towards the beginning of the second operand*/
		
		if(count_comma > 1 ){ /*check how mutch comma heva in line */
			fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
			return false;
		}
		if(count_words(first_operand) != ONE_OPERAND || count_words(second_operand) != ONE_OPERAND ) 	/* check that oprands have only one word */
		{

			if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
																							   /*^*/
			if(count_words(second_operand) != ONE_OPERAND){ /*As above only on the second operand|*/
				if(strstr(second_operand,"[")==NULL || strstr(second_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(second_operand,"[")!=NULL || strstr(second_operand,"]")!=NULL){
					strcpy(insideOP2 , second_operand);
					inside_in_Array(insideOP2);
				}
				if(count_words(insideOP2) == ONE_OPERAND )
					goto next;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
		}
next:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		remove_spaces(second_operand);     /* the second operand without spaces */
		
		/*check that if have two oprands */
		if (is_register(first_operand) && (is_register(second_operand))) 
		{
			opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 2); /*add valus to opcode list*/
			return true;
		}
		
		/*check that if two oprands is vaild is not number */
		if (string_is_integer(first_operand) || string_is_integer(second_operand))
		{
			fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is not start with number */
		if( isdigit(first_operand[0]) || isdigit(second_operand[0]) ){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)||check_if_macro_inti(table_list,second_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is vaild is not # */
		if(first_operand[0] == '#' && second_operand[0] == '#') /*check that  two oprands start with # */
		{
			first_operand+=1;  
			second_operand+=1;  
			 op1 = string_is_integer(first_operand);  
			 op2 = string_is_integer(second_operand); 
			/*check that if two oprands is #number */
			if (op1 && op2){
				fprintf(stderr, "\nERROR: can't use with two '#' of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			/*check that if first oprand is #number secound oprand is macro*/
			else if(op1 && check_if_macro_inti(table_list,second_operand)){
				fprintf(stderr, "\nERROR: can't use with first '#' and second macro of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			/*check that if first oprand is macro  secound oprand is #number*/
			else if(check_if_macro_inti(table_list,first_operand) && op2){
				fprintf(stderr, "\nERROR: can't use with first macro and second '#' of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			/*check that if first oprand is macro secound oprand is macro*/
			else if(check_if_macro_inti(table_list,first_operand) && check_if_macro_inti(table_list,second_operand)){ /*check that oprands is not macro */
				fprintf(stderr, "\nERROR: can't use with two oprands on macro is wrong ,in Line:%d", line_num);
				return false;
			}
			/*check that if first oprand is not macro or secound oprand is not macro*/
			else if(!check_if_macro_inti(table_list,first_operand) || !check_if_macro_inti(table_list,second_operand)){
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
				else if(!check_if_macro_inti(table_list,second_operand )){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
					return false;

				}
			}
		}
		/*check that if first oprand is #number/macro , secound oprand is register*/
		if(first_operand[0] == '#' && is_register(second_operand))
		{
			char *Pfirst_operand = first_operand;
			first_operand += 1;

			 op1 = string_is_integer(first_operand);
			if(op1  && is_register(second_operand) )
			{
				opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 3);
				return true;
			}
			if(check_if_macro_inti(table_list,first_operand) && is_register(second_operand)){
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 3);
				return true;
			}
			if(!check_if_macro_inti(table_list,first_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}
		}
		
		/*check that if first oprand is register , secound oprand is #number/macro*/
	    if(second_operand[0] == '#' && is_register(first_operand))
		{
			second_operand += 1;
			 op2 = string_is_integer(second_operand);
			if(op2  && is_register(first_operand) )
			{
				fprintf(stderr, "\nERROR: can't use with first register and second '#' of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			if(check_if_macro_inti(table_list,second_operand) && is_register(first_operand))
			{
				fprintf(stderr, "\nERROR: can't use with first register and second macro of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			if(!check_if_macro_inti(table_list,second_operand))
			{
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}

		}
		
		/*check that if first oprand is register , secound oprand is array*/
		if(is_register(first_operand) && isalpha(second_operand[0]))
		{

            if(string_only_vaild_char(second_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){/*check that if first oprand is register , secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
					return true;		
				}
			}

			strcpy(inside , second_operand);
			inside_in_Array(inside);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4); 
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
	        {
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);				
				return false;
            } 
            if(string_is_integer(inside)){
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4); 
               	return true;
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}
		
		}
		/*check that if first oprand is array  , secound oprand is register*/ 
		if(is_register(second_operand) && isalpha(first_operand[0]))
		{
			if(string_only_vaild_char(first_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", first_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,first_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
					return true;		
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);				
				return false;
            } 
            if(string_is_integer(inside))
            {
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
               	return true;
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside))
			{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}		
		}
		/*check that if first oprand is #number/macro  , secound oprand is array */
		if(first_operand[0] == '#' && isalpha(second_operand[0]))
		{
			char *Pfirst_operand = first_operand;
			if(string_only_vaild_char(second_operand) != VAILD ){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			first_operand+=1;
			/*check that if secound oprand is NOT_ONLY_LETTERS_AND_NUMBERS*/
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){/*check that if secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					op1 = string_is_integer(first_operand);
					if(op1)
					{
						opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 3);
						return true;
					}
					if(check_if_macro_inti(table_list,first_operand)){
						opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 3);
						return true;
					}
					if(!check_if_macro_inti(table_list,first_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
						return false;
					}		
				}
			}
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			 op1 = string_is_integer(first_operand);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,first_operand))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macros:#%s , %s not vaild , Error in Line:%d",inside, first_operand,line_num);
				return false;
            }
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(first_operand) )
            {
            	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
               	return true;
            }
            if ( check_if_macro_inti(table_list,first_operand) && string_is_integer(inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
                return true;
            }
            if(string_is_integer(inside) && op1){
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 4);
               	return true;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,first_operand))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", first_operand,line_num);
				return false;
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}

		}
		/*check that if first oprand is array  , secound oprand is #number/macro */
		if(second_operand[0] == '#' && isalpha(first_operand[0])){
			if(string_only_vaild_char(first_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			second_operand+=1;
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num))
				{
					fprintf(stderr, "\nERROR: can't use with first label OR VAR and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
				}
			}

		
			
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,second_operand))
			{
                fprintf(stderr, "\nERROR:  can't use with this #%s ,While the  destination oprand is #%s, error in Line:%d",inside,second_operand, line_num);
				return false;
            }else{
            	fprintf(stderr, "\nERROR:  this #%s can't be in destination oprand , error in Line:%d",second_operand, line_num);
                return  false;         
            } 
     

		}

        /*check that if first oprand is array/label  , secound oprand is array/label */ 
		if(isalpha(first_operand[0]) && isalpha(second_operand[0])){
			
			/*check that if label is vaild in first oprand and secound */ 
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num) && check_if_its_label(table_list,second_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
					return true;
				}
			}

			/*check that if first oprand is array  , secound oprand is label */
			if(string_only_vaild_char(first_operand) == VAILD && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,second_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){
                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", insideOP1,line_num);
					return false;
				}
				else if (insideOP1 !=NULL)
				{
                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
			/*check that if first oprand is label  , secound oprand  is array */
			if(string_only_vaild_char(second_operand) == VAILD && string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,first_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if(isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", insideOP2,line_num);
					return false;
				}
				else if (string_is_integer(insideOP2))
				{
                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error, error in Line:%d",insideOP2, line_num);
					return false;	
				}
			}
			/*here is problem*/
			if(string_only_vaild_char(first_operand) == VAILD && string_only_vaild_char(second_operand) == VAILD){	
				
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if (isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2))
				{
                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
                	return true;
           		} 
				if (isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1) )
				{
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", insideOP1,line_num);	
					return false;
           		}
            	if (isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2) )
            	{
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", insideOP2,line_num);
					return false;
    	        }
            	if(string_is_integer(insideOP1) && string_is_integer(insideOP2)){

               		opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
               		return true;					
				} 
				if(string_is_integer(insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){
        	  	    opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
    	           	return true;
				} 
				if(string_is_integer(insideOP2) && isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){
    		        opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_MOV, 0 ,line_num, 5);
               		return true;	
				} 
				if(isdigit(insideOP1[0]) && !string_is_integer(insideOP1)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP1 , line_num);
					return false;
				}
				if(isdigit(insideOP2[0]) && !string_is_integer(insideOP2)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP2 , line_num);
					return false;
				}	
			}
		} 
 }

 if(cmp_definiton)
 {	
		*cmp_definiton = '\0' ;
		if(cmp_definiton[3] != ' ' && cmp_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		cmp_definiton +=1;
		
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used cmp definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		 first_operand = cmp_definiton += 2; 		/*To advance to the beginning of the first operand */
		 definition_comma =strstr(cmp_definiton, ","); /*Distribution of operands into two*/
		 if(definition_comma == NULL){
		 	fprintf(stderr, "\nERROR: Used cmp definition must be with ',' Between two operands ,error in Line:%d", line_num);
			return false;
		 }
		 *definition_comma = '\0' ;  	/*Enter 0 where separating the operands*/
		 second_operand = definition_comma +1;  /*Progress towards the beginning of the second operand*/
		if(count_comma > 1 ){ /*check how mutch comma heva in line */
			fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
			return false;
		}
		if(count_words(first_operand) != ONE_OPERAND || count_words(second_operand) != ONE_OPERAND ) 	/* check that oprands have only one word */
		{

			if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next1;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
																							   /*^*/
			if(count_words(second_operand) != ONE_OPERAND){ /*As above only on the second operand|*/
				if(strstr(second_operand,"[")==NULL || strstr(second_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(second_operand,"[")!=NULL || strstr(second_operand,"]")!=NULL){
					strcpy(insideOP2 , second_operand);
					inside_in_Array(insideOP2);
				}
				if(count_words(insideOP2) == ONE_OPERAND )
					goto next1;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
		}
next1:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		remove_spaces(second_operand);     /* the second operand without spaces */
		
		/*check that if have two oprands */
		if (is_register(first_operand) && (is_register(second_operand))) 
		{
			opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 2); /*add valus to opcode list*/
			return true;
		}
		
		/*check that if two oprands is vaild is not number */
		if (string_is_integer(first_operand) || string_is_integer(second_operand))
		{
			fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is not start with number */
		if( isdigit(first_operand[0]) || isdigit(second_operand[0]) ){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)||check_if_macro_inti(table_list,second_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is vaild is not # */
		if(first_operand[0] == '#' && second_operand[0] == '#') /*check that  two oprands start with # */
		{
			char *Pfirst_operand = first_operand;
			char *Psecond_operand = second_operand;
			first_operand+=1;  
			second_operand+=1;  
			 op1 = string_is_integer(first_operand);  
			 op2 = string_is_integer(second_operand); 
			/*check that if two oprands is #number */
			if (op1 && op2){
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			/*check that if first oprand is #number secound oprand is macro*/
			else if(op1 && check_if_macro_inti(table_list,second_operand)){
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			/*check that if first oprand is macro  secound oprand is #number*/
			else if(check_if_macro_inti(table_list,first_operand) && op2){
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			/*check that if first oprand is macro secound oprand is macro*/
			else if(check_if_macro_inti(table_list,first_operand) && check_if_macro_inti(table_list,second_operand)){ /*check that oprands is not macro */
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			/*check that if first oprand is not macro or secound oprand is not macro*/
			else if(!check_if_macro_inti(table_list,first_operand) || !check_if_macro_inti(table_list,second_operand)){
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
				else if(!check_if_macro_inti(table_list,second_operand )){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
					return false;

				}
			}
		}
		/*check that if first oprand is #number/macro , secound oprand is register*/
		if(first_operand[0] == '#' && is_register(second_operand)){
			char *Pfirst_operand = first_operand;
			first_operand += 1;

			 op1 = string_is_integer(first_operand);
			if(op1  && is_register(second_operand) )
			{
				opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
				return true;
			}
			if(check_if_macro_inti(table_list,first_operand) && is_register(second_operand))
			{
				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
				return true;
			}
			if(!check_if_macro_inti(table_list,first_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}
			

		}
		
		/*check that if first oprand is register , secound oprand is #number/macro*/
	    if(second_operand[0] == '#' && is_register(first_operand))
		{
			char *Psecond_operand = second_operand;
			second_operand += 1;
			 op2 = string_is_integer(second_operand);
			if(op2  && is_register(first_operand) )
			{
				opcode_list_add(opcode_list , ABSOLUTE , first_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			if(check_if_macro_inti(table_list,second_operand) && is_register(first_operand)){

				opcode_list_add(opcode_list , ABSOLUTE , first_operand , Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3); /*add valus to opcode list*/
				return true;
			}
			if(!check_if_macro_inti(table_list,second_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}

		}
		
		/*check that if first oprand is register , secound oprand is array*/
		if(is_register(first_operand) && isalpha(second_operand[0]))
		{

            if(string_only_vaild_char(second_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if first oprand is register , secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
					return true;		
				}
			}
					
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4); 
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);	
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4); 
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}
		
		}
		/*check that if first oprand is array  , secound oprand is register*/ 
		if(is_register(second_operand) && isalpha(first_operand[0]))
		{
			if(string_only_vaild_char(first_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", first_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,first_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
					return true;		
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);
				return false;
            } 
            if(string_is_integer(inside)){
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}		
		}
		/*check that if first oprand is #number/macro  , secound oprand is array */
		if(first_operand[0] == '#' && isalpha(second_operand[0])){
			char *Pfirst_operand = first_operand;
			if(string_only_vaild_char(second_operand) != VAILD ){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			first_operand+=1;
			/*check that if secound oprand is NOT_ONLY_LETTERS_AND_NUMBERS*/
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					op1 = string_is_integer(first_operand);
					if(op1)
					{
						opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
						return true;
					}
					if(check_if_macro_inti(table_list,first_operand)){
						opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
						return true;
					}
					if(!check_if_macro_inti(table_list,first_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
						return false;
					}		
				}
			}
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			 op1 = string_is_integer(first_operand);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,first_operand))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);
				return false;
            } 
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(first_operand) )
            {
            	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
            }
            if ( check_if_macro_inti(table_list,first_operand) && string_is_integer(inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
                return true;
            }
            if(string_is_integer(inside) && op1){
                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,first_operand)){
            	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}

		}
		/*check that if first oprand is array  , secound oprand is #number/macro */
		if(second_operand[0] == '#' && isalpha(first_operand[0])){
			char *Psecond_operand = second_operand;
			if(string_only_vaild_char(first_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			second_operand+=1;
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num))
				{
					op2 = string_is_integer(second_operand);
					if(op2)
					{
						opcode_list_add(opcode_list , ABSOLUTE ,first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
						return true;
					}
					if(check_if_macro_inti(table_list,second_operand)){
						opcode_list_add(opcode_list , ABSOLUTE , first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 3);
						return true;
					}
					if(!check_if_macro_inti(table_list,second_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
						return false;
					}
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			op2 = string_is_integer(second_operand);
			
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,second_operand))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
                return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", inside,line_num);
				return false;
            } 
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(second_operand) )
            {
            	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
            }
            if ( check_if_macro_inti(table_list,second_operand) && string_is_integer(inside))
			{
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
                return true;
            }
            if(string_is_integer(inside) && op2)
            {
                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, Psecond_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 4);
               	return true;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,second_operand))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", second_operand,line_num);
               	return false;		
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
				return false; 
			}
		}
        /*check that if first oprand is array/label  , secound oprand is array/label */ 
		if(isalpha(first_operand[0]) && isalpha(second_operand[0])){
			
			/*check that if label is vaild in first oprand and secound */ 
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num) && check_if_its_label(table_list,second_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
					return true;
				}
			}

			/*check that if first oprand is array  , secound oprand is label */
			if(string_only_vaild_char(first_operand) == VAILD && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,second_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (insideOP1 !=NULL)
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
			/*check that if first oprand is label  , secound oprand  is array */
			if(string_only_vaild_char(second_operand) == VAILD && string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,first_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if(isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
				}
				else if (string_is_integer(insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error, error in Line:%d",insideOP2, line_num);
					return false;	
				}
			}
			/*here is problem*/
			if(string_only_vaild_char(first_operand) == VAILD && string_only_vaild_char(second_operand) == VAILD){	
				
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);

				if (isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
                	return true;
           		} 
					if (isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
           		}
            	if (isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
    	        }
            	if(string_is_integer(insideOP1) && string_is_integer(insideOP2)){

               		opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
               		return true;
					
				} 
				if(string_is_integer(insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

        	  	    opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
    	           	return true;
					
				} 
				if(string_is_integer(insideOP2) && isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

    		        opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_CMP, 0 ,line_num, 5);
               		return true;
					
				} 
				if(isdigit(insideOP1[0]) && !string_is_integer(insideOP1)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP1 , line_num);
					return false;
				}
				if(isdigit(insideOP2[0]) && !string_is_integer(insideOP2)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP2 , line_num);
					return false;
				}
			
			}

		} 
 }
 if(add_definiton)
 {
		*add_definiton = '\0' ;
		if(add_definiton[3] != ' ' && add_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		add_definiton +=1;
		
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used add definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		 first_operand = add_definiton += 2; 		/*To advance to the beginning of the first operand */
		 definition_comma =strstr(add_definiton, ","); /*Distribution of operands into two*/
		 
		 if(definition_comma == NULL){
		 	fprintf(stderr, "\nERROR: Used add definition must be with ',' Between two operands ,error in Line:%d", line_num);
			return false;
		 }
		 *definition_comma = '\0' ;  	/*Enter 0 where separating the operands*/
		 second_operand = definition_comma +1;  /*Progress towards the beginning of the second operand*/
		if(count_comma > 1 ){ /*check how mutch comma heva in line */
			fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
			return false;
		}
		if(count_words(first_operand) != ONE_OPERAND || count_words(second_operand) != ONE_OPERAND ) 	/* check that oprands have only one word */
		{

			if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next2;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
																							   /*^*/
			if(count_words(second_operand) != ONE_OPERAND){ /*As above only on the second operand|*/
				if(strstr(second_operand,"[")==NULL || strstr(second_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(second_operand,"[")!=NULL || strstr(second_operand,"]")!=NULL){
					strcpy(insideOP2 , second_operand);
					inside_in_Array(insideOP2);
				}
				if(count_words(insideOP2) == ONE_OPERAND )
					goto next2;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
		}
next2:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		remove_spaces(second_operand);     /* the second operand without spaces */
		
		/*check that if have two oprands */
		if (is_register(first_operand) && (is_register(second_operand))) 
		{

				opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if two oprands is vaild is not number */
		if (string_is_integer(first_operand) || string_is_integer(second_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		/*check that if two oprands is not start with number */
		if( isdigit(first_operand[0]) || isdigit(second_operand[0]) ){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)||check_if_macro_inti(table_list,second_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is vaild is not # */
		if(first_operand[0] == '#' && second_operand[0] == '#') /*check that  two oprands start with # */
		{
			first_operand+=1;  
			second_operand+=1;  
			 op1 = string_is_integer(first_operand);  
			 op2 = string_is_integer(second_operand); 
			/*check that if two oprands is #number */
			if (op1 && op2){
				fprintf(stderr, "\nERROR: can't use with two '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is #number secound oprand is macro*/
			else if(op1 && check_if_macro_inti(table_list,second_operand)){
					fprintf(stderr, "\nERROR: can't use with first '#' and second macro of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro  secound oprand is #number*/
			else if(check_if_macro_inti(table_list,first_operand) && op2){
					fprintf(stderr, "\nERROR: can't use with first macro and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro secound oprand is macro*/
			else if(check_if_macro_inti(table_list,first_operand) && check_if_macro_inti(table_list,second_operand)){ /*check that oprands is not macro */
				fprintf(stderr, "\nERROR: can't use with two oprands on macro is wrong ,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is not macro or secound oprand is not macro*/
			else if(!check_if_macro_inti(table_list,first_operand) || !check_if_macro_inti(table_list,second_operand)){
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
				else if(!check_if_macro_inti(table_list,second_operand )){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
					return false;

				}
			}
		}
		/*check that if first oprand is #number/macro , secound oprand is register*/
		if(first_operand[0] == '#' && is_register(second_operand)){
			char *Pfirst_operand = first_operand;
			first_operand += 1;

			 op1 = string_is_integer(first_operand);
			if(op1)
			{

				opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 3);
				return true;
			}
			if(check_if_macro_inti(table_list,first_operand) && is_register(second_operand)){

				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 3);
				return true;
			}
			if(!check_if_macro_inti(table_list,first_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}
			

		}
		
		/*check that if first oprand is register , secound oprand is #number/macro*/
	    if(second_operand[0] == '#' && is_register(first_operand))
		{
			second_operand += 1;
			 op2 = string_is_integer(second_operand);
			if(op2  && is_register(first_operand) )
			{

				fprintf(stderr, "\nERROR: can't use with first register and second '#' of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			if(check_if_macro_inti(table_list,second_operand) && is_register(first_operand)){

				fprintf(stderr, "\nERROR: can't use with first register and second macro of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			if(!check_if_macro_inti(table_list,second_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}

		}
		
		/*check that if first oprand is register , secound oprand is array*/
		if(is_register(first_operand) && isalpha(second_operand[0]))
		{

            if(string_only_vaild_char(second_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if first oprand is register , secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
					return true;		
				}
			}
					
			strcpy(inside , second_operand);
			inside_in_Array(inside);		
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4); 
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d" ,inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4); 
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}
		
		}
		/*check that if first oprand is array  , secound oprand is register*/ 
		if(is_register(second_operand) && isalpha(first_operand[0]))
		{
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", first_operand, line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
					return true;		
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}		
		}
		/*check that if first oprand is #number/macro  , secound oprand is array */
		if(first_operand[0] == '#' && isalpha(second_operand[0])){
			char *Pfirst_operand = first_operand;
			if(string_only_vaild_char(second_operand) != VAILD ){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			first_operand+=1;
			/*check that if secound oprand is NOT_ONLY_LETTERS_AND_NUMBERS*/
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					op1 = string_is_integer(first_operand);
					if(op1)
					{

						opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 3);
						return true;
					}
					if(check_if_macro_inti(table_list,first_operand)){

						opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 3);
						return true;
					}
					if(!check_if_macro_inti(table_list,first_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
						return false;
					}		
				}
			}
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			 op1 = string_is_integer(first_operand);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,first_operand))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macros:#%s , %s not vaild , Error in Line:%d",inside, first_operand,line_num);
				return false;
            }
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(first_operand) )
            {

            	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
               	return true;
            }
            if ( check_if_macro_inti(table_list,first_operand) && string_is_integer(inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
                	return true;
            }
            if(string_is_integer(inside) && op1){

                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 4);
               	return true;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,first_operand)){

            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", first_operand,line_num);
				return false;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}

		}
		/*check that if first oprand is array  , secound oprand is #number/macro */
		if(second_operand[0] == '#' && isalpha(first_operand[0])){
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			second_operand+=1;
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					fprintf(stderr, "\nERROR: can't use with first label OR VAR and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
				}
			}

		
			
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,second_operand))
			{

                	fprintf(stderr, "\nERROR:  can't use with this #%s ,While the  destination oprand is #%s, error in Line:%d",inside,second_operand, line_num);
					return false;
            }else{
            	fprintf(stderr, "\nERROR:  this #%s can't be in destination oprand , error in Line:%d",second_operand, line_num);
                	return  false;         
            } 
     

		}

        /*check that if first oprand is array/label  , secound oprand is array/label */ 
		if(isalpha(first_operand[0]) && isalpha(second_operand[0])){
			
			/*check that if label is vaild in first oprand and secound */ 
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num) && check_if_its_label(table_list,second_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
					return true;
				}
			}

			/*check that if first oprand is array  , secound oprand is label */
			if(string_only_vaild_char(first_operand) == VAILD && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,second_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (insideOP1 !=NULL)
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
			/*check that if first oprand is label  , secound oprand  is array */
			if(string_only_vaild_char(second_operand) == VAILD && string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,first_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if(isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
				}
				else if (string_is_integer(insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error, error in Line:%d",insideOP2, line_num);
					return false;	
				}
			}
			/*here is problem*/
			if(string_only_vaild_char(first_operand) == VAILD && string_only_vaild_char(second_operand) == VAILD){	
				
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if (isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
                	return true;
           		} 
					if (isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
           		}
            	if (isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
    	        }
            	if(string_is_integer(insideOP1) && string_is_integer(insideOP2)){

               		opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
               		return true;
					
				} 
				if(string_is_integer(insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

        	  	    opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
    	           	return true;
					
				} 
				if(string_is_integer(insideOP2) && isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

    		        opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_ADD, 0 ,line_num, 5);
               		return true;
					
				} 
				if(isdigit(insideOP1[0]) && !string_is_integer(insideOP1)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP1 , line_num);
					return false;
				}
				if(isdigit(insideOP2[0]) && !string_is_integer(insideOP2)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP2 , line_num);
					return false;
				}
			
			}

		} 	

 }
 if(sub_definiton)
 {

		*sub_definiton = '\0' ;
		if(sub_definiton[3] != ' ' && sub_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		sub_definiton +=1;
		
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used sub definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		 first_operand = sub_definiton += 2; 		/*To advance to the beginning of the first operand */
		 definition_comma =strstr(sub_definiton, ","); /*Distribution of operands into two*/
		 if(definition_comma == NULL){
		 	fprintf(stderr, "\nERROR: Used sub definition must be with ',' Between two operands ,error in Line:%d", line_num);
			return false;
		 }
		 *definition_comma = '\0' ;  	/*Enter 0 where separating the operands*/
		 second_operand = definition_comma +1;  /*Progress towards the beginning of the second operand*/
		if(count_comma > 1 ){ /*check how mutch comma heva in line */
			fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
			return false;
		}
		if(count_words(first_operand) != ONE_OPERAND || count_words(second_operand) != ONE_OPERAND ) 	/* check that oprands have only one word */
		{

			if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next3;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
																							   /*^*/
			if(count_words(second_operand) != ONE_OPERAND){ /*As above only on the second operand|*/
				if(strstr(second_operand,"[")==NULL || strstr(second_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(second_operand,"[")!=NULL || strstr(second_operand,"]")!=NULL){
					strcpy(insideOP2 , second_operand);
					inside_in_Array(insideOP2);
				}
				if(count_words(insideOP2) == ONE_OPERAND )
					goto next3;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
		}
next3:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		remove_spaces(second_operand);     /* the second operand without spaces */
		
		/*check that if have two oprands */
		if (is_register(first_operand) && (is_register(second_operand))) 
		{

				opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if two oprands is vaild is not number */
		if (string_is_integer(first_operand) || string_is_integer(second_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		/*check that if two oprands is not start with number */
		if( isdigit(first_operand[0]) || isdigit(second_operand[0]) ){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)||check_if_macro_inti(table_list,second_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is vaild is not # */
		if(first_operand[0] == '#' && second_operand[0] == '#') /*check that  two oprands start with # */
		{
			first_operand+=1;  
			second_operand+=1;  
			 op1 = string_is_integer(first_operand);  
			 op2 = string_is_integer(second_operand); 
			/*check that if two oprands is #number */
			if (op1 && op2){
				fprintf(stderr, "\nERROR: can't use with two '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is #number secound oprand is macro*/
			else if(op1 && check_if_macro_inti(table_list,second_operand)){
					fprintf(stderr, "\nERROR: can't use with first '#' and second macro of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro  secound oprand is #number*/
			else if(check_if_macro_inti(table_list,first_operand) && op2){
					fprintf(stderr, "\nERROR: can't use with first macro and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro secound oprand is macro*/
			else if(check_if_macro_inti(table_list,first_operand) && check_if_macro_inti(table_list,second_operand)){ /*check that oprands is not macro */
				fprintf(stderr, "\nERROR: can't use with two oprands on macro is wrong ,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is not macro or secound oprand is not macro*/
			else if(!check_if_macro_inti(table_list,first_operand) || !check_if_macro_inti(table_list,second_operand)){
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
				else if(!check_if_macro_inti(table_list,second_operand )){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
					return false;

				}
			}
		}
		/*check that if first oprand is #number/macro , secound oprand is register*/
		if(first_operand[0] == '#' && is_register(second_operand)){
			char *Pfirst_operand = first_operand;
			first_operand += 1;

			 op1 = string_is_integer(first_operand);
			if(op1  && is_register(second_operand) )
			{

				opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 3);
				return true;
			}
			if(check_if_macro_inti(table_list,first_operand) && is_register(second_operand)){

				opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 3);
				return true;
			}
			if(!check_if_macro_inti(table_list,first_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}
			

		}
		
		/*check that if first oprand is register , secound oprand is #number/macro*/
	    if(second_operand[0] == '#' && is_register(first_operand))
		{
			second_operand += 1;
			 op2 = string_is_integer(second_operand);
			if(op2  && is_register(first_operand) )
			{

				fprintf(stderr, "\nERROR: can't use with first register and second '#' of oprands is wrong,in Line:%d", line_num);
				return false;
			}
			if(check_if_macro_inti(table_list,second_operand) && is_register(first_operand)){

				fprintf(stderr, "\nERROR: can't use with first register and second macro of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			if(!check_if_macro_inti(table_list,second_operand)){
				fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
				return false;
			}

		}
		
		/*check that if first oprand is register , secound oprand is array*/
		if(is_register(first_operand) && isalpha(second_operand[0]))
		{

            if(string_only_vaild_char(second_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if first oprand is register , secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
					return true;		
				}
			}
					
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4); 
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d" ,inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4); 
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}
		
		}
		/*check that if first oprand is array  , secound oprand is register*/ 
		if(is_register(second_operand) && isalpha(first_operand[0]))
		{
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", first_operand, line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
					return true;		
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}		
		}
		/*check that if first oprand is #number/macro  , secound oprand is array */
		if(first_operand[0] == '#' && isalpha(second_operand[0])){
			char *Pfirst_operand = first_operand;
			if(string_only_vaild_char(second_operand) != VAILD ){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			first_operand+=1;
			/*check that if secound oprand is NOT_ONLY_LETTERS_AND_NUMBERS*/
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					op1 = string_is_integer(first_operand);
					if(op1)
					{

						opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 3);
						return true;
					}
					if(check_if_macro_inti(table_list,first_operand)){

						opcode_list_add(opcode_list , ABSOLUTE , Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 3);
						return true;
					}
					if(!check_if_macro_inti(table_list,first_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
						return false;
					}		
				}
			}
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			 op1 = string_is_integer(first_operand);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,first_operand))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macros:#%s , %s not vaild , Error in Line:%d",inside, first_operand,line_num);
				return false;
            }
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(first_operand) )
            {

            	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
               	return true;
            }
            if ( check_if_macro_inti(table_list,first_operand) && string_is_integer(inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
                	return true;
            }
            if(string_is_integer(inside) && op1){

                opcode_list_add(opcode_list , ABSOLUTE ,Pfirst_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 4);
               	return true;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,first_operand)){

            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", first_operand,line_num);
				return false;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}

		}
		/*check that if first oprand is array  , secound oprand is #number/macro */
		if(second_operand[0] == '#' && isalpha(first_operand[0])){
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			second_operand+=1;
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					fprintf(stderr, "\nERROR: can't use with first label OR VAR and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
				}
			}

		
			
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,second_operand))
			{

                	fprintf(stderr, "\nERROR:  can't use with this #%s ,While the  destination oprand is #%s, error in Line:%d",inside,second_operand, line_num);
					return false;
            }else{
            	fprintf(stderr, "\nERROR:  this #%s can't be in destination oprand , error in Line:%d",second_operand, line_num);
                	return  false;         
            } 
     

		}

        /*check that if first oprand is array/label  , secound oprand is array/label */ 
		if(isalpha(first_operand[0]) && isalpha(second_operand[0])){
			
			/*check that if label is vaild in first oprand and secound */ 
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num) && check_if_its_label(table_list,second_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
					return true;
				}
			}

			/*check that if first oprand is array  , secound oprand is label */
			if(string_only_vaild_char(first_operand) == VAILD && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,second_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (insideOP1 !=NULL)
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
			/*check that if first oprand is label  , secound oprand  is array */
			if(string_only_vaild_char(second_operand) == VAILD && string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,first_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if(isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
				}
				else if (string_is_integer(insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error, error in Line:%d",insideOP2, line_num);
					return false;	
				}
			}
			/*here is problem*/
			if(string_only_vaild_char(first_operand) == VAILD && string_only_vaild_char(second_operand) == VAILD){	
				
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if (isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
                	return true;
           		} 
					if (isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
           		}
            	if (isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
    	        }
            	if(string_is_integer(insideOP1) && string_is_integer(insideOP2)){

               		opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
               		return true;
					
				} 
				if(string_is_integer(insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

        	  	    opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
    	           	return true;
					
				} 
				if(string_is_integer(insideOP2) && isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

    		        opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_SUB, 0 ,line_num, 5);
               		return true;
					
				} 
				if(isdigit(insideOP1[0]) && !string_is_integer(insideOP1)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP1 , line_num);
					return false;
				}
				if(isdigit(insideOP2[0]) && !string_is_integer(insideOP2)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP2 , line_num);
					return false;
				}
			
			}

		} 	

 }
 if(not_definiton)
 {
		
		if(not_definiton[3] != ' ' && not_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}	
		remove_spaces(opcode_str); /* remove_spaces from the string */
		not_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used not definition not at the strat of the line,in Line:%d ", line_num);
			return false;
		}	
		first_operand = not_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next4;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next4:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_NOT, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{

				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_NOT, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_NOT, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_NOT, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}
 }
 if(clr_definiton)
 {	
		*clr_definiton = '\0' ;	
		if(clr_definiton[3] != ' ' && clr_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		clr_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used clr definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = clr_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next5;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next5:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_CLR, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in Line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_CLR, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_CLR, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_CLR, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}
 }
if(lea_definiton)
 {		
		*lea_definiton = '\0' ;
		if(lea_definiton[3] != ' ' && lea_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		lea_definiton +=1;
		
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used lea definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		 first_operand = lea_definiton += 2; 		/*To advance to the beginning of the first operand */
		 definition_comma =strstr(lea_definiton, ","); /*Distribution of operands into two*/
		 if(definition_comma == NULL){
		 	fprintf(stderr, "\nERROR: Used lea definition must be with ',' Between two operands ,error in Line:%d", line_num);
			return false;
		 }
		 *definition_comma = '\0' ;  	/*Enter 0 where separating the operands*/
		 second_operand = definition_comma +1;  /*Progress towards the beginning of the second operand*/
		if(count_comma > 1 ){ /*check how mutch comma heva in line */
			fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
			return false;
		}
		if(count_words(first_operand) != ONE_OPERAND || count_words(second_operand) != ONE_OPERAND ) 	/* check that oprands have only one word */
		{

			if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next6;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
																							   /*^*/
			if(count_words(second_operand) != ONE_OPERAND){ /*As above only on the second operand|*/
				if(strstr(second_operand,"[")==NULL || strstr(second_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(second_operand,"[")!=NULL || strstr(second_operand,"]")!=NULL){
					strcpy(insideOP2 , second_operand);
					inside_in_Array(insideOP2);
				}
				if(count_words(insideOP2) == ONE_OPERAND )
					goto next6;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
		}
next6:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		remove_spaces(second_operand);     /* the second operand without spaces */
		
		/*check that if have two oprands */
		if (is_register(first_operand) && (is_register(second_operand))) 
		{
			    fprintf(stderr, "2 oprands");
				fprintf(stderr, "\nERROR: can't use with two registers is, wrong in Line:%d", line_num); /*print error*/
				return false;
		}
		
		/*check that if two oprands is vaild is not number */
		if (string_is_integer(first_operand) || string_is_integer(second_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		/*check that if two oprands is not start with number */
		if( isdigit(first_operand[0]) || isdigit(second_operand[0]) ){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)||check_if_macro_inti(table_list,second_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if two oprands is vaild is not # */
		if(first_operand[0] == '#' && second_operand[0] == '#') /*check that  two oprands start with # */
		{
			first_operand+=1;  
			second_operand+=1;  
			 op1 = string_is_integer(first_operand);  
			 op2 = string_is_integer(second_operand); 
			/*check that if two oprands is #number */
			if (op1 && op2){
				fprintf(stderr, "\nERROR: can't use with two '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is #number secound oprand is macro*/
			else if(op1 && check_if_macro_inti(table_list,second_operand)){
					fprintf(stderr, "\nERROR: can't use with first '#' and second macro of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro  secound oprand is #number*/
			else if(check_if_macro_inti(table_list,first_operand) && op2){
					fprintf(stderr, "\nERROR: can't use with first macro and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is macro secound oprand is macro*/
			else if(check_if_macro_inti(table_list,first_operand) && check_if_macro_inti(table_list,second_operand)){ /*check that oprands is not macro */
				fprintf(stderr, "\nERROR: can't use with two oprands on macro is wrong ,in Line:%d", line_num);
					return false;
			}
			/*check that if first oprand is not macro or secound oprand is not macro*/
			else if(!check_if_macro_inti(table_list,first_operand) || !check_if_macro_inti(table_list,second_operand)){
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
				else if(!check_if_macro_inti(table_list,second_operand )){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",second_operand , line_num);
					return false;

				}
			}
		}
		/*check that if first oprand is #number/macro , secound oprand is register*/
		if(first_operand[0] == '#' && is_register(second_operand)){
			first_operand += 1;

			 op1 = string_is_integer(first_operand);
			if(op1  && is_register(second_operand) )
			{

				fprintf(stderr, "\nERROR:  use with  number and register , #%s , %s ,is wrong ,in Line:%d",first_operand ,second_operand, line_num);
				return false;
			}
			if(check_if_macro_inti(table_list,first_operand) && is_register(second_operand)){

				fprintf(stderr, "\nERROR:  use with  macro and register , #%s , %s ,is wrong ,in Line:%d",first_operand ,second_operand, line_num);
				return false;
			}
			if(!check_if_macro_inti(table_list,first_operand)){
				fprintf(stderr, "\nERROR: can't use with wrong macro , #%s ,is error in Line:%d",first_operand , line_num);
				return false;
			}
			

		}
		
		/*check that if first oprand is register , secound oprand is #number/macro*/
	    if(second_operand[0] == '#' && is_register(first_operand))
		{
			second_operand += 1;
			 op2 = string_is_integer(second_operand);
			if(op2  && is_register(first_operand) )
			{

				fprintf(stderr, "\nERROR:  use with  register and num , #%s , %s ,is wrong ,in Line:%d",second_operand ,first_operand, line_num);
				return false;
			}
			if(check_if_macro_inti(table_list,second_operand) && is_register(first_operand)){

				fprintf(stderr, "\nERROR:  use with  register and macro , #%s , %s ,is wrong ,in Line:%d",second_operand ,first_operand, line_num);
				return false;
			}
			if(!check_if_macro_inti(table_list,second_operand)){
				fprintf(stderr, "\nERROR: can't use with wrong macro , #%s ,is error in Line:%d",second_operand , line_num);
				return false;
			}

		}
		
		/*check that if first oprand is register , secound oprand is array*/
		if(is_register(first_operand) && isalpha(second_operand[0]))
		{

            if(string_only_vaild_char(second_operand) != VAILD){
				fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
				return false;
			}
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if first oprand is register , secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{

					fprintf(stderr, "\nERROR:  use with  register and label or var and num , #%s , %s ,is wrong ,in Line:%d",second_operand ,first_operand, line_num);
					return false;		
				}
			}
					
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	fprintf(stderr, "\nERROR:  use with  register and macro , #%s , %s ,is wrong ,in Line:%d",second_operand ,first_operand, line_num);
					return false;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d" ,inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

               	fprintf(stderr, "\nERROR:  use with  register and num , #%s , %s ,is wrong ,in Line:%d",second_operand ,first_operand, line_num);
				return false;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}
		
		}
		/*check that if first oprand is array  , secound oprand is register*/ 
		if(is_register(second_operand) && isalpha(first_operand[0]))
		{
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", first_operand, line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 4);
					return true;		
				}
			}
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside))
			{

                	opcode_list_add(opcode_list , ABSOLUTE , first_operand , second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 4);
                	return true;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",inside, line_num);
				return false;
            } 
            if(string_is_integer(inside)){

                opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 4);
               	return true;
					
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}		
		}
		/*check that if first oprand is #number/macro  , secound oprand is array */
		if(first_operand[0] == '#' && isalpha(second_operand[0])){
			if(string_only_vaild_char(second_operand) != VAILD ){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			first_operand+=1;
			/*check that if secound oprand is NOT_ONLY_LETTERS_AND_NUMBERS*/
			if(string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				/*check that if secound oprand is label */
				if(check_if_its_label(table_list,second_operand,line_num))
				{
					op1 = string_is_integer(first_operand);
					if(op1)
					{
						fprintf(stderr, "\nERROR:  use with  number  and label or VAR , #%s , %s ,is wrong ,in Line:%d",first_operand,second_operand , line_num);
						return false;
					}
					if(check_if_macro_inti(table_list,first_operand)){

						fprintf(stderr, "\nERROR:  use with  macro and label or VAR or VAR , #%s , %s ,is wrong ,in Line:%d",first_operand,second_operand , line_num);
						return false;
					}
					if(!check_if_macro_inti(table_list,first_operand)){
						fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
						return false;
					}		
				}
			}
			strcpy(inside , second_operand);
			inside_in_Array(inside);
			 op1 = string_is_integer(first_operand);

			if (isalpha(inside[0]) && check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,first_operand))
			{

                	fprintf(stderr, "\nERROR: Use with macro and array[macro] , #%s , %s ,is wrong  in Line:%d",inside, first_operand,line_num);
				return false;
            } 
            if (isalpha(inside[0]) && !check_if_macro_inti(table_list,first_operand) && !check_if_macro_inti(table_list,inside))
            {
            	fprintf(stderr, "\nERROR: This macros:#%s , %s not vaild , Error in Line:%d",inside, first_operand,line_num);
				return false;
            }
            if (isalpha(inside[0]) &&  check_if_macro_inti(table_list,inside) && string_is_integer(first_operand) )
            {

            	fprintf(stderr, "\nERROR:  use with number and array[macro] , #%s , %s ,is wrong ,in Line:%d" ,first_operand,second_operand , line_num);
				return false;
            }
            if ( check_if_macro_inti(table_list,first_operand) && string_is_integer(inside))
			{

                	fprintf(stderr, "\nERROR:  use with macro and array[number] , #%s , %s ,is wrong ,in Line:%d" ,first_operand,second_operand , line_num);
				return false;
            }
            if(string_is_integer(inside) && op1){

                fprintf(stderr, "\nERROR:  use with  number and array[number] , #%s , %s ,is wrong ,in Line:%d" ,first_operand ,second_operand, line_num);
				return false;
            }
            if(string_is_integer(inside) && !check_if_macro_inti(table_list,first_operand)){

            	fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d", first_operand,line_num);
				return false;	
			} 
			if(isdigit(inside[0]) && !string_is_integer(inside)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false;
			}
			else{
				fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", inside , line_num);
					return false; 
			}

		}
		/*check that if first oprand is array  , secound oprand is #number/macro */
		if(second_operand[0] == '#' && isalpha(first_operand[0])){
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR: This label:%s is not valid for operand, invalid operation,in Line:%d", second_operand, line_num);
					return false;
			}
			second_operand+=1;
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num))
				{

					fprintf(stderr, "\nERROR: can't use with first label OR VAR and second '#' of oprands is wrong,in Line:%d", line_num);
					return false;
				}
			}

		
			
			strcpy(inside , first_operand);
			inside_in_Array(inside);
			if (check_if_macro_inti(table_list,inside) && check_if_macro_inti(table_list,second_operand))
			{

                	fprintf(stderr, "\nERROR:  can't use with this #%s ,While the  destination oprand is #%s, error in Line:%d",inside,second_operand, line_num);
					return false;
            }else{
            	fprintf(stderr, "\nERROR:  this #%s can't be in destination oprand , error in Line:%d",second_operand, line_num);
                	return  false;         
            } 
     

		}

        /*check that if first oprand is array/label  , secound oprand is array/label */ 
		if(isalpha(first_operand[0]) && isalpha(second_operand[0])){
			
			/*check that if label is vaild in first oprand and secound */ 
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list,first_operand,line_num) && check_if_its_label(table_list,second_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
					return true;
				}
			}

			/*check that if first oprand is array  , secound oprand is label */
			if(string_only_vaild_char(first_operand) == VAILD && string_only_letters_or_numbers(second_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,second_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (insideOP1 !=NULL)
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
			/*check that if first oprand is label  , secound oprand  is array */
			if(string_only_vaild_char(second_operand) == VAILD && string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				
				if(!check_if_its_label(table_list,first_operand,line_num))
				{
					return false;
				}
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
				if(isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
                	return true;	
				}
				else if(isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
				}
				else if (string_is_integer(insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error, error in Line:%d",insideOP2, line_num);
					return false;	
				}
			}
			/*here is problem*/
			if(string_only_vaild_char(first_operand) == VAILD && string_only_vaild_char(second_operand) == VAILD){	
				
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				
				strcpy(insideOP2 , second_operand);
				inside_in_Array(insideOP2);
			
				if (isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2))
				{

                	opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
                	return true;
           		} 
					if (isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
           		}
            	if (isalpha(insideOP2[0]) && !check_if_macro_inti(table_list,insideOP2) )            {
            		fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP2, line_num);
					return false;
    	        }
            	if(string_is_integer(insideOP1) && string_is_integer(insideOP2)){

               		opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
               		return true;
					
				} 
				if(string_is_integer(insideOP1) && isalpha(insideOP2[0]) && check_if_macro_inti(table_list,insideOP2)){

        	  	    opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
    	           	return true;
					
				} 
				if(string_is_integer(insideOP2) && isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

    		        opcode_list_add(opcode_list , ABSOLUTE ,first_operand, second_operand , OPCODE_MNEMONIC_LEA, 0 ,line_num, 5);
               		return true;
					
				} 
				if(isdigit(insideOP1[0]) && !string_is_integer(insideOP1)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP1 , line_num);
					return false;
				}
				if(isdigit(insideOP2[0]) && !string_is_integer(insideOP2)){
					fprintf(stderr, "\nERROR: This [%s] Syntax error , error in Line:%d", insideOP2 , line_num);
					return false;
				}
			
			}

		} 	

 }
 if(inc_definiton)
 {
		
		*inc_definiton = '\0' ;
		if(inc_definiton[3] != ' ' && inc_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		inc_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used inc definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = inc_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next7;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next7:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_INC, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_INC, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_INC, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}

 }
 if(dec_definiton)
 {				
		*dec_definiton = '\0' ;
		if(dec_definiton[3] != ' ' && dec_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		dec_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used dec definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = dec_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next8;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next8:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_DEC, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label:%s is not valid is wrong , error in line:%d" ,first_operand , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_DEC, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_DEC, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_DEC, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}

 } 
 if(jmp_definiton)
 {		
		*jmp_definiton = '\0' ;
		if(jmp_definiton[3] != ' ' && jmp_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		jmp_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used jmp definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = jmp_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next9;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next9:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_JMP, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_JMP, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}

 }
 if(bne_definiton)
 {
		*bne_definiton = '\0' ;
		if(bne_definiton[3] != ' ' && bne_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		bne_definiton +=1;		
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used bne definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = bne_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next10;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next10:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_BNE, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_BNE, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}

 }
 if(red_definiton)
 {		
		*red_definiton = '\0' ;
		if(red_definiton[3] != ' ' && red_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		red_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used red definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = red_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next11;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next11:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_RED, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_RED, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_RED, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_RED, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}	

 } 
 if(prn_definiton)
 {		
		*prn_definiton = '\0' ;
		if(prn_definiton[3] != ' ' && prn_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		prn_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used prn definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = prn_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next12;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next12:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{

				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				char *Pfirst_operand = first_operand;
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , Pfirst_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
				if(check_if_macro_inti(table_list,first_operand)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , Pfirst_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){

					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
					
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_PRN, 0 ,line_num, 3); /*add valus to opcode list*/
                	return true;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}

 }

 if(jsr_definiton)
 {		
		*jsr_definiton = '\0' ;
		if(jsr_definiton[3] != ' ' && jsr_definiton[3] != '\t'){
			fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
			return false;
		}
		remove_spaces(opcode_str); /* remove_spaces from the string */
		jsr_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used jsr definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = jsr_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) != ONE_OPERAND){ /*check how many word have if more one */				
				if(strstr(first_operand,"[")==NULL || strstr(first_operand,"]")==NULL){  /*See if it's because of the array*/
					fprintf(stderr, "\nERROR: Syntax error , in Line:%d", line_num);
					return false;
				}
				if(strstr(first_operand,"[")!=NULL|| strstr(first_operand,"]")!=NULL){  /*See if it's because of the array*/
					strcpy(insideOP1 , first_operand); 
					inside_in_Array(insideOP1); /*Copy the content between the brackets*/
				}
				if(count_words(insideOP1) == ONE_OPERAND ) /*If there is one word move on if you do not return an error*/
					goto next13;
				else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
				}
			}
next13:
		remove_spaces(first_operand); 		/* the first operand without spaces */
		if (is_register(first_operand))
		{
				opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_JSR, 0 ,line_num, 2); /*add valus to opcode list*/
				return true;
		}
		
		/*check that if  oprand is vaild is not number */
		if (string_is_integer(first_operand))
		{
				fprintf(stderr, "\nERROR: oprands can't be number is Invalid operation ,in line:%d", line_num); /*print error*/
				return false;
		}
		if(isdigit(first_operand[0])){
			fprintf(stderr, "\nERROR: The oprands can't start with number , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check if oprands label not macro */
		if(check_if_macro_inti(table_list,first_operand)){
			fprintf(stderr, "\nERROR: The label can't be macro , wrong in Line:%d", line_num); /*print error*/
			return false;
		}
		/*check that if first oprand #number/macro*/		
		if(first_operand[0] == '#')
		{
				first_operand+=1;
				op1 = string_is_integer(first_operand);
				if(op1)
				{
					fprintf(stderr, "\nERROR: Using a number:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: Using a macro:#%s is an illegal operation, error in line:%d",first_operand , line_num);
					return false;
				}
				if(!check_if_macro_inti(table_list,first_operand)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",first_operand , line_num);
					return false;
				}
		}
		if(isalpha(first_operand[0])){
			/*check that if label is vaild first oprand or secound */ 
			if(string_only_vaild_char(first_operand) != VAILD){
					fprintf(stderr, "\nERROR:oprands label is not valid is wrong , error in line:%d" , line_num);
					return false;
			}
			if(string_only_letters_or_numbers(first_operand) != NOT_ONLY_LETTERS_AND_NUMBERS){
				if(check_if_its_label(table_list ,first_operand,line_num))
				{
					opcode_list_add(opcode_list , ABSOLUTE , NULL , first_operand , OPCODE_MNEMONIC_JSR, 0 ,line_num, 2); /*add valus to opcode list*/
					return true;
				}
			}
			/*check that if first oprand is array*/
			if(isalpha(first_operand[0])){
				strcpy(insideOP1 , first_operand);
				inside_in_Array(insideOP1);
				if(isalpha(insideOP1[0]) && check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;	
				}
				else if(isalpha(insideOP1[0]) && !check_if_macro_inti(table_list,insideOP1)){
					fprintf(stderr, "\nERROR: This macro:#%s not vaild , Error in Line:%d",insideOP1, line_num);
					return false;
				}
				else if (string_is_integer(insideOP1))
				{
	
					fprintf(stderr, "\nERROR:oprands array is not valid is wrong , error in line:%d" , line_num);
					return false;
				}
				else{
					fprintf(stderr, "\nERROR: This [%s] is Syntax error , error in Line:%d",insideOP1, line_num);
					return false;	
				}
			}
		}
		
 }
 if(rts_definiton)
 {		
		*rts_definiton = '\0' ;
		remove_spaces(opcode_str); /* remove_spaces from the string */
		rts_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used rts definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = rts_definiton += 2; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) == NO_OPERANDS) 	/* check that oprands have only one word */
		{
					opcode_list_add(opcode_list , ABSOLUTE , NULL , NULL , OPCODE_MNEMONIC_RTS, 0 ,line_num, 1); /*add valus to opcode list*/
                	return true;
		}else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
		}
 }
 if(stop_definiton)
 {		
	    *stop_definiton = '\0' ;
		remove_spaces(opcode_str); /* remove_spaces from the string */
		stop_definiton +=1;
		if(opcode_str[0] != '\0') /* if there is a charchter before the dot . */
		{
			fprintf(stderr, "\nERROR: Used stop definition not at the strat of the line,in Line:%d", line_num);
			return false;
		}	
		first_operand = stop_definiton += 3; 		/*To advance to the beginning of the first operand */
		if(count_words(first_operand) == NO_OPERANDS) 	/* check that oprands have only one word */
		{
					opcode_list_add(opcode_list , ABSOLUTE , NULL , NULL , OPCODE_MNEMONIC_STOP, 0 ,line_num, 1); /*add valus to opcode list*/
                	return true;
		}else{
					fprintf(stderr, "\nERROR: The amount of operands is incorrect,in Line:%d", line_num); /*print error*/
					return false;
		}
 }
 if(  !mov_definiton || !cmp_definiton || !add_definiton || !sub_definiton || !not_definiton || !clr_definiton || !lea_definiton ||!inc_definiton
   || !dec_definiton || !jmp_definiton || !bne_definiton || !red_definiton || !prn_definiton ||!jsr_definiton  || !rts_definiton || !stop_definiton ){
		fprintf(stderr, "\nERROR: Unrecognized command syntax error,in Line:%d", line_num);
		return false;
	}
	else{
		return false;
	}
}

void inside_in_Array(char *str){
			char *check_if_is_arryL=NULL;
			char *inside = NULL;
			char *check_if_is_arryR = NULL;

	        check_if_is_arryL = strstr(str, "[");
			*check_if_is_arryL = '\0';
			inside =check_if_is_arryL +1 ;
		    check_if_is_arryR = strstr(inside, "]");
			*check_if_is_arryR='\0';
			
			strcpy(str , inside);
}
size_t opcode_get_size(opcode_list_t *opcode_list){
	return 0;
}

opcode_list_error_t opcode_list_init(opcode_list_t *opcode_list)
{
	opcode_list->first = NULL;
	opcode_list->last = NULL;
	opcode_list->size = 0;
	return OPCODE_TABLE_ERROR_SUCCESS;
}

opcode_list_error_t opcode_list_add(opcode_list_t *opcode_list, int are, char *src_operand, char *des_operand, int opcode, int reserved,int line_num, int size)
{
	opcode_t *new_entry = NULL;
	if (opcode_list == NULL)
		return OPCODE_TABLE_ERROR_GIVE_NULL;

	/* create a new entry */
	new_entry = (opcode_t*)malloc(sizeof(opcode_t));
	if (new_entry == NULL)
	{
		printf("\ncan't allocate memory\n");
		exit(1);
	}

	/* get all the prameters inside the entry */
	new_entry->are = are;
	new_entry->opcode = opcode;
	new_entry->reserved = reserved;
	new_entry->line_num = line_num;
	new_entry->size = size;
	new_entry->next = NULL;

	if (des_operand != NULL)
	{
		/* allocate memory for the string(destination operand) */
		char *new_des_operand = (char*)malloc((sizeof(char)*strlen(des_operand)) + 1); /*plus 1 for null('\0')*/
		if (new_des_operand == NULL)
		{
			printf("\ncan't allocate memory\n");
			exit(1);
		}
		strcpy(new_des_operand, des_operand);
		new_entry->des_operand = new_des_operand;
	}
	else	/* the des_operand is NULL */
	{
		new_entry->des_operand = NULL;
	}

	if (src_operand != NULL)
	{
		/* allocate memory for the string(source operand) */
		char *new_src_operand = (char*)malloc((sizeof(char)*strlen(src_operand)) + 1); /*plus 1 for null('\0')*/
		if (new_src_operand == NULL)
		{
			printf("\ncan't allocate memory\n");
			exit(1);
		}
		strcpy(new_src_operand, src_operand);
		new_entry->src_operand = new_src_operand;
	}
	else	/* the src_operand is NULL */
	{
		new_entry->src_operand = NULL;
	}


	/* if the list is empty */
	if (opcode_list->first == NULL)
	{
		opcode_list->first = new_entry;
		opcode_list->last = opcode_list->first;
		opcode_list->size++;

		return OPCODE_TABLE_ERROR_SUCCESS;
	}
	else
	{
		opcode_list->last->next = new_entry;
		opcode_list->last = opcode_list->last->next;
		opcode_list->size++;

		return OPCODE_TABLE_ERROR_SUCCESS;
	}

}


opcode_list_error_t opcode_list_destroy(opcode_list_t *opcode_list)
{
	opcode_t* current = opcode_list->first;
	opcode_t* Pnext = NULL;
	if (opcode_list == NULL)
	{
		return OPCODE_TABLE_ERROR_GIVE_NULL;
	}

	while (current != NULL)
	{
		Pnext = current->next;
		if (current->des_operand != NULL)	/*  free the des_operand, if its exist */
			free(current->des_operand);
		if (current->src_operand != NULL)	/* free the src_operand, if its exist */
			free(current->src_operand);

		free(current);		/* free the entery */
		current = Pnext;
	}

	return OPCODE_TABLE_ERROR_SUCCESS;
}

/* for debug use */
void opcode_table_print_list(opcode_list_t *opcode_list)
{
	opcode_t* current = opcode_list->first;
	int i = 1;

	while (current != NULL)
	{
		printf("\nnode %d:", i);
		printf("\nare: %d", current->are);
		printf("\ndes_operand: %s", current->des_operand);
		printf("\nsrc_operand: %s", current->src_operand);
		printf("\nopcode: %d", current->opcode);
		printf("\nreserved: %d", current->reserved);
		printf("\nline number: %d", current->line_num);
		printf("\nsize: %d", current->size);
		printf("\n-------------------------------------------------\n");

		current = current->next;
		i++;
	}
}



/**
* This function handle register operand.
* if both opernad are registers it will write the information to the file.
* if src opernad is register it will write the information to the file.
* if des opernad is register it will write the information to the file.
* It return which operands are register: both, only src, only des and none.
*/
TAKE_CARE handle_rgister(char *word, char *src_operand_str, char *des_operand_str, int *ic, FILE *output_code)
{
	char *src_register_area = word + 6; /*the area to write the src register bits */
	int src_register_area_size = 3;

	char *des_register_area = word + 9;	/*the area to write the des register bits */
	int des_register_area_size = 3;

	char *register_ARE_area = word + 12; /*the area to write the register A,R,E bits */
	int register_ARE_area_size = 2;

	/* two operands are registers, write to the file */
	if (is_register(src_operand_str) && is_register(des_operand_str))
	{
		char *src_value_str = src_operand_str + 1;	/* get the register number as string*/
		int src_value_int = atoi(src_value_str);	/* convert it to integer */

		char *des_value_str = des_operand_str + 1;	/* get the register number as string*/
		int des_value_int = atoi(des_value_str);	/* convert it to integer */


		/* write the src operand in the second word in binary*/
		dec_to_binary(src_register_area, src_value_int, src_register_area_size);

		/* write the des operand in the second word in binary*/
		dec_to_binary(des_register_area, des_value_int, des_register_area_size);

		/* write the A,R,E in the second word in binary*/
		dec_to_binary(register_ARE_area, ABSOLUTE, register_ARE_area_size);

		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */

		return BOTH_SRC_DES_OPERAND;
	}

	/* src operands is register, write to the file */
	else if (is_register(src_operand_str))
	{
		char *src_value_str = src_operand_str + 1; /* get the register number as string*/
		int src_value_int = atoi(src_value_str);				 /* convert it to integer */

		/* write the src operand in the second word in binary*/
		dec_to_binary(src_register_area, src_value_int, src_register_area_size);

		/* write the A,R,E in the second word in binary*/
		dec_to_binary(register_ARE_area, ABSOLUTE, register_ARE_area_size);

		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */

		return SRC_OPERAND_ONLY;
	}
	/* des operands is register, write to the file */
	else if (is_register(des_operand_str))
	{
		char *des_value_str = des_operand_str + 1; /* get the register number as string*/
		int des_value_int = atoi(des_value_str);                                 /* convert it to integer */

		/* write the des operand in the second word in binary*/
		dec_to_binary(des_register_area, des_value_int, des_register_area_size);

		/* write the A,R,E in the second word in binary*/
		dec_to_binary(register_ARE_area, ABSOLUTE, register_ARE_area_size);

		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;  /* move forword the line counter */
		clean_word(word);       /* turn all the cell in the array back to zero */


		return DES_OPERAND_ONLY;
	}
	/* none of the operands are registers */
	else
		return NONE;
}


/**
* This function handle the rest of the operands, NOT including register.
* It write to the file the operand extra information.
* The 3 option to deal with are: direct addressing (#), array, label(that is not array).
*/
bool handle_operand(symbol_table_list_t *list, char *word, char *operand_str, int line_num, int *ic, FILE *output_code, FILE *output_extern)
{
	/* maping the word */
	char *value_area = word;
	int value_area_size = 12;
	char *ARE_area = word + 12;
	int ARE_area_size = 2;

	/* if its a direct addressing (#) */
	/*********************************/
	if (strstr(operand_str, "#"))
	{
		char *value_str = operand_str + 1;
		int value_int = 0;
		/* if the value is macro and not an integer */
		if (!string_is_integer(value_str))
		{
			symbol_table_entry_t *node_for_macro = NULL;
			symbol_table_find(list, value_str, &node_for_macro);
			if (node_for_macro == NULL)
			{
				printf("\nERROR: in function 'handle_operand', didn't find macro in symbol table. not supposed to happen.");
			}
			value_int = node_for_macro->address_value;
		}
		else /* if the value is integer, and not a macro */
		{
			value_int = atoi(value_str);
		}

		/* write the value of the operand as binary number */
		dec_to_binary(value_area, value_int, value_area_size);

		/* write the A,R,E as binary number */
		dec_to_binary(ARE_area, ABSOLUTE, ARE_area_size);

		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */

		return true;
	}

	/* if its an array */
	/******************/
	else if (strstr(operand_str, "["))
	{
		/* declerations */
		char *square_brackets = strstr(operand_str, "[");
		char *square_brackets_end = strstr(operand_str, "]");
		char *cell_num_str = square_brackets + 1;
		int cell_num_int = 0;
		char *array_name = operand_str;
		symbol_table_entry_t *node = NULL;
		*square_brackets = '\0';
		*square_brackets_end = '\0';
		/*************************************************/

		/* if the value is macro and not an integer */
		if (!string_is_integer(cell_num_str))
		{
			symbol_table_entry_t *node_for_macro = NULL;
			symbol_table_find(list, cell_num_str, &node_for_macro);
			cell_num_int = node_for_macro->address_value;
		}
		else /* if the value is integer, and not a macro */
		{
			cell_num_int = atoi(cell_num_str);
		}
		/* if the argument inside the array is less than zero(error) */
		if (cell_num_int < 0)
		{
			fprintf(stderr, "\nERROR: the argument inside the array can't be less than zero. In line: %d", line_num);
			return false;
		}

		/* if the label dont exist in the symbol table(error) */
		if (symbol_table_find(list, array_name, &node) != SYMBOL_TABLE_ERROR_SUCCESS)
		{
			fprintf(stderr, "\nERROR: used label name: %s, that didn't initialize. In line: %d", array_name, line_num);
			return false;
		}

		/* if the label name is macro */
		if (node->kind == SYMBOL_TYPE_MACRO)
		{
			fprintf(stderr,"\nERROR: used label name: %s, that is already initialize as macro name. In line: %d", array_name, line_num);
			return false;
		}

		/* write the second word as binary number */
		if (node->is_extern) /* if its external*/
		{
			dec_to_binary(value_area, node->address_value, value_area_size);
			dec_to_binary(ARE_area, EXTERNAL, ARE_area_size);
			if (*ic <1000)
				fprintf(output_extern, "%s\t0%d\n", operand_str, *ic); /* print the word to a extern file */
			else
				fprintf(output_extern, "%s\t%d\n", operand_str, *ic);
		}
		else	/* if it relocatable */
		{
			dec_to_binary(value_area, node->address_value, value_area_size);
			dec_to_binary(ARE_area, RELOCATABLE, ARE_area_size);
		}

		
		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */


		/* write the third word as binary */
		dec_to_binary(value_area, cell_num_int, value_area_size);
		dec_to_binary(ARE_area, ABSOLUTE, ARE_area_size);

		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */

		return true;
	}

	/* if its a label(and not array) */
	/********************************/
	else
	{
		symbol_table_entry_t *node = NULL;
		/* if the label dont exist in the symbol table(error) */
		if (symbol_table_find(list, operand_str, &node) != SYMBOL_TABLE_ERROR_SUCCESS)
		{
			fprintf(stderr, "\nERROR: used label name: %s, that didn't initialize. In line: %d", operand_str, line_num);
			return false;
		}

		/* if the label name is macro */
		if (node->kind == SYMBOL_TYPE_MACRO)
		{
			fprintf(stderr, "\nERROR: used label name: %s, that is already initialize as macro name. In line: %d", operand_str, line_num);
			return false;
		}

		/* write the second word as binary number */
		if (node->is_extern) /* if its external*/
		{
			dec_to_binary(value_area, node->address_value, value_area_size);
			dec_to_binary(ARE_area, EXTERNAL, ARE_area_size);
			if (*ic <1000)
				fprintf(output_extern, "%s\t0%d\n", operand_str, *ic); /* print the word to a extern file */
			else
				fprintf(output_extern, "%s\t%d\n", operand_str, *ic);
		}
		else	/* if it relocatable */
		{
			dec_to_binary(value_area, node->address_value, value_area_size);
			dec_to_binary(ARE_area, RELOCATABLE, ARE_area_size);
		}


		word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
		fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
		*ic = *ic + 1;	/* move forword the line counter */
		clean_word(word);	/* turn all the cell in the array back to zero */

		return true;
	}

}


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
bool opcode_encode(opcode_t *opcode_entry, symbol_table_list_t *list, int *ic, FILE *output_code, FILE *output_extern)
{
	char word[MAX_WORD_SIZE] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };	/* word size, dont forget the null in the end */


	/* map the first word */
	char *reserved_area = word;			/* the area to write the reserved bits in the first word */
	char *opcode_area = reserved_area + RESERVED_AREA_SIZE;	/* the area to write the opcode bits in the first word */
	char *src_addressing_area = opcode_area + OPCODE_AREA_SIZE; /* the area to write the src addressing bits in the first word */
	char *des_addressing_mode = src_addressing_area + SRC_ADDRESSING_AREA_SIZE; /* the area to write the des addressing bits in the first word */
	char *are_area = des_addressing_mode + DES_ADDRESSING_AREA_SIZE; /*the area to write the A,R,E bits in the first word */

	word[MAX_WORD_SIZE - 1] = '\0';
	clean_word(word);

	/*************************/
	/* write the first word */
	/***********************/

	/* write reserved area as binary, in the first word string */
	dec_to_binary(reserved_area, opcode_entry->reserved, RESERVED_AREA_SIZE);


	/* write opcode area as binary, in the first word string */
	dec_to_binary(opcode_area, opcode_entry->opcode, OPCODE_AREA_SIZE);


	/* write src operand area as binary, in the first word string */
	/*************************************************************/
	if (opcode_entry->src_operand == NULL)
	{
		dec_to_binary(src_addressing_area, 0, SRC_ADDRESSING_AREA_SIZE);
	}
	/* if its a immidiate addressing */
	else if (strstr(opcode_entry->src_operand, "#"))
		dec_to_binary(src_addressing_area, IMMEDIATE_ADDRESSING, SRC_ADDRESSING_AREA_SIZE);
	/* if its register */
	else if (is_register(opcode_entry->src_operand))
		dec_to_binary(src_addressing_area, DIRECT_REGISTER_ADDRESSING, SRC_ADDRESSING_AREA_SIZE);
	/* if its an array */
	else if (strstr(opcode_entry->src_operand, "["))
		dec_to_binary(src_addressing_area, PERMANENT_INDEX_ADDRESSING, SRC_ADDRESSING_AREA_SIZE);
	/* if its a label(not an array) */
	else
		dec_to_binary(src_addressing_area, DIRECT_ADDRESSING, SRC_ADDRESSING_AREA_SIZE);


	/* write des operand area as binary, in the first word string */
	/*************************************************************/
	if (opcode_entry->des_operand == NULL)
	{
		dec_to_binary(des_addressing_mode, 0, DES_ADDRESSING_AREA_SIZE);
	}
	/* if its a immidiate addressing */
	else if (strstr(opcode_entry->des_operand, "#"))
		dec_to_binary(des_addressing_mode, IMMEDIATE_ADDRESSING, DES_ADDRESSING_AREA_SIZE);
	/* if its register */
	else if (is_register(opcode_entry->des_operand))
		dec_to_binary(des_addressing_mode, DIRECT_REGISTER_ADDRESSING, DES_ADDRESSING_AREA_SIZE);
	/* if its an array */
	else if (strstr(opcode_entry->des_operand, "["))
		dec_to_binary(des_addressing_mode, PERMANENT_INDEX_ADDRESSING, DES_ADDRESSING_AREA_SIZE);
	/* if its a label(not an array) */
	else
		dec_to_binary(des_addressing_mode, DIRECT_ADDRESSING, DES_ADDRESSING_AREA_SIZE);


	/* write A,R,E area as binary, in the first word string */
	dec_to_binary(are_area, ABSOLUTE, ARE_AREA_SIZE);

	word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
	fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
	*ic = *ic + 1;	/* move forword the line counter */
	clean_word(word);	/* turn all the cell in the array back to zero */




	/**************************/
	/* write the extra words */
	/************************/
	/* IF THE TWO OPERANDS ARE NULL */
	if (opcode_entry->src_operand == NULL && opcode_entry->des_operand == NULL)
	{
		return true;
	}
	/* IF THE SOURCE OPERAND IS NULL */
	if (opcode_entry->src_operand == NULL)
	{
		if (is_register(opcode_entry->des_operand)) /* if its a register */
		{
			char *des_register_area = word + 9;	/*the area to write the des register bits */
			int des_register_area_size = 3;

			char *register_ARE_area = word + 12; /*the area to write the register A,R,E bits */
			int register_ARE_area_size = 2;
			char *des_value_str = opcode_entry->des_operand + 1; /* get the register number as string*/
			int des_value_int = atoi(des_value_str);				 /* convert it to integer */



			/* write the des operand in the second word in binary*/
			dec_to_binary(des_register_area, des_value_int, des_register_area_size);

			/* write the A,R,E in the second word in binary*/
			dec_to_binary(register_ARE_area, ABSOLUTE, register_ARE_area_size);

			word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
			fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
			*ic = *ic + 1;	/* move forword the line counter */
			clean_word(word);	/* turn all the cell in the array back to zero */


			return true;

		}
		if (!handle_operand(list, word, opcode_entry->des_operand, opcode_entry->line_num, ic, output_code, output_extern))
			return false;

		return true;
	}
	/* IF THE DESTINATION OPERAND IS NULL */
	if (opcode_entry->des_operand == NULL)
	{
		if (is_register(opcode_entry->src_operand)) /* if its a register */
		{
			char *src_register_area = word + 6;	/*the area to write the src register bits */
			int src_register_area_size = 3;

			char *register_ARE_area = word + 12; /*the area to write the register A,R,E bits */
			int register_ARE_area_size = 2;
			char *src_value_str = opcode_entry->src_operand + 1; /* get the register number as string*/
			int src_value_int = atoi(src_value_str);				 /* convert it to integer */


			/* write the des operand in the second word in binary*/
			dec_to_binary(src_register_area, src_value_int, src_register_area_size);

			/* write the A,R,E in the second word in binary*/
			dec_to_binary(register_ARE_area, ABSOLUTE, register_ARE_area_size);

			word[MAX_WORD_SIZE - 1] = '\0'; /* put a null at the end of the string */
			fprintf(output_code, "%d\t%s\n", *ic, word); /* print the word to a tmp file */
			*ic = *ic + 1;	/* move forword the line counter */
			clean_word(word);	/* turn all the cell in the array back to zero */


			return true;

		}
		if (!handle_operand(list, word, opcode_entry->src_operand, opcode_entry->line_num, ic, output_code, output_extern))
			return false;

		return true;
	}
	/* IF BOTH OPERANDS ARE REGISTERS */
	if (is_register(opcode_entry->src_operand) && is_register(opcode_entry->des_operand))
	{
		/* handel the source and the destination operands, which are a registers  */
		handle_rgister(word, opcode_entry->src_operand, opcode_entry->des_operand, ic, output_code);
		return true;
	}
	/* IF THE SOURCE OPERAND IS REGISTER */
	if (is_register(opcode_entry->src_operand))
	{
		/* handel the source operand, which is a register  */
		handle_rgister(word, opcode_entry->src_operand, opcode_entry->des_operand, ic, output_code);

		/* handle the des operand, and wirte to the file if there are no error */
		if (!handle_operand(list, word, opcode_entry->des_operand, opcode_entry->line_num, ic, output_code, output_extern))
			return false;

		return true;
	}
	/* IF THE DESTINATION OPERAND IS REGISTER */
	if (is_register(opcode_entry->des_operand))
	{
		/* handle the src operand, and wirte to the file if there are no error */
		if (!handle_operand(list, word, opcode_entry->src_operand, opcode_entry->line_num, ic, output_code, output_extern))
			return false;

		/* handel the destination operand, which is a register  */
		handle_rgister(word, opcode_entry->src_operand, opcode_entry->des_operand, ic, output_code);

		return true;
	}
	/* IF NONE OF THE OPERANDS ARE REGISTERS */
	else
	{
		bool flag = true; /* if the flag is false there is an error */

		/* handle the src operand, and wirte to the file if there are no error */
		if (!handle_operand(list, word, opcode_entry->src_operand, opcode_entry->line_num, ic, output_code, output_extern))
			flag = false;

		/* handle the des operand, and wirte to the file if there are no error */
		if (!handle_operand(list, word, opcode_entry->des_operand, opcode_entry->line_num, ic, output_code, output_extern))
			flag = false;

		return flag;
	}

}
