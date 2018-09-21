#include "AS.h"

CAnt::CAnt(void)
{
}
CAnt::~CAnt(void)
{
}

CAntSystem::CAntSystem(void)
{
}
CAntSystem::~CAntSystem(void)
{

}

/*******************************************************************
* Function Name	: CAntSystem::InitData()
* Spec			: ��Ⱥ�㷨��ʼ���������ľ��룬��ʼ����������Ϣ��
* Input			: 
* Output		: 
*******************************************************************/
void CAntSystem::InitData()
{
	// ��ʼ��������·��Ϊһ�����ֵ
	cAntBest.totalPath = MAX_DIST;

	// ���������ľ���
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		for (int j = 0; j < SUBAREA_COUNT; j++)
		{
			float dx = subareaArray[i][0] - subareaArray[j][0];
			float dy = subareaArray[i][1] - subareaArray[j][1];
			float distTemp = sqrt(pow(dx, 2) + pow(dy, 2));
			distSubarea[i][j] = distTemp;
		}
	}

	// �����ʼ����Ϣ��ֵ
	float distTotal = 0.0;
	for (int i = 0; i < SUBAREA_COUNT - 1; i++)
	{
		distTotal = distTotal + distSubarea[i][i + 1];
	}
	distTotal = distTotal + distSubarea[SUBAREA_COUNT - 1][0];
	float trialZero = ANT_COUNT / distTotal;

	// ��ʼ����Ϣ��
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		for (int j = 0; j < SUBAREA_COUNT; j++)
		{
			trialSubarea[i][j] = trialZero;
		}
	}
}

/*******************************************************************
* Function Name	: CAntSystem::Search()
* Spec			: ������������������ȫ·�������·�������Ϣ��
* Input			:
* Output		:
*******************************************************************/
void CAntSystem::Search()
{
	// �����ڽ���ѭ��
	for (int i = 0; i < MAX_ITERATION; i++)
	{
		// ÿֻ��������һ��ȫ·��
		for (int j = 0; j < ANT_COUNT; j++)
		{
			cAntArray[j].Search();
		}

		// ����ѡȡ���·��������
		for (int j = 0; j < ANT_COUNT; j++)
		{
			if (cAntArray[j].totalPath < cAntBest.totalPath)
			{
				cAntBest = cAntArray[j];
			}
		}

		// ������Ϣ��
		UpdateTrial();

		// ���ÿ�ε�����·������
		cout << "����������" << i << " ��·����: " << cAntBest.totalPath << endl;
	}
}

/*******************************************************************
* Function Name	: CAnt::Search()
* Spec			: ÿֻ���ϵ�·������
* Input			:
* Output		:
*******************************************************************/
void CAnt::Search()
{
	// ��ʼ�����ϵĳ�����
	Init();

	// ֻҪ����û���߹�ȫ���������ͼ����ƶ�
	while (antVisitedSubareaCount < SUBAREA_COUNT)
	{
		Move();
	}

	// ÿ���������������з��������·������
	CalPathLength();
}

/*******************************************************************
* Function Name	: CAnt::Init()
* Spec			: ÿֻ���ϵĳ�ʼ��
* Input			:
* Output		:
*******************************************************************/
void CAnt::Init()
{
	// ��ʼ������ȥ����ûȥ���ķ���
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		// ����ȥ�������ı��
		antVisitedSubarea[i] = 0;

		// δȥ���ı�1��ȥ����0
		antUnvisitSubarea[i] = 1;
	}

	// �����߹�·�����ȳ�ʼ��Ϊ0
	totalPath = 0.0;

	// Ϊ�������ѡ��һ�������ķ���
	antCurSubareaNo = GetRandom(0, SUBAREA_COUNT);

	// ����ȥ���ķ���
	antVisitedSubarea[0] = antCurSubareaNo;

	// ����δȥ���ı��б��
	antUnvisitSubarea[antCurSubareaNo] = 0;

	// ȥ����������������
	antVisitedSubareaCount = 1;
}

/*******************************************************************
* Function Name	: CAnt::Move()
* Spec			: �����ƶ�����һ������
* Input			: 
* Output		: 
*******************************************************************/
void CAnt::Move()
{
	// ѡ����һ������
	int nextSubareaNo = SelectNextSubarea();

	// ��¼�����ƶ���·��
	antVisitedSubarea[antVisitedSubareaCount] = nextSubareaNo;

	// ����δȥ���ı��б��
	antUnvisitSubarea[nextSubareaNo] = 0;

	// ���µ�ǰ���������ķ���
	antCurSubareaNo = nextSubareaNo;

	// ��������ȥ������������
	antVisitedSubareaCount++;
}

/*******************************************************************
* Function Name	: CAnt::SelectNextSubarea()
* Spec			: ѡ����һ���ƶ��ķ���
* Input			:
* Output		:
*******************************************************************/
int CAnt::SelectNextSubarea()
{
	int nextSubarea = -1;

	float probTotal = 0.0;
	vector<float> prob(SUBAREA_COUNT, 0);
	// ���������֮���ѡ�����
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		if (antUnvisitSubarea[i] == 1)		// ûȥ���ķ���
		{
			prob[i] = pow(trialSubarea[antCurSubareaNo][i], ALPHA)*pow(1.0 / distSubarea[antCurSubareaNo][i], BETA);
			probTotal = probTotal + prob[i];
		}
		else                                // ȥ���ķ�����ѡ�и���Ϊ0
		{
			prob[i] = 0.0;
		}
	}

	// ��������ѡ��
	float probTemp = 0.0;
	if (probTotal > 0.0)
	{
		probTemp = GetRandom(0.0, probTotal);
		for (int i = 0; i < SUBAREA_COUNT; i++)
		{
			if (antUnvisitSubarea[i] == 1)
			{
				probTemp = probTemp - prob[i];
				if (probTemp < 0.0)
				{
					nextSubarea = i;
					break;
				}
			}
		}
	}

	// �����������Ϣ�غ�С�������ܺ�Ϊ0���򷵻���һ��ûȥ���ķ���
	if (nextSubarea == -1)
	{
		for (int i = 0; i < SUBAREA_COUNT; i++)
		{
			if (antUnvisitSubarea[i] == 1)
			{
				nextSubarea = i;
				break;
			}
		}
	}

	return nextSubarea;
}

/*******************************************************************
* Function Name	: CAnt::CalPathLength()
* Spec			: �����߹��������ܳ���
* Input			:
* Output		:
*******************************************************************/
void CAnt::CalPathLength()
{
	// ��ʼ��·���ܳ���Ϊ0
	totalPath = 0.0;

	int indexA = 0;
	int indexB = 0;
	for (int i = 1; i < SUBAREA_COUNT; i++)
	{
		indexA = antVisitedSubarea[i - 1];
		indexB = antVisitedSubarea[i];
		totalPath = totalPath + distSubarea[indexA][indexB];
	}
	// ������ͷ��β�ķ���
	indexA = antVisitedSubarea[0];
	totalPath = totalPath + distSubarea[indexA][indexB];
}

/*******************************************************************
* Function Name	: CAntSystem::UpdateTrial()
* Spec			: ������Ϣ��
* Input			:
* Output		:
*******************************************************************/
void CAntSystem::UpdateTrial()
{
	vector<vector<float>> trialTemp(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));

	// �����µ���Ϣ��
	int indexA = 0;
	int indexB = 0;
	for (int i = 0; i < ANT_COUNT; i++)
	{
		for (int j = 1; j < SUBAREA_COUNT; j++)
		{
			indexA = cAntArray[i].antVisitedSubarea[j - 1];
			indexB = cAntArray[i].antVisitedSubarea[j];
			trialTemp[indexA][indexB] = trialTemp[indexA][indexB] + Q / cAntArray[i].totalPath;
			trialTemp[indexB][indexA] = trialTemp[indexA][indexB];
		}
		// ͷ��β��������Ϣ��
		indexA = cAntArray[i].antVisitedSubarea[0];
		trialTemp[indexA][indexB] = trialTemp[indexA][indexB] + Q / cAntArray[i].totalPath;
		trialTemp[indexB][indexA] = trialTemp[indexA][indexB];
	}

	// ������Ϣ�صĻӷ�
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		for (int j = 0; j < SUBAREA_COUNT; j++)
		{
			trialSubarea[i][j] = trialSubarea[i][j] * RHO + trialTemp[i][j];
		}
	}
}

/*******************************************************************
* Function Name	: GetRandom
* Spec			: �õ�һ����Χ�ڵ��������
* Input			: ��С�����Χ
* Output		: �õ�һ���������
*******************************************************************/
int GetRandom(int numLow,int numUpper)
{
	//srand(time(0));
	return numLow + rand() / (RAND_MAX + 1)*(numUpper - numLow);
}

/*******************************************************************
* Function Name	: GetRandom
* Spec			: �õ�һ����Χ�ڵ����������
* Input			: ��С�����Χ
* Output		: �õ�һ�����������
*******************************************************************/
float GetRandom(float numLow, float numUpper)
{
	//srand(time(0));
	return numLow + rand() / ((float)RAND_MAX + 1.0)*(numUpper - numLow);
}

void main()
{
	// ��ʼ���������
	srand(time(0));

	// ��ʼ����
	CAntSystem antSystem;

	// ��ʼ������
	antSystem.InitData();

	// ��������������·��
	antSystem.Search();

	getchar();
}