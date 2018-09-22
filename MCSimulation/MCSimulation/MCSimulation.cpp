#include "MCSimulation.h"
#include "NetlistToMap.h"
#include "LogicFunction.h"
#include "CircuitProcess.h"
#include <time.h>
#include <math.h>

//0.99
//0.9586
//0.9686



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
		*修改该部分程序可以获得不同需求下的可靠性
		*/

		/*整个电路可靠性*/
		for (int k = 0; k < m_Module->OutputNum; k++){
			if (m_Module->Gates[m_Module->Output_head[k]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[k]].NodeValue[1]){
				er_count++;
				break;
			}	
		}

		/*电路输出端信号可靠性*/
		/*if (m_Module->Gates[m_Module->Output_head[0]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[0]].NodeValue[1]){
			er_count++;
		}*/

		/*if (m_Module->Gates[m_Module->Output_head[1]].NodeValue[0] != m_Module->Gates[m_Module->Output_head[1]].NodeValue[1]){
			er_count++;
		}*/
	}
	float ret = (float)(((float)er_count) / Loopcm);
	return (1 - ret);
}


float MCSimulation::Approximate_CircuitTest()
{
	/*AXA1*/	int iotemp[] = { 13, 10, 7, 4, 1, 28, 15, 31 };
	/*AMA2*/	//int iotemp[] = { 1, 2, 3, 4, 5, 23, 22, 13 };
	/*InXA1*/	//int iotemp[] = { 3, 1, 2, 4, 5, 10, 13, 11 };
	/*add8_ACA_I_N8_Q5*/	 //int iotemp[] = {-1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 };
	/*add8_ACA_II_N8_Q4*/	 //int iotemp[] = {-1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 };
	/*add8_GDA_St_N8_M8_P1*/ //int iotemp[] = {-1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 };

	CircuitProcess *cp;
	cp = new CircuitProcess();
	//divede the circuit level by gate
	cp->DivideByGate(m_Module);
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

		for (j = getSize(iotemp) - m_Module->OutputNum; j < getSize(iotemp); j++){
			fcValue |= m_Module->Gates[iotemp[j]].NodeValue[1] << (j -(getSize(iotemp) - m_Module->OutputNum));  
			smValue |= m_Module->Gates[iotemp[j]].NodeValue[0] << (j - (getSize(iotemp) - m_Module->OutputNum));
		}
		//accurate value calculate then calculate the reliability 
		for (i = getSize(iotemp) - m_Module->OutputNum - 1; i > 1; i -= 2){
			a |= m_Module->Gates[iotemp[i]].NodeValue[1] << ((i / 2) - 1);
			b |= m_Module->Gates[iotemp[i-1]].NodeValue[1] << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + m_Module->Gates[iotemp[0]].NodeValue[1];

		//printf("fc:%d  ac:%d\n", fcValue, acValue);

		//近似电路MC仿真
		if (smValue <= max(acValue, fcValue) && smValue >= min(acValue, fcValue))
			er_count++;

	}
	
	float ret = (float)(((float)er_count) / Loopap);
	return ret;
}
