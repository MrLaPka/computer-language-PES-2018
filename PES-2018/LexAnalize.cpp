#include "stdafx.h"	
#include "Graphs.h"
#include "FST.h"
#include "LexAnalize.h"
#include "Error.h"


namespace LexAnalaize
{
	bool flagToCheck = false;//проверка для выполнения прогр.
	bool checkIf = false;
	Graph graph[N_GRAPHS] =   // графы для к.а.
	{
		{ LEX_LIB, FST::FST(GRAPH_STD) },
		{ LEX_IF, FST::FST(GRAPH_IF) },
		{ LEX_ELSE, FST::FST(GRAPH_ELSE) },
		{ LEX_LITERAL, FST::FST(GRAPH_NUMBER_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_LITERAL, FST::FST(TRUE) },
		{ LEX_LITERAL, FST::FST(FALSE) },
		{ LEX_SP, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_DECLARE, FST::FST(GRAPH_DECLARE) },
		{ LEX_STRCON, FST::FST(GRAPH_STRCON) },
		{ LEX_LENGTH, FST::FST(GRAPH_LENGTH) },
		{ LEX_ISEMPTY, FST::FST(GRAPH_ISEMPTY) },
		{ LEX_MULT, FST::FST(GRAPH_START) },
		{ LEX_INTEGER, FST::FST(GRAPH_INTEGER) },
		{ LEX_WORD, FST::FST(GRAPH_STRING) },
		{ LEX_BOOL, FST::FST(GRAPH_BOOL) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_OUT, FST::FST(GRAPH_OUT) },
		{ LEX_ID, FST::FST(GRAPH_ID) }
	};
	// дальше лексический анализ
	LEX toLexAnalize(In::IN &in, Log::LOG &log, Parm::PARM &newParm)
	{
		LEX Tables;
		Tables.Lextable = LT::Build(MB4);  // создание таблицы лексем
		Tables.IDtable = IT::Build(TI_MAXSIZE);  // создание таблицы идентификаторов

		//"флаги" для проверки наличия функций и их парметров
		int amountOfParameters = 0;
		int libFun = NULL;		
		int funcend = NULL;
		int mainFun = NULL;
		int specialFun = NULL;
		bool checkP = false;     //проверка параметров
		bool checkF = false;      //проверка на функцию
		char buf[255] = "";
		char mainBuf[255] = ""; 
		char customFunBuf[255] = "";
		char indBuf[255] = "";

		IT::IDDATATYPE typeOfId;
		IT::IDDATATYPE typeOfFunc; 


		for (int i = 0; i < In::SomeTokens::tokensSize; i++) // начинаем просмотр массива слов
		{
			for (int j = 0; j < N_GRAPHS; j++)    // ищем подходящий автомат для слова	
			{
				FST::FST fstex(in.tokens[i].token, graph[j].graph);   // попытка разбора
				if (FST::execute(fstex)) // если разбор успешный то 
				{
					switch (graph[j].lexema)     // ищем подходящий кейз для лексемы
					{

					case LEX_MULT:   // если главная ф-ция
					{
						mainFun++;   //увеличим с 0 до 1 , всё правильно , необходима ровно 1 гл ф-ция
						if (mainFun > 1)    // если найдено более 1-ой главной ф-ции
							Log::writeError(log.stream, Error::GetError(113, in.tokens[i].line, NULL)); // делаем запись в журнал ошибок

						strcpy_s(mainBuf, "m");
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line);   // запись в таблицу , с фиксацией номера строки 
						LT::Add(Tables.Lextable, entrylt);    // добавление лексемы в таблицу 
						break; //выход
					}
				
					case LEX_LIB:   // если стандартная библиотека
					{
						libFun++;
						if (libFun > 1)    // если найдено более 1-ой библиотеки
							Log::writeError(log.stream, Error::GetError(403, in.tokens[i].line, NULL)); // делаем запись в журнал ошибок

						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line);// запись в таблицу , с фиксацией номера строки 
						LT::Add(Tables.Lextable, entrylt);// добавление лексемы в таблицу 
						break; //выход
					}

					case LEX_STRCON:  // лексема функции конкатенации
					{

						specialFun = 1;
						IT::Entry entryit("strcon", i, IT::STR, IT::F); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[1] = 'w';
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[2] = 'w';
						break;
					}



					case LEX_LENGTH: // лексема функции степень 
					{
						specialFun = 1;
						IT::Entry entryit("length", i, IT::INT, IT::F); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						break;
					}

					case LEX_ISEMPTY: // лексема функции степень 
					{
						specialFun = 1;
						IT::Entry entryit("isempty", i, IT::BOOL, IT::F); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						break;
					}


					case LEX_ID://идентификтор
					{
						if (Tables.Lextable.table[i - 1].lexema != LEX_FUNCTION && IT::IsId(Tables.IDtable, in.tokens[i].token) == -1)
						{
							strncpy(buf, mainBuf, ID_MAXSIZE - 1);
							strcat_s(buf, in.tokens[i].token);
							strncpy(in.tokens[i].token, buf, ID_MAXSIZE - 1);
						}

						strncpy_s(indBuf, in.tokens[i].token, ID_MAXSIZE - 1); // копирование имени идентифкатора
						strncpy_s(in.tokens[i].token, in.tokens[i].token, ID_MAXSIZE - 1); //копирование имени
						if (IT::IsId(Tables.IDtable, in.tokens[i].token) == -1) // если данный идентификатор не найден повторно 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // если предыдущей лексемой была лексема функциии
							{
								strncpy_s(customFunBuf, indBuf, ID_MAXSIZE - 1); // копируем обозначение принадлежности к имени идентификатора
								strncpy_s(mainBuf, indBuf, ID_MAXSIZE - 1);

								typeOfFunc = typeOfId;     // указываем тип идентификатора
								if (Tables.Lextable.table[i - 2].lexema == LEX_INTEGER) typeOfId = IT::INT;
								if (Tables.Lextable.table[i - 2].lexema == LEX_WORD) typeOfId = IT::STR;
								if (Tables.Lextable.table[i - 2].lexema == LEX_BOOL) typeOfId = IT::BOOL;
								IT::Entry entryit(customFunBuf, i, typeOfId, IT::F); // создаём элемент ТИ с занесением всех собранных данных
								IT::Add(Tables.IDtable, entryit); // добавляем созданный элемент
								LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // создание
																																				 // элемента таблицы лексем
								LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
								checkP = true; // наличие параметров
								if (in.tokens[i + 2].token[0] == LEX_RIGHTBRACKET) // если лексема через одну лексема )
									checkP = false; // параметры закончились
								checkF = true;
								break;

							}

							
							else if (Tables.Lextable.table[i - 1].lexema == LEX_INTEGER) //если предыдущая лесема была лексемой целого типа данных 
							{
								if (Tables.Lextable.table[i - 2].lexema == LEX_DECLARE)//если предыдущая лесема была type
								{
									if (Tables.Lextable.table[i - 2].lexema == LEX_SP)//если предыдущая лесема была /*+-(){}...
									{
										std::cout << Tables.Lextable.table[i - 2].lexema << std::endl;
										Log::writeError(log.stream, Error::GetError(108, in.tokens[i].line, NULL)); // если не сработал инф ошибка о том что
										flagToCheck = true;																		  //  небыло предварительного определения
									}
								}
								if (checkP == false) // если идентификатор типа переменная не является параметром
								{
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::V); // создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit); //заносим в табл инд
								}
								else if (checkP == true) // иначе если параметр
								{
									int qwe = i;
									while (Tables.IDtable.table[qwe].idtype != IT::F)
										qwe--;
									Tables.IDtable.table[qwe].parms[kol] = 'n';
									kol++;
									if (kol == INT_MAXSIZE_PARM)
									{
										Log::writeError(log.stream, Error::GetError(115, in.tokens[i].line, NULL));
										flagToCheck = true;
									}
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::P);// создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit);//заносим в табл инд
																	 //
									if (in.tokens[i + 1].token[0] == LEX_RIGHTBRACKET) // если следующая лексема лексема )
									{
										checkP = false; // параметры закончились
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].amountOfParameters = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // создание элемента табл лекс
								LT::Add(Tables.Lextable, entrylt); // добавление в табл лекс
								break;
							}//тож самое с string и bool
							else if (Tables.Lextable.table[i - 1].lexema == LEX_WORD) 
							{
								if (checkP == false) 
								{
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::V);
									IT::Add(Tables.IDtable, entryit); 
								}
								else if (checkP == true) 
								{
									int qwe = i;
									while (Tables.IDtable.table[qwe].idtype != IT::F)
										qwe--;
									Tables.IDtable.table[qwe].parms[kol] = 'w';
									kol++;
									if (kol == INT_MAXSIZE_PARM)
									{
										Log::writeError(log.stream, Error::GetError(103, in.tokens[i].line, NULL));
										flagToCheck = true;
									}
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::P);
									IT::Add(Tables.IDtable, entryit);
																	 
									if (in.tokens[i + 1].token[0] == LEX_RIGHTBRACKET) 
									{
										checkP = false; 
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].amountOfParameters = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); 
								LT::Add(Tables.Lextable, entrylt); 
								break;
							}

							else if (Tables.Lextable.table[i - 1].lexema == LEX_BOOL) 
							{
							if (checkP == false) 
							{
								IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::V); 
								IT::Add(Tables.IDtable, entryit); 
							}
							else if (checkP == true) 
							{
								int qwe = i;
								while (Tables.IDtable.table[qwe].idtype != IT::F)
									qwe--;
								Tables.IDtable.table[qwe].parms[kol] = 'a';
								kol++;
								if (kol == INT_MAXSIZE_PARM)
								{
									Log::writeError(log.stream, Error::GetError(103, in.tokens[i].line, NULL));
									flagToCheck = true;
								}
								IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::P);
								IT::Add(Tables.IDtable, entryit);
																 
								if (in.tokens[i + 1].token[0] == LEX_RIGHTBRACKET) 
								{
									checkP = false;
									int qwe = i;
									while (Tables.IDtable.table[qwe].idtype != IT::F)
										qwe--;
									Tables.IDtable.table[qwe].amountOfParameters = kol;
									kol = 0;
								}
							}

							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); 
							LT::Add(Tables.Lextable, entrylt); 
							break;
							}

							Log::writeError(log.stream, Error::GetError(108, in.tokens[i].line, NULL)); 
						}
						
						else 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_INTEGER || Tables.Lextable.table[i - 1].lexema == LEX_WORD || Tables.Lextable.table[i - 1].lexema == LEX_BOOL || Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION)
																																																	 
							{
								Log::writeError(log.stream, Error::GetError(107, in.tokens[i].line, NULL)); 
								flagToCheck = true;
							}
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); 
							LT::Add(Tables.Lextable, entrylt); 
							break;
						}
						break;
					}

					case LEX_IF:
					{
						checkIf = true;
						IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
						IT::Add(Tables.IDtable, entryit); //добавление в таблицу
						IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // установление значения литерала
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
						LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
						break;
					}

					case LEX_ELSE:
					{
						if (!checkIf) {
							Log::writeError(log.stream, Error::GetError(116, in.tokens[i + 1].line, NULL));
						}
						else {
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
						}
						break;
					}

					case LEX_LITERAL: // лексема литерала
					{
						switch (Tables.Lextable.table[i - 1].lexema) // проверим предыдущие лексемы
						{
						case LEX_EQUAL: // лексема =
						{
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::F) // если предыдущая была лекс ф-ции
							{
								Log::writeError(log.stream, Error::GetError(115, in.tokens[i].line, NULL)); // ошибка !!!  нельзя присваивать значение ф-ции
								flagToCheck = true;
							}
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::V ||    //если перед = была переменная или параметр
								Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::P)
							{
								IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, in.tokens[i - 2].token), in.tokens[i].token); // устанавливаем им их значение в поле табл идентификаторов
							}
							break;
						}

						
						case LEX_LEFTBRACKET: 
						case LEX_COMMA: 
						case LEX_OUT:  
						case LEX_RETURN:
						{
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
							break;
						}

						default:
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
						}
						break;
					}

					case LEX_SP: // лексема сепараторов
					{
						if (Tables.Lextable.table[i - 1].lexema == LEX_SP)
							Error::GetError(605, in.tokens[i].line, NULL);
						switch (in.tokens[i].token[0])
						{
						case LEX_SEPARATOR:
						{
							if (Tables.Lextable.table[i - 2].lexema == LEX_RETURN) // если  вернуться на 2 лекс назад и будет лексема return
							{
								int qwe = i; int qwe1 = i + 1;
								while (Tables.IDtable.table[qwe].idtype != IT::F)
									qwe--;
								if (Tables.IDtable.table[qwe].iddatatype != Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype)
								{
									Log::writeError(log.stream, Error::GetError(405, in.tokens[i].line, NULL));  // возвращаемое значение не совпадает с типом функции
									flagToCheck = true;
								}
								kol = 0;
								IT::IDDATATYPE retType; // созданим воращаемое значение ф-ции
								if (Tables.Lextable.table[i - 1].lexema == LEX_LITERAL) // если предыдущая литерал
									retType = Tables.IDtable.table[Tables.IDtable.size - 1].iddatatype; // данный литерал и есть возвр ф-ции значение
							}

						}

						default:
						{
							LT::Entry entrylt(in.tokens[i].token[0], in.tokens[i].line); // сохдаём элемент табл лексем
							LT::Add(Tables.Lextable, entrylt); // занесение элемента в таблицу лексем
						}
						}
						break;
					}
					case LEX_WORD: // лексема типа данных
					case LEX_INTEGER: // лексема типа данных
					case LEX_BOOL: // лексема типа данных
						typeOfId = graph[j].graph.type; // заполнение поля типа данных
					default:
					{
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line); // создание эл. табл лекс.
						LT::Add(Tables.Lextable, entrylt); // занесение в табл. лекс
						break;
					}
					}
					break;
				}
			}
		}
		if (!mainFun) // если небыло гл ф-ции
		{
			flagToCheck = true;
			Log::writeError(log.stream, Error::GetError(400)); // ошибка отсутствие главной ф-ции
		}
		if (libFun == 0 && specialFun == 1)
			Log::writeError(log.stream, Error::GetError(402)); // не подключена библиотека
		for (int i = 0; i < Tables.Lextable.size; i++)
		{
			if (Tables.Lextable.table[i].lexema == LEX_SP && Tables.Lextable.table[i - 1].lexema == LEX_SP)
			{
				flagToCheck = true;
				Log::writeError(log.stream, Error::GetError(106, in.tokens[i + 1].line, NULL));
			}
			else if (Tables.Lextable.table[i - 1].lexema == LEX_SP && Tables.Lextable.table[i - 2].lexema == LEX_SP)
			{

				flagToCheck = true;
				Log::writeError(log.stream, Error::GetError(106, in.tokens[i + 1].line, NULL));
			}
			else if (Tables.Lextable.table[i - 2].lexema == LEX_SP && Tables.Lextable.table[i - 3].lexema == LEX_SP)
			{
				flagToCheck = true;
				Log::writeError(log.stream, Error::GetError(106, in.tokens[i + 1].line, NULL));
			}
			else if (Tables.Lextable.table[i - 2].lexema == LEX_EQUAL && Tables.Lextable.table[i - 3].lexema == LEX_EQUAL)
			{
				flagToCheck = true;
				Log::writeError(log.stream, Error::GetError(106, in.tokens[i + 1].line, NULL));
			}
		}

		if (!flagToCheck)
		{
			std::cout << "Check Out.html "<<endl;
			for (int i = 0; i < Tables.Lextable.size; i++) // проверка выражний и не стандратных функций
			{
				if (Tables.Lextable.table[i].lexema == LEX_ID || Tables.Lextable.table[i].lexema == LEX_LITERAL)
				{
					if (Tables.IDtable.table[Tables.Lextable.table[i].indid].idtype == IT::F)
					{
						if (Tables.Lextable.table[i + 1].lexema != LEX_LEFTBRACKET)
							Log::writeError(log.stream, Error::GetError(409, in.tokens[i + 1].line, NULL));
					}
				}
				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_EQUAL)
					flagToCheck = true;
				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_ID && Tables.Lextable.table[i + 1].lexema != LEX_LEFTBRACKET) // если мы словили равно, а до равно стоит идентификатор
				{
					if (Tables.Lextable.table[i + 1].lexema == LEX_MINUS)
						Log::writeError(log.stream, Error::GetError(118, in.tokens[i + 1].line, NULL));
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::STR) // и этот идентификатор строкового типа 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype != IT::STR) // а после равно идентификатор не равный типу ворд
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEPARATOR && Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].idtype != IT::F) // или же функция
							Log::writeError(log.stream, Error::GetError(404, in.tokens[i + 2].line, NULL));
					}

					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::BOOL)
					{
						if ((Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::STR)|| (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::INT)) // а после равно идентификатор не равный типу BOOL
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEPARATOR && Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].idtype != IT::F) // или же функция
							Log::writeError(log.stream, Error::GetError(408, in.tokens[i + 2].line, NULL));
					}

					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::INT) // и этот идентификатор целочисленного типа 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::STR) //если следующий строкового типа, то ошибка
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::BOOL) //если следующий строкового типа, то ошибка
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						int j = i, k = i;
						while (Tables.Lextable.table[j].lexema != LEX_SEPARATOR) //ищем ;
						{
							if (Tables.Lextable.table[j].lexema == LEX_TO_CLOSE_BLOCK)//если раньше нашли закрывающуй конструкцию, то ошибка
								Log::writeError(log.stream, Error::GetError(119, in.tokens[i + 1].line, NULL));
							j++;
						}
						bool param = false;
						if (Tables.Lextable.table[j].lexema == LEX_SEPARATOR) // если нашли ;
						{
							while (k != j) // запускаем цикл проверки выражения
							{

								if (Tables.Lextable.table[k].lexema == LEX_RIGHTBRACKET) // если это были параметры функции
									param = false;
								if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL) // если это идентификатор
								{
									if (Tables.IDtable.table[Tables.Lextable.table[k].indid].idtype == IT::F)
									{
										if (Tables.Lextable.table[k + 1].lexema == LEX_LEFTBRACKET)
										{
											param = true;
										}
										else
										{
											Log::writeError(log.stream, Error::GetError(409, in.tokens[i + 1].line, NULL));
										}
									}
									else
									{
										if (Tables.IDtable.table[Tables.Lextable.table[k].indid].iddatatype == IT::STR && param == false && libFun == 0)
										{
											Log::writeError(log.stream, Error::GetError(402, in.tokens[i + 1].line, NULL));
										}
									}
								}
								k++;
							}
						}
					}
				}
			}
			bool flag = false; int some_count = 0; char some_buffer[INT_MAXSIZE_PARM];
			for (int i = 0; i < Tables.Lextable.size; i++)
			{

				if (Tables.Lextable.table[i].lexema == LEX_FUNCTION && flag == false && flagToCheck == false)
				{

					int j = i, funct = i;
					while (Tables.Lextable.table[j].lexema != LEX_RIGHTBRACKET)
						j++;
					int count = 0;
					for (int p = i; p != j; p++)
					{
						if (Tables.Lextable.table[p].lexema == LEX_INTEGER)
						{
							Tables.IDtable.table[Tables.Lextable.table[funct + 1].indid].parms[count] = 'n';
							count++;
						}

						if (Tables.Lextable.table[p].lexema == LEX_WORD)
						{
							Tables.IDtable.table[Tables.Lextable.table[funct + 1].indid].parms[count] = 'w';
							count++;
						}

						if (Tables.Lextable.table[p].lexema == LEX_BOOL)
						{
							Tables.IDtable.table[Tables.Lextable.table[funct + 1].indid].parms[count] = 'a';
							count++;
						}
					}
				}
				if (Tables.Lextable.table[i].lexema == LEX_MULT)
					flag = true;

				if (flag == true)
				{
					if (Tables.Lextable.table[i].lexema == LEX_ID || Tables.Lextable.table[i].lexema == LEX_LITERAL)
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i].indid].idtype == IT::F)
						{
							int param = 0;

							if (Tables.Lextable.table[i + 1].lexema == LEX_LEFTBRACKET)
							{
								int k = i + 1;
								while (Tables.Lextable.table[k].lexema != LEX_RIGHTBRACKET)
								{
									if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL)
									{
										if (Tables.IDtable.table[Tables.Lextable.table[k].indid].iddatatype == IT::STR)
											some_buffer[some_count] = 'w';
										if (Tables.IDtable.table[Tables.Lextable.table[k].indid].iddatatype == IT::INT)
											some_buffer[some_count] = 'n';
										if (Tables.IDtable.table[Tables.Lextable.table[k].indid].iddatatype == IT::BOOL)
											some_buffer[some_count] = 'a';
										some_count++;
									}
									k++;
								}
								if (Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters < 0)
									Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 0;
								if (some_count != Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters)
								{
									Log::writeError(log.stream, Error::GetError(406, in.tokens[i + 1].line, NULL));
								}
								for (int r = 0; r < some_count; r++)
									if (some_buffer[r] != Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[r])
									{
										Log::writeError(log.stream, Error::GetError(407, in.tokens[i + 1].line, NULL));
									}
								some_count = 0;
							}
						}
					}
				}
			}
		}
		return Tables;
	}
}