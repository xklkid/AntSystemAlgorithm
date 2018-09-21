#ifndef AS_H_
#define AS_H_

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

using namespace std;

const int ALPHA = 1;			// �������ӣ���Ϣ�صļ�Ȩ
const int BETA = 5;				// �������ӣ����м�ľ��뵹�����ܼ��ȣ��ļ�Ȩ
const float RHO = 0.5;			// ��Ϣ�ص�����ϵ��
const float Q = 1.0;			// ��Ϣ������ǿ��ϵ��
const int ANT_COUNT = 20;		// ���ϸ���
const int SUBAREA_COUNT = 20;	// ��������
const int MAX_ITERATION = 100;	// ����������
const float MAX_DIST = 10e9;	// ���ľ���

// ������ľ���
vector <vector<float>> distSubarea(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));
// ���������Ϣ��
vector <vector<float>> trialSubarea(SUBAREA_COUNT, vector<float>(SUBAREA_COUNT, 0.0));

// �����������
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
// ������
class CAnt
{
public:
	int antVisitedSubarea[SUBAREA_COUNT];		// �����߹��ķ���
	int antUnvisitSubarea[SUBAREA_COUNT];		// ����û�߹��ķ���
	float totalPath;							// �����߹���·������
	int antCurSubareaNo;						// ���ϵ�ǰ���ڷ������
	int antVisitedSubareaCount;					// ����ȥ������������
public:
	void Init();								// ��ʼ��
	void Move();								// �����ڷ������ƶ�
	void Search();								// ��������·��
	void CalPathLength();						// �������߹�·���ĳ���
	int SelectNextSubarea();					// ѡ����һ������
public:
	CAnt(void);
	~CAnt(void);
};

// ��Ⱥ��
class CAntSystem
{
public:
	CAnt cAntArray[ANT_COUNT];					// ��������
	CAnt cAntBest;								// ��������·��
public:
	void InitData();							// ���ݳ�ʼ��
	void Search();								// ·������
	void UpdateTrial();							// ������Ϣ��
public:
	CAntSystem(void);
	~CAntSystem(void);
};

int GetRandom(int numLow, int numUpper);
float GetRandom(float numLow, float numUpper);


#endif
