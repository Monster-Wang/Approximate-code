#ifndef _CIRCUITPROCESS__H
#define _CIRCUITPROCESS__H

#include "NetlistToMap.h"
#include "Matrix.h"
#include <set>

#define Rg  0.99


template <class T>
int getSize(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}

struct recvg{
	int fan;
	int cvg;
	vector<int>path1;
	vector<int>path2;
};

struct subCircuit{
	int primeFan;
	vector<int> fans;
	vector<int> outputs;
	vector<vector<int>> cir;
};

class CircuitProcess{

public:
	CircuitProcess(NetlistToMap *map);
	~CircuitProcess();
	/*divide the circuit by gate*/
	void DivideByGate(Struct_Module *Module);
	/*if rnd = -1, the input vector will be generated randomly*/
	/*if rnd >=0, the input vector will be seted the value of rnd*/
	void InputInit(Struct_Module *Module, vector<int>io, int rnd);

	vector<int> IOPinGenerate(Struct_Module *Module);
	vector<recvg> RecvgDectect(Struct_Module *md);
	void PathSearch(Struct_Module *md, vector<int> ph, int start);
	void RecvgRecal(Struct_Module *md, vector<recvg> rev);
	void GatePGMcal(Struct_Module *Module, int GateNode);
	
	void OutputPathfanDectect(Struct_Module *md, int outtag);
	vector<subCircuit> GetDependentoutput(Struct_Module *md);
	Struct_Matrix fantest(Struct_Module* md, subCircuit sC);

	vector<int> fcValueGenerate(Struct_Module* Module);

	float ReliabilityCal();
	float AXA1test();
	float add8_006test();
	float add8_Q5test();
	float add8_Q4test();

private:
	Struct_Module *m_Module;
	vector<vector<int>> allPath;
	vector<recvg> RecvgCircuit;
	vector<int> iotemp;
	vector<subCircuit> FanofOutpath;
	set<int> FanNode;
	vector<int> FCValue;
};

#endif