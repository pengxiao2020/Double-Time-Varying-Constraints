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


//�ý����������Ҫ���ܣ�����ʵ�����õĴ����ݿⲢ�����ݱ��浽����


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
// 


// 1.��ģ
//(1)��ά��·��ģ
//const int X = 120, Y = 120;     // ������С����(�ɹ�)
const int X = 1200, Y = 1200;	// һ����1200�У�1200��
vector<vector <bool>> Road(X, vector<bool>(Y, false));	//Ĭ�ϸ�ֵΪ 0

//(2)ʱ�������
#define TTS 2160	//TTS = Total Time Slot
const int T = TTS;

//(3)��Դ������� ����4�࣬�ֱ�ָ�� CPU, GPU, Memory, Disk
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
#define tEcsNum (S_EcsNum+L_EcsNum)  //��վ����

int ver = 1;//��ʾ���� L_EcsNum = 2ʱ�����ֻ�վ�������
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

const int M = tEcsNum;
const int SCR = ECS_SCR;


// 3.������Ϣ���
#define S_CarNum 40000			//С�ͳ���������
#define L_CarNum 0			//���ͳ���������
#define tCarNum (S_CarNum+L_CarNum)  //������������

const int N = tCarNum;

struct ResourceCost
{
	double unitcost_cpu = 10;
	double unitcost_gpu = 40;
	double unitcost_memory = 1;
	double unitcost_disk = 0.03;
};
ResourceCost gamma;	//gamma�� 4 �൥λ��Դ��CPU,GPU,Memory,Disk���ĵ�λʱ��ʹ�óɱ�



