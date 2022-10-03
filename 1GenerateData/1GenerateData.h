#pragma once
#include "GlobalDefinition.h"
#include "CarInfo.h"
#include "EcsInfo.h"

//利用全局函数重载 << 运算符(C++核心编程 p40, 4.5.2), 使其能输出坐标点对象
ostream& operator<<(ostream& cout, Coordinate& coord);

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出坐标点对象资源容量 [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);

// 1.建模
void Modeling();

// 2.生成基站信息
//创造vector容器，用来存放生成的基站信息
vector<EcsInfo> vEcsInfo;

//1.生成基站信息
void GenerateEcsInfo();
//确认基站类型  CalcEcsType = Calculate Type of ECS 
void CalcEcsType(EcsInfo& ecs);
//计算基站的二维坐标位置  CalcEcsDL = Calculate Deployment Location of ECS 
void CalcEcsDL(EcsInfo& ecs);
//计算 ECS 的信号覆盖范围: 使用 vector 容器存放所有能被信号覆盖的坐标点(x,y)
void CalcEcsSCA(EcsInfo& ecs);

//第二部分：车辆相关
//存放车辆信息的容器：vector<CarInfo>
vector<CarInfo> vCarInfo;
//创造vector容器，分别用来存放不同类型的车辆信息
vector<CarInfo> S_vCarInfo;	//存放小型车辆的容器 S_vCarInfo = Small vCarInfo
vector<CarInfo> L_vCarInfo;	//存放大型车辆的容器 L_vCarInfo = Large vCarInfo

vector<int> vCarAT;			//存放车辆到达时间 ArrivalTime 的容器
vector<Coordinate> vCarIL;	//存放车辆初始位置 InitLoc 的容器
vector<string> vCarDD;		//存放车辆的 4 种行驶方向 DrivingDirection ：向上，向下，左行，右行
							//行驶方向是基于整个道路建模的角度而言的，而非车辆个体的直行,倒车,左转和右转
vector<double>vCarBC;		//存放车辆投标系数 BidCoefficient 的容器 
							//一半车辆的投标系数为[0.25,1],另一半车辆的投标系数为[1,4]
//1.生成车辆信息
void GenerateCarInfo();
//(1)生成车辆到达时间的函数
//产生符合 Normal Distribution 的随机数，用以生成车辆到达时间
void InitialCarArrivalTime();
//(2)生成车辆随机初始位置的函数
void InitialCarLocation();
//(3)生成车辆随机初始行驶方向的函数
void InitialCarDrivingDirection();
//(4)生成车辆随机投标系数的函数
void InitialCarBidCoefficient();
//(5)设置车辆类型的函数 
void SetCarType(CarInfo& car);
//(6)设置车辆初始行驶方向的函数
void SetCarInitialDrivingDirection(CarInfo& car);
//(7)设置车辆任务执行时间的函数
void SetCarExecutionTime(CarInfo& car);
//(8)设置车辆任务截止时间的函数
void SetCarDeadlineTime(CarInfo& car);
//(9)设置车辆任务任务资源需求(静态)的函数
void SetCarTaskResourceDemand(CarInfo& car);
//(10)设置车辆任务投标的函数
void SetCarBid(CarInfo& car);
//(11)计算车辆的全程实时驾驶信息:包括行驶方向,实时速度和实时位置
void CalcCarEntireDrivingInformation(CarInfo& car);
//(12)计算车辆的时变部署约束矩阵 Deployment Constraints Matrix
void CalcCarDeploymentConstraintsMatrix(CarInfo& car);
//(13)计算车辆的时变资源需求矩阵 Resource Demands Matrix
void CalcCarResourceDemandsMatrix(CarInfo& car);

//第三部分：将生成的数据保存到本地
//1.将基站信息保存到本地供用户查看
void SaveEcsInfo();

//2.将车辆信息(除可连接矩阵外)保存到本地
void SaveCarInfo();
//封装一个函数，把车辆的各个属性保存到不同文件中，供算法调入使用
void SaveCarInfoAsInput(vector<CarInfo>& Car, string type, int carnum);