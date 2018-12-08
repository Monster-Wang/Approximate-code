#include <iostream>
#include "CircuitProcess.h"
#include "LogicFunction.h"
#include "Matrix.h"
#include <string.h>
#include <math.h>
#include <vector>
#include <set>
#include <map>


CircuitProcess::CircuitProcess(NetlistToMap *map){

	m_Module = map->FetchTotalModule();
	//divde the cicuit by gate
	DivideByGate(m_Module);
	//输入输出端子初始化
	iotemp = IOPinGenerate(m_Module);

	//generate the function value of the circuit
	//FCValue = fcValueGenerate(m_Module);

	//detect the reconvert circuit
	//RecvgCircuit = RecvgDectect(m_Module);

	FanofOutpath = GetDependentoutput(m_Module);
}

CircuitProcess::~CircuitProcess(){
	
}


float CircuitProcess::ReliabilityCal(){	

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1;

	float ret = 0.0f;
	
	int loop = (int)pow(2, Module->InputNum);
	for (int m = 0; m < loop; m++){

		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;

		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)Module->Gates[iotemp[i]].sProbability) << ((i / 2) - 1);
			b |= ((int)Module->Gates[iotemp[i - 1]].sProbability) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)Module->Gates[iotemp[0]].sProbability);

		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			for (int j = 0; j < Module->LevelGate[i].size(); j++){
				//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
				GatePGMcal(Module, Module->LevelGate[i][j]);
			}
		}

		//张量积运算
		for (int j = 0; j < FanofOutpath.size(); j++){
			if (FanofOutpath[j].outputs.size() == 1){
				temp1.Row = 1; temp1.Col = 2;
				temp1.Matrix[0][1] = Module->Gates[FanofOutpath[j].outputs[0]].sProbability;
				temp1.Matrix[0][0] = 1.0 - temp1.Matrix[0][1];
				ma->MatrixTensor(&temp1, &temp);
			}
			else{
				temp1 = fantest(Module, FanofOutpath[j]);
				ma->MatrixTensor(&temp1, &temp);
			}
		}

		//张量积运算
		//for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size(); j++){
		//	temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
		//	temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
		//	ma->MatrixTensor(&temp1, &temp);
		//}

		//找出每个输出向量的最大值，然后计算精确值和功能值之间的概率和
		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i] > temp.Matrix[0][fcValue])
				fcValue = i;
		}

		int toplimit = max(acValue, fcValue);
		for (int i = min(acValue, fcValue); i <= toplimit; i++)
			accumP += temp.Matrix[0][i];
		//cout << acValue << "   " << fcValue << endl;
		ret += accumP;

	}

	delete ma;
	return (ret / ((float)pow(2, Module->InputNum)));
}


void CircuitProcess::RecvgRecal(Struct_Module *md, vector<recvg> rev){
	
	Struct_Module module = *md;
	float fantemp, revtemp;

	vector<recvg>::iterator it = rev.begin();
	for (; it != rev.end(); it++){
		fantemp = module.Gates[(*it).fan].sProbability;
		revtemp = 0.0;
		for (int j = 0; j < 2; j++){

			for (int i = 0; i < module.Gates[(*it).fan].OutputNum; i++)
				module.Gates[module.Gates[(*it).fan].Output[i]].sProbability = j;

			for (int i = 0; i < (*it).path1.size() - 1; i++)
				GatePGMcal(&module, (*it).path1[i]);//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
			for (int i = 0; i < (*it).path2.size(); i++)
				GatePGMcal(&module, (*it).path2[i]);//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
			
			if (j)
				revtemp += fantemp * module.Gates[(*it).cvg].sProbability;
			else
				revtemp += (1 - fantemp) * module.Gates[(*it).cvg].sProbability;
		}
		md->Gates[(*it).cvg].sProbability = revtemp;
		for (int i = 0; i < md->Gates[(*it).cvg].OutputNum; i++)
			md->Gates[md->Gates[(*it).cvg].Output[i]].sProbability = revtemp;
	}
}


float CircuitProcess::add8_Q5test(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1, temp2;

	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){

		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;


		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)m_Module->Gates[iotemp[i]].sProbability) << ((i / 2) - 1);
			b |= ((int)m_Module->Gates[iotemp[i - 1]].sProbability) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)m_Module->Gates[iotemp[0]].sProbability);

		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			vector <int>::iterator iter = Module->LevelGate[i].begin();
			for (; iter != Module->LevelGate[i].end(); iter++){
				//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
				GatePGMcal(Module, *iter);
			}
		}

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size() - 7; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}


		temp1.Row = 1; temp1.Col = 8;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;

		float n140, n166, n154, n162, n182, n189;
		float ntemp, s[3] = { 0.0f };
		n140 = Module->Gates[140].sProbability;

		ntemp = 0;
		Lg->XORFunction(s[0], ntemp, Module->Gates[104].sProbability, Rg);
		Lg->ANDFunction(n154, ntemp, Module->Gates[21].sProbability, Rg);
		Lg->ANDFunction(n162, ntemp, Module->Gates[29].sProbability, Rg);
		Lg->ORFunction(n162, n162, Module->Gates[105].sProbability, Rg);
		Lg->ORFunction(n166, n162, n154, Rg);

		ntemp = 0;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);
		
		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n140)*(1 - n166)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n140)*n166*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[0], ntemp, Module->Gates[104].sProbability, Rg);
		Lg->ANDFunction(n154, ntemp, Module->Gates[21].sProbability, Rg);
		Lg->ANDFunction(n162, ntemp, Module->Gates[29].sProbability, Rg);
		Lg->ORFunction(n162, n162, Module->Gates[105].sProbability, Rg);
		Lg->ORFunction(n166, n162, n154, Rg);

		ntemp = 0;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n140*(1 - n166)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n140*n166*pmul;
		}
		ma->MatrixTensor(&temp1, &temp);

		temp1.Row = 1; temp1.Col = 2;
		for (int j = iotemp.size()-4; j < iotemp.size()-2; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}

		temp1.Row = 1; temp1.Col = 4;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;

		float n195,n203,n205;
		//float ntemp, s[2] = { 0.0f };
		n195= Module->Gates[195].sProbability;
		
		ntemp = 0;
		Lg->XORFunction(s[0], ntemp, Module->Gates[125].sProbability, Rg);

		Lg->ANDFunction(n203, ntemp, Module->Gates[87].sProbability, Rg);
		Lg->ANDFunction(n205, ntemp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n205, n205, n203, Rg);
		Lg->ORFunction(s[1], n205, Module->Gates[126].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 2; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n195)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[0], ntemp, Module->Gates[125].sProbability, Rg);

		Lg->ANDFunction(n203, ntemp, Module->Gates[87].sProbability, Rg);
		Lg->ANDFunction(n205, ntemp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n205, n205, n203, Rg);
		Lg->ORFunction(s[1], Module->Gates[205].sProbability, Module->Gates[126].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 2; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n195*pmul;
		}

		ma->MatrixTensor(&temp1, &temp);


		//找出每个输出向量的最大值，然后计算精确值和功能值之间的概率和
		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i]>temp.Matrix[0][fcValue])
				fcValue = i;
		}

		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += temp.Matrix[0][i];

		//cout << acValue << "   " << fcValue << endl;
		ret += accumP;
	}

	return (ret / ((float)pow(2, Module->InputNum)));
}


float CircuitProcess::add8_006test(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1, temp2;

	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){
		
		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;


		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)m_Module->Gates[iotemp[i]].sProbability) << ((i / 2) - 1);
			b |= ((int)m_Module->Gates[iotemp[i - 1]].sProbability) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)m_Module->Gates[iotemp[0]].sProbability);

		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			vector <int>::iterator iter = Module->LevelGate[i].begin();
			for (; iter != Module->LevelGate[i].end(); iter++){
				//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
				GatePGMcal(Module, *iter);
			}
		}

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size() - 4; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}


		temp1.Row = 1; temp1.Col = 16;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;


		float n102, n112;
		float ntmp, s[4] = { 0.0f };
		n102 = Module->Gates[102].sProbability;

		//n102=0，n112=0
		ntmp = 0;
		Lg->XORFunction(s[0], ntmp, Module->Gates[72].sProbability, Rg);
		Lg->ANDFunction(s[1], ntmp, Module->Gates[73].sProbability, Rg);
		Lg->ORFunction(s[1], s[1], Module->Gates[76].sProbability, Rg);
		Lg->XORFunction(s[1], s[1], Module->Gates[81].sProbability, Rg);

		Lg->ANDFunction(n112, ntmp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n112, n112, Module->Gates[98].sProbability, Rg);

		ntmp = 0;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n102)*(1 - n112)*pmul;
		}

		//n102=0，n112=1
		ntmp = 1;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n102)*n112*pmul;
		}

		//n102=1，n112=0
		ntmp = 1;
		Lg->XORFunction(s[0], ntmp, Module->Gates[72].sProbability, Rg);
		Lg->ANDFunction(s[1], ntmp, Module->Gates[73].sProbability, Rg);
		Lg->ORFunction(s[1], s[1], Module->Gates[76].sProbability, Rg);
		Lg->XORFunction(s[1], s[1], Module->Gates[81].sProbability, Rg);

		Lg->ANDFunction(n112, ntmp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n112, n112, Module->Gates[98].sProbability, Rg);

		ntmp = 0;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n102*(1 - n112)*pmul;
		}

		//n102=1，n112=1
		ntmp = 1;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n102*n112*pmul;
		}

		ma->MatrixTensor(&temp1, &temp);

        
		//找出每个输出向量的最大值，然后计算精确值和功能值之间的概率和
		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i]>temp.Matrix[0][fcValue])
				fcValue = i;
		}

		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += temp.Matrix[0][i];

		//cout << acValue << "   " << fcValue << endl;
		ret += accumP;
	}

	return (ret / ((float)pow(2, Module->InputNum)));
}


void CircuitProcess::DivideByGate(Struct_Module *Module)
{
	//Gate inLeve init
	for (int i = 0; i < MAX_GATES_IN_MODULE; i++){
		Module->Gates[i].inLevel = 1000;
	}

	Module->Level = 0;
	Struct_Gate temp;
	int Stop_Flag = 0;
	bool JumpFlag = false;
	vector <int>::iterator iter, item;//定义临时迭代器

	//按逻辑门分层，先把原始输入做为第一层，然后逐级递归
	//分层结构第一层为原始输入
	for (int i = 0; i < Module->InputNum; i++){
		temp = Module->Gates[Module->Input_head[i]];
		Module->LevelGate[0].push_back(temp.Tag);
		Module->Gates[temp.Tag].inLevel = 0;
		if (temp.OutputNum > 1){
			for (int j = 0; j < temp.OutputNum; j++){
				Module->Gates[temp.Output[j]].inLevel = 0;
			}
		}
	}
	Module->Level++;

	//根据第一层逻辑门逐层递归对余下逻辑门分层
	//先根据分层中门的输出，找到以其为输入的门，然后再找到该门的另一个输入，
	//判断该输入是不是已经被分过层，如果是则该门划在此层，如果不是则跳过
	for (int i = 1; i < MAX_LEVELNUM; i++){
		for (iter = Module->LevelGate[i - 1].begin(); iter != Module->LevelGate[i - 1].end(); iter++){
			if (Module->Gates[*iter].OutputNum == 1){//该门输出没有扇出
				temp = Module->Gates[Module->Gates[*iter].Output[0]];
				JumpFlag = false;
				for (int k = 0; k < temp.InputNum; k++){
					if (Module->Gates[temp.Input[k]].inLevel >(i - 1)){
						JumpFlag = true;
						break;
					}
				}
				if (!JumpFlag){
					Module->LevelGate[i].push_back(temp.Tag);
					Module->Gates[temp.Tag].inLevel = i;
					if (temp.OutputNum > 1){
						for (int k = 0; k < temp.OutputNum; k++){
							Module->Gates[temp.Output[k]].inLevel = i;
						}
					}
				}
			}
			else if (Module->Gates[*iter].OutputNum > 1){//该门输出有扇出
				for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
					if (Module->Gates[Module->Gates[*iter].Output[j]].OutputNum != 0){//判断该扇出节点是不是直接输出节点，如果是则跳过
						temp = Module->Gates[Module->Gates[Module->Gates[*iter].Output[j]].Output[0]];
						JumpFlag = false;
						for (int k = 0; k < temp.InputNum; k++){
							if (Module->Gates[temp.Input[k]].inLevel >(i - 1)){
								JumpFlag = true;
								break;
							}
						}
						if (!JumpFlag){
							Module->LevelGate[i].push_back(temp.Tag);
							Module->Gates[temp.Tag].inLevel = i;
							if (temp.OutputNum > 1){
								for (int k = 0; k < temp.OutputNum; k++){
									Module->Gates[temp.Output[k]].inLevel = i;
								}
							}
						}
					}
				}
			}
		}

		//delete the same logic gate in the container
		for (iter = ++Module->LevelGate[i].begin(); iter != Module->LevelGate[i].end();){
			item = find(Module->LevelGate[i].begin(), iter, *iter);
			//if the two iterator "iter" and "item" are different, in the container has same elememt
			if (item != iter){
				iter = Module->LevelGate[i].erase(iter);
			}
			else{
				iter++;
			}
		}

		Module->Level++;
		//cycle stop condition, all the gate are devided
		Stop_Flag += Module->LevelGate[i].size();
		if (Stop_Flag == Module->GateNum){
			break;
		}
	}
}


void CircuitProcess::PathSearch(Struct_Module *md, vector<int> ph, int start){

	ph.push_back(start);

	if (0 == md->Gates[start].OutputNum)//end the search
		allPath.push_back(ph);
	else if (1 == md->Gates[start].OutputNum)
		PathSearch(md, ph, (md->Gates[start].Output[0]));
	else if (2 <= md->Gates[start].OutputNum){
		int temp = start;
		for (int i = 0; i < md->Gates[start].OutputNum; i++)
			PathSearch(md, ph, md->Gates[start].Output[i]);
	}
}


vector<recvg> CircuitProcess::RecvgDectect(Struct_Module *md){

	vector<recvg> rv;

	for (int i = 1; i < md->Level; i++){
		vector<int>::iterator iter = md->LevelGate[i].begin();
		for (; iter != md->LevelGate[i].end(); iter++){
			if (md->Gates[*iter].OutputNum >= 2){
				int fan = *iter;
				vector<int> sPath;
				allPath.clear();
				PathSearch(md, sPath, fan);//search all the path of a fan node 

				//find the reconverget submodule				
				vector<vector<int>>::iterator it = allPath.begin(), it1;
				for (; it != allPath.end(); it++){
					for (it1 = it + 1; it1 != allPath.end(); it1++){
						if ((*it)[1] != (*it1)[1]){     //the two path are different
							vector<int> path1 = *it, path2 = *it1;
							int ptr1 = 1, ptr2 = 1;
							while ((ptr1 < path1.size()) && ptr2 < path2.size()){
								if (md->Gates[path1[ptr1]].inLevel < md->Gates[path2[ptr2]].inLevel)
									ptr1++;
								else if (md->Gates[path1[ptr1]].inLevel > md->Gates[path2[ptr2]].inLevel)
									ptr2++;
								else if (md->Gates[path1[ptr1]].inLevel == md->Gates[path2[ptr2]].inLevel){
									if (path1[ptr1] == path2[ptr2]){
										recvg tmp;
										tmp.fan = path1[0];
										tmp.cvg = path1[ptr1];
										int cnt1 = 0, cnt2 = 0;
										while (++cnt1 <= ptr1)
											tmp.path1.push_back(path1[cnt1]);
										while (++cnt2 <= ptr2)
											tmp.path2.push_back(path2[cnt2]);

										//check repeated
										bool isRepeated = false;
										for (int k = 0; k < rv.size(); k++){
											isRepeated = false;
											cnt1 = 0; cnt2 = 0;
											if (path1[0] == rv[k].fan && path1[ptr1] == rv[k].cvg){
												while (cnt1 < ptr1){
													if (tmp.path1[cnt1] != rv[k].path1[cnt1])
														break;
													cnt1++;
												}
												while (cnt2 < ptr2){
													if (tmp.path2[cnt2] != rv[k].path2[cnt2])
														break;
													cnt2++;
												}
											}
											if (cnt1 == ptr1 && cnt2 == ptr2){
												isRepeated = true;
												break;
											}
										}
										if (!isRepeated)
											rv.push_back(tmp);
										break;
									}
									ptr1++; ptr2++;
								}
							}
						}
					}
				}
			}
		}
	}
	return rv;
}


vector<int> CircuitProcess::IOPinGenerate(Struct_Module *Module){
	/*AXA1: { 13, 10, 7, 4, 1, 15, 28, 31 }*/
	/*AMA2: { 1, 2, 3, 4, 5, 23, 22, 13 }*/
	/*InXA1: { 3, 1, 2, 4, 5, 10, 13, 11 }*/
	/*add8_ACA_I_N8_Q5: {-1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 }*/
	/*add8_ACA_II_N8_Q4: {-1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 }*/
	/*add8_GDA_St_N8_M8_P1: {-1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 }*/
	/*add8_006: {-1, 1, 2, 3, 6, 9, 12, 15, 19, 23, 28, 33, 36, 39, 42, 45, 48, 51, 52, 88, 101, 107, 108, 115, 116, 118}*/

	map<string, vector<int>> ioterminal;

	vector<int> iotmp = { 1, 2, 3, 4, 5, 23, 22, 13 };
	ioterminal.insert(map<string, vector<int>>::value_type("AMA2", iotmp));
	iotmp.clear();

	iotmp = { 3, 1, 2, 4, 5, 10, 13, 11 };
	ioterminal.insert(map<string, vector<int>>::value_type("InXA1", iotmp));
	iotmp.clear();

	iotmp = { 13, 10, 7, 4, 1, 15, 28, 31 };
	ioterminal.insert(map<string, vector<int>>::value_type("AXA1", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 2, 3, 6, 9, 12, 15, 19, 23, 28, 33, 36, 39, 42, 45, 48, 51, 52, 88, 101, 107, 108, 115, 116, 118 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_006", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_ACA_I_N8_Q5", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_ACA_II_N8_Q4", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_GDA_St_N8_M8_P1", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 5, 9, 13, 17, 22, 27, 31, 35, 40, 45, 49, 53, 56, 61, 64, 70, 92, 107, 122, 110, 123, 113, 124, 128 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_GDA_St_N8_M4_P2", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 66, 109, 129, 143, 144, 145, 146, 147, 151 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_GDA_St_N8_M8_P3", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 66, 119, 146, 166, 175, 182, 190, 191, 195 };
	ioterminal.insert(map<string, vector<int>>::value_type("add8_GDA_St_N8_M8_P6", iotmp));
	iotmp.clear();

	iotmp = { 65, 9, 69, 1, 42, 31, 55, 60, 97, 91, 46, 23, 4, 34, 12, 82, 145, 405, 250, 304, 354, 452, 404, 453, 406, 362, 407, 446, 454, 462, 470, 475 };
	ioterminal.insert(map<string, vector<int>>::value_type("mul8_128", iotmp));
	iotmp.clear();

	iotmp = { -1, 52, 27, 55, 1, 70, 79, 37, 82, 93, 73, 86, 30, 4, 42, 16, 59, 400, 419, 389, 175, 263, 470, 420, 469, 398, 401, 422, 452, 465, 473, 481, 486 };
	ioterminal.insert(map<string, vector<int>>::value_type("mul8_024", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 17, 18, 5, 74, 40, 59, 24, 45, 66, 52, 79, 8, 87, 32, 95, 246, 616, 237, 621, 121, 623, 622, 624, 512, 559, 307, 350, 448, 540, 322, 349 };
	ioterminal.insert(map<string, vector<int>>::value_type("mul8_332", iotmp));
	iotmp.clear();

	string str = Module->Name;
	vector <int> temp = ioterminal[str];
	return temp;
}


void CircuitProcess::InputInit(Struct_Module *Module, vector<int> io, int rnd)
{

	long ipt = rnd;
	for (int i = 0; i < Module->InputNum; i++){
		if ((ipt >> i & 1))
			Module->Gates[Module->Input_head[i]].sProbability = 1.0f;
		else
			Module->Gates[Module->Input_head[i]].sProbability = 0.0f;

		if (Module->Gates[Module->Input_head[i]].OutputNum > 1){
			for (int j = 0; j < Module->Gates[Module->Input_head[i]].OutputNum; j++){
				Module->Gates[Module->Gates[Module->Input_head[i]].Output[j]].sProbability
					= Module->Gates[Module->Input_head[i]].sProbability;
			}
		}
	}
}


void CircuitProcess::GatePGMcal(Struct_Module *Module,int GateNode){
	
	LogicFunction *Lg = new LogicFunction();

	float *src, x1, x2;

	src = &Module->Gates[GateNode].sProbability;
	x1 = Module->Gates[Module->Gates[GateNode].Input[0]].sProbability;
	x2 = Module->Gates[Module->Gates[GateNode].Input[1]].sProbability;

	switch (Module->Gates[GateNode].Type)
	{
	case TYPE_AND:
		Lg->ANDFunction(*src, x1, x2, Rg);
		//if the logic gate has more then two inputs 
		if (Module->Gates[GateNode].InputNum > 2){
			for (int j = 2; j < Module->Gates[GateNode].InputNum; j++){
				x1 = Module->Gates[Module->Gates[GateNode].Input[j]].sProbability;
				Lg->ANDFunction(*src, x1, *src, Rg);
			}
		}
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_OR:
		Lg->ORFunction(*src, x1, x2, Rg);
		if (Module->Gates[GateNode].InputNum > 2){
			for (int j = 2; j < Module->Gates[GateNode].InputNum; j++){
				x1 = Module->Gates[Module->Gates[GateNode].Input[j]].sProbability;
				Lg->ORFunction(*src, x1, *src, Rg);
			}
		}
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_NAND:
		Lg->NANDFunction(*src, x1, x2, Rg);
		if (Module->Gates[GateNode].InputNum > 2){
			for (int j = 2; j < Module->Gates[GateNode].InputNum; j++){
				x1 = Module->Gates[Module->Gates[GateNode].Input[j]].sProbability;
				Lg->NANDFunction(*src, x1, *src, Rg);
			}
		}
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_NOR:
		Lg->NORFunction(*src, x1, x2, Rg);
		if (Module->Gates[GateNode].InputNum > 2){
			for (int j = 2; j < Module->Gates[GateNode].InputNum; j++){
				x1 = Module->Gates[Module->Gates[GateNode].Input[j]].sProbability;
				Lg->NORFunction(*src, x1, *src, Rg);
			}
		}
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_XOR:
		Lg->XORFunction(*src, x1, x2, Rg);
		if (Module->Gates[GateNode].InputNum > 2){
			for (int j = 2; j < Module->Gates[GateNode].InputNum; j++){
				x1 = Module->Gates[Module->Gates[GateNode].Input[j]].sProbability;
				Lg->XORFunction(*src, x1, *src, Rg);
			}
		}
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_NOT:
		Lg->NOTFunction(*src, x1, Rg);
		//put the probability value for every fan cone of the gate
		if (Module->Gates[GateNode].OutputNum > 1){
			for (int j = 0; j < Module->Gates[GateNode].OutputNum; j++)
				Module->Gates[Module->Gates[GateNode].Output[j]].sProbability = *src;
		};
		break;
	case TYPE_FAN:
		//*src = Module->Gates[Module->Gates[GateNode].Input[0]].sProbability;
		break;
	default:
		cout << "The logic gate type not found!" << endl;
		break;
	}

	delete Lg;
}


vector<subCircuit> CircuitProcess::GetDependentoutput(Struct_Module *md){
	
	vector<set<int>> ret;
	vector<subCircuit> ssC;

	vector<int> outIO;
	outIO.insert(outIO.begin(), iotemp.end() - md->OutputNum, iotemp.end());

	for (int i = 0; i < outIO.size(); i++){
		FanNode.clear();
		OutputPathfanDectect(md, outIO[i]);
		ret.push_back(FanNode);
	}

	for (int i=ret.size()-1; i >= 0; i--){
		subCircuit sc;
		memset(&sc,0,sizeof(sc));

		if (ret[i].size()){
			set<int>::iterator it = ret[i].begin();
			sc.outputs.push_back(outIO[i]);

			int ptr = 0;
			for (int j = i - 1; j >= 0; j--){
				if (ret[j].size()){
					set<int>::iterator it1 = ret[j].begin();
					it = ret[i].begin();
					bool isContain = true;

					while (it1 != ret[j].end()){
						if (ret[j].size() > ret[i].size())
							break;
						if (*it != *it1){
							isContain = false;
							break;
						}
						it++; it1++;
					}
					if (isContain){
						if (sc.outputs[sc.outputs.size() - 1] != outIO[j + 1])
							break;
						sc.outputs.push_back(outIO[j]);
						ptr++;
					}
				}
			}
			if (sc.outputs.size() > 1){
				map<int, int> mapout;
				for (int j = 0; j < sc.outputs.size(); j++){
					int pp = 0;
					for (int k = 0; k < outIO.size(); k++){
						if (outIO[k] == sc.outputs[j]){
							pp = k;
							break;
						}	
					}
					mapout.insert(map<int, int>::value_type(pp, sc.outputs[j]));
				}
				sc.outputs.clear();
				for (map<int, int>::iterator mapit = mapout.begin(); mapit != mapout.end(); mapit++)
					sc.outputs.push_back((*mapit).second);
				ssC.push_back(sc);
				i -= ptr;
			}
			else{
				ssC.push_back(sc);
			}
		}
		else{
			sc.outputs.push_back(outIO[i]);
			ssC.push_back(sc);
		}
	}

	map<int, subCircuit> scmap;
	for (int i = 0; i < ssC.size(); i++){
		int j = 0;
		for (j = 0; j < outIO.size(); j++){
			if (outIO[j] == ssC[i].outputs[0])
				break;
		}
		scmap.insert(map<int, subCircuit>::value_type(j, ssC[i]));
	}

	ssC.clear();
	map<int, subCircuit>::iterator it = scmap.begin();
	for (; it != scmap.end(); it++){
		ssC.push_back((*it).second);
	}

	for (int i = 0; i < ssC.size(); i++){
		for (int j = 0; j < ssC[i].outputs.size(); j++){
			for (int k = 0; k < outIO.size(); k++){
				if (outIO[k] == ssC[i].outputs[j]){
					if (ret[k].size()){
						//set<int>::iterator it = ret[k].begin();
						//for (; it != ret[k].end(); it++);
						vector<int>::iterator itm;
						itm = find(ssC[i].fans.begin(), ssC[i].fans.end(), *(--ret[k].end()));
						if (itm == ssC[i].fans.end())
						    ssC[i].fans.push_back(*(--ret[k].end()));
						break;
					}
				}	
			}
		}
	}

	for (int i = 0; i < ssC.size(); i++){
		if (ssC[i].outputs.size() > 1){
			ssC[i].primeFan = ssC[i].fans[0];
		}
	}


	/*search the subcircuit of the fan to output*/
	/*this part need not optimization*/
	vector<int> sPath;
	vector<subCircuit>::iterator itsc = ssC.begin();
	for (; itsc != ssC.end(); itsc++){
		if ((*itsc).outputs.size() > 1){
			vector<int> *subcir = new vector<int>[md->Level];
			allPath.clear();
			PathSearch(md, sPath, (*itsc).primeFan);//search all the path of a fan node 

			vector<vector<int>>::iterator it1 = allPath.begin();
			for (; it1 != allPath.end(); it1++){
				for (vector<int>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++){
					int lvl = md->Gates[*it2].inLevel;
					vector<int>::iterator item = find(subcir[lvl].begin(), subcir[lvl].end(), *it2);

					if (item == subcir[lvl].end())
						subcir[lvl].push_back(*it2);
				}
			}
			for (int i = 0; i < md->Level; i++){
				if (subcir[i].size())
					(*itsc).cir.push_back(subcir[i]);
			}
		}
	}
	return ssC;
}


void CircuitProcess::OutputPathfanDectect(Struct_Module *md, int outtag){

	for (int i = 0; i < md->Gates[outtag].InputNum; i++){

		if (1 >= md->Gates[outtag].inLevel){
			continue;
		}
		else{
			if (TYPE_FAN == md->Gates[outtag].Type){
				FanNode.insert(md->Gates[outtag].Input[0]);
			}
			OutputPathfanDectect(md, md->Gates[outtag].Input[i]);
		}
	}
}


Struct_Matrix CircuitProcess::fantest(Struct_Module *md, subCircuit& sC){

	int fannum = sC.fans.size();
	int outnum = sC.outputs.size();

	int *fantemp = new int[fannum];
	float *sptemp = new float[fannum];

	Struct_Matrix temp1;
	temp1.Row = 1; temp1.Col = (int)pow(2, outnum);
	for (int i = 0; i < temp1.Col; i++)
		temp1.Matrix[0][i] = 0.0;


	for (int k = 0; k < (int)pow(2, fannum);k++){
		for (int i = 0; i < fannum; i++){
			fantemp[fannum - i - 1] = (k >> i) & 0x1;
			sptemp[i] = 0.0;
		}
		//for (int i = 0; i < fannum; i++)
		//	cout << fantemp[i] << "  "; cout << endl;

		for (int i = 0; i < sC.cir.size(); i++){
			for (int k = 0; k < sC.cir[i].size(); k++){
				//calculating signal probability of the node
				GatePGMcal(md, sC.cir[i][k]);
				//if the node is fan father,that reset the signal probability of the node ;
				for (int j = 0; j < fannum; j++){
					if (sC.cir[i][k] == sC.fans[j]){
						sptemp[j] = md->Gates[sC.cir[i][k]].sProbability;
						//cout << sptemp[j] <<endl;	
						md->Gates[sC.cir[i][k]].sProbability = fantemp[j];
						for (int jj = 0; jj < md->Gates[sC.cir[i][k]].OutputNum; jj++)
							md->Gates[md->Gates[sC.cir[i][k]].Output[jj]].sProbability = md->Gates[sC.cir[i][k]].sProbability;
						break;
					}
				}
			}
		}
		//calculate the part of output node combination probability 
		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < outnum; jj++){
				if ((ii >> jj) & 0x1)
					pmul *= md->Gates[sC.outputs[jj]].sProbability;
				else
					pmul *= 1.0 - md->Gates[sC.outputs[jj]].sProbability;
			}
			for (int jj = 0; jj < fannum; jj++){
				if (fantemp[jj])
					pmul *= sptemp[jj];
				else
					pmul *= 1.0 - sptemp[jj];
			}
			temp1.Matrix[0][ii] += pmul;
		}
	}

	delete [] fantemp; fantemp = NULL;
	delete [] sptemp; sptemp = NULL;
	return temp1;
}
