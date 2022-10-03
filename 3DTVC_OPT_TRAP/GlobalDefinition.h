#pragma once
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <deque>
#include <map>
#include <algorithm>
#include <functional>		//�ڽ���������
#include <numeric>

#include <random>
#include <tchar.h>
#include <windows.h>

#include <utility>		//ʹ�� pair �Ŀ�

using namespace std;


#include <ilcplex/ilocplex.h>

//����Ŀ ��ע�� �Ǳ�Ҫ��ʾ�Ĳ��Դ��룬��Ҫ�����빦�ܣ�������ȡ��ע��

// /../��ʾ����ǰĿ¼��һ��

//�ý����������Ҫ���ܣ�ʵ���������Ż��� OPT-TRAP�����������ȡ�����ݣ���������Դ����Ͷ��۷���


//ʵ��˼·��
// (1)���Դ���׶�
// ��·��ģ����ϵ��120 * 120
// ʱ��ۣ�T = 100
// ��վ�źŸ��ǰ뾶 30
// ��������N = 10
// (2)ʵ��׶�
// ��·��ģ����ϵ��1200 * 1200
// ʱ��ۣ�T = 1440
// ��վ�źŸ��ǰ뾶��300
// ��������N = 100, 500, 5000, 10000, 20000


// 1.��ģ
//(1)��ά��·��ģ
//const int X = 120, Y = 120;     // ������С����(�ɹ�)
const int X = 1200, Y = 1200;	// һ����1200�У�1200��
vector<vector <bool>> Road(X, vector<bool>(Y, false));	//Ĭ�ϸ�ֵΪ 0

//(2)ʱ�������
#define TTS 2160	//TTS = Total Time Slot
const int T = TTS;

//(3)��Դ������� ����4�࣬�ֱ�ָ�� CPU, GPU, Memory, Disk�����õı�����
#define RESOURCE 4;
const int R = RESOURCE;

// ���� ��Դ �ṹ��
// (1)��վ��Դ����,(2)������������Դ��������
// ��ʹ�øýṹ��
struct Resource
{
	//��Դ�����а���������Դ
	int cpu = 0;	//��Դ:cpu
	int gpu = 0;	//��Դ:gpu
	int memory = 0;	//��Դ:memory
	int disk = 0;	//��Դ:disk

	Resource& operator=(const Resource& rhs)
	{
		this->cpu = rhs.cpu;
		this->gpu = rhs.gpu;
		this->memory = rhs.memory;
		this->disk = rhs.disk;
		return *this;
	}
};

struct Coordinate
{
	int x = 0;		//��ʾ x �����ֵ
	int y = 0;		//��ʾ y �����ֵ

	//ʹ���������ܹ���ֵ (C++ ���ı�� 4.5.4 p40)
	Coordinate& operator= (const Coordinate& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

	//���� == �����, ʹ�����ж����������Ƿ����(C++ ���ı�� 4.5.5)
	bool operator==(Coordinate& coord)
	{
		if ((this->x == coord.x) && (this->y == coord.y))
			return true;
		else
			return false;
	}
};

// 2.��վ��Ϣ���
#define ECS_SCR 300		//ECS���źŸ��ǰ뾶 ͳһΪ 300 m����������, SCR = Signal Coverage Radius
#define S_EcsNum 8		//С�ͻ�վ��
#define L_EcsNum 0		//���ͻ�վ��
#define T_EcsNum (S_EcsNum+L_EcsNum)  //��վ����

int ver = 2;//��ʾ���� L_EcsNum = 2ʱ�����ֻ�վ�������
			//���磺�� L_EcsNum = 2 ʱ,��� ver = 1, ��̨ L_EcsNum �Ĳ���λ����(30,30)��(90,90); 
			//						   ��� ver = 2, ����λ����(30,90)��(90,30) 

#define S_ECS_RC_CPU 20			//С�ͻ�վCPU����		40		50		100
#define S_ECS_RC_GPU 10			//С�ͻ�վGPU����		20		50		100
#define S_ECS_RC_MEMORY	40		//С�ͻ�վMEMORY����	80		200		400
#define S_ECS_RC_DISK 150		//С�ͻ�վDISK����		500		1600	3200

#define L_ECS_RC_CPU 40 		//���ͻ�վCPU����			
#define L_ECS_RC_GPU 20			//���ͻ�վGPU����
#define L_ECS_RC_MEMORY	80		//���ͻ�վMEMORY����
#define L_ECS_RC_DISK 300		//���ͻ�վDISK����

const int M = T_EcsNum;
const int SCR = ECS_SCR;

string Status;					//��õĽ�����ʣ�Optimal��ʾ���Ž�
double SocialWelfare = 0.00;	//���������ḣ��
double RunTime = 0.00;			//TRAP-IP�㷨������ʱ��

// 3.������Ϣ���
#define RS_S_CarNum 500		//������Ѿ������ȡ��С�ͳ���������		
#define RS_L_CarNum 0		//������Ѿ������ȡ�Ĵ��ͳ���������
#define RS_T_CarNum (RS_S_CarNum + RS_L_CarNum)  //������Ѿ������ȡ�ĳ�����������
const int N = RS_T_CarNum;

struct ResourceCost
{
	double unitcost_cpu = 10;
	double unitcost_gpu = 40;
	double unitcost_memory = 1;
	double unitcost_disk = 0.03;
};
ResourceCost gamma;	//gamma�� 4 �൥λ��Դ��CPU,GPU,Memory,Disk���ĵ�λʱ��ʹ�óɱ�

// 4.�����ļ�����
const int count_DTVC_OPT_TRAP = 10;	//OPT_TRAP_EX���� �ܵ�ִ�д���, ��Ϊ 10
int ExeNum = 1;					//�ڼ������� OPT_TRAP_EX���ƣ���ʼֵΪ 1
int count_TRAP_IP = 1;			//��¼���ŷ����㷨 TRAP-IP() �����õĴ�������ʼֵΪ 1

#define EcsInfoFile "../data/2RandomSampling/exp1/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"
#define EcsInfoFile_RCM "../data/2RandomSampling/exp1/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"
//������ ������Ϣ, ʱ����Դ���� �� ʱ�䲿��Լ������ �ֿ�����
#define RS_CarInfoFile_S "../data/2RandomSampling/exp1/data_RS_CarInfo_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"		//С�ͳ���������Ϣ
#define RS_CarInfoFile_S_RDM "../data/2RandomSampling/exp1/data_RS_Car_RDM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//С�ͳ�����ʱ����Դ�������
#define RS_CarInfoFile_S_DCM "../data/2RandomSampling/exp1/data_RS_Car_DCM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//С�ͳ�����ʱ�䲿��Լ������

#define RS_CarInfoFile_L "../data/2RandomSampling/exp1/data_RS_CarInfo_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"		//���ͳ���������Ϣ
#define RS_CarInfoFile_L_RDM "../data/2RandomSampling/exp1/data_RS_Car_RDM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//���ͳ�����ʱ����Դ�������
#define RS_CarInfoFile_L_DCM "../data/2RandomSampling/exp1/data_RS_Car_DCM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//���ͳ�����ʱ�䲿��Լ������

int AllocResult[N];			//��ŷ�����������
double Payment[N];			//��ų������������

//����һ����ά����cnt[i][j][t]���������ϵͳ�����ӽ�� cnt = connectivity
//cnt[i][j][t] = cplex.getValue(x[i][j][t]);
int cnt[N][M][T];
//����һ����ά����tmp_cnt[i][j][t]���ݴ�����ϵͳ�����ӽ�����������ݻָ�
int tmp_cnt[N][M][T];


struct Resource UsedResource;	//����ϵͳʹ�õ���Դ
struct Resource TotalResource;	//����ϵͳ����Դ����

// TRU = Total Resource Utilization ����Դ������
double TRU_cpu = 0.00;		//��CPU������
double TRU_gpu = 0.00;		//��GPU������
double TRU_memory = 0.00;	//��Memory������
double TRU_disk = 0.00;		//��Disk������

int T_ServedCarNum = 0;	//��������
int S_ServedCarNum = 0;	//�����С�ͳ�����
int L_ServedCarNum = 0;	//����Ĵ��ͳ�����

double T_ServedCarRatio = 0.00;		//����������
double S_ServedCarRatio = 0.00;		//С�ͳ�������������
double L_ServedCarRatio = 0.00;		//���ͳ�������������

