#include <iostream>
#include "MCSimulation.h"
#include "NetlistToMap.h"
#include "LogicFunction.h"
#include "CircuitProcess.h"
#include <time.h>
#include <math.h>


MCSimulation::MCSimulation(NetlistToMap *map)
{
	m_map = map;
	m_Module = m_map->FetchTotalModule();
}


float MCSimulation::Combination_CircuitTest()
{
	CircuitProcess *cp;
	cp = new CircuitProcess();
	//divede the circuit level by gate
	cp->DivideByGate(m_Module);

	//set the rand seed
	srand(time(NULL));
	er_count == 0;

	for (long i = 0; i < Loopcm; i++){        
		cp->InputGenerate(m_Module,-1);
		cp->smValueCal(m_Module);
		cp->fcValueCal(m_Module);

		/*
		*�޸ĸò��ֳ�����Ի�ò�ͬ�����µĿɿ���
		*/

		/*������·�ɿ���*/
		for (int k = 0; k < m_Module->OutputNum; k++){
			if (m_Module->Gates[m_Module->Output_head[k]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[k]].NodeValue[1]){
				er_count++;
				break;
			}	
		}

		/*��·������źſɿ���*/
		/*if (m_Module->Gates[m_Module->Output_head[0]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[0]].NodeValue[1]){
			er_count++;
		}*/

		/*if (m_Module->Gates[m_Module->Output_head[1]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[1]].NodeValue[1]){
			er_count++;
		}*/

		//if (m_Module->Gates[31].NodeValue[0] == 1){
		//	er_count++;
		//}
	}
	float ret = (float)(((float)er_count) / Loopcm);
	return (1 - ret);
}


float MCSimulation::Approximate_CircuitTest()
{
	CircuitProcess *cp;
	cp = new CircuitProcess();
	//divede the circuit level by gate
	cp->DivideByGate(m_Module);

	vector<int> iotemp = cp->IOPinGenerate(m_Module);
	//set the rand seed
	srand(time(NULL));

	er_count = 0;

	for (int k = 0; k < Loopap; k++)
	{
		int smValue = 0, acValue=0, fcValue = 0;
		int a = 0, b = 0;
		int i = 0, j = 0;

		cp->InputGenerate(m_Module, -1);
		//function value
		cp->fcValueCal(m_Module);
		//simulaton value
		cp->smValueCal(m_Module);

		for (j = iotemp.size() - m_Module->OutputNum; j < iotemp.size(); j++){
			fcValue |= m_Module->Gates[iotemp[j]].NodeValue[1] << (j - (iotemp.size() - m_Module->OutputNum));
			smValue |= m_Module->Gates[iotemp[j]].NodeValue[0] << (j - (iotemp.size() - m_Module->OutputNum));
		}
		//accurate value calculate then calculate the reliability 
		for (i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= m_Module->Gates[iotemp[i]].NodeValue[1] << ((i / 2) - 1);
			b |= m_Module->Gates[iotemp[i-1]].NodeValue[1] << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + m_Module->Gates[iotemp[0]].NodeValue[1];

		//printf("fc:%d  ac:%d\n", fcValue, acValue);

		//��·���ʧЧ����ͳ��
		if (smValue <= max(acValue, fcValue) && smValue >= min(acValue, fcValue))
			er_count++;

	}
	
	float ret = (float)(((float)er_count) / Loopap);
	return ret;
}


float MCSimulation::Mul_CircuitTest()
{
	CircuitProcess *cp;
	cp = new CircuitProcess();
	//divede the circuit level by gate
	cp->DivideByGate(m_Module);

	vector<int> iotemp = cp->IOPinGenerate(m_Module);
	//set the rand seed
	srand(time(NULL));

	er_count = 0;

	for (int k = 0; k < Loopap; k++)
	{
		int smValue = 0, acValue = 0, fcValue = 0;
		int a = 0, b = 0;
		int i = 0, j = 0;

		cp->InputGenerate(m_Module, 0xffff);
		//function value
		cp->fcValueCal(m_Module);
		//simulaton value
		//cp->smValueCal(m_Module);

		for (j = iotemp.size() - m_Module->OutputNum; j < iotemp.size(); j++){
			fcValue |= m_Module->Gates[iotemp[j]].NodeValue[1] << (j - (iotemp.size() - m_Module->OutputNum));
			smValue |= m_Module->Gates[iotemp[j]].NodeValue[0] << (j - (iotemp.size() - m_Module->OutputNum));
			/*std::cout << iotemp[j] << "  " << (j - (iotemp.size() - m_Module->OutputNum))
				                   << "  " << m_Module->Gates[iotemp[j]].NodeValue[1] << endl;*/
		}
		//accurate value calculate then calculate the reliability 
		for (i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= m_Module->Gates[iotemp[i]].NodeValue[1] << ((i-1) / 2);
			b |= m_Module->Gates[iotemp[i - 1]].NodeValue[1] << ((i - 1) / 2);
		}
		acValue = a * b;

		//printf("fc:%d  ac:%d\n", fcValue, acValue);

		//��·���ʧЧ����ͳ��
		if (smValue <= max(acValue, fcValue) && smValue >= min(acValue, fcValue))
			er_count++;

	}

	float ret = (float)(((float)er_count) / Loopap);
	return ret;
}