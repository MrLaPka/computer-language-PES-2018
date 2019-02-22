#include "stdafx.h"
#include "GRB.h"
//синтакс
#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NL('S'), TS('@'), 7,
		Rule(NL('S'), GRB_ERROR_SERIES, 7,  
			Rule::Chain(10, TS('w'), TS('f'), TS('i'), TS('('), NL('F'), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(10, TS('n'), TS('f'), TS('i'), TS('('), NL('F'), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(10, TS('a'), TS('f'), TS('i'), TS('('), NL('F'), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(9, TS('w'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(9, TS('n'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(9, TS('a'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NL('N'), TS('}'), NL('S')),
			Rule::Chain(4, TS('s'), TS('{'), NL('N'), TS('}'))
		),
		Rule(NL('F'), GRB_ERROR_SERIES + 1, 6, 
			Rule::Chain(4, TS('w'), TS('i'), TS(','), NL('F')),
			Rule::Chain(2, TS('w'), TS('i')),
			Rule::Chain(4, TS('n'), TS('i'), TS(','), NL('F')),
			Rule::Chain(2, TS('n'), TS('i')),
			Rule::Chain(4, TS('a'), TS('i'), TS(','), NL('F')),
			Rule::Chain(2, TS('a'), TS('i'))
		),
		Rule(NL('N'), GRB_ERROR_SERIES + 2, 32,
			Rule::Chain(5, TS('c'), TS('a'), TS('i'), TS(';'), NL('N')),
			Rule::Chain(5, TS('c'), TS('n'), TS('i'), TS(';'), NL('N')),
			Rule::Chain(5, TS('c'), TS('w'), TS('i'), TS(';'), NL('N')),
			Rule::Chain(3, TS('c'), TS('a'), NL('N')),
			Rule::Chain(3, TS('c'), TS('n'), NL('N')),
			Rule::Chain(3, TS('c'), TS('w'), NL('N')),
			Rule::Chain(5, TS('i'), TS('='), NL('E'), TS(';'), NL('N')),
			Rule::Chain(5, TS('i'), TS('('), NL('W'), TS(')'), TS(';')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(8, TS('d'), TS('('), NL('D'), TS(')'), TS('{'), NL('N'),TS('}'),NL('N')),
			Rule::Chain(7, TS('d'), TS('('), NL('D'), TS(')'), TS('{'), NL('N'), TS('}')),
			Rule::Chain(8, TS('d'), TS('('), NL('E'), TS(')'), TS('{'), NL('N'), TS('}'), NL('N')),
			Rule::Chain(7, TS('d'), TS('('), NL('E'), TS(')'), TS('{'), NL('N'), TS('}')),
			Rule::Chain(4, TS('e'),  TS('{'), NL('N'), TS('}')),
			Rule::Chain(5, TS('e'),  TS('{'), NL('N'), TS('}'), NL('N')),
			Rule::Chain(6, TS('o'), TS('('), NL('E'), TS(')'), TS(';'), NL('N')),
			Rule::Chain(3, TS('r'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), TS('l'), TS(';')),
			Rule::Chain(3, TS('r'), NL('D'), TS(';')),
			Rule::Chain(4, TS('c'), TS('a'), TS('i'), TS(';')),
			Rule::Chain(4, TS('c'), TS('n'), TS('i'), TS(';')),
			Rule::Chain(4, TS('c'), TS('w'), TS('i'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NL('E'), TS(';')),
			Rule::Chain(2, TS('z'),TS(';')),
			Rule::Chain(3, TS('z'), TS(';'),NL('N')),
			Rule::Chain(5, TS('o'), TS('('), NL('E'), TS(')'), TS(';')),
			Rule::Chain(6, TS('k'), TS('('), NL('W'), TS(')'), TS(';'), NL('N')),
			Rule::Chain(7, TS('k'), TS('('), NL('W'), TS(')'), NL('E'), TS(';'), NL('N')),
			Rule::Chain(6, TS('y'), TS('('), NL('W'), TS(')'), TS(';'), NL('N')),
			Rule::Chain(7, TS('y'), TS('('), NL('W'), TS(')'), NL('E'), TS(';'), NL('N')),
			Rule::Chain(6, TS('b'), TS('('), NL('W'), TS(')'), TS(';'), NL('N')),
			Rule::Chain(7, TS('b'), TS('('), NL('W'), TS(')'), NL('M'), TS(';'), NL('N'))

	),
		Rule(NL('E'), GRB_ERROR_SERIES + 3, 16, 
			Rule::Chain(1, TS('i')),
			Rule::Chain(2, TS('i'), NL('E')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('i'), NL('M')),
			Rule::Chain(2, TS('l'), NL('M')),

			Rule::Chain(3, TS('('), NL('W'), TS(')')),
			Rule::Chain(4, TS('('), NL('W'), TS(')'), NL('M')),
			Rule::Chain(4, TS('('), NL('E'), TS(')'), NL('M')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NL('M')),

			Rule::Chain(4, TS('k'), TS('('), NL('W'), TS(')')),
			Rule::Chain(5, TS('k'), TS('('), NL('W'), TS(')'), NL('E')),
			Rule::Chain(4, TS('y'), TS('('), NL('W'), TS(')')),
			Rule::Chain(5, TS('y'), TS('('), NL('W'), TS(')'), NL('E')),
			Rule::Chain(4, TS('b'), TS('('), NL('W'), TS(')')),
			Rule::Chain(5, TS('b'), TS('('), NL('W'), TS(')'), NL('M'))
		),
		Rule(NL('M'), GRB_ERROR_SERIES + 5, 3, 
			Rule::Chain(2, TS('+'), NL('E')),
			Rule::Chain(2, TS('-'), NL('E')),
			Rule::Chain(2, TS('*'), NL('E'))
		),
		Rule(NL('W'), GRB_ERROR_SERIES + 4, 4, 
			Rule::Chain(3, TS('i'), TS(','), NL('W')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('l'), TS(','), NL('W')),
			Rule::Chain(1, TS('l'))
		),
		Rule(NL('D'), GRB_ERROR_SERIES + 6, 12, //параметры iftrue
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('l'), TS('>'), NL('E')),
			Rule::Chain(3, TS('l'), TS('<'), NL('E')),
			Rule::Chain(3, TS('i'), TS('$'), NL('E')),
			Rule::Chain(4, TS('i'), TS('!'), TS('$'), NL('E')),
			Rule::Chain(3, TS('i'), TS('>'), NL('E')),
			Rule::Chain(3, TS('i'), TS('<'), NL('E')),
			Rule::Chain(2, TS('!'), TS('i')),
			Rule::Chain(4, TS('!'), TS('i'), TS('>'), NL('E')),
			Rule::Chain(4, TS('!'), TS('i'), TS('<'), NL('E')),
			Rule::Chain(4, TS('!'), TS('i'), TS('$'), NL('E')),
			Rule::Chain(5, TS('!'), TS('i'), TS('!'), TS('$'), NL('E'))
		)
	);

	Rule::Chain::Chain(short psize, short s, ...)  //конструктор цепочки - правой части правила(кол-во символов в цепочке, терминал или нетерминал...)
	{
		nt = new short[size = psize];    //цепочка терминалов
		int*p = (int*)&s;                      //присваиваем символ указателю п
		for (short i = 0; i < psize; ++i)
			nt[i] = (short)p[i]; //заполняем цепочку терминалов
	};

	Rule::Rule(short pnn, int piderror, short psize, Chain c, ...) //конструктор правила
	{															//(нетерминал, идентификатор диагностического сообщения, количество цепочек(правых частей правила), множество цепочек (правых частей правила)
		nn = pnn;    //нетерминал
		iderror = piderror; //идентификатор
		chains = new Chain[size = psize]; //место для цепочки
		Chain*p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i]; //заполняем множество цепочек
	};

	Greibach::Greibach(short pstartN, short pstbottom, short psize, Rule r, ...)//конструктор граматики Грейбаха(стартовый символ, дно стека, количество правил, правила...)
	{
		startN = pstartN; //стартовый символ
		stbottomT = pstbottom;//дно стека
		rules = new Rule[size = psize];//выделяем память
		Rule*p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];//заполняем правила
	};
	Greibach getGreibach() { return greibach; }; //получить грамматику

	short Greibach::getRule(short pnn, Rule& prule) //получить правило (левый символ правила, возвращаемое правило грамматики)
	{
		short rc = -1;
		short k = 0;
		while (k < size&&rules[k].nn != pnn)
			k++;   //пока К меньше количества правил и пока левый символ правила не равен парметру ф-ции
		if (k < size)
			prule = rules[rc = k];    //возвращаемое правило граматики равно правилу с индексом К
		return rc; //возвращается номер правила или -1
	};

	Rule Greibach::getRule(short n) //получить правило по номеру
	{
		Rule rc;      //создаём правило рц
		if (n < size)rc = rules[n];  //присваиваем правилу рц правило н
		return rc; //возвращаем созданное правило
	};

	char*Rule::getCRule(char*b, short nchain) //получить правило в виде N->цепочка (буфер, номер цепочки(правой части) в правиле)
	{
		char bchain[200]; //строка
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00; //нетерминал -> 
		chains[nchain].getCChain(bchain); //получает правую сторонц правила
		strcat_s(b, sizeof(bchain) + 5, bchain);//добавляем строку (куда, с какого элемента, строку)
		return b;
	};

	short Rule::getNextChain(short t, Rule::Chain& pchain, short j) //получить следующую за j подходящую цепочку, вернуть её номер или -1 
	{                                                                //(первый символ цепочки, возвращаемая цепочка, номер цепочки)
		short rc = -1;
		while (j < size&&chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};

	char*Rule::Chain::getCChain(char*b) //получить правую сторону правила
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};