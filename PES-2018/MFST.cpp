#include "stdafx.h"
#include "GRB.h"
#include "MFST.h"
#include "LexAnalize.h"
#include "Log.h"
//�������
//������ ��������� �������� � ���������� �������
int FST_TRACE_n = -1;

char rbuf[205], sbuf[205], lbuf[1024];

namespace MFST
{
	MFST::MfstState::MfstState() // ����������� �� ���������
	{
		lenta_position = 0; //������� �� �����
		nrule = -1; //����� �������� �������
		nrulechain = -1; //����� ������� �������
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) //����������� (������� �� �����, ���� ��������, ����� ������� ������� � �������� �������)
	{
		lenta_position = pposition; //������� �� �����
		st = pst; //���� ��������
		nrulechain = pnrulechain;//����� �������� ������� � �������
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) //����������� (������� �� �����, ���� ��������,����� �������� �������, ����� ������� �������)
	{
		lenta_position = pposition;//������� � �����
		st = pst;//���� ��������
		nrule = pnrule;//����� �������� �������
		nrulechain = pnrulechain;//����� ������� �������
	};
	Mfst::MfstDiagnosis::MfstDiagnosis()//  ���  ����� �����������
	{
		lenta_position = -1;
		rc_stepen = ERRORR;
		nrule = -1;
		nrule_chain = -1;
	};
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEPEN prc_stepen, short pnrule, short pnrule_chain)//�����������
	{                                                     //(������� �� �����, ��� ���������� ����, ����� �������, ����� ������� �������)
		lenta_position = plenta_position; //������� �� �����
		rc_stepen = prc_stepen;//��� ���������� ����
		nrule = pnrule;//����� �������
		nrule_chain = pnrule_chain;//����� ������� �������
	};
	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; }; //����������� �� ��������� 
	Mfst::Mfst(LexAnalaize::LEX plex, GRB::Greibach pgrebach)//(��������� ������ ������������ �����������, ���������� �������)
	{
		grebach = pgrebach; //�������
		lex = plex; //��� ������ ���� �����������
		lenta = new short[lenta_size = lex.Lextable.size];//������ ����� = ������� ������ ������� ������
		for (int k = 0; k < lenta_size; k++)lenta[k] = TS(lex.Lextable.table[k].lexema);//������� � ����� ���������
		lenta_position = 0;
		st.push(grebach.stbottomT);//��������� ��� �����
		st.push(grebach.startN);//��������� ��������� ������
		nrulechain = -1;//���������� ������� ����� -1
	};//����� ������ � �������
	Mfst::RC_STEPEN Mfst::stepen(const Log::LOG &log) //��������� ��� ��������
	{
		RC_STEPEN rc = ERRORR; //��� �������� = ������ ��������
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top())) //��������� ��������� ������� ����� � ��������� ���������� �� ��
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)//�������, ���� ����� ������� ���� ��� ������, ���� ��� �� ����
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)//�������� ��������� ������� � ������� � �����, ���� ���������� -1
					{
						MFST_TRACE1(log)//�����
							savestate(log);//��������� ��������� ��������
						st.pop();//��������� �������� �����.
						push_chain(chain); //��������� ������� ������� � ����
						rc = NS_OK; //������� ������� � �������... ������� �������� � ����
						MFST_TRACE2(log)//�����
					}
					else
					{
						MFST_TRACE4(log, "TNS_NORULECHAIN/NS_NORULE")//�����
							savediagnois(NS_NORULECHAIN); //��� ����������
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE; //������������ ��������� ��������
					};
				}
				else rc = NS_ERROR;//����������� �������������� ������ ����������
			}
			else if ((st.top() == lenta[lenta_position]))//���� ������� ������ ����� ����� ������� �����
			{
				lenta_position++; //���������� �����
				st.pop();//��������� ������� �����
				nrulechain = -1; //����� �������� ������� ����� -1
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else { MFST_TRACE4(log, "TS_NOK/NS_NORULECHIN") rc = reststate(log) ? TS_NOK : NS_NORULECHAIN; };
		}
		else { rc = LENTA_END; MFST_TRACE4(log, "LENTA_END") };
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain) //��������� ������� ������� � ���� (������� �������)
	{
		for (int k = chain.size - 1; k >= 0; k--)st.push(chain.nt[k]); //� = ������ �������-1. ������� ������� � ����
		return true;
	};
	bool Mfst::savestate(const Log::LOG &log) //��������� ��������� ��������
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain)); //���� ��� ���������� ���������. ������� ����������� ���������
		MFST_TRACE6(log, "SAVESTATE:", storestate.size());
		return true;
	};
	bool Mfst::reststate(const Log::LOG &log) //������������ ��������� ��������
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5(log, "RESSTATE")
				MFST_TRACE2(log)
		};
		return rc;
	};
	bool Mfst::savediagnois(RC_STEPEN prc_stepen)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER&&lenta_position <= diagnosis[k].lenta_position)k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_stepen, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)diagnosis[j].lenta_position = -1;
		};
		return rc;
	};
	bool Mfst::start(const Log::LOG &log)
	{
		bool rc = false;
		RC_STEPEN rc_stepen = ERRORR;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_stepen = stepen(log);
		while (rc_stepen == NS_OK || rc_stepen == NS_NORULECHAIN || rc_stepen == TS_OK || rc_stepen == TS_NOK)rc_stepen = stepen(log);
		switch (rc_stepen)
		{
		case LENTA_END:         MFST_TRACE4(log, "------>LENTA_END")
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d:����� ����� %d, �������������� ������ �������� ��� ������ ", 0, lenta_size);
			*log.stream << std::setw(4) << std::left << 0 << ":����� ����� " << lenta_size << ", �������������� ������ �������� ��� ������ " << std::endl;
			rc = true;
			break;
		case NS_NORULE:         MFST_TRACE4(log, "------>NS_NORULE")
			*log.stream << "-------------------------------------------------------------------------------------" << std::endl;
			*log.stream << getDiagnosis(0, buf) << std::endl;
			*log.stream << getDiagnosis(1, buf) << std::endl;
			*log.stream << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN:       MFST_TRACE4(log, "------>NS_NORULECHAIN") break;
		case NS_ERROR:             MFST_TRACE4(log, "------>NS_ERROR") break;
		case ERRORR:             MFST_TRACE4(log, "------>ERRORR") break;
		};
		return rc;

	};

	char* Mfst::getCSt(char*buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			short p = st._Get_container()[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	};
	char *Mfst::getCLenta(char*buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};
	char* Mfst::getDiagnosis(short n, char*buf)//������ ������ ���������
	{
		char *rc = "";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::GetError(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: ������ %d,%s", err.id, lex.Lextable.table[lpos].nofl, err.message);
			printf("%d: ������ %d,%s\n", err.id, lex.Lextable.table[lpos].nofl, err.message);
			rc = buf;
		};
		return rc;
	};
	void Mfst::printrules(const Log::LOG &log)//�������: ������� ������� ������
	{
		*log.stream << endl;
		*log.stream << endl;
		*log.stream << "-------------------������ �������-------------------------- " << endl;
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
	};
	bool Mfst::savededucation()//��������� ������� ������� ������
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		};
		return true;
	};
};