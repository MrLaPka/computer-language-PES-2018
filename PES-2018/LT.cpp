#include "stdafx.h"
#include "LT.h"
#include "Error.h"

namespace LT
{
	Entry::Entry()
	{
		this->lexema = NULL;
		this->nofl = NULL;
		this->indid = NOT_NUMBER;
	}

	Entry::Entry(char lexema, int nofl, int indid)
	{
		this->lexema = lexema;
		this->nofl = nofl;
		this->indid = indid;
	}

	LexTable Build(int size)
	{
		LexTable lextable;
		if (size <= MB4)
			lextable.maxSize = size;
		else
			throw ERROR_THROW(117);
		lextable.size = NULL;
		lextable.table = new Entry[size];
		return  lextable;
	}

	void Add(LexTable &lextable, Entry entry)
	{
		lextable.table[lextable.size].indid = entry.indid;
		lextable.table[lextable.size].lexema = entry.lexema;
		lextable.table[lextable.size].nofl = entry.nofl;

		if (lextable.size < MB4)
			lextable.size++;
		else
			throw ERROR_THROW(117);
	}
}