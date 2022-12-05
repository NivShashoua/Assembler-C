#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "bool.h"

#define MAX_FILE_NAME (52)

bool file_is_empty(FILE *file);

int main(int argc, const char *argv[])
{
	FILE *input, *output_code_tmp, *output_code, *output_entry, *output_extern;


	int i;
	if (argc == 1)
	{
		printf("\nThere are no files that the assembler can use, please insert a file name\n");
		return 0;
	}
	/* scan all the files attached, and use the assembler */
	for (i = 1; i < argc; i++)
	{
		char file_name[MAX_FILE_NAME] = { 0 };
		char output_file_name[MAX_FILE_NAME] = { 0 };
		char entry_file_name[MAX_FILE_NAME] = { 0 };
		char extern_file_name[MAX_FILE_NAME] = { 0 };
		
		char *as_def = NULL;


		/* check if the name of the file is valied */
		/******************************************/
		if (strlen(argv[i]) > MAX_FILE_NAME - 2)
		{
			printf("\nThe name of file is too long, can be maximum: %d charcters. stop assembler.\n", (MAX_FILE_NAME-2));
			return 0;
		}
		strcpy(file_name, argv[i]);
		
		as_def = strstr(file_name, ".as");
		if (as_def == NULL) /* if the file name doesn't have .as at the end(error) */
		{
			printf("\nThe name of file input file have to end with '.as'. stop assembler.\n");
			return 0;
		}
		*as_def = '\0';
		if (*file_name == '\0') /* if the the name before the file is empty(error) */
		{
			printf("\nThe name of file input is illegal. stop assembler.\n");
			return 0;
		}
		if (*(as_def + 3) != '\0') /* if the filename extension is diffrent from .as (error)*/
		{
			printf("\nThe name of file input file have to end with '.as'. stop assembler.\n");
			return 0;
		}



		/* open all the files */
		/*********************/

		/*input file*/
		input = fopen(argv[i], "r");
		if (input == NULL)
		{
			printf("\nCan't open the input file: %s, maybe its not exist in the directory. stop assembler\n",argv[i]);
			return 0;
		}

		/* tmp file */
		output_code_tmp = tmpfile();
		if (output_code_tmp == NULL)
		{
			printf("\ncan't creat temp file\n");
			exit(1);
		}
	
		/* output code file */
		strcpy(output_file_name, file_name);
		strcat(output_file_name, ".ob");
		output_code = fopen(output_file_name, "w+");
		if (output_code == NULL)
		{
			printf("\ncan't creat output code file\n");
			exit(1);
		}
	

		/* out put entry file */
		strcpy(entry_file_name, file_name);
		strcat(entry_file_name, ".ent");
		output_entry = fopen(entry_file_name, "w+");
		if (output_entry == NULL)
		{
			printf("\ncan't creat entry file\n");
			exit(1);
		}

		/* out put extern file */
		strcpy(extern_file_name, file_name);
		strcat(extern_file_name, ".ext");
		output_extern = fopen(extern_file_name, "w+");
		if (output_extern == NULL)
		{
			printf("\ncan't creat extern file\n");
			exit(1);
		}


		/* use the assembler, close and remove files if they are empty */
		/**************************************************************/

		if (assembler(input, output_code_tmp, output_code, output_entry, output_extern))
		{
			/* if there is an error */
			fclose(input);
			fclose(output_code_tmp);
			fclose(output_code);
			fclose(output_entry);
			fclose(output_extern);

			remove(output_file_name);
			remove(entry_file_name);
			remove(extern_file_name);
			printf("\n"); /* print an extra line, if there is error */
		}
		else
		{
			/* check if the output_code file is empty */
			if (file_is_empty(output_code))
			{
				fclose(output_code);
				remove(output_file_name);
			}
			else
			{
				fclose(output_code);
			}

			/* check if the entry file is empty */
			if (file_is_empty(output_entry))
			{
				fclose(output_entry);
				remove(entry_file_name);
			}
			else
			{
				fclose(output_entry);
			}

			/* check if the extern file is empty */
			if (file_is_empty(output_extern))
			{
				fclose(output_extern);
				remove(extern_file_name);
			}
			else
			{
				fclose(output_extern);
			}


			fclose(input);
		}

	}

	return 0;
}

/* find if the file is empty(return true) */
bool file_is_empty(FILE *file)
{
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0)
		return true;

	return false;
}
