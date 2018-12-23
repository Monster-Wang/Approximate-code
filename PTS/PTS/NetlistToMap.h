#ifndef _NETLISTTOMAP_H_
#define _NETLISTTOMAP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;

//设置电路属性
#define MAX_INPUT_TO_GATE		40   //电路中逻辑门的最大输入数目
#define MAX_OUTPUT_TO_GATE		40   //电路中逻辑门的输出节点端扇出数目
#define MAX_INPUT_TO_MODULE		250  //电路最大原始输入数目
#define MAX_OUTPUT_TO_MODULE	150  //电路中最大原始输出数目
#define MAX_GATES_IN_MODULE		4000 //电路中门数上限
#define MAX_LEVELNUM            1000 //分层后分得层数上限 
#define MAX_IN_GATES		    1300 //电路的最大宽度上限

//节点类型
#define TYPE_INVALID 0
#define TYPE_INPUT	 1
#define TYPE_FAN	 2
#define TYPE_AND	 3
#define TYPE_OR		 4
#define TYPE_NAND	 5
#define TYPE_NOR	 6
#define TYPE_NOT	 7
#define TYPE_XOR	 8
#define TYPE_BUFF    9
#define TYPE_MODULE	 100

typedef struct
{
	bool NodeValue[2];
	char Name[10];						//门的名称
	int Input[MAX_INPUT_TO_GATE];		//指向输入端
	int Output[MAX_OUTPUT_TO_GATE];		//指向输出端
	int	Type;							//节点类型
	int Tag;							//节点编号
	int InputNum;                       //门的输入端口数
	int OutputNum;			            //门输出端口数，包含扇出
	int inLevel;                        //门所在的层号
	float sProbability;                 //节点信号概率P{s=1}
}Struct_Gate;


typedef struct
{
	char Name[30];                          //网表文件名
	int Input_head[MAX_INPUT_TO_MODULE];	//每个模块的输入链表
	int Output_head[MAX_OUTPUT_TO_MODULE];	//每个模块的输出链表
	int NodeNum;							//模块包含的节点数
	int GateNum;                            //模块包含的门数
	int InputNum;                           //模块输入端口数
	int OutputNum;                          //模块输出端口数
	int Level;							    //每个模块包含的层次数
	vector<int> LevelGate[MAX_LEVELNUM];    //一维表示层号，二维表示本层的门号
	Struct_Gate Gates[MAX_GATES_IN_MODULE]; //网表包含门信息,数组索引号就是门输出节点编号
}Struct_Module;


class NetlistToMap
{
public:
	NetlistToMap(char *modulename);
	~NetlistToMap();

	//解析网表，导成图的形式进行存储
	bool ParseNetlist(char *filename);
	//检测字符是否是空、/t、回车、*等特殊字符，这些字符应该跳过
	bool IsSpecialChar(char c);
	//检测字符串是否是数字
	bool IsNumWord(char str[]);
	//根据节点名称在图中搜索该节点的序号，返回0代表没有搜索到
	int SearchGateByName(char str[]);
	//输出整个电路模块
	Struct_Module *FetchTotalModule();	
	
private:
	Struct_Module *m_TotalModule;
};

#endif