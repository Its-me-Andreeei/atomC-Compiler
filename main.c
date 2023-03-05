//
//  main.c
//  atomC
//
//  Created by Andrei Karolyi on 28.02.2023.
//

#include <stdio.h>
#include "./Lexer/utils.h"
#include "./Lexer/lexer.h"

int main(int argc, const char * argv[]) {
    char *pch;
    Token *token_list;
    //pch=loadFile("./tests_ALEX/testlex.c");
    pch=loadFile("./tests_ALEX/try.c");
    token_list=tokenize(pch);
    showTokens(token_list);
    return 0;
}
