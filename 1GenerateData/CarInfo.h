#pragma once
#include "GlobalDefinition.h"

struct CarInfo
{
    //属性
    int id;         //车辆编号:1~tCarNum，
    string type;    //车辆类型: S / M / L
    vector<int> RTV;        //车辆实时速度(时变): 以m/s为单位，范围: v = [5,10]m/s = [50,100]m/t, RTV = Real-Time Velocity
    vector<string> DD;      //随机车辆行驶方向: 直行,调头,左转,右转 straight / back / left / right, DD = DrivingDirection
                            //仅在(30,30),(30,90),(90,30),(90,90)处改变行驶方向 
    vector<Coordinate> RTL;      //车辆实时位置, 服从均匀分布 U[0,L],  cl = Current Location

    int at = 0;         //到达时间a[i], 服从正态分布 N[T/2,(T/8)^2], at = ArriveTime
    int dt = 0;         //任务完成截止时间 d[i], dt = DeadlineTime
    int et = 0;         //执行时间 e[i], et = ExecutionTime
    struct Resource TRD;    //任务资源需求 s[i]，共有四类资源：(CPU, GPU, Memory, Disk)
    double bc = 0.00;   //投标系数 mu[i], 服从均匀分布 U[0.25, 4], bc = BidCoefficient
    double bid = 0.00;      //投标b[i], b = Bid

    //车辆的时变资源需求矩阵(Task Resource Demand Matrix)
    vector<vector<int>> RDM;

    //车辆的时变部署约束矩阵(Deployment Constraint Matrix)
    vector<vector<bool>> DCM = vector<vector<bool>>(M, vector<bool>(T, false));

    //付款 Payment
    double pay = 0.00;       // p = Payment

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

		vector<vector<bool>> dcm = vector<vector<bool>>(M, (vector<bool>(T, false)));	//暂存 rhs.DCM
		dcm = rhs.DCM;
		this->DCM = dcm;

		return *this;
	}

};