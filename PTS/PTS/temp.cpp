

float CircuitProcess::add8_Q5test(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1, temp2;

	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){

		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;


		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)m_Module->Gates[iotemp[i]].sProbability) << ((i / 2) - 1);
			b |= ((int)m_Module->Gates[iotemp[i - 1]].sProbability) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)m_Module->Gates[iotemp[0]].sProbability);

		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			vector <int>::iterator iter = Module->LevelGate[i].begin();
			for (; iter != Module->LevelGate[i].end(); iter++){
				//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
				GatePGMcal(Module, *iter);
			}
		}

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size() - 7; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}


		temp1.Row = 1; temp1.Col = 8;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;

		float n140, n166, n154, n162, n182, n189;
		float ntemp, s[3] = { 0.0f };
		n140 = Module->Gates[140].sProbability;

		ntemp = 0;
		Lg->XORFunction(s[0], ntemp, Module->Gates[104].sProbability, Rg);
		Lg->ANDFunction(n154, ntemp, Module->Gates[21].sProbability, Rg);
		Lg->ANDFunction(n162, ntemp, Module->Gates[29].sProbability, Rg);
		Lg->ORFunction(n162, n162, Module->Gates[105].sProbability, Rg);
		Lg->ORFunction(n166, n162, n154, Rg);

		ntemp = 0;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);
		
		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n140)*(1 - n166)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n140)*n166*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[0], ntemp, Module->Gates[104].sProbability, Rg);
		Lg->ANDFunction(n154, ntemp, Module->Gates[21].sProbability, Rg);
		Lg->ANDFunction(n162, ntemp, Module->Gates[29].sProbability, Rg);
		Lg->ORFunction(n162, n162, Module->Gates[105].sProbability, Rg);
		Lg->ORFunction(n166, n162, n154, Rg);

		ntemp = 0;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n140*(1 - n166)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[1], ntemp, Module->Gates[111].sProbability, Rg);
		Lg->ANDFunction(n182, ntemp, Module->Gates[37].sProbability, Rg);
		Lg->ANDFunction(n189, ntemp, Module->Gates[47].sProbability, Rg);
		Lg->ORFunction(n189, n189, Module->Gates[112].sProbability, Rg);
		Lg->ORFunction(s[2], n189, n154, Rg);
		Lg->ORFunction(s[2], s[2], Module->Gates[117].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 3; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n140*n166*pmul;
		}
		ma->MatrixTensor(&temp1, &temp);

		temp1.Row = 1; temp1.Col = 2;
		for (int j = iotemp.size()-4; j < iotemp.size()-2; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}

		temp1.Row = 1; temp1.Col = 4;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;

		float n195,n203,n205;
		//float ntemp, s[2] = { 0.0f };
		n195= Module->Gates[195].sProbability;
		
		ntemp = 0;
		Lg->XORFunction(s[0], ntemp, Module->Gates[125].sProbability, Rg);

		Lg->ANDFunction(n203, ntemp, Module->Gates[87].sProbability, Rg);
		Lg->ANDFunction(n205, ntemp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n205, n205, n203, Rg);
		Lg->ORFunction(s[1], n205, Module->Gates[126].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 2; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n195)*pmul;
		}

		ntemp = 1;
		Lg->XORFunction(s[0], ntemp, Module->Gates[125].sProbability, Rg);

		Lg->ANDFunction(n203, ntemp, Module->Gates[87].sProbability, Rg);
		Lg->ANDFunction(n205, ntemp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n205, n205, n203, Rg);
		Lg->ORFunction(s[1], Module->Gates[205].sProbability, Module->Gates[126].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 2; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n195*pmul;
		}

		ma->MatrixTensor(&temp1, &temp);


		//找出每个输出向量的最大值，然后计算精确值和功能值之间的概率和
		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i]>temp.Matrix[0][fcValue])
				fcValue = i;
		}

		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += temp.Matrix[0][i];

		//cout << acValue << "   " << fcValue << endl;
		ret += accumP;
	}

	return (ret / ((float)pow(2, Module->InputNum)));
}


float CircuitProcess::add8_006test(){
	LogicFunction *Lg;
	Lg = new LogicFunction();

	Struct_Module *Module = m_Module;
	Matrix *ma = new Matrix();
	Struct_Matrix temp, temp1, temp2;

	float ret = 0.0f;

	for (int m = 0; m < (int)pow(2, Module->InputNum); m++){
		
		//the temp matrix initial
		temp.Col = temp.Row = 1;
		temp.Matrix[0][0] = 1;
		temp1.Row = 1; temp1.Col = 2;


		//init the primary input 
		InputInit(Module, iotemp, m);

		int acValue = 0, a = 0, b = 0;

		for (int i = iotemp.size() - m_Module->OutputNum - 1; i > 0; i -= 2){
			a |= ((int)m_Module->Gates[iotemp[i]].sProbability) << ((i / 2) - 1);
			b |= ((int)m_Module->Gates[iotemp[i - 1]].sProbability) << ((i / 2) - 1);
		}
		iotemp[0] < 0 ? acValue = a + b : acValue = a + b + ((int)m_Module->Gates[iotemp[0]].sProbability);

		//node pribability calculate
		for (int i = 1; i < Module->Level; i++){
			vector <int>::iterator iter = Module->LevelGate[i].begin();
			for (; iter != Module->LevelGate[i].end(); iter++){
				//根据逻辑门的编号计算逻辑门的PGM，即输出端信号概率
				GatePGMcal(Module, *iter);
			}
		}

		//张量积运算
		for (int j = iotemp.size() - Module->OutputNum; j < iotemp.size() - 4; j++){
			temp1.Matrix[0][0] = 1 - Module->Gates[iotemp[j]].sProbability;
			temp1.Matrix[0][1] = Module->Gates[iotemp[j]].sProbability;
			ma->MatrixTensor(&temp1, &temp);
		}


		temp1.Row = 1; temp1.Col = 16;
		for (int ii = 0; ii < temp1.Col; ii++)
			temp1.Matrix[0][ii] = 0.0;


		float n102, n112;
		float ntmp, s[4] = { 0.0f };
		n102 = Module->Gates[102].sProbability;

		//n102=0，n112=0
		ntmp = 0;
		Lg->XORFunction(s[0], ntmp, Module->Gates[72].sProbability, Rg);
		Lg->ANDFunction(s[1], ntmp, Module->Gates[73].sProbability, Rg);
		Lg->ORFunction(s[1], s[1], Module->Gates[76].sProbability, Rg);
		Lg->XORFunction(s[1], s[1], Module->Gates[81].sProbability, Rg);

		Lg->ANDFunction(n112, ntmp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n112, n112, Module->Gates[98].sProbability, Rg);

		ntmp = 0;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n102)*(1 - n112)*pmul;
		}

		//n102=0，n112=1
		ntmp = 1;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += (1 - n102)*n112*pmul;
		}

		//n102=1，n112=0
		ntmp = 1;
		Lg->XORFunction(s[0], ntmp, Module->Gates[72].sProbability, Rg);
		Lg->ANDFunction(s[1], ntmp, Module->Gates[73].sProbability, Rg);
		Lg->ORFunction(s[1], s[1], Module->Gates[76].sProbability, Rg);
		Lg->XORFunction(s[1], s[1], Module->Gates[81].sProbability, Rg);

		Lg->ANDFunction(n112, ntmp, Module->Gates[91].sProbability, Rg);
		Lg->ORFunction(n112, n112, Module->Gates[98].sProbability, Rg);

		ntmp = 0;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n102*(1 - n112)*pmul;
		}

		//n102=1，n112=1
		ntmp = 1;
		Lg->XORFunction(s[2], ntmp, Module->Gates[84].sProbability, Rg);
		Lg->ANDFunction(s[3], ntmp, Module->Gates[85].sProbability, Rg);
		Lg->ORFunction(s[3], s[3], Module->Gates[86].sProbability, Rg);

		for (int ii = 0; ii < temp1.Col; ii++){
			float pmul = 1.0;
			for (int jj = 0; jj < 4; jj++){
				if ((ii >> jj) & 1)
					pmul *= s[jj];
				else
					pmul *= (1 - s[jj]);
			}
			temp1.Matrix[0][ii] += n102*n112*pmul;
		}

		ma->MatrixTensor(&temp1, &temp);

        
		//找出每个输出向量的最大值，然后计算精确值和功能值之间的概率和
		int fcValue = 0; float accumP = 0;
		for (int i = 1; i < temp.Col; i++){
			if (temp.Matrix[0][i]>temp.Matrix[0][fcValue])
				fcValue = i;
		}

		for (int i = min(acValue, fcValue); i <= max(acValue, fcValue); i++)
			accumP += temp.Matrix[0][i];

		//cout << acValue << "   " << fcValue << endl;
		ret += accumP;
	}

	return (ret / ((float)pow(2, Module->InputNum)));
}