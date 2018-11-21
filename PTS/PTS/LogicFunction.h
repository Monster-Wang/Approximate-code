#ifndef _LOGICFUNCTION_H_
#define _LOFICFUNCTION_H_


class LogicFunction{

public:
	LogicFunction();
	~LogicFunction();

	void ANDFunction(float& src, float& x1, float& x2, float rg);
	void ORFunction(float& src, float& x1, float& x2, float rg);
	void NOTFunction(float& src, float& x1, float rg);
	void NANDFunction(float& src, float& x1, float& x2, float rg);
	void NORFunction(float& src, float& x1, float& x2, float rg);
	void XORFunction(float& src, float& x1, float& x2, float rg);

};



#endif

