#pragma once
#include "GlobalDefinition.h"
#include "EcsInfo.h"
#include "CarInfo.h"

//利用全局函数重载 << 运算符(C++核心编程 p40, 4.5.2), 使其能输出坐标点对象
ostream& operator<<(ostream& cout, Coordinate& coord);

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出坐标点对象资源容量 [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出基站信息，方便测试
ostream& operator<<(ostream& cout, EcsInfo& ecs);

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出车辆信息，方便测试
ostream& operator<<(ostream& cout, CarInfo& car);

//第一步：读入数据
vector<EcsInfo> vEcsInfo;	//存放所有基站信息的容器

vector<CarInfo> T_vCarInfo;	//存放所有车辆信息的容器

//vector<CarInfo> S_vCarInfo;	//存放小型车辆信息的容器
//vector<CarInfo> M_vCarInfo;	//存放中型车辆信息的容器
//vector<CarInfo> L_vCarInfo;	//存放大型车辆信息的容器

//读入基站信息，一次性读完。
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile);
//读入车辆信息，分类型读入
void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, vector<CarInfo>& Car, int CarNum);

//第二步：每个时间槽执行一次OTRAP算法
//vector<CarInfo> BeAllocatedCar;	//获得分配且正在执行任务的车辆集合
vector<CarInfo> UnAllocatedCar;	//未获得分配且处于任务有效期的车辆集合

vector<CarInfo> AllocResult;		//截止至[1,t]，获得分配的所有车辆集合
vector<CarInfo> cts_AllocResult;	//当前时间槽的分配结果 cts = current tims slot

vector<CarInfo> tmp_X;			//存放每次运行 OTRAP_ALLOC 后得到分配的车辆集合，下次运行 OTRAP_ALLOC 前清空

//1.DTVC_OTRAP 算法框架
void DTVC_OTRAP(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& Car);		//DTVC_OTRAP 算法框架

//2.DTVC_OTRAP 分配函数
void DTVC_OTRAP_ALLOC(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar);	// DTVC_OTRAP 分配函数
bool EcsInDegree_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2);	//按基站入度升序对基站排序
void CalcCarValueDensity(EcsInfo& ecs, int cts);	//计算：在当前时间槽 t ，能够连接 ecs 的所有车辆的资源密度 Car[i].f
bool CarValueDensity_Descending(const CarInfo& car1, const CarInfo& car2);	//按 车辆资源密度Car[i].f 非递增顺序对车辆排序
bool EscId_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2);	//根据 ECS_id 升序，重新排列 vEcsInfo 中 ECS 的顺序

//3.DTVC_OTRAP 支付函数（二分法）
void DTVC_OTRAP_PAY(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar, vector<CarInfo>& cts_AR);


//第三步：输出结果,保存到本地
void SaveResult();
void CalcResourceUtilizationRatio();	//计算资源利用率
void CalcServedCarRatio();		//计算服务车辆比例
void CalcNormalPayCarRatio();		//计算正常付款的车辆比例
