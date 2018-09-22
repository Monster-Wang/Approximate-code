#include <windows.h>
#include "psapi.h"
#include <iostream>
#include "NetlistToMap.h"
#include "MCSimulation.h"
#include "LogicFunction.h"


int main(){
	NetlistToMap *circuitnet;

	float memory;
	double Seconds;
	LARGE_INTEGER StartTime, EndTime, Freq;

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	QueryPerformanceCounter(&StartTime);
	QueryPerformanceFrequency(&Freq);

	char NetName[] = "AXA1";//网表文件名没有不加后缀，只需要修改此处即可，请确保资源文件后缀名为".isc"

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

	printf("gate relability:%2f\n", Rg);

	MCSimulation *mcsim;
	mcsim = new MCSimulation(circuitnet);

	float retcm = mcsim->Combination_CircuitTest();
	printf("combincircuit relability:%f\n", retcm);

	float retap = mcsim->Approximate_CircuitTest();
	printf("approxcircuit relability:%f\n", retap);

	
	


	QueryPerformanceCounter(&EndTime);
	Seconds = ((double)EndTime.QuadPart - (double)StartTime.QuadPart)
		/ (double)(Freq.QuadPart);
	printf("Need Time: %.4fs\n", Seconds);
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	memory = pmc.WorkingSetSize / 1024.00;
	printf("Need Memory: %.1fKB\n", memory);
	system("pause");
	return 0;
}