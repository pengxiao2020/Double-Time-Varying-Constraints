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
#include <functional>		//内建函数对象
#include <numeric>

#include <random>
#include <tchar.h>
#include <windows.h>

#include <utility>		//使用 pair 的库

using namespace std;

//本项目 已注释 非必要显示的测试代码，若要检查代码功能，请自行取消注释

//实验思路：
// (1)测试代码阶段
// 道路建模坐标系：120 * 120
// 时间槽：T = 100
// 基站信号覆盖半径 30
// 车辆数：N = 10
// (2)实验阶段
// 道路建模坐标系：1200 * 1200
// 时间槽：T = 2160
// 基站信号覆盖半径：300
// 车辆数：N = 100, 500, 5000, 10000, 20000


// 1.建模
//(1)二维道路建模
//const int X = 120, Y = 120;     // 测试用小矩阵(成功)
const int X = 1200, Y = 1200;	// 一共有1200行，1200列
vector<vector <bool>> Road(X, vector<bool>(Y, false));	//默认赋值为 0

//(2)时间槽设置
#define TTS 2160	//TTS = Total Time Slot
const int T = TTS;

//(3)资源种类个数 共有4类，分别指代 CPU, GPU, Memory, Disk（无用的变量）
#define RESOURCE 4;
const int R = RESOURCE;

// 定义 资源 结构体
// (1)基站资源容量,(2)车辆的任务资源需求向量
// 都使用该结构体
struct Resource
{
	//资源向量中包含四种资源
	int cpu = 0;	//资源:cpu
	int gpu = 0;	//资源:gpu
	int memory = 0;	//资源:memory
	int disk = 0;	//资源:disk

	Resource& operator=(const Resource& rhs)
	{
		this->cpu = rhs.cpu;
		this->gpu = rhs.gpu;
		this->memory = rhs.memory;
		this->disk = rhs.disk;
		return *this;
	}

	bool operator==(Resource& rhs)	//重载 等于运算符，使资源结构体可以判断“相等关系”
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
	int x = 0;		//表示 x 坐标的值
	int y = 0;		//表示 y 坐标的值

	//使坐标点对象能够赋值 (C++ 核心编程 4.5.4 p40)
	Coordinate& operator= (const Coordinate& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

	//重载 == 运算符, 使其能判断坐标点对象是否相等(C++ 核心编程 4.5.5)
	bool operator==(Coordinate& coord)
	{
		if ((this->x == coord.x) && (this->y == coord.y))
			return true;
		else
			return false;
	}
};

// 2.基站信息相关
#define ECS_SCR 300		//ECS的信号覆盖半径 统一为 300 m，无论类型, SCR = Signal Coverage Radius
#define S_EcsNum 8		//小型基站数
#define L_EcsNum 0		//大型基站数
#define T_EcsNum (S_EcsNum+L_EcsNum)  //基站总数

int ver = 2;//表示：当 L_EcsNum = 2时的两种基站部署情况
			//例如：当 L_EcsNum = 2 时,如果 ver = 1, 两台 L_EcsNum 的部署位置在(30,30)和(90,90); 
			//						   如果 ver = 2, 则部署位置在(30,90)和(90,30) 

#define S_ECS_RC_CPU 20			//小型基站CPU容量		40		50		100
#define S_ECS_RC_GPU 10			//小型基站GPU容量		20		50		100
#define S_ECS_RC_MEMORY	40		//小型基站MEMORY容量	80		200		400
#define S_ECS_RC_DISK 150		//小型基站DISK容量		500		1600	3200

#define L_ECS_RC_CPU 40 		//大型基站CPU容量			
#define L_ECS_RC_GPU 20			//大型基站GPU容量
#define L_ECS_RC_MEMORY	80		//大型基站MEMORY容量
#define L_ECS_RC_DISK 300		//大型基站DISK容量

const int M = T_EcsNum;
const int SCR = ECS_SCR;

// 3.车辆信息相关
#define RS_S_CarNum 8000		//读入的已经随机抽取的小型车辆任务数		
#define RS_L_CarNum 2000		//读入的已经随机抽取的大型车辆任务数
#define RS_T_CarNum (RS_S_CarNum + RS_L_CarNum)  //读入的已经随机抽取的车辆任务总数
const int N = RS_T_CarNum;

struct ResourceCost
{
	double unitcost_cpu = 10;
	double unitcost_gpu = 40;
	double unitcost_memory = 1;
	double unitcost_disk = 0.03;
};
ResourceCost gamma;	//gamma是 4 类单位资源（CPU,GPU,Memory,Disk）的单位时间使用成本

// 4.读入文件定义
const int count_DTVC_OTRAP = 10;	//DTVC_OTRAP 机制 总的执行次数, 设为 10
int ExeNum = 1;						//第几次运行 DTVC_OTRAP 机制，初始值为 1

//基站的 基本信息 和 资源容量矩阵 分开保存
#define EcsInfoFile "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"
#define EcsInfoFile_RCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum)+ ".txt"

//车辆的 任务信息, 时变资源请求 和 时变部署约束矩阵 分开保存
///exp3/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + "_v2/data_RS_CarInfo_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"
#define RS_CarInfoFile_S "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_CarInfo_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"		//小型车辆任务信息
#define RS_CarInfoFile_S_RDM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_RDM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//小型车辆的资源需求矩阵
#define RS_CarInfoFile_S_DCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_DCM_S_"+ to_string(RS_S_CarNum) + "-" + to_string(ExeNum) + ".txt"	//小型车辆的部署约束矩阵

#define RS_CarInfoFile_L "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_CarInfo_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"		//大型车辆任务信息
#define RS_CarInfoFile_L_RDM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_RDM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//大型车辆的资源需求矩阵
#define RS_CarInfoFile_L_DCM "../data/2RandomSampling/exp2/data_CarInfo_20%_8000_2000/data_RS_Car_DCM_L_"+ to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt"	//大型车辆的部署约束矩阵

// 5.程序使用到的变量和定义
const double epsilon = 0.000001;	//定义一个较小的数字，DTVC_OTRAP_PAY 确定付款使用

double TotalSocialWelfare = 0.00;	//截止至[1,t], 求解的社会福利
double cts_SocialWelfare = 0.00;	//当前时间槽求得的社会福利 cts = current tims slot

double RunTime = 0.00;					//OTRAP 算法的运行时间
double PreWork_RunTime = 0.00;			//1. OTRAP 算法准备工作所花的时间
//double ReturnResource_RunTime = 0.00;	//1.1 完成任务的 Cars 归还 ECS 资源所花的时间
double DelOvertimeTask_RunTime = 0.00;	//1.2 删除 UnAllocatedCar 中的过期车辆任务所花的时间
double AddArrivedCar_RunTime = 0.00;	//1.3 将新到达的车辆任务添加进集合 UnAllocatedCar 所花的时间 

double ALLOC_RunTime = 0.00;			//2.OTRAP_ALLOC 分配函数运行时间

double PAY_RunTime = 0.00;				//3.OTRAP_PAY 支付函数运行时间

struct Resource UsedResource;	//整个系统使用了的资源
struct Resource TotalResource;	//整个系统的资源总量

// TRU = Total Resource Utilization 总资源利用率
double TRU_cpu = 0.00;		//总CPU利用率
double TRU_gpu = 0.00;		//总GPU利用率
double TRU_memory = 0.00;	//总Memory利用率
double TRU_disk = 0.00;		//总Disk利用率

int T_ServedCarNum = 0;	//服务车辆数
int S_ServedCarNum = 0;	//服务的小型车辆数
int L_ServedCarNum = 0;	//服务的大型车辆数

double T_ServedCarRatio = 0.00;		//服务车辆比例
double S_ServedCarRatio = 0.00;		//小型车辆任务服务比例
double L_ServedCarRatio = 0.00;		//大型车辆任务服务比例

int T_NormalPayedCarNum = 0;	//正常付款的获胜车辆总数
int S_NormalPayedCarNum = 0;	//正常付款的获胜小型车辆数
int L_NormalPayedCarNum = 0;	//正常付款的获胜大型车辆数

double T_NormalPayedCarRatio = 0;	//正常付款的获胜车辆比例
double S_NormalPayedCarRatio = 0;	//正常付款的获胜小型车辆数
double L_NormalPayedCarRatio = 0;	//正常付款的获胜大型车辆数
