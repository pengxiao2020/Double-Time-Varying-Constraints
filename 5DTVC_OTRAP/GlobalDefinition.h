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

//����Ŀ ��ע�� �Ǳ�Ҫ��ʾ�Ĳ��Դ��룬��Ҫ�����빦�ܣ�������ȡ��ע��

//ʵ��˼·��
// (1)���Դ���׶�
// ��·��ģ����ϵ��120 * 120
// ʱ��ۣ�T = 100
// ��վ�źŸ��ǰ뾶 30
// ��������N = 10
// (2)ʵ��׶�
// ��·��ģ����ϵ��1200 * 1200
// ʱ��ۣ�T = 2160
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

	bool operator==(Resource& rhs)	//���� �����������ʹ��Դ�ṹ������жϡ���ȹ�ϵ��
	{
		if ((this->cpu == rhs.cpu) && (this->gpu == rhs.gpu)
			&& (this->memory == rhs.memory) && (this->disk == rhs.disk))
		{
			return true;
		}
		else
			return false;
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

// 3.������Ϣ���
#define RS_S_CarNum 8000		//������Ѿ������ȡ��С�ͳ���������		
#define RS_L_CarNum 2000		//������Ѿ������ȡ�Ĵ��ͳ���������
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
const int count_DTVC_OTRAP = 10;	//DTVC_OTRAP ���� �ܵ�ִ�д���, ��Ϊ 10
int ExeNum = 1;						//�ڼ������� DTVC_OTRAP ���ƣ���ʼֵΪ 1

//��վ�� ������Ϣ �� ��Դ�������� �ֿ�����
#define EcsInfoFile "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"
#define EcsInfoFile_RCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"

//������ ������Ϣ, ʱ����Դ���� �� ʱ�䲿��Լ������ �ֿ�����
///exp3/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + "_v2/data_RS_CarInfo_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"
#define RS_CarInfoFile_S "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_CarInfo_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"		//С�ͳ���������Ϣ
#define RS_CarInfoFile_S_RDM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_RDM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//С�ͳ�������Դ�������
#define RS_CarInfoFile_S_DCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_DCM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//С�ͳ����Ĳ���Լ������

#define RS_CarInfoFile_L "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_CarInfo_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"		//���ͳ���������Ϣ
#define RS_CarInfoFile_L_RDM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_RDM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//���ͳ�������Դ�������
#define RS_CarInfoFile_L_DCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_DCM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//���ͳ����Ĳ���Լ������

// 5.����ʹ�õ��ı����Ͷ���
const double epsilon = 0.000001;	//����һ����С�����֣�DTVC_OTRAP_PAY ȷ������ʹ��

double TotalSocialWelfare = 0.00;	//��ֹ��[1,t], ������ḣ��
double cts_SocialWelfare = 0.00;	//��ǰʱ�����õ���ḣ�� cts = current tims slot

double RunTime = 0.00;					//OTRAP �㷨������ʱ��
double PreWork_RunTime = 0.00;			//1. OTRAP �㷨׼������������ʱ��
//double ReturnResource_RunTime = 0.00;	//1.1 �������� Cars �黹 ECS ��Դ������ʱ��
double DelOvertimeTask_RunTime = 0.00;	//1.2 ɾ�� UnAllocatedCar �еĹ��ڳ�������������ʱ��
double AddArrivedCar_RunTime = 0.00;	//1.3 ���µ���ĳ���������ӽ����� UnAllocatedCar ������ʱ�� 

double ALLOC_RunTime = 0.00;			//2.OTRAP_ALLOC ���亯������ʱ��

double PAY_RunTime = 0.00;				//3.OTRAP_PAY ֧����������ʱ��

struct Resource UsedResource;	//����ϵͳʹ���˵���Դ
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

int T_NormalPayedCarNum = 0;	//��������Ļ�ʤ��������
int S_NormalPayedCarNum = 0;	//��������Ļ�ʤС�ͳ�����
int L_NormalPayedCarNum = 0;	//��������Ļ�ʤ���ͳ�����

double T_NormalPayedCarRatio = 0;	//��������Ļ�ʤ��������
double S_NormalPayedCarRatio = 0;	//��������Ļ�ʤС�ͳ�����
double L_NormalPayedCarRatio = 0;	//��������Ļ�ʤ���ͳ�����
