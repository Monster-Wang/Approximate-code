#include "CircuitProcess.h"
#include "LogicFunction.h"
#include "MCSimulation.h"
#include <map>

CircuitProcess::CircuitProcess(){
	;
}

void CircuitProcess::smValueCal(Struct_Module *Module){
	bool x1, x2;
	LogicFunction Lg;

	for (int m = 1; m < Module->Level; m++){
		vector<int>::iterator iter = Module->LevelGate[m].begin();
		for (; iter != Module->LevelGate[m].end(); iter++){
			x1 = Module->Gates[Module->Gates[*iter].Input[0]].NodeValue[0];
			x2 = Module->Gates[Module->Gates[*iter].Input[1]].NodeValue[0];

			switch (Module->Gates[*iter].Type){
				case TYPE_AND:
					Module->Gates[*iter].NodeValue[0] = Lg.erAND(x1, x2, Rg);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[0];
							Module->Gates[*iter].NodeValue[0] = Lg.erAND(x1, Module->Gates[*iter].NodeValue[0], Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				case TYPE_OR:
					Module->Gates[*iter].NodeValue[0] = Lg.erOR(x1, x2, Rg);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[0];
							Module->Gates[*iter].NodeValue[0] = Lg.erOR(x1, Module->Gates[*iter].NodeValue[0], Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				case TYPE_NAND:
					Module->Gates[*iter].NodeValue[0] = Lg.erNAND(x1, x2, Rg);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[0];
							Module->Gates[*iter].NodeValue[0] = Lg.erNAND(x1, Module->Gates[*iter].NodeValue[0], Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				case TYPE_NOR:
					Module->Gates[*iter].NodeValue[0] = Lg.erNOR(x1, x2, Rg);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[0];
							Module->Gates[*iter].NodeValue[0] = Lg.erNOR(x1, Module->Gates[*iter].NodeValue[0], Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				case TYPE_XOR:
					Module->Gates[*iter].NodeValue[0] = Lg.erXOR(x1, x2, Rg);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[0];
							Module->Gates[*iter].NodeValue[0] = Lg.erXOR(x1, Module->Gates[*iter].NodeValue[0], Rg);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				case TYPE_NOT:
					Module->Gates[*iter].NodeValue[0] = Lg.erNOT(x1, Rg);
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[0] = Module->Gates[*iter].NodeValue[0];
							}
						}
					};break;
				default:
					printf("the gate type not found, please check the result of 'DivideByGate()' function is right!");
					break;
			}
		}
	}
}


void CircuitProcess::fcValueCal(Struct_Module *Module){
	bool x1, x2;
	LogicFunction Lg;

	for (int m = 1; m < Module->Level; m++){
		vector<int>::iterator iter = Module->LevelGate[m].begin();
		for (; iter != Module->LevelGate[m].end(); iter++){
			x1 = Module->Gates[Module->Gates[*iter].Input[0]].NodeValue[1];
			x2 = Module->Gates[Module->Gates[*iter].Input[1]].NodeValue[1];

			switch (Module->Gates[*iter].Type){
				case TYPE_AND:
					Module->Gates[*iter].NodeValue[1] = x1 & x2;

					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[1];
							Module->Gates[*iter].NodeValue[1] = x1 & Module->Gates[*iter].NodeValue[1];
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}
						}
					};break;
				case TYPE_OR:
					Module->Gates[*iter].NodeValue[1] = x1 | x2;
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[1];
							Module->Gates[*iter].NodeValue[1] = x1 | Module->Gates[*iter].NodeValue[1];
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}
						}
					};break;
				case TYPE_NAND:
					Module->Gates[*iter].NodeValue[1] = !(x1 & x2);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[1];
							Module->Gates[*iter].NodeValue[1] = !(x1 & Module->Gates[*iter].NodeValue[1]);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}
						}
					};break;
				case TYPE_NOR:
					Module->Gates[*iter].NodeValue[1] = !(x1 | x2);
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[1];
							Module->Gates[*iter].NodeValue[1] = !(x1 | Module->Gates[*iter].NodeValue[1]);
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}

						}
					};break;
				case TYPE_XOR:
					Module->Gates[*iter].NodeValue[1] = x1 ^ x2;
					//the logic gate's inputs more then two 
					if (Module->Gates[*iter].InputNum > 2){
						for (int p = 2; p < Module->Gates[*iter].InputNum; p++){
							x1 = Module->Gates[Module->Gates[*iter].Input[p]].NodeValue[1];
							Module->Gates[*iter].NodeValue[1] = x1 ^ Module->Gates[*iter].NodeValue[1];
						}
					}
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}
						}
					};break;
				case TYPE_NOT:
					Module->Gates[*iter].NodeValue[1] = !x1;
					//put the probability value for every fan cone of the gate
					if (Module->Gates[*iter].OutputNum > 1){
						for (int j = 0; j < Module->Gates[*iter].OutputNum; j++){
							for (int k = 0; k < 4; k++){
								Module->Gates[Module->Gates[*iter].Output[j]].NodeValue[1] = Module->Gates[*iter].NodeValue[1];
							}
						}
					};break;
				default:
					printf("the gate type not found, please check the result of 'DivideByGate()' function is right!");
					break;
			}
		}
	}
}


void CircuitProcess::InputGenerate(Struct_Module *Module,int rnd)
{
	long ipt;
	if (-1 == rnd)
		ipt = rand() % ((int)pow(2, Module->InputNum));
	else if (rnd >= 0)
		ipt = rnd;
	else
		printf("Input vector error, please set the paremeter correctly!");

	for (int i = 0; i < Module->InputNum; i++){
		Module->Gates[Module->Input_head[i]].NodeValue[0] = (ipt >> i & 1);
		Module->Gates[Module->Input_head[i]].NodeValue[1] = (ipt >> i & 1);
		if (Module->Gates[Module->Input_head[i]].OutputNum > 1){
			for (int j = 0; j < Module->Gates[Module->Input_head[i]].OutputNum; j++){
				Module->Gates[Module->Gates[Module->Input_head[i]].Output[j]].NodeValue[0] = Module->Gates[Module->Input_head[i]].NodeValue[0];
				Module->Gates[Module->Gates[Module->Input_head[i]].Output[j]].NodeValue[1] = Module->Gates[Module->Input_head[i]].NodeValue[1];
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


vector <int> CircuitProcess::IOPinGenerate(Struct_Module *Module){
	/*AXA1*/	//int iotemp[] = { 13, 10, 7, 4, 1, 15, 28, 31 };
	/*AMA2*/	//int iotemp[] = { 1, 2, 3, 4, 5, 23, 22, 13 };
	/*InXA1*/	//int iotemp[] = { 3, 1, 2, 4, 5, 10, 13, 11 };
	/*add8_ACA_I_N8_Q5*/	 //int iotemp[] = {-1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 };
	/*add8_ACA_II_N8_Q4*/	 //int iotemp[] = {-1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 };
	/*add8_GDA_St_N8_M8_P1*/ //int iotemp[] = {-1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 };

	//{65, 69, 42, 55, 97, 46, 4, 12}//第一个乘数，从低位到高位
	//{9, 1, 31, 60, 91, 23, 34, 82}//第二个乘数，从低位到高位
	//{145, 405, 250, 304, 354, 452, 404, 453, 406, 362, 407, 446, 454, 462, 470, 475}//积，从低位到高位
	map <string, vector<int>> ioterminal;
	vector <int> iotmp = { 13, 10, 7, 4, 1, 15, 28, 31 };
	ioterminal.insert(map <string, vector<int>>::value_type("AXA1", iotmp));
	iotmp.clear();

	iotmp = { 1, 2, 3, 4, 5, 23, 22, 13 };
	ioterminal.insert(map <string, vector<int>>::value_type("AMA2", iotmp));
	iotmp.clear();

	iotmp = { 3, 1, 2, 4, 5, 10, 13, 11 };
	ioterminal.insert(map <string, vector<int>>::value_type("InXA1", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 12, 17, 24, 31, 40, 49, 57, 65, 71, 77, 81, 85, 89, 93, 127, 153, 179, 199, 200, 201, 202, 206 };
	ioterminal.insert(map <string, vector<int>>::value_type("add8_ACA_I_N8_Q5", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 11, 15, 20, 25, 29, 33, 38, 43, 47, 51, 55, 59, 63, 67, 91, 113, 131, 116, 132, 119, 133, 137 };
	ioterminal.insert(map <string, vector<int>>::value_type("add8_ACA_II_N8_Q4", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 47, 52, 70, 71, 72, 73, 74, 75, 76, 80 };
	ioterminal.insert(map <string, vector<int>>::value_type("add8_GDA_St_N8_M8_P1", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 2, 3, 6, 9, 12, 15, 19, 23, 28, 33, 36, 39, 42, 45, 48, 51, 52, 88, 101, 107, 108, 115, 116, 118 };
	ioterminal.insert(map <string, vector<int>>::value_type("add8_006", iotmp));
	iotmp.clear();

	iotmp = { 65, 9, 69, 1, 42, 31, 55, 60, 97, 91, 46, 23, 4, 34, 12, 82, 145, 405, 250, 304, 354, 452, 404, 453, 406, 362, 407, 446, 454, 462, 470, 475 };
	ioterminal.insert(map <string, vector<int>>::value_type("mul8_128", iotmp));
	iotmp.clear();

	iotmp = {-1, 1, 4, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 66, 109, 129, 143, 144, 145, 146, 147, 151};
	ioterminal.insert(map <string, vector<int>>::value_type("add8_GDA_St_N8_M8_P3", iotmp));
	iotmp.clear();

	iotmp = { -1, 52, 27, 55, 1, 70, 79, 37, 82, 93, 73, 86, 30, 4, 42, 16, 59, 400, 419, 389, 175, 263, 470, 420, 469, 398, 401, 422, 452, 465, 473, 481, 486 };
	ioterminal.insert(map <string, vector<int>>::value_type("mul8_024", iotmp));
	iotmp.clear();

	iotmp = { -1, 1, 17, 18, 5, 74, 40, 59, 24, 45, 66, 52, 79, 8, 87, 32, 95, 246, 616, 237, 621, 121, 623, 622, 624, 512, 559, 307, 350, 448, 540, 322, 349 };
	ioterminal.insert(map <string, vector<int>>::value_type("mul8_332", iotmp));
	iotmp.clear();

	string str = Module->Name;
	vector <int> temp = ioterminal[str];
	return temp;
}