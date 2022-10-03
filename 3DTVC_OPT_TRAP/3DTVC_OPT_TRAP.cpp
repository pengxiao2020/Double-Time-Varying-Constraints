#include "3DTVC_OPT_TRAP.h"
#include <ilcplex/ilocplex.h>


/*
TRAP-IP调试信息：
第一种方案（舍）：
1.将 约束1 中的条件表达式 写在了 for(int t=0; t<T; t++) 之后，而不是之前
2.将目标函数，约束2，分配矩阵赋值函数，计算资源利用率的遍历条件改为：for (int t = 0; t < T; t++)
而不是for (int t = carp[i].at; (t <= (carp[i].dt - carp[i].et + 1)); t++)

第二种方案(采用)：
1.将 约束1 中的条件表达式 写在了 for(int t=0; t<T; t++) 之后，而不是之前
2.TRAP-IP 中：将目标函数，约束2，分配矩阵赋值函数；TRAP-PAY中：计算资源利用率的遍历条件为：
for (int t = carp[i].at; (t <= (carp[i].dt - carp[i].et + 1)); t++)

*/

ILOSTLBEGIN

//自定义数据结构
typedef IloArray <IloBoolArray> Bool2dArray;		//二维布尔类型数组
typedef IloArray <Bool2dArray> Bool3dArray;			//三维布尔类型数组

typedef IloArray <IloIntArray> Int2dArray;		//二维整数数组
typedef IloArray <IloIntArray> Float2dArray;	//二维浮点数数组
typedef IloArray <Int2dArray> Int3dArray;		//三维整数数组

typedef IloArray <IloIntVarArray> Int2dVarArray;	//二维整数变量数组
typedef IloArray <IloIntVarArray> Float2dVarArray;	//二维浮点数变量数组
typedef IloArray <Int2dVarArray> Int3dVarArray;		//三维整数变量数组

typedef IloArray <IloNumVarArray> Num2dVarArray;	//二维变量数组
typedef IloArray <Num2dVarArray> Num3dVarArray;		//三维变量数组

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出坐标点对象(x,y)
ostream& operator<<(ostream& cout, Coordinate& coord)
{
	cout << "(" << coord.x << "," << coord.y << ")";
	return cout;
}

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出坐标点对象资源容量 [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd)
{
	cout << "[" << trd.cpu << "," << trd.gpu << "," << trd.memory << "," << trd.disk << "]";
	return cout;
}

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 基站 信息，方便测试
ostream& operator<<(ostream& cout, EcsInfo& ecs)
{
	cout << "ECS[" << ecs.id << "]的id = " << ecs.id << ",    ";
	cout << "type = " << ecs.type << ",    ";
	cout << "SCR = " << ecs.SCR << ",    ";
	cout << "DL = " << ecs.DL << ",    ";
	cout << "RC = " << ecs.RC << ",    ";
	cout << "RCM = " << endl;
	for (auto rit = ecs.RCM.begin(); rit != ecs.RCM.end(); rit++)
	{
		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
		{
			cout << (*cit) << " ";
		}
		cout << endl;
	}
	return cout;
}

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 车辆 信息，方便测试
ostream& operator<<(ostream& cout, CarInfo& car)
{
	cout << "Car[" << car.id << "] 的 id = " << car.id << ",  ";
	cout << "type = " << car.type << ",  ";
	cout << "at = " << car.at << ",  ";
	cout << "dt = " << car.dt << ",  ";
	cout << "et = " << car.et << ",  ";
	cout << "TRD = " << car.TRD << ",  ";
	cout << "bid(bc) = " << car.bid << "(" << car.bc << "),  ";
	cout << "pay = " << car.pay << endl;
	cout << "RDM = " << endl;
	for (auto rit = car.RDM.begin(); rit != car.RDM.end(); rit++)
	{
		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
		{
			cout << (*cit) << " ";
		}
		cout << endl;
	}
	cout << "DCM = " << endl;
	for (auto rit = car.DCM.begin(); rit != car.DCM.end(); rit++)
	{
		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
		{
			cout << (*cit) << " ";
		}
		cout << endl;
	}
	return cout;
}


int LessthanOrEqual(int a, int b)
{
	if (a <= b)
		return a;
	else
		return b;
}

int GreaterthanOrEqual(int a, int b)
{
	if (a >= b)
		return a;
	else
		return b;
}

void RemoveCar_i(CarInfo* carp, int id)
{
	//备份被移除车辆的信息
	RemovedCar = carp[id];

	for (int i = 0; i < N; i++)	//备份全局系统连接矩阵
	{
		for (int j = 0; j < M; j++)
		{
			for (int t = 0; t < T; t++)
			{
				tmp_cnt[i][j][t] = cnt[i][j][t];
			}
		}
	}

	carp[id].bid = 0.00;		//被踢出的车辆 id 的投标置 0 

	carp[id].TRD.cpu = 0;	//被踢出的车辆 id 的资源需求置 0
	carp[id].TRD.gpu = 0;
	carp[id].TRD.memory = 0;
	carp[id].TRD.disk = 0;

	for (int r = 0; r < R; r++)
	{
		for (int t = 0; t < carp[id].et; t++)
		{
			carp[id].RDM[r][t] = 0;	//被踢出的车辆 id 的时变资源需求矩阵置 0
		}
	}

	for (int j = 0; j < M; j++)
	{
		for (int t = 0; t < T; t++)
		{
			if (carp[id].DCM[j][t] != 0 || cnt[id][j][t] != 0)
			{
				carp[id].DCM[j][t] = 0;	//被踢出的车辆 id 的部署约束矩阵 DCM 置 0 
				cnt[id][j][t] = 0;	//被踢出的车辆 id 的系统连接矩阵 cnt[id] 置 0 
			}
		}
	}
}

void RecoverCar_i(CarInfo* carp, int id)
{
	carp[id] = RemovedCar;	//恢复被踢出车辆 carp[id] 的信息

	//恢复系统连接矩阵 cnt[i][j][t]
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			for (int t = 0; t < T; t++)
			{
				cnt[i][j][t] = tmp_cnt[i][j][t];
			}
		}
	}
}

void CalcResourceUtilizationRatio()
{
	//统计被使用的资源
	//根据 cons1 和 cons2 , 在算法运行过程[1,T]中，车辆 x[i] 只会在某个时刻 t 连接一次基站。
	//假如车辆 i 在某个时间槽 t 已经连接过某个基站 j (x[i][j][t] = 1) or (Connected = 1)，那么不必再考察剩余时间的情况
	
	//int Connected = 0;
	//for (int i = 0; i < N; i++)
	//{
	//	for (int j = 0; j < M && (Connected != 1); j++)
	//	{
	//		//for (int t = carP[i].at; t <= carP[i].dt - carP[i].et + 1 && (Connected != 1); t++)	//原式
	//		for (int t = carP[i].at - 1; t <= carP[i].dt - carP[i].et && (Connected != 1); t++)
	//		{
	//			//仅当 连接结果 * 可连接矩阵 = 1 (x[i][j][t] * delta[i][j][t])时，才能够确定“一定连接”
	//			if (cnt[i][j][t] * carP[i].DCM[j][t] == 1)
	//			//if(carP[i].beAllocated == true)
	//			{
	//				Connected = 1;
	//				//统计使用了的资源总量
	//				UsedResource.cpu += carP[i].TRD.cpu;
	//				UsedResource.gpu += carP[i].TRD.gpu;
	//				UsedResource.memory += carP[i].TRD.memory;
	//				UsedResource.disk += carP[i].TRD.disk;
	//			}
	//		}
	//	}
	//	Connected = 0;
	//}

	for (int i = 0; i < N; i++)
	{
		//统计使用了的资源
		if (carP[i].beAllocated == true)
		{
			//统计使用了的资源总量
			UsedResource.cpu += carP[i].TRD.cpu;
			UsedResource.gpu += carP[i].TRD.gpu;
			UsedResource.memory += carP[i].TRD.memory;
			UsedResource.disk += carP[i].TRD.disk;
		}
	}

	//统计资源容量总量
	for (int j = 0; j < M; j++)
	{
		//统计所有基站的资源总量
		TotalResource.cpu += ECS[j].RC.cpu;
		TotalResource.gpu += ECS[j].RC.gpu;
		TotalResource.memory += ECS[j].RC.memory;
		TotalResource.disk += ECS[j].RC.disk;
	}

	//计算资源利用率
	if (M != 0)
	{
		TRU_cpu = (double)UsedResource.cpu / ((double)TotalResource.cpu * (double)T);
		TRU_gpu = (double)UsedResource.gpu / ((double)TotalResource.gpu * (double)T);
		TRU_memory = (double)UsedResource.memory / ((double)TotalResource.memory * (double)T);
		TRU_disk = (double)UsedResource.disk / ((double)TotalResource.disk * (double)T);
	}
}

void CalcServedCarRatio()
{
	/*for (int i = 0; i < N; i++)
	{
		int cnt_i = 0;
		if (carP[i].beAllocated == true)
		{
			for (int j = 0; j < M && (cnt_i != 1); j++)
			{
				for (int t = 0; t < T && (cnt_i != 1); t++)
				{
					cnt_i = cnt[i][j][t] * carP[i].DCM[j][t];
					if (cnt_i == 1)
					{
						T_ServedCarNum++;
						if (carP[i].type == "S")
						{
							S_ServedCarNum++;
						}
						else if (carP[i].type == "L")
						{
							L_ServedCarNum++;
						}
					}
				}
			}
		}
	}*/

	for (int i = 0; i < N; i++)
	{
		if (carP[i].beAllocated == true)
		{
			T_ServedCarNum++;
			if (carP[i].type == "S")
			{
				S_ServedCarNum++;
			}
			else if (carP[i].type == "L")
			{
				L_ServedCarNum++;
			}
		}
	}

	if (N != 0)
	{
		T_ServedCarRatio = double((double)T_ServedCarNum / (double)N);
	}
	if (RS_S_CarNum != 0)
	{
		S_ServedCarRatio = double((double)S_ServedCarNum / (double)T_ServedCarNum);
	}
	if (RS_L_CarNum != 0)
	{
		L_ServedCarRatio = double((double)L_ServedCarNum / (double)T_ServedCarNum);
	}
}

void LoadEcsInfo(string EcsInfofile, string EcsRCMfile)
{
	ifstream ifs_ECS(EcsInfofile);		//基站信息(除资源容量矩阵)文件
	ifstream ifs_ECS_RCM(EcsRCMfile);	//基站的资源容量矩阵文件
	for (int j = 0; j < M; j++)	//读入基站信息
	{
		ifs_ECS >> ECS[j].id;
		ifs_ECS >> ECS[j].type;
		ifs_ECS >> ECS[j].SCR;
		ifs_ECS >> ECS[j].DL.x;
		ifs_ECS >> ECS[j].DL.y;
		ifs_ECS >> ECS[j].RC.cpu;
		ifs_ECS >> ECS[j].RC.gpu;
		ifs_ECS >> ECS[j].RC.memory;
		ifs_ECS >> ECS[j].RC.disk;

		vector<vector<int>> rcm = vector<vector<int>>(R, (vector<int>(T, 0)));
		for (auto rit = rcm.begin(); rit != rcm.end(); rit++)	//读入资源容量矩阵
		{
			for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			{
				int tmp;
				ifs_ECS_RCM >> tmp;
				(*cit) = tmp;
			}
		}
		ECS[j].RCM = rcm;

	}
	ifs_ECS.close();
	ifs_ECS_RCM.close();

	int count_S_ECS = 0, count_L_ECS = 0;
	for (int j = 0; j < M; j++)
	{
		if (ECS[j].type == "S")
			count_S_ECS++;
		else if (ECS[j].type == "L")
			count_L_ECS++;
		else
			cout << "基站类型错误！请重新运行程序，再读入文件！" << endl;
	}
	if (count_S_ECS != 0)
	{
		cout << "成功读入小型基站信息：" << count_S_ECS << " 条！" << endl;
	}
	if (count_L_ECS != 0)
	{
		cout << "成功读入大型基站信息：" << count_L_ECS << " 条！" << endl;
	}
}

void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, int CarNum, CarInfo* carp)
{
	ifstream ifs_Car(CarInfofile);		//车辆任务信息文件
	ifstream ifs_Car_RDM(CarRDMfile);	//车辆的时变资源需求矩阵
	ifstream ifs_Car_DCM(CarDCMfile);	//车辆的时变部署约束矩阵
	for (int i = 0; i < CarNum; i++)	//读入车辆信息
	{
		ifs_Car >> carp[i].id;
		ifs_Car >> carp[i].type;
		ifs_Car >> carp[i].at;
		ifs_Car >> carp[i].dt;
		ifs_Car >> carp[i].et;
		ifs_Car >> carp[i].TRD.cpu;
		ifs_Car >> carp[i].TRD.gpu;
		ifs_Car >> carp[i].TRD.memory;
		ifs_Car >> carp[i].TRD.disk;
		ifs_Car >> carp[i].bid;
		ifs_Car >> carp[i].bc;
		carp[i].beAllocated = false;
		carp[i].pay = 0.00;

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(carp[i].et, 0)));
		for (auto rit = rdm.begin(); rit != rdm.end(); rit++)	//读入时变资源需求矩阵
		{
			for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			{
				int tmp;
				ifs_Car_RDM >> tmp;
				(*cit) = tmp;
			}
		}
		carp[i].RDM = rdm;

		vector<vector<bool>> dcm = vector<vector<bool>>(M, (vector<bool>(T, false)));
		for (auto rit = dcm.begin(); rit != dcm.end(); rit++)	//读入时变部署约束矩阵
		{
			for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			{
				bool tmp;
				ifs_Car_DCM >> tmp;
				(*cit) = tmp;
			}
		}
		carp[i].DCM = dcm;
	}
	ifs_Car.close();
	ifs_Car_RDM.close();
	ifs_Car_DCM.close();

	int count_S_Car = 0, count_L_Car = 0;
	for (int i = 0; i < CarNum; i++)
	{
		if (carp[i].type == "S")
			count_S_Car++;
		else if (carp[i].type == "L")
			count_L_Car++;
		else
			cout << "车辆类型错误！请重新运行程序，再读入文件！" << endl;
	}
	if (count_S_Car != 0)
	{
		cout << "成功读入小型车辆信息：" << count_S_Car << " 条！" << endl;
	}
	if (count_L_Car != 0)
	{
		cout << "成功读入大型车辆信息：" << count_L_Car << " 条！" << endl;
	}
}

int* TRAP_IP(CarInfo* carp)
{
	int* X = new int[N];	//定义一个临时数组存放分配结果,然后输出。	X[i] = x[i][j][t] * carp[i].CM[j][t]

	IloEnv env;
	try
	{
		IloModel model(env);
		//创建求解对象
		IloCplex cplex(model);

		//1.设置未知变量（决策变量）
		//三维决策变量 X={x[1],...x[i],...x[N]}，其中，每个x[i]都是一个M*T的二维数组
		Num3dVarArray x(env, N);
		for (IloInt i = 0; i < N; i++)		//i < N
		{
			x[i] = Num2dVarArray(env, M);	//每辆车都要考虑与所有基站 M 连接的情形
			for (IloInt j = 0; j < M; j++)	//j < M
			{
				x[i][j] = IloNumVarArray(env, T);	//在整个时间槽[1,T]内考虑是否相连
				for (IloInt t = 0; t < T; t++)
				{
					//x[i][j][t] : 车辆i在时间点t与基站j 相连
					//也即约束3：x[i][j][t] 只能取 [0,1]
					x[i][j][t] = IloNumVar(env, 0, 1, ILOINT);
					//只有在 t[i] = [ a[i], d[i] - e[i] + 1 ], 分配才是有效的
					/*if ((t < CT[i].at) || (t > (CT[i].dt - CT[i].et + 1)))
					{
						model.add(x[i][j][t] < 1);
					}*/
					model.add(x[i][j][t]);
				}
			}
		}

		//2.设置目标函数
		// Maximize V = sigma {b[i] * delta[i][j][t] * x[i][j][t]}
		IloExpr V_max(env);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				//for (int t = carp[i].at - 1; t <= (carp[i].dt - carp[i].et + 1); t++) // 原条件
				// 下标从 0 开始
				for (int t = carp[i].at - 1; t <= (carp[i].dt - carp[i].et); t++)
				{
					V_max += carp[i].bid * carp[i].DCM[j][t] * x[i][j][t];
				}
			}
		}
		model.add(IloMaximize(env, V_max));
		V_max.end();



		//第四步：添加约束条件
		/* 约束1 :
		   如果基站 j 与 车辆 i 在时间点 ω 建立连接（x[i][j][ω] = 1），那么，
		   在接下来的时间段[ω= t- ei+1, t] = [ω, ω+ei]（i 的执行时间）内，其余所有用户的请求都要小于当前基站的资源容量，
		   否则，不予分配。换句话说，
		   如果 基站 j 满足 车辆 i 的请求，且 j 的剩余资源还可以满足其他车辆的请求，那么 j 就会分配给 车辆 i 资源；
		   如果分配给 i 资源后， j 不能再满足其他车辆的请求，那就不分配给 i 资源。
		   即：分配资源时，基站 j 总要保证自己能随时服务于车辆请求。*/
		for (int j = 0; j < M; j++)
		{
			for (int t = 0; t < T; t++)
			{
				IloExpr cons1_cpu(env);
				IloExpr cons1_gpu(env);
				IloExpr cons1_memory(env);
				IloExpr cons1_disk(env);
				for (int r = 0; r < R; r++)
				{
					for (int i = 0; i < N; i++)
					{
						//if (t >= carp[i].at && t <= carp[i].dt)	//原式
						if (t >= carp[i].at - 1 && t <= carp[i].dt - 1)
						{
							//车辆接收分配的时间 t 的区间范围：[a[i], d[i]-e[i]+1]
							//最晚不能晚于 可接收分配时间 d[i]-e[i]+1
							//最早不得早于 a[i]
							//for (int omega = LessthanOrEqual(t, carp[i].dt - carp[i].et + 1); omega >= GreaterthanOrEqual(t - carp[i].et + 1, carp[i].at); omega--)	//原式
							for (int omega = LessthanOrEqual(t, carp[i].dt - carp[i].et); omega >= GreaterthanOrEqual(t - carp[i].et + 1, carp[i].at - 1); omega--)
							{
								////静态资源需求向量约束
								//cons1_cpu += carp[i].TRD.cpu * carp[i].DCM[j][omega] * x[i][j][omega];
								//cons1_gpu += carp[i].TRD.gpu * carp[i].DCM[j][omega] * x[i][j][omega];
								//cons1_memory += carp[i].TRD.memory * carp[i].DCM[j][omega] * x[i][j][omega];
								//cons1_disk += carp[i].TRD.disk * carp[i].DCM[j][omega] * x[i][j][omega];

								//时变资源请求约束
								//原代码中：carp[i].RDM[r][t-omega]
								if(r==0)
									cons1_cpu += carp[i].RDM[r][t - omega] * carp[i].DCM[j][t] * x[i][j][omega];
								else if(r==1)
									cons1_gpu += carp[i].RDM[r][t - omega] * carp[i].DCM[j][t] * x[i][j][omega];
								else if(r==2)
									cons1_memory += carp[i].RDM[r][t - omega] * carp[i].DCM[j][t] * x[i][j][omega];
								else	// r==3
									cons1_disk += carp[i].RDM[r][t - omega] * carp[i].DCM[j][t] * x[i][j][omega];
							}
						}
					}
				}
				model.add(cons1_cpu <= ECS[j].RC.cpu);
				model.add(cons1_gpu <= ECS[j].RC.gpu);
				model.add(cons1_memory <= ECS[j].RC.memory);
				model.add(cons1_disk <= ECS[j].RC.disk);

				//清空约束 cons1
				cons1_cpu.end();
				cons1_gpu.end();
				cons1_memory.end();
				cons1_disk.end();
			}
		}

		//约束2 cons2 : 
		//可连接矩阵 delta[i][j][t] 和 确认连接矩阵 x[i][j][t] 的乘积只能取[0,1]
		for (int i = 0; i < N; i++)
		{
			IloExpr cons2(env);
			for (int j = 0; j < M; j++)
			{
				//约束2 cons2
				//for (int t = carp[i].at; t <= (carp[i].dt - carp[i].et + 1); t++)	//原式
				for (int t = carp[i].at - 1; t <= (carp[i].dt - carp[i].et); t++)
				{
					cons2 += carp[i].DCM[j][t] * x[i][j][t];
				}
			}
			model.add(0 <= cons2 <= 1);
			cons2.end();
		}

		cplex.solve();

		//处理异常 & 显示结果
		if (!cplex.solve())
		{
			env.error() << "未能求解出问题的解." << endl;
			throw(-1);
		}

		//格式：
		//vals（数组）：存放求得的决策变量(vars)：x1,x2,x3 的值
		//cplex.getStatus()输出Optimal，表示求得的解是最优解
		//cplex.getObjValue()输出202.5，表示目标函数求解的最大值
		//vals(数组)：输入[40, 17.5, 42.5]，表示决策变量 x1, x2, x3的值
		//cplex.getValues(存放结果的容器名，统一的模型变量名) = cplex.getValues(三维数组名，vars) ：取一行的值
		//cplex.getValue() : 取一列的值

		if (count_TRAP_IP == 1)	//求解的最大社会福利 和 算法运行时间 仅在第 1 次执行 TRAP-IP 时保存
		{
			Status = cplex.getStatus();
			SocialWelfare = cplex.getObjValue();

			//测试：打印求解结果
			//cout << "求得的解的性质是：Status = " << cplex.getStatus() << endl;	//cplex在 TRIP_IP 外不可用
			//cout << "求解的最大社会福利是：SocialWelfare_max = " << SocialWelfare << endl;

			for (int i = 0; i < N; i++)	//保存OPT_TRIP求解的结果，仅保存第一次的计算结果
			{
				for (int j = 0; j < M; j++)
				{
					for (int t = 0; t < T; t++)
					{
						cnt[i][j][t] = cplex.getValue(x[i][j][t]);
					}
				}
			}
			count_TRAP_IP++;
		}

		int cnt_i = 0;	//cnt_i = connectivity_i 车辆 i 的连接性
		//根据 cons2 , 在算法运行过程[1,T]中，车辆 x[i] 只会在某个时刻 t 连接一次基站。
		//假如车辆 i 在某个时间槽 t 已经连接过某个基站 j (x[i][j][t] = 1) or (cnt_i = 1)，那么不必再考察剩余时间的情况
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; (j < M) && (cnt_i != 1); j++)
			{
				//for (int t = carp[i].at; (t <= (carp[i].dt - carp[i].et + 1)) && (cnt_i != 1); t++)	//原式
				for (int t = carp[i].at - 1; (t <= (carp[i].dt - carp[i].et)) && (cnt_i != 1); t++)
					//for (int t = 0; t < T && (cnt_i != 1); t++)
				{
					cnt_i = (cplex.getValue(x[i][j][t]) * carp[i].DCM[j][t]);
					/*cout << "x[" << i << "][" << j << "][" << t << "] = " << cplex.getValue(x[i][j][t]);
					cout << " carp[" << i << "].CM[" << j << "][" << t << "] = " << carp[i].CM[j][t];
					cout << "cnt_i = " << cnt_i << endl;*/
					if (cnt_i == 1)
					{
						X[i] = 1;
					}
					else
					{
						X[i] = 0;
					}
				}
			}
			cnt_i = 0;	//重置 cnt_i
		}

	}

	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
	catch (...) {
		cerr << "Error" << endl;
	}

	env.end();

	return X;

}

void TRAP_PAY(int* X)
{
	//1.创建一个数组 ar[N] 存放分配结果
	//X[]即 分配结果 AllocResult[]
	int ar[N];
	for (int i = 0; i < N; i++)
	{
		ar[i] = X[i];
	}

	//2.创造 2 个结构体数组Car_nip[N], Car_nib[N] 作为 Car[N]的副本，只操作Car_nip[N]，Car_nib[N], 不改变 Car[N]
	CarInfo* Car_nip = new CarInfo[N];	 //Car[N]的副本1，nip = no i participate 开辟空间
	CarInfo* Car_nib = new CarInfo[N];	//Car[N]的副本2，nib = no i bid 开辟空间
	for (int i = 0; i < N; i++)		//复制结构体数组Car[N]
	{
		Car_nip[i] = carP[i];
		Car_nib[i] = carP[i];
	}
	//3.计算：每辆车的付款
	for (int i = 0; i < N; i++)
	{
		//额外添加的cout 命令属于测试代码，已注释
		if (ar[i] == 1)	//如果i被选中（得到分配）
		{
			//3.1 Car_nip[N] 用于计算：没有 i 参与拍卖时，所有获胜者的投标之和
			//根据问题定义，踢出Car_cpP[i]，等价于把Car_cpP1[i]的TRD, CM, x[i][j][t]，bid 都置为0.
			//cout << "1.移除获胜车辆 Car_nip[" << Car_nib[i].id << "]，计算没有它参与时，其余获胜车辆的投标之和(Car_nip[])" << endl;
			RemoveCar_i(Car_nip, i);
			int* X_nip = TRAP_IP(Car_nip);	// X_nip: 没有 i 参与拍卖时的分配结果
			int* ar_nip = new int[N];
			//cout << "计算第 " << i + 1 << " 辆车的付款时，" << endl;
			//cout << "ar_nip[" << N << "] =[";
			for (int k = 0; k < N; k++)
			{
				ar_nip[k] = X_nip[k];
				//cout << ar_nip[k] << ", ";
			}
			//cout << "]." << endl;
			double sumBid_nip = 0.00;	// 没有 i 参与拍卖时，所有获胜者的投标之和，nip = no i participate 
			for (int k = 0; k < N; k++)	//
			{
				sumBid_nip += (double)ar_nip[k] * Car_nip[k].bid;
			}
			//cout << "sumBid_nip = " << sumBid_nip << endl;
			RecoverCar_i(Car_nip, i);//恢复 Car_i 的信息
			delete[] ar_nip;

			//3.2 Car_nib[N] 用于计算：有 i 参与拍卖时，所有获胜者的投标之和（不包括 i 的投标）
			//cout << "2.计算：有获胜车辆 Car_nib[" << Car_nib[i].id << "] 参与拍卖时，所有获胜者的投标之和（不包括 Car_nib["<< Car_nib[i].id <<" ]的投标" << endl;
			double i_bid = Car_nib[i].bid;	//记录 i 的投标
			//cout << "计算 sumBid_nib前：";
			//cout << "Car_nib[" << i + 1 << "].bid = " << Car_nib[i].bid << endl;
			//cout  << i + 1 << "_bid = " << i_bid << endl;
			double sumBid_nib = 0.00;	// 有 i 参与拍卖，但“不包括 i 的投标”的获胜者投标之和。 nib = no i bid
			Car_nib[i].bid = 0.00;		//将 i 的投标置 0 
			for (int k = 0; k < N; k++)
			{
				sumBid_nib += ar[k] * Car_nib[k].bid;
			}
			//cout << "sumBid_nib = " << sumBid_nib << endl;
			double tmp_pay = sumBid_nip - sumBid_nib;	//计算 i 的付款
			//Payment[i] = tmp_pay;
			if (tmp_pay >= 0 && tmp_pay <= i_bid)	//保证 p[i] <= b[i]
			{
				Payment[i] = tmp_pay;
			}
			else
			{
				Payment[i] = tmp_pay;	//记录错误付款
				//cout << endl;
				//cout << "Car[" << i + 1 << "]的付款为负或超过投标，计算错误！" << endl;
				//cout << endl;
			}

			//cout << "计算 sumBid_nib后，恢复投标：Car_nib[" << i + 1 << "].bid前：";
			//cout << "Car_nib[" << i + 1 << "].bid = " << Car_nib[i].bid << endl;
			//cout << i + 1 << "_bid = " << i_bid << endl;

			Car_nib[i].bid = i_bid;	//恢复 i 的投标

			//cout << "计算 sumBid_nib后，恢复投标：Car_nib[" << i + 1 << "].bid后：";
			//cout << "Car_nib[" << i + 1 << "].bid = " << Car_nib[i].bid << endl;
			//cout << i + 1 << "_bid = " << i_bid << endl;
		}
		else
		{
			Payment[i] = 0.00;
		}
	}
	delete[] Car_nip;
	delete[] Car_nib;


	//如果数组ar[N]里的所有元素都非0，那么就返回 true
	/*if (std::all_of(std::cbegin(ar), std::cend(ar), [](int num) {return num == 1; }))
	{

	}*/
}

void SaveResult()
{
	CalcResourceUtilizationRatio();		//计算资源利用率
	CalcServedCarRatio();				//计算服务车辆比例

	ofstream ofs;

	// 保存算法 DTVC_OPT_TRAP 的求解结果
	string OPTSolutionFile = "../data/ExperimentalResults/exp1/3DTVC_OPT_TRAP/3DTVC_OPT_TRAP_Solution_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//示例实验结果
	ofs.open(OPTSolutionFile, ios::out | ios::trunc);

	//数据保存格式：
	//车辆序号	车辆ID(车辆类型)	到达时间	截止时间	执行时间	任务资源需求			投标(系数)			得到分配的时间	连接的基站(基站类型)	连接情况
	//SeqNum	CarId(type)			AT			DT			ET			TaskResourceDemand		Bid(Coefficient)	BAT				ConnectToECS(type)		Connectivity
	//Note : 
	//(1)因为读入的车辆顺序不同，所以 i+1 和 Car[i].id 可能不同；
	//(2)在矩阵表示中 t∈[0,T-1]，但实际展示中 t ∈[1,T]，故写入文件时，使用 t+1
	//仅保存得到分配的车辆任务信息
	ofs << "SeqNum\t" << "Car_Id(Type)\t" << "AT\t" << "DT\t" << "ET\t" << "TaskResourceDemand\t" << "Bid(coefficient)\t" << "ConnectToECS(Type)\t" << "BeAllocatedTime\t" << "Connectivity" << endl;
	int seqnum = 1;		//记录有几个车辆获得分配
	for (int i = 0; i < N; i++)
	{
		int cnt_i = 0;
		if (carP[i].beAllocated == true)	//仅当车辆被标记为“得到分配时”，才输出其信息
		{
			for (int j = 0; j < M && (cnt_i != 1); j++)
			{
				for (int t = 0; t < T && (cnt_i != 1); t++)
				{
					cnt_i = cnt[i][j][t] * carP[i].DCM[j][t];
					if (cnt_i == 1)
					{
						ofs << seqnum << "\t";
						ofs << carP[i].id << "(" << carP[i].type << ")\t\t";
						ofs << carP[i].at << "\t";
						ofs << carP[i].dt << "\t";
						ofs << carP[i].et << "\t";
						ofs << carP[i].TRD << "\t\t\t";
						ofs << carP[i].bid << "(" << carP[i].bc << ")\t";
						ofs << ECS[j].id << "(" << ECS[j].type << ")\t\t";
						ofs << t + 1 << "\t\t";
						ofs << cnt_i << endl;
						seqnum++;
					}
				}
			}
		}
		cnt_i = 0;
	}
	ofs << endl;

	//输出求得的总社会福利
	ofs << "1.社会福利: SocialWelfare_max = " << SocialWelfare << endl;
	ofs << endl;

	//输出算法运行时间
	ofs << "2.算法运行时间: RunTime = " << RunTime << " ms" << endl;
	ofs << endl;

	//输出资源利用率
	ofs << "3.资源利用率: ResourceUtilizationRatio " << endl;
	ofs << "总CPU利用率: TRU_cpu = " << TRU_cpu << endl;
	ofs << "总GPU利用率: TRU_gpu = " << TRU_gpu << endl;
	ofs << "总Memory利用率: TRU_memory = " << TRU_memory << endl;
	ofs << "总Disk利用率: TRU_disk = " << TRU_disk << endl;
	ofs << endl;


	//输出服务车辆比例
	ofs << "4.服务车辆任务数(服务车辆任务比例): T_ServedCarNum(T_ServedCarRatio) = " << T_ServedCarNum << "(" << T_ServedCarRatio << ")" << endl;
	ofs << "其中，小型车辆任务服务占比: S_ServedCarNum(S_ServedCarRatio) = " << S_ServedCarNum << "(" << S_ServedCarRatio << ")" << endl;
	ofs << "其中，大型车辆任务服务占比: L_ServedCarNum(L_ServedCarRatio) = " << L_ServedCarNum << "(" << L_ServedCarRatio << ")" << endl;
	ofs << endl;

	ofs.close();
}

int main()
{
	for (; ExeNum <= count_DTVC_OPT_TRAP; ExeNum++)
	//if (ExeNum <= count_DTVC_OPT_TRAP)
	{
		cout << "这是第 " << ExeNum << " 次运行 DTVC_OPT_TRAP 算法！" << endl;

		//将使用过的全局变量初始化为 0
		count_TRAP_IP = 1;

		SocialWelfare = 0;
		RunTime = 0;

		UsedResource.cpu = 0.00, UsedResource.gpu = 0.00, UsedResource.memory = 0.00, UsedResource.disk = 0.00;
		TotalResource.cpu = 0.00, TotalResource.gpu = 0.00, TotalResource.memory = 0.00, TotalResource.disk = 0.00;
		TRU_cpu = 0.00, TRU_gpu = 0.00, TRU_memory = 0.00, TRU_disk = 0.00;

		T_ServedCarNum = 0, S_ServedCarNum = 0, L_ServedCarNum = 0;
		T_ServedCarRatio = 0, S_ServedCarRatio = 0, L_ServedCarRatio = 0;

		for (int i = 0; i < N; i++)
		{
			AllocResult[i] = 0;
			Payment[i] = 0;
			for (int j = 0; j < M; j++)
			{
				for (int t = 0; t < T; t++)
				{
					cnt[i][j][t] = 0;
					tmp_cnt[i][j][t] = 0;
				}
			}
		}

		//第一步：读入数据
		//(1)读入基站信息
		LoadEcsInfo(EcsInfoFile,EcsInfoFile_RCM);

		//(2)读入车辆任务信息
		LoadCarInfo(RS_CarInfoFile_S, RS_CarInfoFile_S_RDM, RS_CarInfoFile_S_DCM, RS_S_CarNum, carP);
		//LoadCarInfo(RS_CarInfoFile_L, RS_CarInfoFile_S_RDM, RS_CarInfoFile_L_DCM, RS_L_CarNum, carP + RS_S_CarNum);

		////测试 : 能否正确读入数据(成功)
		//cout << "读入的基站信息如下：" << endl;
		//for (int j = 0; j < M; j++)
		//{
		//	cout << ECS[j] << endl;
		//}
		//cout << endl;
		//cout << "读入的车辆任务信息如下：" << endl;
		//for (int i = 0; i < 5; i++)
		//{
		//	cout << carP[i] << endl;
		//}

		cout << "在时间槽[1, " << T << "]，开始运行 DTVC_OPT_TRAP 算法！" << endl;

		time_t tBegin, tEnd;
		tBegin = clock();	//开始计时

		//第二步：执行分配函数，返回分配结果（数组） Solution of TRAP-IP
		int* arP = new int[N];	// arP = AllocResultPointer = 分配结果指针
		arP = TRAP_IP(carP);	// 动态数组 arP 保存每次运行 TRAP_IP 的结果
		for (int i = 0; i < N; i++)
		{
			AllocResult[i] = arP[i];
			if (AllocResult[i] == 1)
			{
				carP[i].beAllocated = true;	//将其得到分配的标志位置 true 
			}
		}
		delete[] arP;

		//第三步：执行支付函数
		TRAP_PAY(AllocResult);
		for (int i = 0; i < N; i++)
		{
			carP[i].pay = Payment[i];
		}

		tEnd = clock();	//计时结束

		cout << "第 " << ExeNum << " 次运行 DTVC_OPT_TRAP 算法结束！" << endl;
		cout << endl;

		RunTime = static_cast<double>(tEnd - tBegin) / CLOCKS_PER_SEC * 1000;

		////测试：打印算法运行时间
		//cout << "DTVC_OPT_TRAP 算法的运行时间为：RunTime = " << RunTime << "ms" << endl;
		////测试：打印求解结果
		////cout << "求得的解的性质是：Status = " << cplex.getStatus() << endl;	//cplex在 TRIP_IP 外不可用
		//cout << "求解的最大社会福利是：SocialWelfare_max = " << SocialWelfare << endl;
		////打印运行算法之后的结果(成功)
		//cout << "计算完成后的车辆任务信息如下：" << endl;
		//for (int i = 0; i < N; i++)
		//{
		//	cout << "车辆 " << i << " : id = " << carP[i].id
		//		<< " type = " << carP[i].type
		//		<< " InitLoc(m) = " << carP[i].InitLoc
		//		<< " v(m/t) = " << carP[i].v
		//		<< " at = " << carP[i].at
		//		<< " dt = " << carP[i].dt
		//		<< " et = " << carP[i].et
		//		<< " TR[4] = [" << carP[i].TR->cpu << ", " << carP[i].TR->gpu << ", " << carP[i].TR->memory << ", " << carP[i].TR->disk << "] "
		//		<< " bid = " << carP[i].bid << " CM = "
		//		<< " BeAllocated = " << carP[i].beAllocated << endl;
		//	/*for (int j = 0; j < M; j++)
		//	{
		//		for (int t = 0; t < T; t++)
		//		{
		//			cout << Car[i].CM[j][t] << " ";
		//		}
		//		cout << endl;
		//	}*/
		//}
		////测试：打印分配结果（成功）
		//cout << "TRAP-IP的分配结果为：" << endl;
		//for (int i = 0; i < N; i++)	//保存OPT_TRIP求解的结果，仅保存第一次的计算结果
		//{
		//	for (int j = 0; j < M; j++)
		//	{
		//		for (int t = 0; t < T; t++)
		//		{
		//			if (cnt[i][j][t] == 1)
		//			{
		//				cout << "x[" << i + 1 << "][" << j + 1 << "][" << t + 1 << "] = " << cnt[i][j][t] << endl;
		//			}
		//		}
		//	}
		//}
		//cout << "综上，分配结果 AllocResult[" << N << "] = [";
		//for (int i = 0; i < N; i++)
		//{
		//	if (i < N - 1)
		//	{
		//		cout << AllocResult[i] << ", ";
		//	}
		//	else
		//	{
		//		cout << AllocResult[i] << "]." << endl;
		//	}
		//}
		////测试：打印付款结果
		//cout << "每辆车的付款为 : " << endl;
		//for (int i = 0; i < N; i++)
		//{
		//	cout << "Car[" << i + 1 << "].pay = " << carP[i].pay << endl;
		//}
		//cout << endl;
		//cout << "综上：Payment[N] = [";
		//for (int i = 0; i < N; i++)
		//{
		//	if (i < N - 1)
		//	{
		//		cout << Payment[i] << ", ";
		//	}
		//	else
		//	{
		//		cout << Payment[i] << "]." << endl;
		//	}
		//}

		//第四步：输出结果
		SaveResult();
	}

	system("pause");
	return 0;

}