#ifndef ENUMS_H_
#define ENUMS_H_
 
/* Addressing methods */
typedef enum {
	INVALID_ADDRESS_METHOD = -1,
	IMMEDIATE = 0,
	DIRECT = 1,
	COPY_PREVIOUS = 2,
	DIRECT_REGISTER = 3
} ADDRESS_METHOD;
 
/* ERA options */
typedef enum {
	NON_AER = -1,
	ABSOLUTE = 0,
	EXTERNAL = 1,
	RELOCATABLE = 2
} ERA;
 
/* Number of operands */
typedef enum {
	NO_OPERANDS = 0,
	ONE_OPERAND = 1,
	TWO_OPERANDS = 2
} OPERANDS_COUNT;
 

/* for function: string_only_letter */
typedef enum {
	NOT_ONLY_LETTERS_AND_NUMBERS,
	ONLY_NUMBERS,
	ONLY_CAPITAL_LETTERS,
	ONLY_SMALL_LETTERS,
	SMALL_AND_CAPITAL_AND_NUM,
	BOTH_SMALL_AND_CAPITAL,
	BOTH_SMALL_AND_NUM,
	BOTH_CAPITAL_AND_NUM,
	VAILD,
	NOT_VAILD
}STRING_ONLY_LETTER;


/* for function handle_redister */
typedef enum{
	SRC_OPERAND_ONLY,
	DES_OPERAND_ONLY,
	BOTH_SRC_DES_OPERAND,
	NONE
}TAKE_CARE;

#endif /* ENUMS_H_ */
