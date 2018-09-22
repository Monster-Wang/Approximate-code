#include "CircuitProcess.h"
#include "LogicFunction.h"
#include "MCSimulation.h"

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