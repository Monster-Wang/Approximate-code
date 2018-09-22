#ifndef _LOGICFUNCTION_H_
#define _LOFICFUNCTION_H_


class LogicFunction{

public:
	LogicFunction();

	bool erAND(bool &x1, bool &x2, float rg);
	bool erOR(bool &x1, bool &x2, float rg);
	bool erNOT(bool &x1, float rg);
	bool erNAND(bool &x1, bool &x2, float rg);
	bool erNOR(bool &x1, bool &x2, float rg);
	bool erXOR(bool &x1, bool &x2, float rg);
	bool erCompare(int er, int rnd,bool val);
};



#endif

