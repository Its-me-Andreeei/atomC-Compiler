//
//  main.c
//  atomC
//
//  Created by Andrei Karolyi on 28.02.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include "./Lexer/utils.h"
#include "./Lexer/lexer.h"

int main(int argc, const char * argv[]) {
    char *pch;
    Token *token_list;
    
    if(argc != 2)
    {
        fprintf(stderr, "Error at line %d : No input file given, please provide one C file.\n", __LINE__);
        exit(EXIT_FAILURE);
    }
    pch = loadFile(argv[1]);
    token_list=tokenize(pch);
    showTokens(token_list);
    return 0;
}
