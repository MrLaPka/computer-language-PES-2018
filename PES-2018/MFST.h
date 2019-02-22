#include <iostream>
#include <stack> 
#include "GRB.h"
#include "LexAnalize.h"
#include "Log.h"
//���� ����������, ������������ ������������ ��������� Mfst ��� ������������� ����������� ��������� �������� 

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

typedef std::stack<short> MFSTSTSTACK;	//���� ��������

namespace MFST
{
//���������: ��� ���������� ��������� ��������; ��������� ���������
	//�������� ���������� ��� ����, ����� ����� ����������� ���������
	//	� ����� ��������� � ����������� ��������������  ������� 
	//	��������������� �������(� ���� ��������������������� ��������).
	struct MfstState				//��������� ��������(��� ����������
	{
		short lenta_position;		//������� �� �����
		short nrule;				//����� �������� �������
		short nrulechain;			//����� ������� �������
		MFSTSTSTACK st;				//���� ��������
		MfstState();
		MfstState(
			short pposition,		//������� �� �����
			MFSTSTSTACK pst,		//���� ��������
			short pnrulechain		//����� ������� �������, �������� �������
		);
		MfstState(
			short pposition,		//������� �� �����
			MFSTSTSTACK pst,		//���� ��������
			short pnrule,			//����� �������� �������
			short pnrulechain		//����� ������� �������, �������� �������
		);
	};
	//������������� ������� ��������� ��������
	struct Mfst					//���������� �������	
	{
		enum RC_STEPEN			//��� �������� ������� stepen
		{
			NS_OK,				//������� ������� � �������, ������� �������� � ����
			NS_NORULE,			//�� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,		//�� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,			//����������� �������������� ������ ����������
			TS_OK,				//������� ������ ����� == ������� �����, ������������ �����, ��� �����
			TS_NOK,				//������� ������ ����� != ������� �����, ������������� ���������
			LENTA_END,			//������� ������� ����� >= lenta_size
			ERRORR			
		};
		struct MfstDiagnosis		//�����������
		{
			short lenta_position;	//������� �� �����
			RC_STEPEN rc_stepen;		//��� ���������� ����
			short nrule;			//����� �������
			short nrule_chain;		//����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(			//�����������
				short plenta_position,	//������� �� �����
				RC_STEPEN prc_stepen,		//��� ���������� ����
				short pnrule,			//����� �������
				short pnrule_chain		//����� ������� �������
			);
		}
		diagnosis[MFST_DIAGN_NUMBER];	//��������� ����� �������� ���������
		short* lenta;				//����������������(TS/NL) ����� (�� LEX)
		short lenta_position;			//������� ������� �� �����
		short nrule;					//����� �������� �������
		short nrulechain;				//����� ������� �������, �������� �������
		short lenta_size;				//������ �����
		GRB::Greibach grebach;			//���������� �������
		LexAnalaize::LEX lex;					//��������� ������ ������������ �����������
		MFSTSTSTACK st;					//���� ��������		
		std::stack<MfstState> storestate;	//���� ��� ���������� ���������
		Mfst();
		Mfst(
			LexAnalaize::LEX plex,			//��������� ������ ������������ �����������
			GRB::Greibach pgrebach		//���������� �������
		);
		char* getCSt(char*buf);			//�������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);//�����: n �������� � pos
		char*getDiagnosis(short n, char*buf);			//�������� n-�� ������ ����������� ��� 0�00
		bool savestate(const Log::LOG &log);					//��������� ��������� ��������
		bool reststate(const Log::LOG &log);					//������������ ��������� ��������
		bool push_chain(					//��������� ������� ������� � ����
			GRB::Rule::Chain chain				//������� �������
		);
		RC_STEPEN stepen(const Log::LOG &log);			//��������� ��� ��������
		bool start(const Log::LOG &log);			//��������� �������
		bool savediagnois(
			RC_STEPEN pprc_stepen			//��� ���������� ����
		);
		void printrules(const Log::LOG &log);		//������� ������������������ ������
		struct Deducation		//�����
		{
			short size;		//���������� ����� � ������
			short* nrules;	//������ ������ ����������
			short* nrulechains;//������ ������� ������ ���������� (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();	//��������� ������ ������
	};
};
