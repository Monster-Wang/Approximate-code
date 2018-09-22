#ifndef _CIRCUITPROCESS__H
#define _CIRCUITPROCESS__H

#include "NetlistToMap.h"

#define Rg  0.99


template <class T>
int getSize(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}

class CircuitProcess{

public:
	CircuitProcess(NetlistToMap *map);
	~CircuitProcess();
	/*divide the circuit by gate*/
	void DivideByGate(Struct_Module *Module);
	/*if rnd = -1, the input vector will be generated randomly*/
	/*if rnd >=0, the input vector will be seted the value of rnd*/
	void InputInit(Struct_Module *Module, int rnd);
	float ReliabilityCal();
	int* ioSeriesGenerate(Struct_Module *Module);
	
private:
	Struct_Module *m_Module;
};

#endif