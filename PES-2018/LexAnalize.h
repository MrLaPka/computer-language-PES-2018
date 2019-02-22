#pragma once

#include "stdafx.h"
#include "IT.h"
#include "LT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"

static int kol;

namespace LexAnalaize
{
	struct LEX
	{
		LT::LexTable Lextable;
		IT::IdTable	IDtable;
	};

	struct Graph
	{
		char lexema;
		FST::FST graph;
	};

	LEX toLexAnalize(In::IN &in, Log::LOG &log, Parm::PARM &newParm);
}
