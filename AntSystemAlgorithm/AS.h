#ifndef AS_H_
#define AS_H_

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;

const int ALPHA = 1;			// 启发因子，信息素的加权
const int BETA = 5;				// 期望因子，城市间的距离倒数（能见度）的加权
const float RHO = 0.5;			// 信息素的蒸发系数
const float Q = 1.0;			// 信息素增加强度系数
const int ANT_COUNT = 20;		// 蚂蚁个数
const int SUBAREA_COUNT = 20;	// 分区个数
const int MAX_ITERATION = 100;	// 最大迭代次数
const float MAX_DIST = 10e9;	// 最大的距离

// 分区间的距离
vector <vector<float>> distSubarea(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));
// 分区间的信息素
vector <vector<float>> trialSubarea(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));

// 分区块的坐标
int subareaArray[SUBAREA_COUNT][SUBAREA_COUNT] = { { 35, 3 },
												{ 25, 27 },
												{ 107, 25 },
												{ 14, 20 },
												{ 140, 49 },
												{ 49, 22 },
												{ 86, 46 },
												{ 158, 42 },
												{ 144, 49 },
												{ 137, 72 },
												{ 139, 95 },
												{ 119, 80 },
												{ 105, 96 },
												{ 93, 85 },
												{ 67, 38 },
												{ 149, 57 },
												{ 45, 88 },
												{ 158, 113 },
												{ 168, 72 },
												{ 20, 107 } };
// 蚂蚁类
class CAnt
{
public:
	int antVisitedSubarea[SUBAREA_COUNT];		// 蚂蚁走过的分区
	int antUnvisitSubarea[SUBAREA_COUNT];		// 蚂蚁没走过的分区
	float totalPath;							// 蚂蚁走过的路径长度
	int antCurSubareaNo;						// 蚂蚁当前所在分区编号
	int antVisitedSubareaCount;					// 蚂蚁去过分区的数量
public:
	void Init();								// 初始化
	void Move();								// 蚂蚁在分区间移动
	void Search();								// 蚂蚁搜索路径
	void CalPathLength();						// 计算已走过路径的长度
	int SelectNextSubarea();					// 选择下一个分区
public:
	CAnt(void);
	~CAnt(void);
};

// 蚁群类
class CAntSystem
{
public:
	CAnt cAntArray[ANT_COUNT];					// 蚂蚁数组
	CAnt cAntBest;								// 最优蚂蚁路径
public:
	void InitData();							// 数据初始化
	void Search();								// 路径搜索
	void UpdateTrial();							// 更新信息素
public:
	CAntSystem(void);
	~CAntSystem(void);
};

int GetRandom(int numLow, int numUpper);
float GetRandom(float numLow, float numUpper);


#endif
