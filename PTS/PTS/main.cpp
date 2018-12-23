#include <windows.h>
#include "psapi.h"
#include <iostream>
#include "NetlistToMap.h"
#include "LogicFunction.h"
#include "CircuitProcess.h"
#include <string>

using namespace std;

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



int main(){
	NetlistToMap *circuitnet;
	LARGE_INTEGER StartTime, EndTime, Freq;



	char NetName[] = "mul8_024";  //网表文件名不加后缀，只需要修改此处即可，请确保资源文件后缀名为".isc"
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

	/*calculating reliability of the circuit*/
	float ret = cp->ReliabilityCal();

	QueryPerformanceCounter(&EndTime);
	float Seconds = ((double)EndTime.QuadPart - (double)StartTime.QuadPart)
		/ (double)(Freq.QuadPart);

	printf("circuit: %s\n", NetName);
	printf("The reliability of gates: %.2f\n", Rg);
	printf("The reliability of circuit: %.6f\n", ret);
	printf("Need Time: %.4fs\n", Seconds);
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	float memory = pmc.WorkingSetSize / 1024.0f / 1024.0f;
	printf("Need Memory: %.3fMb\n", memory);
	system("pause");

	return 0;
}