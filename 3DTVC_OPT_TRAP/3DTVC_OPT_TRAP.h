#pragma once
#include "GlobalDefinition.h"
#include "EcsInfo.h"
#include "CarInfo.h"

//利用全局函数重载 << 运算符(C++核心编程 p40, 4.5.2), 使其能输出坐标点对象
ostream& operator<<(ostream& cout, Coordinate& coord);

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出坐标点对象资源容量 [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);
//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 基站 信息，方便测试
ostream& operator<<(ostream& cout, EcsInfo& ecs);
//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 车辆 信息，方便测试
ostream& operator<<(ostream& cout, CarInfo& car);

//第一步：读入数据
//读入基站信息，一次性读完。
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile);
//读取车辆信息，分类型读入
void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, int CarNum, CarInfo* carp);


//第二步：分配函数，计算最优分配结果
int* TRAP_IP(CarInfo* carp);
int LessthanOrEqual(int a, int b);		//返回小于等于的整数左值
int GreaterthanOrEqual(int a, int b);	//返回大于等于的整数左值

//第三步：支付函数，计算各车辆付款
void TRAP_PAY(int* X);
//封装一个函数，把 Car[i] 踢出车辆集合Car[]。
//参数：env, model, cplex, 车辆数组[], 要踢出的车辆
void RemoveCar_i(CarInfo* carp, int id);
//封装一个函数，恢复 Car[i] 的信息
void RecoverCar_i(CarInfo* carp, int id);

//第四步：输出结果,保存到本地
void SaveResult();

void CalcResourceUtilizationRatio();	//计算资源利用率

void CalcServedCarRatio();	//计算服务车辆比例

