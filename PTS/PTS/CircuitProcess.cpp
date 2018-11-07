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
	//detect the reconvert circuit
	RecvgCircuit = RecvgDectect(m_Module);
}


float CircuitProcess::ReliabilityCal(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1;

	float *src, *x1, *x2;
	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){
		//the temp matrix initial
		temp.Col = temp.Row = 1; 
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;

		//init the primary input 
		InputInit(Module,iotemp, m);
		
		int acValue = 0,a = 0, b = 0;

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
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

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size(); j++){
			temp1.Matrix[0][0] = Module->Gates[iotemp[j]].Probability[0];
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].Probability[1];
			ma->MatrixTensor(&temp1, &temp);
		}

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
	Struct_Matrix temp, temp1;

	float *src, *x1, *x2;
	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){
		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;
		int acValue = 0, a = 0, b = 0;

		//init the primary input 
		InputInit(Module, iotemp, m);

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
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
				default:
					cout << "The logic gate type not found!" << endl;
					break;
				}
			}
		}

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size(); j++){
			temp1.Matrix[0][0] = Module->Gates[iotemp[j]].Probability[0];
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].Probability[1];
			ma->MatrixTensor(&temp1, &temp);
		}

		float n102, n112;
		float s5, s6, s7, c1;




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


float CircuitProcess::AXA1test(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();

	float *src;
	float ret = 0.0f;
	float *x1, *x2;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){

		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)Module->Gates[iotemp[i]].Probability[1]) << ((i / 2) - 1);
			b |= ((int)Module->Gates[iotemp[i - 1]].Probability[1]) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)Module->Gates[iotemp[0]].Probability[1]);

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
				default:
					cout << "The logic gate type not found!" << endl;
					break;
				}
			}
		}


		float n26[2], n27[2], n28[2], n31[2];
		float n00 = 0, n01 = 0, n10 = 0, n11 = 0;

		n26[0] = Module->Gates[26].Probability[0];
		n26[1] = Module->Gates[26].Probability[1];

		n27[0] = 1.0; n27[1] = 0;
		n28[0] = n27[0]; n28[1] = n27[1];

		Lg->ANDFunction(n31, n27, Module->Gates[18].Probability, Rg);
		Lg->ORFunction(n31, n31, Module->Gates[22].Probability, Rg);
		Lg->NOTFunction(n31, n31, Rg);
		n00 += n26[0] * n31[0] * n28[0];
		n01 += n26[0] * n31[0] * n28[1];
		n10 += n26[0] * n31[1] * n28[0];
		n11 += n26[0] * n31[1] * n28[1];

		n27[0] = 0.0; n27[1] = 1.0;
		n28[0] = n27[0]; n28[1] = n27[1];

		Lg->ANDFunction(n31, n27, Module->Gates[18].Probability, Rg);
		Lg->ORFunction(n31, n31, Module->Gates[22].Probability, Rg);
		Lg->NOTFunction(n31, n31, Rg);
		n00 += n26[1] * n31[0] * n28[0];
		n01 += n26[1] * n31[0] * n28[1];
		n10 += n26[1] * n31[1] * n28[0];
		n11 += n26[1] * n31[1] * n28[1];

		float s0[] = { Module->Gates[14].Probability[0], Module->Gates[14].Probability[1] };
		vector <float>rettemp;
		rettemp = { n00*s0[0], n00*s0[1], n01*s0[0], n01*s0[1], n10*s0[0], n10*s0[1], n11*s0[0], n11*s0[1] };

		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < rettemp.size(); i++){
			if (rettemp[i]>rettemp[fcValue]){
				fcValue = i;
			}
		}

		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += rettemp[i];

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


vector<recvg> CircuitProcess::RecvgDectect(Struct_Module *md){

	vector<recvg> rv;
  
	for (int i = 1; i < md->Level; i++){
		vector<int>::iterator iter = md->LevelGate[i].begin();
		for (; iter != md->LevelGate[i].end(); iter++){
			if (md->Gates[*iter].OutputNum >= 2){
				int fan = *iter;
				vector<int> sPath;
				allPath.clear();
				PathSearch(sPath, fan, md);//search all the path of a fan node 

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


void CircuitProcess::PathSearch(vector<int> ph, int start,Struct_Module *md){

	ph.push_back(start);

	if (md->Gates[start].OutputNum == 0)//end the search
		allPath.push_back(ph);
	else if (md->Gates[start].OutputNum == 1)
		PathSearch(ph, (md->Gates[start].Output[0]), md);
	else if (md->Gates[start].OutputNum >= 2){
		int temp = start;
		for (int i = 0; i < md->Gates[start].OutputNum; i++)
			PathSearch(ph, md->Gates[start].Output[i], md);
	}
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
	/*long ipt = rnd;
	int rshf = 0;
	int i;
	io[0] < 0 ? i = 1 : i = 0;
	for (; i < io.size() - Module->OutputNum; i++, rshf++){
	if ((ipt >> rshf & 1)){
	Module->Gates[io[i]].Probability[0] = 0.0f;
	Module->Gates[io[i]].Probability[1] = 1.0f;
	}
	else {
	Module->Gates[io[i]].Probability[0] = 1.0f;
	Module->Gates[io[i]].Probability[1] = 0.0f;
	}
	if (Module->Gates[io[i]].OutputNum > 1){
	for (int j = 0; j < Module->Gates[io[i]].OutputNum; j++){
	Module->Gates[Module->Gates[io[i]].Output[j]].Probability[0]
	= Module->Gates[io[i]].Probability[0];
	Module->Gates[Module->Gates[io[i]].Output[j]].Probability[1]
	= Module->Gates[io[i]].Probability[1];
	}
	}
	}*/

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


