#pragma once
#include "Error.h"
#include "Log.h"


#define MFST_TRACE_START(LOG) *log.stream << std::setw(4)<<std::left<<"���"<<":" \
	<< std::setw(20) << std::left << "�������"  \
	<< std::setw(30) << std::left << "������� �����" \
	<< std::setw(20) << std::left << "����" \
	<< std::endl;

#define NL(n)GRB::Rule::Chain::N(n)
#define TS(n)GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)

#define MFST_TRACE1(LOG) *log.stream <<std::setw( 4)<<std::left<<++FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << rule.getCRule(rbuf, nrulechain)  \
	<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE2(LOG)    *log.stream <<std::setw( 4)<<std::left<<FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << " "  \
	<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE3(LOG)     *log.stream<<std::setw( 4)<<std::left<<++FST_TRACE_n<<":" \
	<< std::setw(20) << std::left << " "  \
	<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
	<< std::setw(20) << std::left << getCSt(sbuf) \
	<< std::endl;

#define MFST_TRACE4(LOG, c) *log.stream<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(LOG, c) *log.stream<<std::setw(4)<<std::left<<  FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE6(LOG,c,k) *log.stream<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<std::endl;

#define MFST_TRACE7(LOG)  *log.stream<<std::setw(4)<<std::left<<state.lenta_position<<": "\
	<< std::setw(20) << std::left << rule.getCRule(rbuf, state.nrulechain) \
	<< std::endl;


namespace GRB
{
	struct Rule	//������� � ���������� �������
	{
		short  nn;	//����������(����� ������ �������) <0
		int iderror;		//������������� ���������������� ���������
		short size;			//���������� ������� - ������ ������ �������
		struct Chain		//�������(������ ����� �������)
		{
			short size;						//����� �������
			short*nt;					//������� ����������(>0 � ������������ (<0)
			Chain() { size = 0; nt = 0; };
			Chain(
				short psize,				//���������� �������� � �������
				short s, ...			//������� (�������� ��� ����������)
			);
			char* getCChain(char*b);		//�������� ������ ������� �������
			static short T(char t) { return short(t); };//��������
			static short N(char n) { return -short(n); };//����������
			static bool isT(short s) { return s > 0; }			//��������?
			static bool isN(short s) { return !isT(s); }		//����������?
			static char alphabet_to_char(short s) { return isT(s) ? char(s) : char(-s); };//short->char
		}*chains;	//������ ������� - ������ ������ �������

		Rule() { nn = 0x00; size = 0; }
		Rule(
			short pnn,			//���������� (<0)
			int iderror,				//������������� ���������������� ���������
			short psize,				//���������� ������� - ������ ������ �������
			Chain c, ...				//��������� ������� - ������ ������ �������
		);
		char*getCRule(				//�������� ������� � ���� N->�������(��� ����������)
			char*b,					//�����
			short nchain			//����� �������(������ �����) � �������
		);
		short Rule::getNextChain(	//�������� ��������� �� j ���������� �������, ������� � ����� ��� -1
			short t,			//������ ������ �������
			Rule::Chain &pchain,	//������������ �������
			short j					//����� �������
		);
	};
	struct Greibach			//���������� �������
	{
		short size;			//���������� ������
		short startN;	//��������� ������
		short stbottomT;//��� �����
		Rule *rules;			//��������� ������
		Greibach() { short size = 0; startN = 0; stbottomT = 0; rules = 0; };
		Greibach(
			short pstartN,		//��������� ������
			short pstbootomT,		//��� �����
			short psize,				//���������� ������
			Rule r, ...					//�������
		);
		short getRule(		//�������� �������, ������������ ����� ������� ��� -1
			short pnn,	//����� ������ �������
			Rule&prule			//������������ ������� ����������
		);
		Rule getRule(short n);	//�������� ������� �� ������
	};
	Greibach getGreibach();		//�������� ����������
};