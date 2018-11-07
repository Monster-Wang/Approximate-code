#ifndef _CIRCUITPROCESS__H
#define _CIRCUITPROCESS__H

#include "NetlistToMap.h"

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

class CircuitProcess{

public:
	CircuitProcess(NetlistToMap *map);
	~CircuitProcess();
	/*divide the circuit by gate*/
	void DivideByGate(Struct_Module *Module);
	/*if rnd = -1, the input vector will be generated randomly*/
	/*if rnd >=0, the input vector will be seted the value of rnd*/
	void InputInit(Struct_Module *Module, vector<int>io, int rnd);
	float ReliabilityCal();
	float AXA1test();
	vector<int> IOPinGenerate(Struct_Module *Module);
	vector<recvg> RecvgDectect(Struct_Module *md);
	void PathSearch(vector<int> ph, int start, Struct_Module *md);
	float add8_006test();
	
private:
	Struct_Module *m_Module;
	vector<vector<int>> allPath;
	vector<recvg> RecvgCircuit;
	vector<int> iotemp;
};

#endif