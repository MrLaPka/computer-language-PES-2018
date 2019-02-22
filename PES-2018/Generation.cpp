#include "stdafx.h"
#include "Generation.h"

ofstream fout("Out.html");
void Top()
{
	fout << "<html>" << endl;
	fout << "<head>" << endl;
	fout << "<title>" << endl;
	fout << "PES-2018" << endl;
	fout << "</title" << endl;
	fout << "</head>" << endl;
	fout << "<body style='background:black;'>" << endl;
	fout << "<header>" << endl;
	fout << "</header>" << endl;
	fout << "<main style='color:green;font-family:consolas;'>" << endl;
	fout << "<script>" << endl << endl;
}
void Bottom()
{
	fout << "</script>" << endl;
	fout << "</main>" << endl;
	fout << "<footer>" << endl;
	fout << "</footer>" << endl;
	fout << "</body>" << endl;
	fout << "</html>" << endl;
	fout.close();
}



void Generation(LT::LexTable &Lextable, In::SomeTokens *tokens)
{
	Top();
	for (int i = 0; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_PLUS:
		case LEX_MUL:
		case LEX_MINUS:
		case LEX_EQUAL:
		case LEX_BIGGER:
		case LEX_LESS:
		case LEX_NOT:
		{
			fout << " " << Lextable.table[i].lexema;
			break;
		}
		case LEX_COMMA:
		case SPACE:
		{
			fout << Lextable.table[i].lexema << " ";
			break;
		}

		case LEX_DOLLAR: {
			Lextable.table[i].lexema = '=';
			if(Lextable.table[i-1].lexema!='!')
			fout << " " << Lextable.table[i].lexema;
		}

		case LEX_RIGHTBRACKET:
		case LEX_LEFTBRACKET:
		{
			fout << Lextable.table[i].lexema;
			break;
		}
		case LEX_TO_OPEN_BLOCK:
		{

			fout << endl << Lextable.table[i].lexema << endl;
			break;
		}
		case LEX_TO_CLOSE_BLOCK:
		{

			fout << Lextable.table[i].lexema << endl << endl;
			break;
		}
		case LEX_SEPARATOR:
		{
			fout << Lextable.table[i].lexema << endl;
			for (int j = 1; j < i; j++) {
				if (Lextable.table[i - j].lexema == LEX_SEPARATOR) {
					break;
				}
				if (Lextable.table[i - j].lexema == LEX_ID && Lextable.table[i - j+1].lexema == LEX_EQUAL) {
					fout << "if(parseInt(" << tokens[i - j].token << ")<0)" << tokens[i - j].token << "*=0;";
					fout << "if(parseInt(" << tokens[i - j].token << ")>16777216)" << tokens[i - j].token << "=16777216;";
					break;
				}
			}
			break;
		}
		case  LEX_DECLARE:
		{
			fout << "let ";
			break;
		}
		case LEX_LITERAL:
		case LEX_ID:
		{
			fout << tokens[i].token;
			break;
		}
		case LEX_OUT:
		{
		
			fout << "document.write('<br>');" << endl;
			fout << "document.write" << endl;
			break;
		}

		case LEX_IF:
		{
			fout << "if" << endl;
			break;
		}
		case LEX_ELSE:
		{
			fout << "else" << endl;
			break;
		}
		case POINT:
		{
			fout << endl;
			break;
		}
		case LEX_FUNCTION:
		{
			fout << "function ";
			break;
		}
		case LEX_RETURN:
		{
			fout << "return ";
			break;
		}

		case LEX_LIB:
		{
			char * strcon = "function strcon(str0,str1,str2){return str0 = str1+str2;}\n";
			fout << strcon;
			char * isEmpty = "function isEmpty(str){if(!str) return true; return false;}";
			fout << isEmpty;
			break;
		}

		case LEX_STRCON:
		{
			fout << tokens[i + 2].token<<" = strcon";
			break;
		}
		case LEX_LENGTH:
		{
			fout << tokens[i + 2].token<<".length";
			i += 3;
			break;
		}

		case LEX_ISEMPTY:
		{
			fout << "isEmpty(" << tokens[i + 2].token << ")";
			i += 3;
			break;
		}
		}
	}
	Bottom();

}

