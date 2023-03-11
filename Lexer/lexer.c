#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

Token *tokens;	// single linked list of tokens
Token *lastTk;		// the last token in list

//flag enum used in finding an int or enum
enum{
    IS_INT=1,
    IS_DOUBLE=2
};

static const char *token_names[]={
    //keywords
    [ID]="ID",
    [WHILE]="WHILE",
    [TYPE_CHAR]="TYPE_CHAR",
    [TYPE_INT]="TYPE_INT",
    [TYPE_DOUBLE]="TYPE_DOUBLE",
    [IF]="IF",
    [ELSE]="ELSE",
    [STRUCT]="STRUCT",
    [VOID]="VOID",
    [RETURN]="RETURN",
    
    //delimiter
    [END]="END",
    [COMMA]="COMMA",
    [SEMICOLON]="SEMICOLON",
    [LPAR]="LPAR",
    [RPAR]="RPAR",
    [LBRACKET]="LBRACKET",
    [RBRACKET]="RBRACKET",
    [LACC]="LACC",
    [RACC]="RACC",
    
    //operators
    [ASSIGN]="ASSIGN",
    [EQUAL]="EQUAL",
    [ADD]="ADD",
    [SUB]="SUB",
    [MUL]="MUL",
    [DIV]="DIV",
    [DOT]="DOT",
    [AND]="AND",
    [OR]="OR",
    [NOT]="NOT",
    [NOTEQ]="NOTEQ",
    [LESS]="LESS",
    [LESSEQ]="LESSEQ",
    [GREATER]="GREATER",
    [GREATEREQ]="GREATEREQ",
    
    //constants
    [INT]="INT",
    [DOUBLE]="DOUBLE",
    [STRING]="STRING",
    [CHAR]="CHAR"
};
int line=1;		// the current line in the input file

// adds a token to the end of the tokens list and returns it
// sets its code and line
Token *addTk(int code){
	Token *tk=safeAlloc(sizeof(Token));
	tk->code=code;
	tk->line=line;
	tk->next=NULL;
	if(lastTk){
		lastTk->next=tk;
		}else{
		tokens=tk;
		}
	lastTk=tk;
	return tk;
	}

static char *extract(const char *begin,const char *end){
    long length = end - begin;
    if(length < 1)
        error("Error at line %d : Negative length identifier found! Check for bugs", __LINE__);
    char *p = safeAlloc(length * sizeof(char));
    strncpy(p, begin, end-begin);
    p[strlen(p)]='\0';
    return p;
    }

Token *tokenize(const char *pch){
	const char *start;
	Token *tk;

	for(;;){
		switch(*pch){
			case ' ':case '\t':
                pch++;
                break;
			case '\r':		// handles different kinds of newlines (Windows: \r\n, Linux: \n, MacOS, OS X: \r or \n)
				if(pch[1]=='\n')
                    pch++;
				// fallthrough to \n because there is no break 
			case '\n':
				line++;
				pch++;
				break;
			case '\0':addTk(END); return tokens;
			case ',':addTk(COMMA); pch++; break;
            case ';':addTk(SEMICOLON); pch++; break;
            case '(':addTk(LPAR); pch++; break;
            case ')':addTk(RPAR); pch++; break;
            case '[':addTk(LBRACKET); pch++; break;
            case ']':addTk(RBRACKET); pch++; break;
            case '{':addTk(LACC); pch++; break;
            case '}':addTk(RACC); pch++; break;
			case '=':
				if(pch[1]=='=')
                {
					addTk(EQUAL);
					pch+=2;
                }
                else
                {
					addTk(ASSIGN);
					pch++;
                }
				break;
            case '+':addTk(ADD); pch++; break;
            case '-':addTk(SUB); pch++; break;
            case '*':addTk(MUL); pch++; break;
            case '/':
                if(pch[1]=='/')//line comment found!
                {
                    while(*pch!='\n' && *pch!='\0'){pch++;}
                    line++;
                }
                else
                    addTk(DIV); pch++;
                break;
            case '.':addTk(DOT); pch++; break;
            case '&':
                if(pch[1]=='&')
                {
                    addTk(AND);
                    pch+=2;
                }
                else
                    error("Expected: \"&&\", but found \"&\"\n"); // QA ->TBD
                break;
            case '|':
                if(pch[1]=='|')
                {
                    addTk(OR);
                    pch+=2;
                }
                else
                    error("Expected: \"||\", but found \"|\"\n"); // QA ->TBD
                break;
            case '!':
                if(pch[1]=='=')
                {
                    addTk(NOTEQ); pch+=2;
                }
                else
                {
                    addTk(NOT); pch++;
                }
                break;
            case '<':
                if(pch[1]=='=')
                {
                    addTk(LESSEQ); pch+=2;
                }
                else
                {
                    addTk(LESS); pch++;
                }
                break;
            case '>':
                if(pch[1]=='=')
                {
                    addTk(GREATEREQ); pch+=2;
                }
                else
                {
                    addTk(GREATER); pch++;
                }
                break;
			default:
				if(isalpha(*pch)||*pch=='_')
                {
					for(start = pch++; isalnum(*pch) || *pch == '_'; pch++){}
					char *text = extract(start, pch);
					if(strcmp(text,"char")==0)
                    {
                        addTk(TYPE_CHAR);
                    }
                    else if(strcmp(text,"int")==0)
                    {
                        addTk(TYPE_INT);
                    }
                    else if(strcmp(text,"while")==0)
                    {
                        addTk(WHILE);
                    }
                    else if(strcmp(text,"double")==0)
                    {
                        addTk(TYPE_DOUBLE);
                    }
                    else if(strcmp(text,"if")==0)
                    {
                        addTk(IF);
                    }
                    else if(strcmp(text,"else")==0)
                    {
                        addTk(ELSE);
                    }
                    else if(strcmp(text,"return")==0)
                    {
                        addTk(RETURN);
                    }
                    else if(strcmp(text,"void")==0)
                    {
                        addTk(VOID);
                    }
                    else if(strcmp(text,"struct")==0)
                    {
                        addTk(STRUCT);
                    }
					else
                    {
                        tk = addTk(ID);
                        tk->text = text;
                    }
                }
                else if(isdigit(*pch)) //for INTEGER constant   QA-> Not negative integers allowed?
                {
                    int int_or_double_flag = IS_INT; // let number be integer initially
                    for(start=pch++; isdigit(*pch) || strchr(".-+eE", *pch); pch++)
                    {
                        if(*pch == '.' || *pch=='e' || *pch == 'E')
                            int_or_double_flag=IS_DOUBLE;
                    }
                    char *number = extract(start, pch);
                    if(int_or_double_flag==IS_INT)
                    {
                        tk = addTk(INT);
                        tk->i = atoi(number);
                    }
                    else if(int_or_double_flag==IS_DOUBLE)
                    {
                        tk = addTk(DOUBLE);
                        tk->d = atof(number);
                    }
                }
                else if(*pch == '\'') //for characters
                {
                    for(start=pch++; *pch != '\'' && *pch !='\0'; pch++) {}
                    //if(*pch == '\'' && pch-start == 3)
                    if(*pch == '\'' )
                    {
                        char *character = extract(start, pch);
                        tk = addTk(CHAR);
                        tk->c = character[1]; //ignore first " ' "
                        //get over it
                        pch++;
                    }
                    else
                        error("Found initial \"'\" but no ending \"'\" or not mathcing length for character detected\n");
                }
                else if(*pch == '\"')
                {
                    for(start=pch++; *pch != '\"'; pch++) {}
                    if(*pch == '\"') //final " character
                    {
                        char *string = extract(start, pch);
                        tk = addTk(STRING);
                        
                        //Remove " characters from begin and end of string
                        for(int i=0; i<strlen(string)-1; i++)
                            string[i]=string[i+1];
                        
                        string[strlen(string)-1]='\0';
                        tk->text=safeAlloc(strlen(string));
                        strcpy(tk->text, string);
                        pch++; //go over " to next char
                    }
                    else
                        error("Found initial\", but not final ending \" for string\n");
                }
				else error("Invalid character found: %c (%d)\n",*pch,*pch);
			}
		}
	}

void showTokens(const Token *tokens){
	for(const Token *tk=tokens;tk;tk=tk->next){
        //for ID and constants, the values should be printed too
        if(tk->code == ID)
            printf("%d %s : %s\n", tk->line, token_names[tk->code], tk->text);
        else if(tk->code == INT)
            printf("%d %s : %d\n", tk->line, token_names[tk->code], tk->i);
        else if(tk->code == DOUBLE)
            printf("%d %s : %f\n", tk->line, token_names[tk->code], tk->d);
        else if(tk->code == STRING)
            printf("%d %s : %s\n", tk->line, token_names[tk->code], tk->text);
        else if(tk->code == CHAR)
            printf("%d %s : %c\n", tk->line, token_names[tk->code], tk->c);
        else //if it is an IDentifier , also type it's name
            printf("%d %s\n",tk->line, token_names[tk->code]);
		}
}
