#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

Token *tokens;	// single linked list of tokens
Token *lastTk;		// the last token in list

static const char *token_names[]={
    //keywords
    [ID]="ID",
    [TYPE_CHAR]="TYPE_CHAR",
    [TYPE_INT]="TYPE_INT",
    [TYPE_DOUBLE]="TYPE_DOUBLE",
    [IF]="IF",
    [ELSE]="ELSE",
    [STRUCT]="STRUCT",
    [VOID]="VOID",
    
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
    [GREATEREQ]="GREATEREQ"
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
        error("Negative length identifier found! Check for bugs");
    char *p = safeAlloc(length * sizeof(char));
    strncpy(p, begin, end-begin);
    p[strlen(p)]='\0';
    return p;
    }

Token *tokenize(const char *pch){
	const char *start;
	Token *tk;
    int last_type_detected=-1;
	for(;;){
		switch(*pch){
			case ' ':case '\t':pch++;break;
			case '\r':		// handles different kinds of newlines (Windows: \r\n, Linux: \n, MacOS, OS X: \r or \n)
				if(pch[1]=='\n')pch++;
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
				if(pch[1]=='='){
					addTk(EQUAL);
					pch+=2;
					}else{
					addTk(ASSIGN);
					pch++;
					}
				break;
            case '+':addTk(ADD); pch++; break;
            case '-':addTk(SUB); pch++; break;
            case '*':addTk(MUL); pch++; break;
            case '/':addTk(DIV); pch++; break;
            case '.':addTk(DOT); pch++; break;
            case '&':
                if(pch[1]=='&')
                {
                    addTk(AND);
                    pch+=2;
                }
                else
                    error("Expected: \"&&\", but found \"&\""); // QA ->TBD
                break;
            case '|':
                if(pch[1]=='|')
                {
                    addTk(OR);
                    pch+=2;
                }
                else
                    error("Expected: \"||\", but found \"|\""); // QA ->TBD
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
					for(start=pch++;isalnum(*pch)||*pch=='_';pch++){}
					char *text=extract(start,pch);
					if(strcmp(text,"char")==0)
                    {
                        addTk(TYPE_CHAR);
                        last_type_detected=lastTk->code;
                    }
                    else if(strcmp(text,"int")==0)
                    {
                        addTk(TYPE_INT);
                        last_type_detected=lastTk->code;
                    }
                    else if(strcmp(text,"double")==0)
                    {
                        addTk(TYPE_DOUBLE);
                        last_type_detected=lastTk->code;
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
                        tk=addTk(ID);
                        tk->text=text;
                    }
                }
				else error("invalid char: %c (%d)",*pch,*pch);
			}
		}
	}

void showTokens(const Token *tokens){
	for(const Token *tk=tokens;tk;tk=tk->next){
        if(tk->code != ID)
            printf("%d %s\n",tk->line, token_names[tk->code]);
        else //if it is an IDentifier , also type it's name
            printf("%d %s:%s\n", tk->line, token_names[tk->code], tk->text);
		}
	}
