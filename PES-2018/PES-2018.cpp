// PES-2018.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "In.h"
#include "LexAnalize.h"
#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "IT.h"
#include "LT.h"
#include "FST.h"
#include "Generation.h"
#include "MFST.h"
#include "GRB.h"
#include "PN.h"

int _tmain(int argc, wchar_t *argv[])
{
	setlocale(LC_CTYPE, "Russian");
	Log::LOG log;
	try
	{

		Parm::PARM newParm = Parm::getparm(argc, argv); 
		log = Log::getstream(newParm.log);
		Log::writeLog(log); 
		Log::writeParm(log, newParm);
		In::IN in = In::getin(log.stream, newParm.in); // распознование вход. символов
		in.tokens = In::parsOfToken(in.tokens, in.text, in.code);// формирование токенов
		LexAnalaize::LEX launchAnalaize = LexAnalaize::toLexAnalize(in, log, newParm);// запуск лексического анализа
		MFST::Mfst mfst(launchAnalaize, GRB::getGreibach());

		if (Log::Error)
		{
			Log::writeIn(log, in);
			Log::writeLexTable(log, launchAnalaize.Lextable);
			Log::writeIDtable(log, launchAnalaize.IDtable);
			if (newParm.ITtable == true)
			{
				writeIDTableConsole(log, launchAnalaize.IDtable);
			}
			if (newParm.LTtable == true)
			{
				Log::writeLexTableConsole(log, launchAnalaize.Lextable);
			}
			Log::writeIntermediateCode(log, launchAnalaize.Lextable); // вывод в файл промежуточного кода

			MFST_TRACE_START(log);
			mfst.start(log);//старт синтаксического анализа
			mfst.savededucation();
			mfst.printrules(log);//печать дерева разбора
		}

		if (!Log::lack)
		{
			Generation(launchAnalaize.Lextable, in.tokens);// запуск генерации
			PN polishNotation;
			if (polishNotation.ToDoPN(&launchAnalaize))
			{
				*log.stream << endl << endl << "PN";
				Log::writeIntermediateCode(log, launchAnalaize.Lextable);
			}
		}


		Log::Close(log);

		char* HTML = "Out.html";
		system(HTML);
	}

	catch (Error::ERROR e)
	{
		Log::writeError(log.stream, e); 
	}
}