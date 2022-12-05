/* pragma once */
#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"

bool assembler(FILE *input, FILE *output_code_tmp, FILE *output_code, FILE *output_entry, FILE *output_extern);

#endif /* _ASSEMBLER_H */