#pragma once
#include "stdafx.h"
#define MAX_STRING 255
#define IN_MAX_LEN_TEXT 1024*1024
#define IN_MAX_STRING_CODE 1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_SPACE ' '
#define IN_CODE_NULL '\0'

/*
SEPR - ( ) * + - = , / { } @ ;
PROB - пробел и TAB
NL - новая строка
F - запрещённый
T - разрешённый
I - игнорируемый
L - '
*/

#define IN_CODE_TABLE {\
	IN::F,  IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::PROB,  IN::NL,   IN::F,   IN::F,   IN::I,   IN::F, IN::F,\
	IN::F,  IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,   IN::F,   IN::F,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::PROB, IN::SEPR, IN::F, IN::F, IN::SEPR, IN::F, IN::F, IN::L, IN::SEPR, IN::SEPR, IN::SEPR,  IN::SEPR, IN::SEPR, IN::SEPR, IN::T, IN::SEPR,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::F,    IN::SEPR, IN::SEPR,   IN::SEPR, IN::SEPR, IN::F,\
	IN::SEPR,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::F,   IN::F,   IN::F,   IN::F, IN::F,\
	IN::F,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::T,   IN::T,   IN::T,   IN::T, IN::T,\
	IN::T,  IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T,   IN::T,   IN::T,    IN::SEPR, IN::F,   IN::SEPR, IN::F, IN::F,\
	\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F,\
	IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F\
}  

namespace In
{
	struct SomeTokens
	{
		static int tokensSize;  // Количество фраз
		int  line; // строка в которой находится фраза
		char token[1024];

	};

	struct IN
	{
		enum { T = 1, F, I, NL, SEPR, L, PROB };
		int code[256] = IN_CODE_TABLE;
		int size = NULL;
		int lines = NULL;
		int ignor = NULL;
		bool flag = 0;
		unsigned char *text;
		SomeTokens *tokens;
	};

	IN getin(std::ofstream *stream, wchar_t Infile[]);
	SomeTokens* parsOfToken(SomeTokens *tokens, unsigned char *text, int *code);
	void addToken(SomeTokens *tokens, char* &buf, int &posbuf, int line);
}