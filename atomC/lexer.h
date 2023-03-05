#pragma once

enum{
	ID
	// keywords
	,TYPE_CHAR, TYPE_INT, TYPE_DOUBLE, IF, ELSE, RETURN, STRUCT, VOID
	// delimiters
	,COMMA,END, SEMICOLON, RPAR, LPAR, LBRACKET, RBRACKET, LACC, RACC
	// operators
	,ASSIGN,EQUAL, ADD, SUB, MUL, DIV, DOT, AND, OR, NOT, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ
	};

typedef struct Token{
	int code;		// ID, TYPE_CHAR, ...
	int line;		// the line from the input file
	union{
		char *text;		// the chars for ID, STRING (dynamically allocated)
		int i;		// the value for INT
		char c;		// the value for CHAR
		double d;		// the value for DOUBLE
		};
	struct Token *next;		// next token in a simple linked list
	}Token;

Token *tokenize(const char *pch);
void showTokens(const Token *tokens);
