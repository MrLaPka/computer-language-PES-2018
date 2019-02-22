#include "stdafx.h"	
#include "Graphs.h"
#include "FST.h"
#include "LexAnalize.h"
#include "Error.h"


namespace LexAnalaize
{
	bool flagToCheck = false;//�������� ��� ���������� �����.
	bool checkIf = false;
	Graph graph[N_GRAPHS] =   // ����� ��� �.�.
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
	// ������ ����������� ������
	LEX toLexAnalize(In::IN &in, Log::LOG &log, Parm::PARM &newParm)
	{
		LEX Tables;
		Tables.Lextable = LT::Build(MB4);  // �������� ������� ������
		Tables.IDtable = IT::Build(TI_MAXSIZE);  // �������� ������� ���������������

		//"�����" ��� �������� ������� ������� � �� ���������
		int amountOfParameters = 0;
		int libFun = NULL;		
		int funcend = NULL;
		int mainFun = NULL;
		int specialFun = NULL;
		bool checkP = false;     //�������� ����������
		bool checkF = false;      //�������� �� �������
		char buf[255] = "";
		char mainBuf[255] = ""; 
		char customFunBuf[255] = "";
		char indBuf[255] = "";

		IT::IDDATATYPE typeOfId;
		IT::IDDATATYPE typeOfFunc; 


		for (int i = 0; i < In::SomeTokens::tokensSize; i++) // �������� �������� ������� ����
		{
			for (int j = 0; j < N_GRAPHS; j++)    // ���� ���������� ������� ��� �����	
			{
				FST::FST fstex(in.tokens[i].token, graph[j].graph);   // ������� �������
				if (FST::execute(fstex)) // ���� ������ �������� �� 
				{
					switch (graph[j].lexema)     // ���� ���������� ���� ��� �������
					{

					case LEX_MULT:   // ���� ������� �-���
					{
						mainFun++;   //�������� � 0 �� 1 , �� ��������� , ���������� ����� 1 �� �-���
						if (mainFun > 1)    // ���� ������� ����� 1-�� ������� �-���
							Log::writeError(log.stream, Error::GetError(113, in.tokens[i].line, NULL)); // ������ ������ � ������ ������

						strcpy_s(mainBuf, "m");
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line);   // ������ � ������� , � ��������� ������ ������ 
						LT::Add(Tables.Lextable, entrylt);    // ���������� ������� � ������� 
						break; //�����
					}
				
					case LEX_LIB:   // ���� ����������� ����������
					{
						libFun++;
						if (libFun > 1)    // ���� ������� ����� 1-�� ����������
							Log::writeError(log.stream, Error::GetError(403, in.tokens[i].line, NULL)); // ������ ������ � ������ ������

						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line);// ������ � ������� , � ��������� ������ ������ 
						LT::Add(Tables.Lextable, entrylt);// ���������� ������� � ������� 
						break; //�����
					}

					case LEX_STRCON:  // ������� ������� ������������
					{

						specialFun = 1;
						IT::Entry entryit("strcon", i, IT::STR, IT::F); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[1] = 'w';
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[2] = 'w';
						break;
					}



					case LEX_LENGTH: // ������� ������� ������� 
					{
						specialFun = 1;
						IT::Entry entryit("length", i, IT::INT, IT::F); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						break;
					}

					case LEX_ISEMPTY: // ������� ������� ������� 
					{
						specialFun = 1;
						IT::Entry entryit("isempty", i, IT::BOOL, IT::F); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].indid].amountOfParameters = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].indid].parms[0] = 'w';
						break;
					}


					case LEX_ID://������������
					{
						if (Tables.Lextable.table[i - 1].lexema != LEX_FUNCTION && IT::IsId(Tables.IDtable, in.tokens[i].token) == -1)
						{
							strncpy(buf, mainBuf, ID_MAXSIZE - 1);
							strcat_s(buf, in.tokens[i].token);
							strncpy(in.tokens[i].token, buf, ID_MAXSIZE - 1);
						}

						strncpy_s(indBuf, in.tokens[i].token, ID_MAXSIZE - 1); // ����������� ����� �������������
						strncpy_s(in.tokens[i].token, in.tokens[i].token, ID_MAXSIZE - 1); //����������� �����
						if (IT::IsId(Tables.IDtable, in.tokens[i].token) == -1) // ���� ������ ������������� �� ������ �������� 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // ���� ���������� �������� ���� ������� ��������
							{
								strncpy_s(customFunBuf, indBuf, ID_MAXSIZE - 1); // �������� ����������� �������������� � ����� ��������������
								strncpy_s(mainBuf, indBuf, ID_MAXSIZE - 1);

								typeOfFunc = typeOfId;     // ��������� ��� ��������������
								if (Tables.Lextable.table[i - 2].lexema == LEX_INTEGER) typeOfId = IT::INT;
								if (Tables.Lextable.table[i - 2].lexema == LEX_WORD) typeOfId = IT::STR;
								if (Tables.Lextable.table[i - 2].lexema == LEX_BOOL) typeOfId = IT::BOOL;
								IT::Entry entryit(customFunBuf, i, typeOfId, IT::F); // ������ ������� �� � ���������� ���� ��������� ������
								IT::Add(Tables.IDtable, entryit); // ��������� ��������� �������
								LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // ��������
																																				 // �������� ������� ������
								LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
								checkP = true; // ������� ����������
								if (in.tokens[i + 2].token[0] == LEX_RIGHTBRACKET) // ���� ������� ����� ���� ������� )
									checkP = false; // ��������� �����������
								checkF = true;
								break;

							}

							
							else if (Tables.Lextable.table[i - 1].lexema == LEX_INTEGER) //���� ���������� ������ ���� �������� ������ ���� ������ 
							{
								if (Tables.Lextable.table[i - 2].lexema == LEX_DECLARE)//���� ���������� ������ ���� type
								{
									if (Tables.Lextable.table[i - 2].lexema == LEX_SP)//���� ���������� ������ ���� /*+-(){}...
									{
										std::cout << Tables.Lextable.table[i - 2].lexema << std::endl;
										Log::writeError(log.stream, Error::GetError(108, in.tokens[i].line, NULL)); // ���� �� �������� ��� ������ � ��� ���
										flagToCheck = true;																		  //  ������ ���������������� �����������
									}
								}
								if (checkP == false) // ���� ������������� ���� ���������� �� �������� ����������
								{
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::V); // ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit); //������� � ���� ���
								}
								else if (checkP == true) // ����� ���� ��������
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
									IT::Entry entryit(in.tokens[i].token, i, typeOfId, IT::P);// ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit);//������� � ���� ���
																	 //
									if (in.tokens[i + 1].token[0] == LEX_RIGHTBRACKET) // ���� ��������� ������� ������� )
									{
										checkP = false; // ��������� �����������
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].amountOfParameters = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, IT::IsId(Tables.IDtable, in.tokens[i].token)); // �������� �������� ���� ����
								LT::Add(Tables.Lextable, entrylt); // ���������� � ���� ����
								break;
							}//��� ����� � string � bool
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
						IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
						IT::Add(Tables.IDtable, entryit); //���������� � �������
						IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // ������������ �������� ��������
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
						LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
						break;
					}

					case LEX_ELSE:
					{
						if (!checkIf) {
							Log::writeError(log.stream, Error::GetError(116, in.tokens[i + 1].line, NULL));
						}
						else {
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
						}
						break;
					}

					case LEX_LITERAL: // ������� ��������
					{
						switch (Tables.Lextable.table[i - 1].lexema) // �������� ���������� �������
						{
						case LEX_EQUAL: // ������� =
						{
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::F) // ���� ���������� ���� ���� �-���
							{
								Log::writeError(log.stream, Error::GetError(115, in.tokens[i].line, NULL)); // ������ !!!  ������ ����������� �������� �-���
								flagToCheck = true;
							}
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::V ||    //���� ����� = ���� ���������� ��� ��������
								Tables.IDtable.table[IT::IsId(Tables.IDtable, in.tokens[i - 2].token)].idtype == IT::P)
							{
								IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, in.tokens[i - 2].token), in.tokens[i].token); // ������������� �� �� �������� � ���� ���� ���������������
							}
							break;
						}

						
						case LEX_LEFTBRACKET: 
						case LEX_COMMA: 
						case LEX_OUT:  
						case LEX_RETURN:
						{
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
							break;
						}

						default:
							IT::Entry entryit(LIT, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, in.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, in.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
						}
						break;
					}

					case LEX_SP: // ������� �����������
					{
						if (Tables.Lextable.table[i - 1].lexema == LEX_SP)
							Error::GetError(605, in.tokens[i].line, NULL);
						switch (in.tokens[i].token[0])
						{
						case LEX_SEPARATOR:
						{
							if (Tables.Lextable.table[i - 2].lexema == LEX_RETURN) // ����  ��������� �� 2 ���� ����� � ����� ������� return
							{
								int qwe = i; int qwe1 = i + 1;
								while (Tables.IDtable.table[qwe].idtype != IT::F)
									qwe--;
								if (Tables.IDtable.table[qwe].iddatatype != Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype)
								{
									Log::writeError(log.stream, Error::GetError(405, in.tokens[i].line, NULL));  // ������������ �������� �� ��������� � ����� �������
									flagToCheck = true;
								}
								kol = 0;
								IT::IDDATATYPE retType; // �������� ���������� �������� �-���
								if (Tables.Lextable.table[i - 1].lexema == LEX_LITERAL) // ���� ���������� �������
									retType = Tables.IDtable.table[Tables.IDtable.size - 1].iddatatype; // ������ ������� � ���� ����� �-��� ��������
							}

						}

						default:
						{
							LT::Entry entrylt(in.tokens[i].token[0], in.tokens[i].line); // ������ ������� ���� ������
							LT::Add(Tables.Lextable, entrylt); // ��������� �������� � ������� ������
						}
						}
						break;
					}
					case LEX_WORD: // ������� ���� ������
					case LEX_INTEGER: // ������� ���� ������
					case LEX_BOOL: // ������� ���� ������
						typeOfId = graph[j].graph.type; // ���������� ���� ���� ������
					default:
					{
						LT::Entry entrylt(graph[j].lexema, in.tokens[i].line); // �������� ��. ���� ����.
						LT::Add(Tables.Lextable, entrylt); // ��������� � ����. ����
						break;
					}
					}
					break;
				}
			}
		}
		if (!mainFun) // ���� ������ �� �-���
		{
			flagToCheck = true;
			Log::writeError(log.stream, Error::GetError(400)); // ������ ���������� ������� �-���
		}
		if (libFun == 0 && specialFun == 1)
			Log::writeError(log.stream, Error::GetError(402)); // �� ���������� ����������
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
			for (int i = 0; i < Tables.Lextable.size; i++) // �������� �������� � �� ����������� �������
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
				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_ID && Tables.Lextable.table[i + 1].lexema != LEX_LEFTBRACKET) // ���� �� ������� �����, � �� ����� ����� �������������
				{
					if (Tables.Lextable.table[i + 1].lexema == LEX_MINUS)
						Log::writeError(log.stream, Error::GetError(118, in.tokens[i + 1].line, NULL));
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::STR) // � ���� ������������� ���������� ���� 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype != IT::STR) // � ����� ����� ������������� �� ������ ���� ����
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEPARATOR && Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].idtype != IT::F) // ��� �� �������
							Log::writeError(log.stream, Error::GetError(404, in.tokens[i + 2].line, NULL));
					}

					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::BOOL)
					{
						if ((Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::STR)|| (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::INT)) // � ����� ����� ������������� �� ������ ���� BOOL
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEPARATOR && Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].idtype != IT::F) // ��� �� �������
							Log::writeError(log.stream, Error::GetError(408, in.tokens[i + 2].line, NULL));
					}

					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].indid].iddatatype == IT::INT) // � ���� ������������� �������������� ���� 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::STR) //���� ��������� ���������� ����, �� ������
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].indid].iddatatype == IT::BOOL) //���� ��������� ���������� ����, �� ������
							Log::writeError(log.stream, Error::GetError(401, in.tokens[i + 1].line, NULL));
						int j = i, k = i;
						while (Tables.Lextable.table[j].lexema != LEX_SEPARATOR) //���� ;
						{
							if (Tables.Lextable.table[j].lexema == LEX_TO_CLOSE_BLOCK)//���� ������ ����� ����������� �����������, �� ������
								Log::writeError(log.stream, Error::GetError(119, in.tokens[i + 1].line, NULL));
							j++;
						}
						bool param = false;
						if (Tables.Lextable.table[j].lexema == LEX_SEPARATOR) // ���� ����� ;
						{
							while (k != j) // ��������� ���� �������� ���������
							{

								if (Tables.Lextable.table[k].lexema == LEX_RIGHTBRACKET) // ���� ��� ���� ��������� �������
									param = false;
								if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL) // ���� ��� �������������
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