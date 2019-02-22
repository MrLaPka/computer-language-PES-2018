#pragma once
#include "stdafx.h"
#define	SIZE_OF_LEX   1	        //количество символов в лексеме
#define	MB4		 4096	    //максимальный размер таблицы лексем
#define	NOT_NUMBER	 0xffffffff	//нет элемента таблицы идентификаторов
//символы в таблице лексем
#define LEX_SP	             'S'
#define	LEX_INTEGER          'n'
#define	LEX_WORD             'w'
#define	LEX_BOOL             'a'
#define	LEX_ID			     'i'			
#define	LEX_LITERAL		     'l'	
#define	LEX_FUNCTION         'f'
#define	LEX_IF               'd'
#define	LEX_ELSE             'e'
#define	LEX_MULT		     's'			
#define	LEX_RETURN		     'r'			
#define	LEX_OUT		         'o'	
#define LEX_DECLARE          'c'
#define	LEX_SEPARATOR	     ';'
#define	LEX_DOLLAR           '$'
#define	LEX_COMMA		     ','			
#define	LEX_TO_OPEN_BLOCK	 '{'			
#define	LEX_TO_CLOSE_BLOCK	 '}'			
#define	LEX_LEFTBRACKET	     '('			
#define	LEX_RIGHTBRACKET     ')'
#define	LEX_PLUS		     '+'	
#define	LEX_MINUS		     '-'
#define LEX_STRCON		     'k'
#define LEX_LENGTH		     'b'
#define LEX_ISEMPTY		     'y'
#define	LEX_MUL		         '*'					
#define	LEX_EQUAL		     '='
#define	LEX_NOT		         '!'
#define	LEX_BIGGER           '>'
#define	LEX_LESS             '<'
#define	LEX_OP	             'v'
#define LEX_LIB			     'z'
#define	LIT			      "literal"
#define	INTEGER			  "integer"		
#define	STRING			  "string"
#define	BOOLEAN			  "bool"
#define SPACE               ' '
#define POINT               '.'
namespace LT //таблица лексем
{
	struct Entry                       //содержит строки т.л.
	{
		char lexema;					//лексема
		int nofl;							//номер строки в исходном коде
		int indid;						//или индекс в таблице идентификаторов или если его нет, то NOT_NUMBER
		Entry();
		Entry(char lexema, int nofl, int indid = NOT_NUMBER);
	};

	struct LexTable						//экземпл€р таблицы лексем
	{
		int maxSize;					//максимальный размер(емкость) т.л.
		int size;						//размер таблицы лексем в данный момент
		Entry* table;					//строки таблицы лексем
	};

	LexTable Build(int size);		            //Ємкость должна быть < MB4
	void Add(LexTable &lextable, Entry entry);	//добавл€ет новую таблицу лексем
}