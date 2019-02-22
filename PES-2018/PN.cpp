#include "stdafx.h"
#include "PN.h"

int PN::priority(char operation)
{
	switch (operation)
	{
	case '+':
	case '-':
		return 2;
	case '*':
		return 3;
	default:
		return 0;
	}
}

int PN::search(int index, LexAnalaize::LEX *launchAnalaize) //поиск выражения
{
	for (int i = index; i < launchAnalaize->Lextable.size; i++) //перебор лексем
	{
		if (launchAnalaize->Lextable.table[i].lexema == LEX_PLUS || launchAnalaize->Lextable.table[i].lexema == LEX_MINUS
			|| launchAnalaize->Lextable.table[i].lexema == LEX_MUL ) //находим операцию
		{
			for (; ; i--) //перебираем лексемы обратно
			{
				if (launchAnalaize->Lextable.table[i].lexema == LEX_EQUAL || launchAnalaize->Lextable.table[i].lexema == LEX_RIGHTBRACKET)
					return i + 1;
			}
		}
	}
	return -1; //если не нашли возвращаем минус 1
}

bool PN::ToDoPN(LexAnalaize::LEX *launchAnalaize)
{
	std::stack <LT::Entry> operatorsStack;// хранятся операторы
	bool build = false;

	int i = 0;
	int j = 0;
	int k = 0;

	while (search(i, launchAnalaize) != -1)
	{
		j = i = search(i, launchAnalaize);//индекс первого символа после равно
		for (; launchAnalaize->Lextable.table[i].lexema != LEX_SEPARATOR; i++, k++) //читаем до символа конца строки, увеличивая счётчик К
		{
			if (launchAnalaize->Lextable.table[i].lexema == LEX_LEFTBRACKET) //если нашли ( или )
			{
					if ((launchAnalaize->Lextable.table[i - 1].indid != -1 && launchAnalaize->IDtable.table[launchAnalaize->Lextable.table[i - 1].indid].idtype == IT::F)
						|| launchAnalaize->Lextable.table[i - 1].lexema == 'k' || launchAnalaize->Lextable.table[i - 1].lexema == 'b' || launchAnalaize->Lextable.table[i - 1].lexema == 'y')  // Вернёт -1 если это не будет являтся переменной
					{
						while (launchAnalaize->Lextable.table[i - 1].lexema != LEX_RIGHTBRACKET)
						{
							i++;
						}
					}
					else
					{
						operatorsStack.push(launchAnalaize->Lextable.table[i]);// ( добавляем в стек Операторов элемент таблицы лексем
					}
				}
			else if (launchAnalaize->Lextable.table[i].lexema == LEX_RIGHTBRACKET) //если нашли ( или )
			{
					while (operatorsStack.top().lexema != LEX_LEFTBRACKET) // ) посмотреть вехний элемент из стека
					{
						result[size++] = operatorsStack.top();
						operatorsStack.pop(); //извлекает вершину стека операторов
					}
					operatorsStack.pop(); //извлекает вершину стека операторов
			}
			else
			{
				switch (launchAnalaize->Lextable.table[i].lexema) //если это не скобки()
				{
				case LEX_MUL:
				{
					operatorsStack.push(launchAnalaize->Lextable.table[i]);//добавляем в стек Операторов элемент таблицы лексем
					break;
				}
				case LEX_PLUS:
				case LEX_MINUS:
					if (operatorsStack.empty() || priority(launchAnalaize->Lextable.table[i].lexema) == priority(operatorsStack.top().lexema))//пуст
						operatorsStack.push(launchAnalaize->Lextable.table[i]);//добавляем в стек Операторов элемент таблицы лексем 
					else //стек Операторов не пустой
					{
						while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTBRACKET &&
							priority(launchAnalaize->Lextable.table[i].lexema) < priority(operatorsStack.top().lexema)) //если лексема в стеке не (, и приоретет операции меньше  приоритету лежащему в голове стека, то..
						{
							result[size++] = operatorsStack.top(); //заносим верхушку стека в результат
							operatorsStack.pop();//извлекает верхушку стека
						}
						if (!operatorsStack.empty() && operatorsStack.top().lexema == LEX_LEFTBRACKET)// проверяем верхушку стека
						{
							operatorsStack.push(launchAnalaize->Lextable.table[i]);//добавить в стек операторов элемент таблицы лексем
						}
						if (operatorsStack.empty())// проверяем верхушку стека
						{
							operatorsStack.push(launchAnalaize->Lextable.table[i]);//добавить в стек операторов элемент таблицы лексем
						}
					}
					break;
				default:
					result[size++] = launchAnalaize->Lextable.table[i];//заносим таблицу лексем в результат
					break;
				}
			}
		}
		for (; !operatorsStack.empty(); i++)	//выбить все операторы в Выходной стек
		{
			result[size++] = operatorsStack.top(); //заносим все оперыторы в результат
			operatorsStack.pop();// вынимаем верхушку стека
		}
		for (int i = 0; i < size; j++, i++, k--) //заполняяем таблицу лексем, для данного выражения
		{
			launchAnalaize->Lextable.table[j].lexema = result[i].lexema;
			launchAnalaize->Lextable.table[j].indid = result[i].indid;
			launchAnalaize->Lextable.table[j].nofl = result[i].nofl; //нормер стр в тект прогр
			launchAnalaize->IDtable.table[result[i].indid].idxfirstLE;
		}
		for (; k > 0; k--, j++)	//заместить все элементы, "удалённые в процессе создания польской"
		{
			launchAnalaize->Lextable.table[j].lexema = NULL;
			launchAnalaize->Lextable.table[j].indid = NULL;
		}
		size = 0;
		build = true;
	}
	return build;
}