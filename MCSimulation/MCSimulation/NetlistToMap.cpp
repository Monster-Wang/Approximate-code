#include "NetlistToMap.h"
#include <iostream>
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//initial model 
NetlistToMap::NetlistToMap(char *modulename)
{
	m_TotalModule = new Struct_Module;
	
	int i;
	m_TotalModule->NodeNum = m_TotalModule->GateNum = m_TotalModule->InputNum = m_TotalModule->OutputNum = 0;
	//m_TotalModule->Width = 0;
	for(i = 0; i <  MAX_INPUT_TO_MODULE; i++)
		m_TotalModule->Input_head[i] = NULL;
	for(i = 0; i <  MAX_OUTPUT_TO_MODULE; i++)
		m_TotalModule->Output_head[i] = NULL;
	
	memset(&m_TotalModule->Gates, 0, sizeof(m_TotalModule->Gates));

	strcpy(m_TotalModule->Name, modulename);

}


NetlistToMap::~NetlistToMap()
{
	delete m_TotalModule;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool NetlistToMap::ParseNetlist(char *filename)
{
	FILE *fp;
	char Line[122], Word[122], *ptr, *pend;
	bool Loop, Jump, OverIn, OverDef;
	int Place, Fir, i,k=0;
	int Temp, Child, Father;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return false;

	Jump = false;
	OverIn = false;
	OverDef = false;
	do {
		if (!fgets(Line, 120, fp)){
			Loop = false;
		}else
		{
			ptr = &Line [0];
			Place = 1;
			while (*ptr == ' ' || *ptr == '\t')
				ptr++;
			while (!(*ptr == 0 || *ptr == '*' || *ptr == '\n'))
			{
				for (i = 0, pend = ptr; !IsSpecialChar(*pend); pend++, i++)
					Word[i] = *pend;
				Word[i] = 0;
				
				if (Jump)
				{
					OverIn = true; 
					Fir = atoi(Word);
					if (Fir >= MAX_GATES_IN_MODULE)
					{
						fclose(fp);
						return false; // 超过整个模块的最大门数
					}

					Child = m_TotalModule->Gates[Fir].OutputNum;
					m_TotalModule->Gates[Fir].Output[Child++] = Temp;
					if (Child >= MAX_OUTPUT_TO_GATE)
					{
						fclose(fp);
						return false; // 超过最大的扇出数
					}
					m_TotalModule->Gates[Fir].OutputNum = Child;

					Father = m_TotalModule->Gates[Temp].InputNum;
					m_TotalModule->Gates[Temp].Input[Father++] = Fir;
					if (Father >= MAX_INPUT_TO_GATE)
					{
						fclose(fp);
						return false; // 超过最大的扇入数
					}
					m_TotalModule->Gates[Temp].InputNum = Father;
				}
				else
				{
					switch(Place)
					{
					case 1:
						Fir = atoi(Word);
						Temp = Fir;

						if (Temp >= MAX_GATES_IN_MODULE)
						{
							fclose(fp);
							return false; // 超过整个模块的最大门数
						}
						memset(&m_TotalModule->Gates[Temp], 0, sizeof(Struct_Gate));
						m_TotalModule->NodeNum++;
                        m_TotalModule->GateNum++;
						m_TotalModule->Gates[Temp].Tag = Fir;
						break;

					case 2:						
						strcpy(m_TotalModule->Gates[Temp].Name, Word);
						break;

					case 3:						
						if (!strcmp(Word, "inpt") || !strcmp(Word, "INPT"))
						{
							m_TotalModule->GateNum--;
							m_TotalModule->Gates[Temp].Type = TYPE_INPUT;
							Jump = false;
							Fir = m_TotalModule->InputNum;
							m_TotalModule->Input_head[Fir++] = Temp;
							if ( Fir >= MAX_INPUT_TO_MODULE)
							{
								fclose(fp);
								return false; //超过一个模块所允许的最大扇入数
							}
							m_TotalModule->InputNum = Fir;
							OverDef = true;
						}
						else if (!strcmp(Word, "from") || !strcmp(Word, "FROM"))
						{
							m_TotalModule->GateNum--;
							m_TotalModule->Gates[Temp].Type = TYPE_FAN;
							Jump = false;
						}
						else if (!strcmp(Word, "and") || !strcmp(Word, "AND"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_AND;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "or") || !strcmp(Word, "OR"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_OR;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "nand")|| !strcmp(Word, "NAND"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_NAND;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "nor") || !strcmp(Word, "NOR"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_NOR;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "not") || !strcmp(Word, "NOT"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_NOT;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "buff")||!strcmp(Word, "BUFF"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_BUFF;
							Jump = true;
							OverDef = true;
							break;
						}
						else if (!strcmp(Word, "xor") || !strcmp(Word, "XOR"))
						{
							m_TotalModule->Gates[Temp].Type = TYPE_XOR;
							Jump = true;
							OverDef = true;
							break;
						}
						else
						{
							fclose(fp);
							return false; //非法的节点类型
						}
						break;

					case 4:
						if (m_TotalModule->Gates[Temp].Type == TYPE_FAN)
						{
							if (Father = SearchGateByName(Word))
							{
								Fir = m_TotalModule->Gates[Father].OutputNum;
								m_TotalModule->Gates[Father].Output[Fir++] = Temp;
								if (Fir >= MAX_OUTPUT_TO_GATE)
									return false; // 超过最大的扇出数
								m_TotalModule->Gates[Father].OutputNum = Fir;

								Fir = m_TotalModule->Gates[Temp].InputNum;
								m_TotalModule->Gates[Temp].Input[Fir++] = Father;
								if (Fir >= MAX_INPUT_TO_GATE)
									return false; // 超过最大的扇入数
								m_TotalModule->Gates[Temp].InputNum = Fir;

								OverDef = true;
							}
							else
							{
								fclose(fp);
								return false; //搜索的目标节点不存在
							}
						}

						break;
					}
				}

				Place++;
				if (OverDef)
				{
					OverDef = false;
					break;
				}
				ptr = pend;
				while (*ptr == ' ' || *ptr == '\t')
					ptr++;
			}

			if (OverIn)
			{
				OverIn = false;
				Jump = false;
			}
		}
	} while (Loop);

	fclose(fp);

	for (i = 1; i <  MAX_GATES_IN_MODULE; i++)
	{
		if ((m_TotalModule->Gates[i].OutputNum)==0 && (m_TotalModule->Gates[i].Tag)!=TYPE_INVALID)
		{
			Fir = m_TotalModule->OutputNum;
			m_TotalModule->Output_head[Fir++] = i;
			if (Fir >= MAX_OUTPUT_TO_MODULE)
				return false; //超过最大扇出端口数
			m_TotalModule->OutputNum = Fir;
		}
	}
	//test
 	/*printf("PI number is%d\n", m_TotalModule->InputNum);
	for(i=0;i<(m_TotalModule->InputNum);i++)
		printf("PI is%d, ",m_TotalModule->Input_head [i]);//for test
	printf("PO number is%d\n", m_TotalModule->OutputNum);
	for(i=0;i<(m_TotalModule->OutputNum);i++)
		printf("PO is%d, ",m_TotalModule->Output_head [i]);//for test*/
	
	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool NetlistToMap::IsSpecialChar(char c)
{
	if (c==' ' || c=='\t' || c==0 || c=='*' || c=='\n')
		return true;
	return false;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool NetlistToMap::IsNumWord(char str[])
{
	int i;

	for(i = 0; str[i] != 0; i++)
		if (str[i] <  '0' || str[i] > '9')
			return false;

	return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int NetlistToMap::SearchGateByName(char str[])
{
	int i;
	
	for ( i = 1; i <   MAX_GATES_IN_MODULE; i++)
		if (!strcmp(m_TotalModule->Gates[i].Name, str))
			return i;

	 return 0;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
Struct_Module *NetlistToMap:: FetchTotalModule()
{
	return m_TotalModule;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


