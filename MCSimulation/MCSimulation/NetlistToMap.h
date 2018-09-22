#ifndef _NETLISTTOMAP_H_
#define _NETLISTTOMAP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <algorithm>
using namespace std;

//���õ�·����
#define MAX_INPUT_TO_GATE		40   //��·���߼��ŵ����������Ŀ
#define MAX_OUTPUT_TO_GATE		40   //��·���߼��ŵ�����ڵ���ȳ���Ŀ
#define MAX_INPUT_TO_MODULE		250  //��·���ԭʼ������Ŀ
#define MAX_OUTPUT_TO_MODULE	150  //��·�����ԭʼ�����Ŀ
#define MAX_GATES_IN_MODULE		4000 //��·����������
#define MAX_LEVELNUM            1000 //�ֲ��ֵò������� 
#define MAX_IN_GATES		    1300 //��·�����������

//�ڵ�����
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
	int Input[MAX_INPUT_TO_GATE];		//ָ�������
	int Output[MAX_OUTPUT_TO_GATE];		//ָ�������
	int	Type;							//�ڵ�����
	int Tag;							//�ڵ���
	int InputNum;
	int OutputNum;			            //ͳ�����롢����˿���
	char Name[10];						//�ŵ�����
	float Probability[4];               //�ڵ��źŸ���{p0��p0'��p1��p1'}
	int inLevel;
	bool NodeValue[2];
}Struct_Gate;


typedef struct
{
	int Input_head[MAX_INPUT_TO_MODULE];	//ÿ��ģ�����������
	int Output_head[MAX_OUTPUT_TO_MODULE];	//ÿ��ģ����������
	int NodeNum;							//ģ������Ľڵ���
	int GateNum;                            //ģ�����������
	int InputNum;
	int OutputNum;
	Struct_Gate Gates[MAX_GATES_IN_MODULE]; //�����������Ϣ,���������ž���������ڵ���
	char Name[30];                          //�����ļ���
	int Level;							    //ÿ��ģ������Ĳ����
	vector<int> LevelGate[MAX_LEVELNUM];    //һά��ʾ��ţ���ά��ʾ������ź�
}Struct_Module;


class NetlistToMap
{
public:
	NetlistToMap(char *modulename);
	~NetlistToMap();

	//������������ͼ����ʽ���д洢
	bool ParseNetlist(char *filename);
	//����ַ��Ƿ��ǿա�/t���س���*�������ַ�����Щ�ַ�Ӧ������
	bool IsSpecialChar(char c);
	//����ַ����Ƿ�������
	bool IsNumWord(char str[]);
	//���ݽڵ�������ͼ�������ýڵ����ţ�����0����û��������
	int SearchGateByName(char str[]);
	//���������·ģ��
	Struct_Module *FetchTotalModule();	
	
private:
	Struct_Module *m_TotalModule;
};

#endif