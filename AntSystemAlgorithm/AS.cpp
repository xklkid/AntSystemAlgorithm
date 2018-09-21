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
* Spec			: 蚁群算法初始化各分区的距离，初始化各分区信息素
* Input			: 
* Output		: 
*******************************************************************/
void CAntSystem::InitData()
{
	// 初始设置蚂蚁路径为一个最大值
	cAntBest.totalPath = MAX_DIST;

	// 计算分区间的距离
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

	// 计算初始化信息素值
	float distTotal = 0.0;
	for (int i = 0; i < SUBAREA_COUNT - 1; i++)
	{
		distTotal = distTotal + distSubarea[i][i + 1];
	}
	distTotal = distTotal + distSubarea[SUBAREA_COUNT - 1][0];
	float trialZero = ANT_COUNT / distTotal;

	// 初始化信息素
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
* Spec			: 计算所有蚂蚁搜索的全路径，更新分区间信息素
* Input			:
* Output		:
*******************************************************************/
void CAntSystem::Search()
{
	// 迭代内进行循环
	for (int i = 0; i < MAX_ITERATION; i++)
	{
		// 每只蚂蚁搜索一遍全路径
		for (int j = 0; j < ANT_COUNT; j++)
		{
			cAntArray[j].Search();
		}

		// 保留选取最佳路径的蚂蚁
		for (int j = 0; j < ANT_COUNT; j++)
		{
			if (cAntArray[j].totalPath < cAntBest.totalPath)
			{
				cAntBest = cAntArray[j];
			}
		}

		// 更新信息素
		UpdateTrial();

		// 输出每次迭代后路径长度
		cout << "迭代次数：" << i << " 径路长度: " << cAntBest.totalPath << endl;
	}
}

/*******************************************************************
* Function Name	: CAnt::Search()
* Spec			: 每只蚂蚁的路径搜索
* Input			:
* Output		:
*******************************************************************/
void CAnt::Search()
{
	// 初始化蚂蚁的出生地
	Init();

	// 只要蚂蚁没有走过全部分区，就继续移动
	while (antVisitedSubareaCount < SUBAREA_COUNT)
	{
		Move();
	}

	// 每个蚂蚁搜索完所有分区后计算路径长度
	CalPathLength();
}

/*******************************************************************
* Function Name	: CAnt::Init()
* Spec			: 每只蚂蚁的初始化
* Input			:
* Output		:
*******************************************************************/
void CAnt::Init()
{
	// 初始化蚂蚁去过和没去过的分区
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		// 保存去过分区的标号
		antVisitedSubarea[i] = 0;

		// 未去过的标1，去过标0
		antUnvisitSubarea[i] = 1;
	}

	// 蚂蚁走过路径长度初始化为0
	totalPath = 0.0;

	// 为蚂蚁随机选择一个出发的分区
	antCurSubareaNo = GetRandom(0, SUBAREA_COUNT);

	// 更新去过的分区
	antVisitedSubarea[0] = antCurSubareaNo;

	// 更新未去过的表中标号
	antUnvisitSubarea[antCurSubareaNo] = 0;

	// 去过分区的数量增加
	antVisitedSubareaCount = 1;
}

/*******************************************************************
* Function Name	: CAnt::Move()
* Spec			: 蚂蚁移动到下一个分区
* Input			: 
* Output		: 
*******************************************************************/
void CAnt::Move()
{
	// 选择下一个分区
	int nextSubareaNo = SelectNextSubarea();

	// 记录蚂蚁移动的路径
	antVisitedSubarea[antVisitedSubareaCount] = nextSubareaNo;

	// 更新未去过的表中标号
	antUnvisitSubarea[nextSubareaNo] = 0;

	// 更新当前蚂蚁所处的分区
	antCurSubareaNo = nextSubareaNo;

	// 更新蚂蚁去过分区的数量
	antVisitedSubareaCount++;
}

/*******************************************************************
* Function Name	: CAnt::SelectNextSubarea()
* Spec			: 选择下一个移动的分区
* Input			:
* Output		:
*******************************************************************/
int CAnt::SelectNextSubarea()
{
	int nextSubarea = -1;

	float probTotal = 0.0;
	vector<float> prob(SUBAREA_COUNT, 0);
	// 计算各分区之间的选择概率
	for (int i = 0; i < SUBAREA_COUNT; i++)
	{
		if (antUnvisitSubarea[i] == 1)		// 没去过的分区
		{
			prob[i] = pow(trialSubarea[antCurSubareaNo][i], ALPHA)*pow(1.0 / distSubarea[antCurSubareaNo][i], BETA);
			probTotal = probTotal + prob[i];
		}
		else                                // 去过的分区，选中概率为0
		{
			prob[i] = 0.0;
		}
	}

	// 进行轮盘选择
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

	// 如果分区间信息素很小，概率总和为0，则返回任一个没去过的分区
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
* Spec			: 计算走过分区的总长度
* Input			:
* Output		:
*******************************************************************/
void CAnt::CalPathLength()
{
	// 初始化路径总长度为0
	totalPath = 0.0;

	int indexA = 0;
	int indexB = 0;
	for (int i = 1; i < SUBAREA_COUNT; i++)
	{
		indexA = antVisitedSubarea[i - 1];
		indexB = antVisitedSubarea[i];
		totalPath = totalPath + distSubarea[indexA][indexB];
	}
	// 最后加上头和尾的分区
	indexA = antVisitedSubarea[0];
	totalPath = totalPath + distSubarea[indexA][indexB];
}

/*******************************************************************
* Function Name	: CAntSystem::UpdateTrial()
* Spec			: 更新信息素
* Input			:
* Output		:
*******************************************************************/
void CAntSystem::UpdateTrial()
{
	vector<vector<float>> trialTemp(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));

	// 计算新的信息素
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
		// 头和尾分区的信息素
		indexA = cAntArray[i].antVisitedSubarea[0];
		trialTemp[indexA][indexB] = trialTemp[indexA][indexB] + Q / cAntArray[i].totalPath;
		trialTemp[indexB][indexA] = trialTemp[indexA][indexB];
	}

	// 计算信息素的挥发
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
* Spec			: 得到一定范围内的随机整数
* Input			: 最小，最大范围
* Output		: 得到一个随机整数
*******************************************************************/
int GetRandom(int numLow,int numUpper)
{
	//srand(time(0));
	return numLow + rand() / (RAND_MAX + 1)*(numUpper - numLow);
}

/*******************************************************************
* Function Name	: GetRandom
* Spec			: 得到一定范围内的随机浮点数
* Input			: 最小，最大范围
* Output		: 得到一个随机浮点数
*******************************************************************/
float GetRandom(float numLow, float numUpper)
{
	//srand(time(0));
	return numLow + rand() / ((float)RAND_MAX + 1.0)*(numUpper - numLow);
}

void main()
{
	// 初始化随机种子
	srand(time(0));

	// 开始搜索
	CAntSystem antSystem;

	// 初始化数据
	antSystem.InitData();

	// 蚂蚁搜索出最优路径
	antSystem.Search();

	getchar();
}