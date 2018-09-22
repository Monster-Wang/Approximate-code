#include <iostream>
#include "CircuitProcess.h"
#include "LogicFunction.h"
#include "Matrix.h"
#include <string.h>
#include <map>
#include <math.h>



CircuitProcess::CircuitProcess(NetlistToMap *map){

	m_Module = map->FetchTotalModule();
}


float CircuitProcess::ReliabilityCal(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1;

	float *src;
	float *x1, *x2;
	vector <float> relia;

	//divde the cicuit by gate
	DivideByGate(Module);

    /*AXA1*/	int iotemp[] = { 13, 10, 7, 4, 1, 28, 15, 31 };
	/*AMA2*/	//int iotemp[] = { 1, 2, 3, 4, 5, 23, 22, 13 };
	/*InXA1*/	//int iotemp[] = { 3, 1, 2, 4, 5, 10, 13, 11 };
	/*add8_ACA_I_N8_Q5*/	 //int iotemp[] = {-1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 };
	/*add8_ACA_II_N8_Q4*/	 //int iotemp[] = {-1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 };
	/*add8_GDA_St_N8_M8_P1*/ //int iotemp[] = {-1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 };

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){
		//the temp matrix initial
		temp.Col = temp.Row = 1; 
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;

		//init the primary input 
		InputInit(Module, m);
		
		int acValue = 0,a = 0, b = 0;

		for (int i = getSize(iotemp) - m_Module->OutputNum - 1; i > 1; i -= 2){
			a |= ((int)m_Module->Gates[iotemp[i]].Probability[1]) << ((i / 2) - 1);
			b |= ((int)m_Module->Gates[iotemp[i - 1]].Probability[1]) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)m_Module->Gates[iotemp[0]].Probability[1]);


		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			vector <int>::iterator iter = Module->LevelGate[i].begin();
			for (; iter != Module->LevelGate[i].end(); iter++){
				src = Module->Gates[*iter].Probability;
				x1 = Module->Gates[Module->Gates[*iter].Input[0]].Probability;
				x2 = Module->Gates[Module->Gates[*iter].Input[1]].Probability;

				switch (Module->Gates[*iter].Type)
				{
				case TYPE_AND:
					Lg->ANDFunction(src, x1, x2, Rg);
					//if the logic gate has more then two inputs 
					if (Module->Gates[*iter].InputNum > 2){
						for (int j = 2; j < Module->Gates[*iter].InputNum; j++){
							x1 = Module->Gates[Module->Gates[*iter].Input[j]].Probability;
							Lg->ANDFunction(src, x1, src, Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				case TYPE_OR:
					Lg->ORFunction(src, x1, x2, Rg);
					if (Module->Gates[*iter].InputNum > 2){
						for (int j = 2; j < Module->Gates[*iter].InputNum; j++){
							x1 = Module->Gates[Module->Gates[*iter].Input[j]].Probability;
							Lg->ORFunction(src, x1, src, Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				case TYPE_NAND:
					Lg->NANDFunction(src, x1, x2, Rg);
					if (Module->Gates[*iter].InputNum > 2){
						for (int j = 2; j < Module->Gates[*iter].InputNum; j++){
							x1 = Module->Gates[Module->Gates[*iter].Input[j]].Probability;
							Lg->NANDFunction(src, x1, src, Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				case TYPE_NOR:
					Lg->NORFunction(src, x1, x2, Rg);
					if (Module->Gates[*iter].InputNum > 2){
						for (int j = 2; j < Module->Gates[*iter].InputNum; j++){
							x1 = Module->Gates[Module->Gates[*iter].Input[j]].Probability;
							Lg->NORFunction(src, x1, src, Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				case TYPE_XOR:
					Lg->XORFunction(src, x1, x2, Rg);
					if (Module->Gates[*iter].InputNum > 2){
						for (int j = 2; j < Module->Gates[*iter].InputNum; j++){
							x1 = Module->Gates[Module->Gates[*iter].Input[j]].Probability;
							Lg->XORFunction(src, x1, src, Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				case TYPE_NOT:
					Lg->NOTFunction(src, x1, Rg);
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 2; k++)
								Module->Gates[Module->Gates[*iter].Output[j]].Probability[k] = src[k];
						}
					};
					break;
				default :
					cout << "The logic gate type not found!" << endl;
					break;
				}
			}
		}

		//the end of once calculate 
		for (int j = getSize(iotemp) - Module->OutputNum; j < getSize(iotemp); j++){
			temp1.Matrix[0][0] = Module->Gates[iotemp[j]].Probability[0];
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].Probability[1];
			ma->MatrixTensor(&temp1, &temp);
		}

		//找出每个输出向量的最大值，然后计算精确值和功能值之间
		int fcValue = 0; float accumP = 0; 
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i]>temp.Matrix[0][fcValue])
				fcValue = i;
		}
		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += temp.Matrix[0][i];

		//cout << acValue << "   " << fcValue << endl;

		relia.push_back(accumP);
	}

	float ret = 0.0f;
	vector<float>::iterator iter = relia.begin();
	for (; iter != relia.end(); iter++){
		ret += (*iter);
	}
	
	return (ret / ((float)pow(2, Module->InputNum)));
}

void CircuitProcess::InputInit(Struct_Module *Module, int rnd)
{
	long ipt = rnd;
	for (int i = 0; i < Module->InputNum; i++){
		if ((ipt >> i & 1)){
			Module->Gates[Module->Input_head[i]].Probability[0] = 0.0f;
			Module->Gates[Module->Input_head[i]].Probability[1] = 1.0f;
		}
		else {
			Module->Gates[Module->Input_head[i]].Probability[0] = 1.0f;
			Module->Gates[Module->Input_head[i]].Probability[1] = 0.0f;
		}
		if (Module->Gates[Module->Input_head[i]].OutputNum > 1){
			for (int j = 0; j < Module->Gates[Module->Input_head[i]].OutputNum; j++){
				Module->Gates[Module->Gates[Module->Input_head[i]].Output[j]].Probability[0] 
					= Module->Gates[Module->Input_head[i]].Probability[0];
				Module->Gates[Module->Gates[Module->Input_head[i]].Output[j]].Probability[1] 
					= Module->Gates[Module->Input_head[i]].Probability[1];
			}
		}
	}
}


void CircuitProcess::DivideByGate(Struct_Module *Module)
{
	//Gate inLeve init
	for (int i = 0; i < MAX_GATES_IN_MODULE; i++){
		Module->Gates[i].inLevel = 1000;
	}

	Module->Level = 0;
	Struct_Gate temp;
	int x1, x2;
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

