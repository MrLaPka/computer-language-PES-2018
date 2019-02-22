#pragma once
#include "stdafx.h"
#include "LexAnalize.h"
#include "FST.h"

struct PN
{
	LT::Entry result[MB4]; // получившееся pn
	int size = 0;
	enum { LB = '(', RB = ')', EQUAL = '=',PLUS = '+', MINUS = '-', MULT = '*'};


	int priority(char operation); // Приоритет операции
	int search(int number, LexAnalaize::LEX *launchAnalaize); 

	bool ToDoPN(LexAnalaize::LEX *launchAnalaize);
};
