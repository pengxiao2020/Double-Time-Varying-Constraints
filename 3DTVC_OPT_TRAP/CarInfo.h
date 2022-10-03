#pragma once
#include "GlobalDefinition.h"

struct CarInfo
{
	//属性
	int id;			//车辆编号
	string type;	//车辆任务类型
	vector<int> RTV;        //车辆实时速度(时变): 以m/s为单位，范围: v = [5,10]m/s = [50,100]m/t, RTV = Real-Time Velocity
	vector<string> DD;      //随机车辆行驶方向: 直行,调头,左转,右转 straight / back / left / right, DD = DrivingDirection
							//仅在(30,30),(30,90),(90,30),(90,90)处改变行驶方向 
	vector<Coordinate> RTL;      //车辆实时位置, 服从均匀分布 U[0,L],  cl = Current Location

	//车辆任务请求 theta_i
	int at = 0;		//到达时间
	int dt = 0;		//任务截止时间
	int et = 0;		//任务执行时间
	struct Resource TRD;	//任务资源需求 TaskResourceDemand = TRD[4] = {s1, s2, s3, s4} = {CPU, GPU, Memory, Disk}
	double bid = 0.00;	//投标
	double bc = 0.00;	//投标系数
	double pay = 0.00;	//车辆付款

	//车辆的任务资源需求矩阵(Task Resource Demand Matrix)
	//vector<vector<int>> RDM = vector<vector<int>>(R, vector<int>(et, false));
	vector<vector<int>> RDM;

	//车辆的时变部署约束矩阵(Deployment Constraint Matrix)
	vector<vector<bool>> DCM = vector<vector<bool>>(M, vector<bool>(T, false));

	//车辆状态
	bool beAllocated = false;	//标记 车辆 i 是否得到分配

	///重载赋值运算符，使得两个车辆信息可以相互传递
	//lhs = left hand side, 赋值操作的目标
	//rhs = right hand side, 赋值操作的源头
	CarInfo& operator= (const CarInfo& rhs)
	{
		this->id = rhs.id;
		this->type = rhs.type;

		this->at = rhs.at;
		this->dt = rhs.dt;
		this->et = rhs.et;

		this->TRD.cpu = rhs.TRD.cpu;	//资源需求赋值
		this->TRD.gpu = rhs.TRD.gpu;
		this->TRD.memory = rhs.TRD.memory;
		this->TRD.disk = rhs.TRD.disk;

		this->bid = rhs.bid;
		this->bc = rhs.bc;
		this->pay = rhs.pay;

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(rhs.et, 0)));		//暂存 rhs.RDM
		rdm = rhs.RDM;
		this->RDM = rdm;

		vector<vector<bool>> dcm = vector < vector<bool>>(M, (vector<bool>(T, false)));	//暂存 rhs.DCM
		dcm = rhs.DCM;
		this->DCM = dcm;

		//for (int j = 0; j < M; j++)
		//{
		//	for (int t = 0; t < T; t++)
		//	{
		//		this->DCM[j][t] = rhs.DCM[j][t];	//时变部署约束矩阵矩阵赋值
		//	}
		//}

		this->beAllocated = rhs.beAllocated;	//分配标识赋值
		this->pay = rhs.pay;

		return *this;
	}

};

struct CarInfo Car[N];	//该结构体数组存放车辆任务信息
CarInfo* carP = Car;	//carP 指向 Car[N]

//定义一个车辆结构体，暂存被踢出的车辆信息
struct CarInfo RemovedCar;


