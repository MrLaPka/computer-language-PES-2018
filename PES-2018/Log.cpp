#include "stdafx.h"
#include "Parm.h"
#include "Log.h"
#include "Error.h"
#include "In.h"


namespace Log
{

	LOG getstream(wchar_t  logfile[])//на открытие -in файла
	{
		LOG stream;
		stream.stream = new std::ofstream;
		(*stream.stream).open(logfile);
		if (!(*stream.stream).is_open())
			throw ERROR_THROW(110);
		wcscpy_s(stream.logfile, logfile);
		return stream;
	}

	void writeLog(LOG &log) {//протокол
		char buf[80];
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		char* format = "%d.%m.%Y %H:%M:%S";
		strftime(buf, 80, format, timeinfo);
		*log.stream << "\n----- Протокол ------ Дата: " << buf << " -------- \n\n";
	}

	void writeParm(LOG &log, Parm::PARM &newParm)//параметры
	{
		char inTxt[PARM_MAX_SIZE],
			ITTxt[PARM_MAX_SIZE],
			LTTxt[PARM_MAX_SIZE],
			logTxt[PARM_MAX_SIZE];
		wcstombs(inTxt, newParm.in, wcslen(newParm.in) + 1);
		wcstombs(logTxt, newParm.log, wcslen(newParm.log) + 1);
		wcstombs(ITTxt, newParm.lt, wcslen(newParm.lt) + 1);
		wcstombs(LTTxt, newParm.it, wcslen(newParm.it) + 1);
		*log.stream << "\n----- Параметры --------";
		*log.stream << "\n-in: " << inTxt
			<< "\n-log: " << logTxt
			<< "\n-IT " << ITTxt
			<< "\n-LT " << LTTxt;
	}

	void writeIn(LOG &log, In::IN &in) {//исходные данные
		*log.stream << "\n---- Исходные данные ------";
		*log.stream << "\nКоличество символов: " << std::setw(3) << in.size
			<< "\nПроигнорировано: " << std::setw(3) << in.ignor
			<< "\nКоличество строк: " << std::setw(3) << in.lines << "\n\n";
	}

	void writeError(std::ofstream *stream, Error::ERROR &e)//ошибки
	{
		lack = true;
		Error = false;

		if (stream == NULL)  // Пустой указатель
		{
			if (e.position.cells == -1 || e.position.line == -1)
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message << std::endl;
			else if (e.position.cells == NULL)
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line << std::endl;
			else
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line
				<< " Позиция в строке: " << e.position.cells << std::endl;
			system("pause");
		}
		else
		{
			if (e.position.cells == -1 || e.position.line == -1)
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message << std::endl;
			else if (e.position.cells == NULL)
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line << std::endl;
			else
				std::cout << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line
				<< " Позиция в строке: " << e.position.cells << std::endl;

			if (e.position.cells == -1 || e.position.line == -1)
				*stream << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message;
			else if (e.position.cells == NULL)
				*stream << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line;
			else
				*stream << std::endl << "Номер ошибки: " << e.id << " Сообщение ошибки: " << e.message
				<< " Строка: " << e.position.line
				<< " Позиция в строке: " << e.position.cells;
		}
	}

	void writeLexTable(LOG &log, LT::LexTable &Lextable)//Таблица лексем
	{

		*log.stream << "\n---------------Таблица лексем--------------------------------\n" <<
			std::setw(8) << std::left << "Лексема " <<
			std::setw(9) << std::left << "№ строки " <<
			std::setw(15) << std::left << "№ табл. лексем " <<
			std::setw(15) << std::left << "№ табл. идент. " << std::endl;

		for (int i = 0; i < Lextable.size; i++)
		{
			*log.stream << std::setw(8) << std::left << Lextable.table[i].lexema <<
				std::setw(9) << std::left << Lextable.table[i].nofl;
			*log.stream << std::setw(15) << std::left << i;
			*log.stream << std::setw(15) << std::left;
			if (Lextable.table[i].indid != -1)
				*log.stream << Lextable.table[i].indid;
			else
				*log.stream << '-';
			*log.stream << std::setw(8) << std::left;
			*log.stream << std::endl;
		}
		*log.stream << "\n\n";
	}

	void writeIDtable(LOG &log, IT::IdTable &IDtable)//Таблица идентификаторов
	{
		*log.stream << "\n---------------Таблица идентификаторов-------------------------------\n" <<
			std::setw(19) << std::left << "Имя идентификатора " <<
			std::setw(10) << std::left << "Тип даных " <<
			std::setw(11) << std::left << "Тип идентификатора " <<
			std::setw(15) << std::left << "№ табл. лекс. " <<
			std::setw(8) << std::left << "Значение" <<
			std::setw(20) << std::left << "	  К-во параметров(для функций) " <<
			std::setw(20) << std::left << "	  Типы параметров(для функций) " <<
			std::endl;

		for (int i = 0; i < IDtable.size; i++)
		{
			*log.stream << std::setw(10) << std::left <<
				std::setw(19) << std::left << IDtable.table[i].id <<
				std::setw(10) << std::left;

			if (IDtable.table[i].iddatatype == IT::INT)
				*log.stream << INTEGER;
			else if (IDtable.table[i].iddatatype == IT::STR)
				*log.stream << STRING;
			else
			{
				*log.stream << BOOLEAN;
			}

			*log.stream << std::setw(11) << std::left;
			switch (IDtable.table[i].idtype)
			{
			case IT::F:
				*log.stream << "Функция             "; break;
			case IT::V:
				*log.stream << "Переменная          "; break;
			case IT::P:
				*log.stream << "Параметр            "; break;
			case IT::L:
				*log.stream << "Литерал             "; break;
			case IT::S:
				*log.stream << "Стандартная функция "; break;
			}
			*log.stream << std::setw(15) << std::left << IDtable.table[i].idxfirstLE;
			if (IDtable.table[i].iddatatype == IT::INT)
			{
				*log.stream << IDtable.table[i].value.vNUM;
			}
			else if (IDtable.table[i].iddatatype == IT::BOOL) {
				if(IDtable.table[i].value.vBOOL == true)
				*log.stream << "true";
				else
					*log.stream << "false";
			}

			else
			{
				if (IDtable.table[i].value.vSTR.len != 0)
					*log.stream << IDtable.table[i].value.vSTR.str;
				else
					*log.stream << "NULL";
			}
			if (IDtable.table[i].idtype == IT::F && IDtable.table[i].amountOfParameters > 0)
			{
				*log.stream << "	  " << IDtable.table[i].amountOfParameters << "				  ";
				for (int j = 0; j < INT_MAXSIZE_PARM; j++)
				{
					if (IDtable.table[i].parms[j] == 'n')
					{
						*log.stream << "Integer ";
					}
					if (IDtable.table[i].parms[j] == 'w')
					{
						*log.stream << "String ";
					}
					if (IDtable.table[i].parms[j] == 'a')
					{
						*log.stream << "Bool ";
					}
				}
			}
			if (IDtable.table[i].idtype == IT::S && IDtable.table[i].amountOfParameters > 0)
			{
				*log.stream << "	  " << IDtable.table[i].amountOfParameters << "				  ";
				for (int j = 0; j < INT_MAXSIZE_PARM; j++)
				{
					if (IDtable.table[i].parms[j] == 'n')
					{
						*log.stream << "Integer ";
					}
					if (IDtable.table[i].parms[j] == 'w')
					{
						*log.stream << "String ";
					}
					if (IDtable.table[i].parms[j] == 'a')
					{
						*log.stream << "Bool ";
					}
				}
			}
			else if (IDtable.table[i].idtype == IT::F && IDtable.table[i].amountOfParameters < 0)
			{
				*log.stream << "	  0		  		  0";

			}
			else if (IDtable.table[i].idtype == IT::S && IDtable.table[i].amountOfParameters < 0)
			{
				*log.stream << "	  0   			 	  0";
			}
			*log.stream << std::endl;
		}
		*log.stream << "\n\n";
	}

	void writeLexTableConsole(LOG &log, LT::LexTable &Lextable)//таблица лек

	{

		std::cout << "\n---------------Таблица лексем--------------------------------\n" <<
			std::setw(8) << std::left << "Лексема " <<
			std::setw(9) << std::left << "№ строки " <<
			std::setw(15) << std::left << "№ табл. лексем " <<
			std::setw(15) << std::left << "№ табл. идент. " << std::endl;

		for (int i = 0; i < Lextable.size; i++)
		{
			std::cout << std::setw(8) << std::left << Lextable.table[i].lexema <<
				std::setw(9) << std::left << Lextable.table[i].nofl;
			std::cout << std::setw(15) << std::left << i;
			std::cout << std::setw(15) << std::left;
			if (Lextable.table[i].indid != -1)
				std::cout << Lextable.table[i].indid;
			else
				std::cout << '-';
			std::cout << std::setw(8) << std::left;
			std::cout << std::endl;
		}
		std::cout << "\n\n";
	}

	void writeIDTableConsole(LOG &log, IT::IdTable &IDtable)//таблица идентификаторов
	{
		std::cout << "\n---------------Таблица идентификаторов-------------------------------\n" <<
			std::setw(19) << std::left << "Имя идентификатора " <<
			std::setw(10) << std::left << "Тип даных " <<
			std::setw(11) << std::left << "Тип идентификатора " <<
			std::setw(15) << std::left << "№ табл. лекс. " <<
			std::setw(8) << std::left << "Значение" <<
			std::endl;

		for (int i = 0; i < IDtable.size; i++)
		{
			std::cout << std::setw(19) << std::left <<
				std::setw(19) << std::left << IDtable.table[i].id <<
				std::setw(10) << std::left;

			if (IDtable.table[i].iddatatype == IT::INT)
				std::cout << INTEGER;
			else if (IDtable.table[i].iddatatype == IT::BOOL)

				std::cout << BOOLEAN;
			else
				std::cout << STRING;

			std::cout << std::setw(19) << std::left << std::setw(19) << std::left;
			switch (IDtable.table[i].idtype)
			{
			case IT::F:
				std::cout << "Функция"; break;
			case IT::V:
				std::cout << "Переменная"; break;
			case IT::P:
				std::cout << "Параметр"; break;
			case IT::L:
				std::cout << "Литерал"; break;
			case IT::S:
				std::cout << "Стандартная функция"; break;
			}
			std::cout << std::setw(15) << std::left << IDtable.table[i].idxfirstLE;
			if (IDtable.table[i].iddatatype == IT::INT)
				std::cout << IDtable.table[i].value.vNUM;
			else if (IDtable.table[i].iddatatype == IT::BOOL) {
				std::cout << IDtable.table[i].value.vBOOL;
			}
			else
			{
				if (IDtable.table[i].value.vSTR.len != 0)
					std::cout << IDtable.table[i].value.vSTR.str;
				else
					std::cout << "NULL";
			}
			std::cout << std::endl;
		}
		std::cout << "\n\n";
	}

	void writeIntermediateCode(LOG &log, LT::LexTable &Lextable)//польск нотация
	{
		int pb = NULL;
		char*buf = new char[1024];
		*log.stream << "\n---------Промежуточный код------------------------------\n";
		for (int i = 0; i < Lextable.size; i++)
		{
			buf[pb] = Lextable.table[i].lexema;
			pb++;
			if (Lextable.table[i + 1].nofl != Lextable.table[i].nofl)
			{
				buf[pb] = '\0';
				if (Lextable.table[i].nofl < 10)
				{
					*log.stream << '0';
				}

				*log.stream << Lextable.table[i].nofl << ' ';
				*log.stream << buf << std::endl;
				pb = NULL;
			}
		}
	}

	void Close(LOG &log)
	{
		(*log.stream).close();
	};
}