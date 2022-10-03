#pragma once
#include "GlobalDefinition.h"

//EcsInfo 类用来定义基站信息

struct EcsInfo
{
	//属性
	int id;		//基站编号 1~tEcsNum

	string type;	//基站类型 S/M/L 

	int SCR = 0;	//基站的信号覆盖半径 统一为 300 m（无论大小）

	struct Coordinate DL;	//基站部署位置(DeploymentLocation)：井字道路中的位置,使用二维坐标表示(x,y)

	vector<Coordinate> SCA;	//基站信号覆盖范围 SignalCoverageArea：存放所有能够覆盖的坐标点

	struct Resource RC;	//资源容量向量，共有四类资源：(CPU, GPU, Memory, Disk)

	vector<vector<int>> RCM = vector<vector<int>>(R, vector<int>(T, 0));	//资源容量矩阵

	EcsInfo& operator= (const EcsInfo& rhs)
	{
		this->id = rhs.id;
		this->type = rhs.type;
		this->SCR = rhs.SCR;

		this->DL.x = rhs.DL.x;
		this->DL.y = rhs.DL.y;

		this->RC.cpu = rhs.RC.cpu;
		this->RC.gpu = rhs.RC.gpu;
		this->RC.memory = rhs.RC.memory;
		this->RC.disk = rhs.RC.disk;

		vector<vector<int>> rcm = vector<vector<int>>(R, vector<int>(T, 0));	//资源容量矩阵
		rcm = rhs.RCM;
		this->RCM = rcm;

		return *this;
	}
};
