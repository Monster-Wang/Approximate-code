#include <windows.h>
#include "psapi.h"
#include <iostream>
#include "NetlistToMap.h"
#include "LogicFunction.h"
#include "CircuitProcess.h"
using namespace std;

int main(){
	NetlistToMap *circuitnet;

	float memory;
	double Seconds;
	LARGE_INTEGER StartTime, EndTime, Freq;
	
	//AMA2
	//InXA1
	//AXA1
	//add8_006               1
	//add8_ACA_I_N8_Q5       13
	//add8_ACA_II_N8_Q4      7
	//add8_GDA_St_N8_M8_P1   1
	//add8_GDA_St_N8_M4_P2   4
	//add8_GDA_St_N8_M8_P3   6
	//add8_GDA_St_N8_M8_P6   56
	//mul8_128
	//mul8_024
	//mul8_332

	char NetName[] = "add8_006";  //网表文件名没有不加后缀，只需要修改此处即可，请确保资源文件后缀名为".isc"
	char Netpath[100];
	strcpy(Netpath, "circuits\\");
	strcat(Netpath, NetName);
	strcat(Netpath, ".isc");

	circuitnet = new NetlistToMap(NetName);
	if (circuitnet->ParseNetlist(Netpath)){
		printf("Parse Success!\n");
	}
	else{
		printf("Parse Failed!\n");
		system("pause");
	}

	
	CircuitProcess *cp;
	cp = new CircuitProcess(circuitnet);

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	QueryPerformanceCounter(&StartTime);
	QueryPerformanceFrequency(&Freq);

	float ret = cp->ReliabilityCal();
	//float ret = cp->AXA1test();
	//float ret = cp->add8_006test();
	//float ret = cp->add8_Q5test();
	//float ret = cp->add8_Q4test();

	QueryPerformanceCounter(&EndTime);
	Seconds = ((double)EndTime.QuadPart - (double)StartTime.QuadPart)
		/ (double)(Freq.QuadPart);

	printf("circuit: %s\n", NetName);
	printf("The reliability of gates: %.2f\n", Rg);
	printf("The reliability of circuit: %.6f\n", ret);
	printf("Need Time: %.4fs\n", Seconds);
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	memory = pmc.WorkingSetSize / 1024.00;
	printf("Need Memory: %.1fKB\n", memory);
	system("pause");
	return 0;
}