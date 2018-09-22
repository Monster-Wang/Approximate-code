#ifndef _CIRCUITPROCESS__H
#define _CIRCUITPROCESS__H

#include "NetlistToMap.h"

class CircuitProcess{

public:
	CircuitProcess();
	~CircuitProcess();

	/*divide the circuit by gate*/
	void DivideByGate(Struct_Module *Module);

	/*if rnd = -1, the input vector will be generated randomly*/
	/*if rnd >=0, the input vector will be seted the value of rnd*/
	void InputGenerate(Struct_Module *Module,int rnd);

	/*calculating the value of all node in the circuit depend on the input 
	vector with the gate reliability is less then 1*/
	void smValueCal(Struct_Module *Module);

	/*calculating the value of all node in the circuit depend on the input 
	vector with the gate reliability is 1*/
	void fcValueCal(Struct_Module *Module);

};

#endif