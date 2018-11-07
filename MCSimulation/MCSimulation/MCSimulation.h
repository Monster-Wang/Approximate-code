#ifndef _MCSIMULATION__H
#define _MCSIMULARION__H

#include "NetlistToMap.h"

#define Rg   0.96
#define Loopcm 1000000
#define Loopap 1000000

template <class T>
int getSize(T& array)
{
	return (sizeof(array) / sizeof(array[0]));
}


class MCSimulation{

public :
	MCSimulation(NetlistToMap *map);
	~MCSimulation();
	float Combination_CircuitTest();
	float Approximate_CircuitTest();
	float Mul_CircuitTest();

private:
	Struct_Module *m_Module;
	NetlistToMap *m_map;
	long er_count;
};


#endif