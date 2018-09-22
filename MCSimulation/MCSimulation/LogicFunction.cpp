#include "LogicFunction.h"
#include <stdlib.h>
#include <math.h>

LogicFunction::LogicFunction(){

	;
}

bool LogicFunction::erAND(bool &x1, bool &x2, float rg){
	
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = x1 & x2;

	return erCompare(er_rate, rnd, ret);
}

bool LogicFunction::erOR(bool&x1, bool&x2, float rg){
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = x1 | x2;

	return erCompare(er_rate, rnd, ret);
}

bool LogicFunction::erNOT(bool&x1, float rg){
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = !x1;
	
	return erCompare(er_rate, rnd, ret);
}

bool LogicFunction::erNAND(bool&x1, bool&x2, float rg){
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = !(x1 & x2);

	return erCompare(er_rate, rnd, ret);
}

bool LogicFunction::erNOR(bool&x1, bool&x2, float rg){
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = !(x1 | x2);
	
	return erCompare(er_rate, rnd, ret);

}

bool LogicFunction::erXOR(bool&x1, bool&x2, float rg){
	int er_rate = 100 - 100 * rg;
	int rnd = rand() % 100;
	bool ret = x1 ^ x2;

	return erCompare(er_rate, rnd, ret);
}

bool LogicFunction::erCompare(int er, int rnd, bool val){
	switch (er){
	case 1:
		if (0 == rnd)
			return !val;
		break;
	case 2:
		if (0 == rnd || 50 == rnd)
			return !val;
		break;
	case 3:
		if (0 == rnd || 34 == rnd || 67 == rnd)
			return !val;
		break;
	case 4:
		if (0 == rnd || 25 == rnd || 50 == rnd || 75 == rnd)
			return !val;
		break;
	case 5:
		if (0 == rnd || 20 == rnd || 40 == rnd || 60 == rnd || 80 == rnd)
			return !val;
		break;
	case 6:
		if (0 == rnd || 17 == rnd || 34 == rnd || 51 == rnd || 68 == rnd || 85 == rnd)
			return !val;
		break;
	case 7:
		if (0 == rnd || 14 == rnd || 28 == rnd || 42 == rnd || 56 == rnd || 70 == rnd || 84 == rnd)
			return !val;
		break;
	case 8:
		if (0 == rnd || 12 == rnd || 24 == rnd || 36 == rnd || 50 == rnd || 62 == rnd || 75 == rnd || 88 == rnd)
			return !val;
		break;
	case 9:
		if (0 == rnd || 11 == rnd || 22 == rnd || 33 == rnd || 44 == rnd || 55 == rnd || 66 == rnd || 77 == rnd || 88 == rnd)
			return !val;
		break;
	case 10:
		if (0 == rnd || 10 == rnd || 20 == rnd || 30 == rnd || 40 == rnd || 50 == rnd || 60 == rnd || 70 == rnd || 80 == rnd || 90 == rnd)
			return !val;
		break;
	default:
		break;
	}
	return val;
}

