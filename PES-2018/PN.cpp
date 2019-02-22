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

int PN::search(int index, LexAnalaize::LEX *launchAnalaize) //����� ���������
{
	for (int i = index; i < launchAnalaize->Lextable.size; i++) //������� ������
	{
		if (launchAnalaize->Lextable.table[i].lexema == LEX_PLUS || launchAnalaize->Lextable.table[i].lexema == LEX_MINUS
			|| launchAnalaize->Lextable.table[i].lexema == LEX_MUL ) //������� ��������
		{
			for (; ; i--) //���������� ������� �������
			{
				if (launchAnalaize->Lextable.table[i].lexema == LEX_EQUAL || launchAnalaize->Lextable.table[i].lexema == LEX_RIGHTBRACKET)
					return i + 1;
			}
		}
	}
	return -1; //���� �� ����� ���������� ����� 1
}

bool PN::ToDoPN(LexAnalaize::LEX *launchAnalaize)
{
	std::stack <LT::Entry> operatorsStack;// �������� ���������
	bool build = false;

	int i = 0;
	int j = 0;
	int k = 0;

	while (search(i, launchAnalaize) != -1)
	{
		j = i = search(i, launchAnalaize);//������ ������� ������� ����� �����
		for (; launchAnalaize->Lextable.table[i].lexema != LEX_SEPARATOR; i++, k++) //������ �� ������� ����� ������, ���������� ������� �
		{
			if (launchAnalaize->Lextable.table[i].lexema == LEX_LEFTBRACKET) //���� ����� ( ��� )
			{
					if ((launchAnalaize->Lextable.table[i - 1].indid != -1 && launchAnalaize->IDtable.table[launchAnalaize->Lextable.table[i - 1].indid].idtype == IT::F)
						|| launchAnalaize->Lextable.table[i - 1].lexema == 'k' || launchAnalaize->Lextable.table[i - 1].lexema == 'b' || launchAnalaize->Lextable.table[i - 1].lexema == 'y')  // ����� -1 ���� ��� �� ����� ������� ����������
					{
						while (launchAnalaize->Lextable.table[i - 1].lexema != LEX_RIGHTBRACKET)
						{
							i++;
						}
					}
					else
					{
						operatorsStack.push(launchAnalaize->Lextable.table[i]);// ( ��������� � ���� ���������� ������� ������� ������
					}
				}
			else if (launchAnalaize->Lextable.table[i].lexema == LEX_RIGHTBRACKET) //���� ����� ( ��� )
			{
					while (operatorsStack.top().lexema != LEX_LEFTBRACKET) // ) ���������� ������ ������� �� �����
					{
						result[size++] = operatorsStack.top();
						operatorsStack.pop(); //��������� ������� ����� ����������
					}
					operatorsStack.pop(); //��������� ������� ����� ����������
			}
			else
			{
				switch (launchAnalaize->Lextable.table[i].lexema) //���� ��� �� ������()
				{
				case LEX_MUL:
				{
					operatorsStack.push(launchAnalaize->Lextable.table[i]);//��������� � ���� ���������� ������� ������� ������
					break;
				}
				case LEX_PLUS:
				case LEX_MINUS:
					if (operatorsStack.empty() || priority(launchAnalaize->Lextable.table[i].lexema) == priority(operatorsStack.top().lexema))//����
						operatorsStack.push(launchAnalaize->Lextable.table[i]);//��������� � ���� ���������� ������� ������� ������ 
					else //���� ���������� �� ������
					{
						while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTBRACKET &&
							priority(launchAnalaize->Lextable.table[i].lexema) < priority(operatorsStack.top().lexema)) //���� ������� � ����� �� (, � ��������� �������� ������  ���������� �������� � ������ �����, ��..
						{
							result[size++] = operatorsStack.top(); //������� �������� ����� � ���������
							operatorsStack.pop();//��������� �������� �����
						}
						if (!operatorsStack.empty() && operatorsStack.top().lexema == LEX_LEFTBRACKET)// ��������� �������� �����
						{
							operatorsStack.push(launchAnalaize->Lextable.table[i]);//�������� � ���� ���������� ������� ������� ������
						}
						if (operatorsStack.empty())// ��������� �������� �����
						{
							operatorsStack.push(launchAnalaize->Lextable.table[i]);//�������� � ���� ���������� ������� ������� ������
						}
					}
					break;
				default:
					result[size++] = launchAnalaize->Lextable.table[i];//������� ������� ������ � ���������
					break;
				}
			}
		}
		for (; !operatorsStack.empty(); i++)	//������ ��� ��������� � �������� ����
		{
			result[size++] = operatorsStack.top(); //������� ��� ��������� � ���������
			operatorsStack.pop();// �������� �������� �����
		}
		for (int i = 0; i < size; j++, i++, k--) //���������� ������� ������, ��� ������� ���������
		{
			launchAnalaize->Lextable.table[j].lexema = result[i].lexema;
			launchAnalaize->Lextable.table[j].indid = result[i].indid;
			launchAnalaize->Lextable.table[j].nofl = result[i].nofl; //������ ��� � ���� �����
			launchAnalaize->IDtable.table[result[i].indid].idxfirstLE;
		}
		for (; k > 0; k--, j++)	//��������� ��� ��������, "�������� � �������� �������� ��������"
		{
			launchAnalaize->Lextable.table[j].lexema = NULL;
			launchAnalaize->Lextable.table[j].indid = NULL;
		}
		size = 0;
		build = true;
	}
	return build;
}