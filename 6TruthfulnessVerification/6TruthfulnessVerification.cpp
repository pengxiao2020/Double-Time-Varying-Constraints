// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include "6TruthfulnessVerification.h"

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 坐标点对象(x,y)
ostream& operator<<(ostream& cout, Coordinate& coord)
{
	cout << "(" << coord.x << "," << coord.y << ")";
	return cout;
}

//利用全局函数重载 << 运算符(C++核心编程 p40), 使其能输出 资源容量 [CPU, GPU, Memory, Disk]
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
	cout << "dr = " << car.dr << ",  ";
	cout << "f = " << car.f << ",  ";
	cout << "bat = " << car.bat << ",  ";
	cout << "ConnectTo = ECS(" << car.ConnectTo << "),  ";
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

//第一步：读入数据
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile)
{
	ifstream ifs_ECS(EcsInfofile);		//读入基站信息(除资源容量矩阵)
	ifstream ifs_ECS_RCM(EcsRCMfile);	//读入基站的资源容量矩阵
	for (int j = 0; j < M; j++)	//读入基站信息：资源容量
	{
		EcsInfo* ecs = new EcsInfo;	//临时 EcsInfo 对象
		//先读入具体值到对象的属性中，再将该对象放入vector中
		ifs_ECS >> ecs->id;
		ifs_ECS >> ecs->type;
		ifs_ECS >> ecs->SCR;
		ifs_ECS >> ecs->DL.x;
		ifs_ECS >> ecs->DL.y;
		ifs_ECS >> ecs->RC.cpu;
		ifs_ECS >> ecs->RC.gpu;
		ifs_ECS >> ecs->RC.memory;
		ifs_ECS >> ecs->RC.disk;

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
		ecs->RCM = rcm;

		vEcsInfo.push_back(*ecs);
		delete ecs;
	}
	ifs_ECS.close();

	int count_S_ECS = 0, count_L_ECS = 0;
	for (vector<EcsInfo>::iterator ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
	{
		if (ecsit->type == "S")
			count_S_ECS++;
		else if (ecsit->type == "L")
			count_L_ECS++;
		else
			cout << "读入的基站类型错误, 请重新运行程序，再读入基站信息文件！" << endl;
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

void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, vector<CarInfo>& Car, int CarNum)
{
	ifstream ifs_Car(CarInfofile);		//读入车辆任务信息
	ifstream ifs_Car_RDM(CarRDMfile);	//读入车辆的时变资源需求矩阵
	ifstream ifs_Car_DCM(CarDCMfile);	//读入车辆的时变部署约束矩阵
	for (int i = 0; i < CarNum; i++)	//读入车辆信息
	{
		CarInfo* car = new CarInfo;
		ifs_Car >> car->id;
		ifs_Car >> car->type;
		ifs_Car >> car->at;
		ifs_Car >> car->dt;
		ifs_Car >> car->et;
		ifs_Car >> car->TRD.cpu;
		ifs_Car >> car->TRD.gpu;
		ifs_Car >> car->TRD.memory;
		ifs_Car >> car->TRD.disk;
		ifs_Car >> car->bid;
		ifs_Car >> car->bc;

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(car->et, 0)));
		for (auto rit = rdm.begin(); rit != rdm.end(); rit++)	//读入时变资源需求矩阵
		{
			for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			{
				int tmp;
				ifs_Car_RDM >> tmp;
				(*cit) = tmp;
			}
		}
		car->RDM = rdm;

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
		car->DCM = dcm;

		Car.push_back(*car);
		delete car;
	}
	ifs_Car.close();
	ifs_Car_RDM.close();
	ifs_Car_DCM.close();
}

//第二步：每个时间槽执行一次 DTVC_OTRAP 算法
void DTVC_OTRAP(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& Car)
{
	//清空容器
	cts_AllocResult.clear();

	//测试：记录每次运行 DTVC_OTRAP 的准备工作所用的时间
	time_t tPreWork_begin, tPreWork_end;
	tPreWork_begin = clock();	//准备工作开始计时

	//准备工作：

	//(1)删除已经完成任务的车辆(不需要归还资源，该部分代码无用)
	//time_t tReturnResource_begin, tReturnResource_end;	//统计 归还资源 所用的时间
	//tReturnResource_begin = clock();
	//if (!BeAllocatedCar.empty())
	//{
	//	//对于 et = 1 的车辆，在下一个时间槽 cts+1 将其 accom_flag 置 ture 并回收资源
	//	//对于 et > 1 的车辆，在当前时间槽 cts 将其 accom_flag 置 ture 并回收资源
	//	for (auto BACit = BeAllocatedCar.begin(); BACit != BeAllocatedCar.end();)
	//	{
	//		if (cts >= (BACit->bat + BACit->et - 1))	//车辆已经完成任务
	//		{
	//			BACit->accom_flag = true;	//完成标志置1
	//			BACit = BeAllocatedCar.erase(BACit);		//踢出完成任务的车辆
	//		}
	//		else
	//		{
	//			BACit++;
	//		}
	//	}
	//}
	//tReturnResource_end = clock();
	//ReturnResource_RunTime += static_cast<double>(tReturnResource_end - tReturnResource_begin) / CLOCKS_PER_SEC * 1000;

	//2.删除超时的车辆任务请求
	time_t tDelOvertimeTask_begin, tDelOvertimeTask_end;	//统计 删除过期车辆任务 所用的时间
	tDelOvertimeTask_begin = clock();

	if (!UnAllocatedCar.empty())
	{
		for (auto UACit = UnAllocatedCar.begin(); UACit != UnAllocatedCar.end();)
		{
			if (cts > (UACit->dt - UACit->et + 1))	//踢出超时的车辆任务请求
			{
				UACit = UnAllocatedCar.erase(UACit);
			}
			else
			{
				UACit++;
			}
		}
	}

	tDelOvertimeTask_end = clock();
	DelOvertimeTask_RunTime += static_cast<double>(tDelOvertimeTask_end - tDelOvertimeTask_begin) / CLOCKS_PER_SEC * 1000;

	//3.将新到达的车辆任务全部并入容器 UnAllocatedCar
	time_t tAddArrivedCar_begin, tAddArrivedCar_end;	//统计 删除过期车辆任务 所用的时间
	tAddArrivedCar_begin = clock();

	if (!Car.empty())
	{
		////测试：cts = 39时，将 Car[] 中的车辆装入 UnAllocatedCar[] 之前， Car[] 中的车辆信息如下
		//if (cts == 39)
		//{
		//	cout << "测试：cts = 39时，将 Car[] 中的车辆装入 UnAllocatedCar[] 之前， Car[] 中的车辆信息如下 : " << endl;
		//	for (auto carit = Car.begin(); carit != Car.end(); carit++)
		//	{
		//		cout << (*carit) << endl;
		//	}
		//}
		for (auto carit = Car.begin(); carit != Car.end();)
		{
			//仅并入到达时间 at 与当前时间槽 cts 相等的车辆信息(类型)
			if ((*carit).at == cts)
			{
				UnAllocatedCar.push_back(*carit);	//将 at == cts 且未得到分配的车辆 放入容器 UnAllocatedCar
				carit = Car.erase(carit);			//将已经放入 UnAllocatedCar 的车辆 踢出 Car
			}
			else
			{
				carit++;
			}
		}
		/*if (cts == 39)
		{
			cout << "测试：cts = 39时，将 Car[] 中的车辆装入 UnAllocatedCar[] 之后， UnAllocatedCar[] 中的车辆信息如下 : " << endl;
			for (auto carit = UnAllocatedCar.begin(); carit != UnAllocatedCar.end(); carit++)
			{
				cout << (*carit) << endl;
			}
		}*/

	}

	tAddArrivedCar_end = clock();
	AddArrivedCar_RunTime += static_cast<double>(tAddArrivedCar_end - tAddArrivedCar_begin) / CLOCKS_PER_SEC * 1000;

	if (UnAllocatedCar.empty())	//当存在“尚未得到分配的车辆任务”时，才执行 DTVC_OTRAP 算法
	{
		//if(UnAllocatedCar.empty())
			//cout << "当前时间槽 t = " << cts << ", 不存在未被分配的车辆(UnAllocatedCar.empty() = true)，故不执行OTRAP算法！" << endl;
		//else
			//cout << "当前时间槽 t = " << cts << ", 没有可用的资源向量(EcsRCEmpty = true)，故不执行OTRAP算法！" << endl;

		tPreWork_end = clock();		//准备工作结束计时
		PreWork_RunTime += static_cast<double>(tPreWork_end - tPreWork_begin) / CLOCKS_PER_SEC * 1000;

		return;
	}
	else		//执行 DTVC_OTRAP 算法
	{
		vector<EcsInfo> vEcsInfo_cp(ECS);	//ECS副本
		//vector<CarInfo> BeAllocatedCar_cp(BeAllocatedCar);	//BeAllocatedCar副本
		vector<CarInfo> UnAllocatedCar_cp(UnAllocatedCar);		//UnAllocatedCar副本

		////测试：打印副本 vEcsInfo_cp, UnAllocatedCar_cp, BeAllocatedCar_cp(已通过)
		//cout << "副本-基站 信息如下：" << endl;
		//for (int j = 0; j < vEcsInfo_cp.size(); j++)
		//{
		//	cout << "基站：id = " << vEcsInfo_cp[j].id;
		//	cout << " type = " << vEcsInfo_cp[j].type;
		//	cout << " CR = " << vEcsInfo_cp[j].CR;
		//	cout << " loc = " << vEcsInfo_cp[j].loc;
		//	cout << " CA = [" << vEcsInfo_cp[j].CA[0] << "," << vEcsInfo_cp[j].CA[1] << "] ";
		//	cout << " RC = <" << vEcsInfo_cp[j].RC.cpu << " " << vEcsInfo_cp[j].RC.gpu << " " << ECS[j].RC.memory << " " << vEcsInfo_cp[j].RC.disk << "> ";
		//	cout << endl;
		//}
		//cout << endl;
		//cout << "副本-已分配车辆容器 BeAllocatedCar_cp 的车辆信息如下：" << endl;
		//for (int i = 0; i < BeAllocatedCar_cp.size(); i++)
		//{
		//	cout << "车辆 " << i << " : id = " << BeAllocatedCar_cp[i].id
		//		<< " type = " << BeAllocatedCar_cp[i].type
		//		<< " at = " << BeAllocatedCar_cp[i].at
		//		<< " dt = " << BeAllocatedCar_cp[i].dt
		//		<< " et = " << BeAllocatedCar_cp[i].et
		//		<< " TR[4] = [" << BeAllocatedCar_cp[i].TR->cpu << ", " << BeAllocatedCar_cp[i].TR->gpu << ", " << BeAllocatedCar_cp[i].TR->memory << ", " << BeAllocatedCar_cp[i].TR->disk << "] "
		//		<< " bid = " << BeAllocatedCar_cp[i].bid
		//		<< " bat = " << BeAllocatedCar[i].bat
		//		<< " ConnectTo = " << BeAllocatedCar[i].ConnectTo << endl;
		//		/*<< " CM = " << endl;
		//	for (int j = 0; j < M; j++)
		//	{
		//		for (int t = 0; t < T; t++)
		//		{
		//			cout << UnAllocatedCar_cp[i].DCM[j][t] << " ";
		//		}
		//		cout << endl;
		//	}*/
		//	cout << endl;
		//}
		//cout << endl;
		//cout << "副本-未分配车辆容器 UnAllocatedCar_cp 的车辆信息如下：" << endl;
		//for (int i = 0; i < UnAllocatedCar_cp.size(); i++)
		//{
		//	cout << "车辆 " << i << " : id = " << UnAllocatedCar_cp[i].id << " type = " << UnAllocatedCar_cp[i].type
		//		<< " InitLoc(m) = " << UnAllocatedCar_cp[i].InitLoc << " v(m/s) = " << UnAllocatedCar_cp[i].v
		//		<< " at = " << UnAllocatedCar_cp[i].at << " dt = " << UnAllocatedCar_cp[i].dt << " et = " << UnAllocatedCar_cp[i].et
		//		<< " TR[4] = [" << UnAllocatedCar_cp[i].TR->cpu << ", " << UnAllocatedCar_cp[i].TR->gpu << ", " << UnAllocatedCar_cp[i].TR->memory << ", " << UnAllocatedCar_cp[i].TR->disk << "] "
		//		<< " bid = " << UnAllocatedCar_cp[i].bid << endl;
		//		/*<< " CM = "<< endl;
		//	for (int j = 0; j < M; j++)
		//	{
		//		for (int t = 0; t < T; t++)
		//		{
		//			cout << UnAllocatedCar_cp[i].DCM[j][t] << " ";
		//		}
		//		cout << endl;
		//	}
		//	cout << endl;*/
		//}
		//cout << endl;

		tPreWork_end = clock();		//准备工作结束计时
		PreWork_RunTime += static_cast<double>(tPreWork_end - tPreWork_begin) / CLOCKS_PER_SEC * 1000;

		time_t tAlloc_begin, tAlloc_end;
		tAlloc_begin = clock();		//分配函数开始计时

		DTVC_OTRAP_ALLOC(cts, ECS, UnAllocatedCar);

		tAlloc_end = clock();		//分配函数结束计时
		ALLOC_RunTime += static_cast<double>(tAlloc_end - tAlloc_begin) / CLOCKS_PER_SEC * 1000;

		////测试：是否能够成功分配资源，且基站资源容量矩阵显示正确（成功）
		//cout << "当前时间槽为 t = " << cts << endl;
		//cout << "执行 DTVC_OTRAP_ALLOC 后，容器 UnAllocatedCar 中的车辆信息如下：" << endl;
		//for (auto carit = UnAllocatedCar.begin(); carit != UnAllocatedCar.end(); carit++)
		//{
		//	cout << (*carit) << endl;
		//}
		//cout << "执行 DTVC_OTRAP_ALLOC 后，容器 tmp_X 中的车辆信息如下：" << endl;
		//for (auto carit = tmp_X.begin(); carit != tmp_X.end(); carit++)
		//{
		//	cout << (*carit) << endl;
		//}
		//cout << endl;
		//cout << "执行OTRAP_ALLOC后，容器 vEcsInfo 中的基站信息如下：" << endl;
		//for (auto ecsit = ECS.begin(); ecsit != ECS.end(); ecsit++)
		//{
		//	cout << (*ecsit) << endl;
		//}*/

		//保存
		//1.截止至[1,t]获得的社会福利
		TotalSocialWelfare += cts_SocialWelfare;
		//2.当前时间槽 t 的分配结果（未计算付款）
		if (!tmp_X.empty())
		{
			for (auto Xit = tmp_X.begin(); Xit != tmp_X.end(); Xit++)
			{
				cts_AllocResult.push_back(*Xit);
			}
		}

		//只有在 当前时间槽 t 有车辆获得分配(容器 cts_AllocResult 不为空)时，执行OTRAP_PAY 才是有意义的
		if (!cts_AllocResult.empty())
		{
			////测试：cts_AllocResult[] 中的车辆与 tmp_X 中的车辆是否一样，但 pay 未赋值（成功）
			//cout << "执行 DTVC_OTRAP_PAY 前，容器 cts_AllocResult 中的车辆信息如下：" << endl;
			//for (auto cts_ARit = cts_AllocResult.begin(); cts_ARit != cts_AllocResult.end(); cts_ARit++)
			//{
			//	cout << (*cts_ARit) << endl;
			//}
			//cout << endl;

			time_t tPay_begin, tPay_end;
			tPay_begin = clock();	//支付函数计时开始

			DTVC_OTRAP_PAY(cts, vEcsInfo_cp, UnAllocatedCar_cp, cts_AllocResult);

			tPay_end = clock();		//支付函数计时结束
			PAY_RunTime += static_cast<double>(tPay_end - tPay_begin) / CLOCKS_PER_SEC * 1000;

			////测试：cts_AllocResult[] 中的车辆的属性 pay 是否已经计算出来（成功）
			//cout << "执行 DTVC_OTRAP_PAY 后，容器 cts_AllocResult 中的车辆信息（主要看付款）如下：" << endl;
			//for (auto cts_ARit = cts_AllocResult.begin(); cts_ARit != cts_AllocResult.end(); cts_ARit++)
			//{
			//	cout << (*cts_ARit) << endl;
			//}
			//cout << endl;

			//3.统计截止至[1,t]的分配结果（已计算付款）
			for (auto cts_ARit = cts_AllocResult.begin(); cts_ARit != cts_AllocResult.end(); cts_ARit++)
			{
				AllocResult.push_back(*cts_ARit);
			}
		}
		/*else
		{
			cout << "没有车辆在当前时间槽 t = " << cts << " 获得分配（cts_AllocResult.empty() == true），故不执行 OTRAP_PAY 算法！" << endl << endl;
		}*/
	}
}

void DTVC_OTRAP_ALLOC(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar)
{
	//0.每次执行 OTRAP_ALLOC 前要清空容器
	tmp_X.clear();
	cts_SocialWelfare = 0;
	for (auto ecsit = ECS.begin(); ecsit != ECS.end(); ecsit++)
	{
		ecsit->InDegree = 0;				//基站入度置 0，
		ecsit->AbleConnectToECS.clear();	//存放 可与基站j建立连接的车辆的容器 清空
	}

	//1.计算每个基站的入度
	for (auto ecsit = ECS.begin(); ecsit != ECS.end(); ecsit++)
	{
		for (auto carit = UACar.begin(); carit != UACar.end(); carit++)
		{
			//遍历每一辆车的时变部署约束矩阵 DCM 的“第 ecsit->id - 1 行，第 cts - 1 列元素”
			if ((*carit).DCM[(*ecsit).id - 1][cts - 1] == 1)	//数组下标从 0 开始，要减 1 
			{
				(*carit).alloc_flag = true;		//表示车辆 (*carit) 能够与基站相连，但不确定要将任务部署到哪个基站
				//(*carit).ConnectTo = (*ecsit).id;		//默认车辆能连接基站 (*ecsit)，当车辆能连接多个基站时，该值会被覆盖多次，故不赋值
				(*ecsit).InDegree++;
			}
		}
	}

	////测试：打印排序前的基站顺序(成功)
	//cout << "入度递增顺序前的基站信息如下：" << endl;
	//for (auto ecs = ECS.begin(); ecs != ECS.end(); ecs++)
	//{
	//	cout << "基站[" << ecs->id << "]的id = " << ecs->id << ",    ";
	//	cout << "InDegree = " << ecs->InDegree << ",    ";
	//	cout << "DL = " << ecs->DL << ",    ";
	//	cout << "RC = " << ecs->RC << ",    ";
	//	cout << "RCM = " << endl;
	//	for (auto rit = (*ecs).RCM.begin(); rit != (*ecs).RCM.end(); rit++)
	//	{
	//		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
	//		{
	//			cout << (*cit) << " ";
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}

	//2.按基站入度升序，对 ECS 中的所有基站进行排序
	sort(ECS.begin(), ECS.end(), EcsInDegree_Ascending);

	////测试：打印排序后的基站顺序(成功)
	//cout << "入度递增排序后的基站信息如下：" << endl;
	//for (auto ecs = ECS.begin(); ecs != ECS.end(); ecs++)
	//{
	//	cout << "基站[" << ecs->id << "]的id = " << ecs->id << ",    ";
	//	cout << "InDegree = " << ecs->InDegree << ",    ";
	//	cout << "DL = " << ecs->DL << ",    ";
	//	cout << "RC = " << ecs->RC << ",    ";
	//	cout << "RCM = " << endl;
	//	for (auto rit = (*ecs).RCM.begin(); rit != (*ecs).RCM.end(); rit++)
	//	{
	//		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
	//		{
	//			cout << (*cit) << " ";
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}

	//3.按排序后的顺序，逐个考察基站的资源分配情况：
	// 检查基站 能连接 哪些车辆，并将 这些车辆 放入 (*ecsit).AbleToConnectECS 容器中，并计算其价值密度
	// 对 (*ecsit).AbleToConnectECS 容器中车辆按价值密度降序排序，然后根据贪心法为车辆分配资源
	for (auto ecsit = ECS.begin(); ecsit != ECS.end(); ecsit++)
	{
		if ((*ecsit).InDegree != 0)	//仅考察入度不为 0 的基站的情况
		{
			for (auto carit = UACar.begin(); carit != UACar.end();)
			{
				//仅考察那些能够连接基站的车辆，第二个条件防止车辆属性 ConnectTo 被重复赋值
				//当车辆的部署约束矩阵 DCM 的元素值为 1 的时候，将其放进对应基站的容器 (*ecsit).AbleToConnectECS 中
				if (((*carit).alloc_flag == true) && ((*carit).ConnectTo == -1))
				{
					if ((*carit).DCM[(*ecsit).id - 1][cts - 1] == 1)
					{
						(*carit).ConnectTo = (*ecsit).id;	//记录车辆连接的基站编号
						(*ecsit).AbleConnectToECS.push_back(*carit);	//将车辆放进对应基站的容器 (*ecsit).AbleToConnectECS 中
						carit = UACar.erase(carit);	//将车辆从 容器 UACar 中移除
					}
					else
					{
						carit++;
					}
				}
				else
				{
					carit++;
				}
			}
			if (!(*ecsit).AbleConnectToECS.empty())	//只要基站的 AbleConnectToECS 容器不空，就要为里面的车辆分配资源
			{
				CalcCarValueDensity((*ecsit), cts);	//计算车辆(任务)的价值密度

				//按价值密度非递增顺序，对容器 AbleConnectToECS 中的车辆任务排序
				sort((*ecsit).AbleConnectToECS.begin(), (*ecsit).AbleConnectToECS.end(), CarValueDensity_Descending);

				//根据贪心法，逐个满足车辆的任务资源需求，直到资源不足
				for (auto carit = (*ecsit).AbleConnectToECS.begin(); carit != (*ecsit).AbleConnectToECS.end();)
				{
					////第一种计算形式：DTVC_OTRAP 使用（测试成功）
					//if ((*carit).dr > 1)	//资源不够分
					//{
					//	(*carit).alloc_flag = false;
					//	(*carit).ConnectTo = -1;
					//	UACar.push_back(*carit);
					//	carit = (*ecsit).AbleConnectToECS.erase(carit);
					//}
					//else	//资源够分
					//{
					//	//为 (*carit) 分配资源，并更新(*ecsit)资源容量矩阵的对应部分
					//	//更改得到分配的车辆状态
					//	(*carit).bat = cts;					//记录 (*carit) 得到分配的时间
					//	//BACar.push_back(*carit);			//将得到分配的车辆并入 BeAllocatedCar[] 中
					//	tmp_X.push_back(*carit);			//更新 当前时间槽 的分配结果 X[t]= X[t] + x[i][j][t]
					//	cts_SocialWelfare += (*carit).bid;	//更新 当前时间槽 获得的社会福利 V[t] = V[t] + b[i]
					//	//更新(*ecsit)资源容量矩阵的对应部分
					//	auto RCM_rit = (*ecsit).RCM.begin();
					//	auto RDM_rit = (*carit).RDM.begin();
					//	for (; (RCM_rit != (*ecsit).RCM.end()) && (RDM_rit != (*carit).RDM.end()); RCM_rit++, RDM_rit++)
					//	{
					//		auto RCM_cit = (*RCM_rit).begin() + cts - 1;	//定位到(*ecsit)的“当前时间槽”列
					//		auto RDM_cit = (*RDM_rit).begin();
					//		for (; (RCM_cit != (*RCM_rit).begin() + cts - 1 + (*carit).et) && (RDM_cit != (*RDM_rit).end()); RCM_cit++, RDM_cit++)
					//		{
					//			(*RCM_cit) -= (*RDM_cit);
					//		}
					//	}
					//	carit = (*ecsit).AbleConnectToECS.erase(carit);
					//}

					//第二种计算形式：DTVC_OVMAP 使用（测试成功）
					//遍历 ECS 的 RC 矩阵，定位到 [cts, cts + et] 列
					//cts 列 = (*ecsit).RCM.begin() + cts - 1;
					//cts + et 列 = rc_rit != (*ecsit).RCM.begin() + cts - 1 + (*carit).et
					// (1)创建一个临时矩阵，存储在“当前时间槽”列至“当前时间槽 + 车辆执行时间”列的基站资源容量，
					// 即 affected_RCM 存储：为(*carit)分配资源后，而受到影响的那部分基站(*ecsit)的资源容量
					vector<vector<int>> affected_RCM = vector<vector<int>>(R, vector<int>((*carit).et, 0));
					auto affected_RCM_rit = affected_RCM.begin();	//受影响的资源容量矩阵的 行 迭代器
					auto RCM_rit = (*ecsit).RCM.begin();					//遍历 基站资源容量矩阵的 行 迭代器
					for (; (affected_RCM_rit != affected_RCM.end()) && (RCM_rit != (*ecsit).RCM.end()); affected_RCM_rit++, RCM_rit++)
					{
						auto affected_RCM_cit = (*affected_RCM_rit).begin();
						auto RCM_cit = (*RCM_rit).begin() + cts - 1;	//基站资源容量矩阵 定位到“当前时间槽”列
						for (; (affected_RCM_cit != (*affected_RCM_rit).end()) && (RCM_cit != (*RCM_rit).begin() + cts - 1 + (*carit).et); affected_RCM_cit++, RCM_cit++)
						{
							(*affected_RCM_cit) = (*RCM_cit);
						}
					}
					// (2)创建一个临时矩阵，存储 基站(*ecsit) 为 (*carit) 分配资源后的剩余资源量
					vector<vector<int>> remain_RCM = vector<vector<int>>(R, vector<int>((*carit).et, 0));
					auto remain_RCM_rit = remain_RCM.begin();		//(*ecsit)为(*carit)分配资源后的剩余资源量矩阵的 行 的迭代器
					affected_RCM_rit = affected_RCM.begin();	//受影响的(*ecsit)的资源容量矩阵的 行 迭代器
					auto RDM_rit = (*carit).RDM.begin();			//(*carit)的资源需求矩阵的 行 的迭代器
					for (; (remain_RCM_rit != remain_RCM.end()) && (affected_RCM_rit != affected_RCM.end()) && (RDM_rit != (*carit).RDM.end()); remain_RCM_rit++, affected_RCM_rit++, RDM_rit++)
					{
						auto remain_RCM_cit = (*remain_RCM_rit).begin();
						auto affected_RCM_cit = (*affected_RCM_rit).begin();
						auto RDM_cit = (*RDM_rit).begin();
						for (; (remain_RCM_cit != (*remain_RCM_rit).end()) && (affected_RCM_cit != (*affected_RCM_rit).end()) && (RDM_cit != (*RDM_rit).end()); remain_RCM_cit++, affected_RCM_cit++, RDM_cit++)
						{
							(*remain_RCM_cit) = (*affected_RCM_cit) - (*RDM_cit);
						}
					}
					//(3)遍历矩阵 remain_RCM，如果 remain_RCM 中的每个元素都 >= 0 ，说明基站资源够分配，否则，将 (*carit) 放回 UACar[] 并踢出 (*ecsit).AbleConnectToECS 容器
					//设计一个标志位：标识 (*ecsit) 的剩余资源量是否够分配给 (*carit)
					bool ResourceEnough = true;	//默认够分配
					for (auto remain_RCM_rit = remain_RCM.begin(); remain_RCM_rit != remain_RCM.end(); remain_RCM_rit++)
					{
						for (auto remain_RCM_cit = (*remain_RCM_rit).begin(); remain_RCM_cit != (*remain_RCM_rit).end(); remain_RCM_cit++)
						{
							if ((*remain_RCM_cit) < 0)	//资源不够分配
							{
								ResourceEnough = false;
							}
						}
					}
					if (ResourceEnough == true)	//资源够分配
					{
						//(4)为 (*carit) 分配资源，并更新(*ecsit)资源容量矩阵的对应部分
						//更改得到分配的车辆状态
						(*carit).bat = cts;					//记录 (*carit) 得到分配的时间
						//BACar.push_back(*carit);			//将得到分配的车辆并入 BeAllocatedCar[] 中
						tmp_X.push_back(*carit);			//更新 当前时间槽 的分配结果 X[t]= X[t] + x[i][j][t]
						cts_SocialWelfare += (*carit).bid;	//更新 当前时间槽 获得的社会福利 V[t] = V[t] + b[i]
						//更新(*ecsit)资源容量矩阵的对应部分
						RCM_rit = (*ecsit).RCM.begin();
						RDM_rit = (*carit).RDM.begin();
						for (; (RCM_rit != (*ecsit).RCM.end()) && (RDM_rit != (*carit).RDM.end()); RCM_rit++, RDM_rit++)
						{
							auto RCM_cit = (*RCM_rit).begin() + cts - 1;
							auto RDM_cit = (*RDM_rit).begin();
							for (; (RCM_cit != (*RCM_rit).begin() + cts - 1 + (*carit).et) && (RDM_cit != (*RDM_rit).end()); RCM_cit++, RDM_cit++)
							{
								(*RCM_cit) -= (*RDM_cit);
							}
						}
						carit = (*ecsit).AbleConnectToECS.erase(carit);
					}
					else	//资源不够分配
					{
						(*carit).alloc_flag = false;
						(*carit).ConnectTo = -1;
						UACar.push_back(*carit);
						carit = (*ecsit).AbleConnectToECS.erase(carit);
					}
				}

			}
			/*else
			{
				cout << "当前时间槽 t = " << cts << ", 没有任何车辆能够连接基站 ECS " << (*ecsit).id << " ( ECS[" << (*ecsit).id << "].AbleConnectToECS.empty() == true)" << endl;
			}*/
		}
		/*else
		{
			cout << "当前时间槽 t = " << cts << ",基站 ECS " << (*ecsit).id << " 的入度为 0 (InDegree = 0)，不为任何车辆分配资源。" << endl;
		}*/
	}
	//根据 ECS_id 升序，重新排列 vEcsInfo 中 ECS 的顺序
	//sort(ECS.begin(), ECS.end(), EscId_Ascending);

}

void DTVC_OTRAP_PAY(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar, vector<CarInfo>& cts_AR)
{
	//pay过小的原因在于：
	//假设在当前时间槽 cts 中，对于某个特定的基站（资源充足）来说，只有一个车辆连接该基站
	//那么，无论该车辆怎样降低付款，都能获得分配，故车辆的付款会变得极小
	//解决方法是：增大实验的样本数量（读入的车辆数），保证资源分配具有竞争性，即：
	// 对于各个基站而言，每个时间槽 t 都应当能连接多个车辆，此时，若车辆过分降低投标，就会输掉拍卖。

	for (auto ARCarit = cts_AR.begin(); ARCarit != cts_AR.end(); ARCarit++)	// cts_AR = current time slot_Allocate Result
	{
		//int modbidNum = 1;
		double AR_i_bid = 0.00, UB_i = 0.00, LB_i = 0.00;
		//使用临时变量 AR_i_bid 保存 (*carit).bid，在不改变 (*carit).bid 的情况下，满足算法的使用需要
		AR_i_bid = (*ARCarit).bid;
		UB_i = AR_i_bid;
		AR_i_bid = (double)((UB_i + LB_i) / (double)2);	//根据二分法，先降低 AR_i_bid

		while (double(fabs(UB_i - LB_i)) >= epsilon)	//还未求得最小的获胜投标(付款)
		{
			vector<EcsInfo>Ecs_cp(ECS);
			//vector<CarInfo>BACar_cp(BACar);
			vector<CarInfo>UACar_cp(UACar);	//已经被修改 n+1 次bid，不改变UACar[]的任何值，保证上一个胜者的投标恢复

			//改变对应的 UACar_cp[k].id 的投标，再运行 OTRAP_ALLOC
			for (auto UACarit = UACar_cp.begin(); UACarit != UACar_cp.end(); UACarit++)
			{
				if ((*UACarit).id == (*ARCarit).id)
				{
					(*UACarit).bid = AR_i_bid;
					//cout << "第 " << modbidNum << " 次修改后：获胜车辆" << UACar_cp[k].id <<" 的投标为：" << UACar_cp[k].bid << endl;
					//modbidNum++;
				}
			}

			//cout << "OTRAP_PAY 中 运行 OTRAP_ALLOC : " << endl;
			DTVC_OTRAP_ALLOC(cts, Ecs_cp, UACar_cp);
			//此时，tmp_X[]存放：当前时间槽 t ，获胜者 i 降低投标后，再运行OTRAP_ALLOC选出的胜者车辆集合
			//修改 UACar[i].bid, 直到能够确定最佳投标(付款)
			bool StillWin = 0;	//标志位（默认失败）：标识 XCarit 降低投标后是否获胜
			for (auto XCarit = tmp_X.begin(); XCarit != tmp_X.end(); XCarit++)
			{
				if ((*XCarit).id == (*ARCarit).id)	//降低投标后，UACar[i]依然获胜
				{
					StillWin = 1;
					UB_i = (*XCarit).bid;		//仍然获胜的投标作为UB_i，tmp_X[k].bid = AR_i_bid = UACar[k].bid
					AR_i_bid = (double)((UB_i + LB_i) / (double)2);	//再降低 UACar[i].bid
					break;
				}
			}
			if (StillWin == 0)	//降低投标后(投标值为 AR_i_bid)，导致 UACar[i] 不能获胜
			{
				LB_i = AR_i_bid;	//将引起失败的投标值作为LB_i
				AR_i_bid = (double)((UB_i + LB_i) / (double)2);	//适当提高 UACar[i].bid
			}
		}
		(*ARCarit).pay = (double)((UB_i + LB_i) / (double)2);
	}
}

bool EcsInDegree_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2)
{
	if (ecs1.InDegree == ecs2.InDegree)	//若基站的入度相等，按照基站 id 升序排列
		return ecs1.id < ecs2.id;
	else
		return ecs1.InDegree < ecs2.InDegree;
}

void CalcCarValueDensity(EcsInfo& ecs, int cts)
{
	//1.计算车辆 i 的主导资源比例
	//读取 ECS 的资源容量矩阵，在“当前时间槽”列至“当前时间槽 + 车辆执行时间”列的资源容量
	for (auto carit = ecs.AbleConnectToECS.begin(); carit != ecs.AbleConnectToECS.end();)
	{
		//测试：输出 car 的时变资源需求矩阵(成功)
		//bug: 相同时间槽时，打印出的 RDM 是相同的，出错点在 CarInfo.h 中，重载赋值运算符= 中的 .RDM 赋值中，（已解决）
		if ((*carit).at <= (*carit).dt - (*carit).et + 1)
		{
			vector<vector<int>> dr_RCM = vector<vector<int>>(R, vector<int>((*carit).et, 0));	//存放 在“当前时间槽”列至“当前时间槽 + 车辆执行时间”列的基站资源容量
			auto dr_RCM_rit = dr_RCM.begin();	//行迭代器
			auto RCM_rit = ecs.RCM.begin();		//遍历 基站 ecs 的资源容量矩阵的 行迭代器
			for (; (RCM_rit != ecs.RCM.end()) && (dr_RCM_rit != dr_RCM.end()); RCM_rit++, dr_RCM_rit++)
			{
				auto RCM_cit = (*RCM_rit).begin() + cts - 1;	//基站的资源容量矩阵 定位到“当前时间槽”列
				auto dr_RCM_cit = (*dr_RCM_rit).begin();
				for (; (RCM_cit != (*RCM_rit).begin() + cts - 1 + (*carit).et) && (dr_RCM_cit != (*dr_RCM_rit).end()); RCM_cit++, dr_RCM_cit++)
				{
					(*dr_RCM_cit) = (*RCM_cit);
				}
			}
			////测试：截取的 RC 矩阵是否正确(成功)
			//cout << "car[" << (*carit).id << "].et = " << (*carit).et << ", 其连接的 ECS[" << ecs.id << "] 在时间槽 [cts, cts + et] = [" << cts << "," << cts + (*carit).et - 1 << "] 列的资源容量矩阵为：" << endl;
			//for (auto rit = dr_RCM.begin(); rit != dr_RCM.end(); rit++)
			//{
			//	for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			//	{
			//		cout << (*cit) << " ";
			//	}
			//	cout << endl;
			//}
			////测试：输出 car 的时变资源需求矩阵(成功)
			//cout << "car[" << (*carit).id << "].et = " << (*carit).et << ", 其资源需求矩阵为：" << endl;
			//for (auto rit = (*carit).RDM.begin(); rit != (*carit).RDM.end(); rit++)
			//{
			//	for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			//	{
			//		cout << (*cit) << " ";
			//	}
			//	cout << endl;
			//}

			double max_dr = 0;	//暂存主导资源比例
			dr_RCM_rit = dr_RCM.begin();
			auto RDM_rit = (*carit).RDM.begin();
			for (; (RDM_rit != (*carit).RDM.end()) && (dr_RCM_rit != dr_RCM.end()); RDM_rit++, dr_RCM_rit++)
			{
				auto dr_RCM_cit = (*dr_RCM_rit).begin();
				auto RDM_cit = (*RDM_rit).begin();
				for (; (RDM_cit != (*RDM_rit).end()) && (dr_RCM_cit != (*dr_RCM_rit).end()); RDM_cit++, dr_RCM_cit++)
				{
					//dr = 车辆在 特定时间槽 t 请求的资源量 / ECS在特定时间槽 t 时的资源容量
					double tmp = (double)((*RDM_cit) / (double)(*dr_RCM_cit));
					if (tmp > max_dr)
					{
						max_dr = tmp;
					}
				}
			}

			(*carit).dr = max_dr;	//算出 主导资源比例
			(*carit).f = (double)((*carit).bid / ((*carit).dr * (double)(*carit).et));	//算出 资源密度

			//测试
			//cout << "car[" << (*carit).id << "].dr = " << (*carit).dr << ", car[" << (*carit).id << "].f = " << (*carit).f << endl;
			//cout << endl;

			carit++;
		}
		else
		{
			carit = ecs.AbleConnectToECS.erase(carit);
		}
	}
}

bool CarValueDensity_Descending(const CarInfo& car1, const CarInfo& car2)
{
	if (car1.f == car2.f)	//若车辆的资源密度相等，按照到达时间升序排列（越早到达，任务越紧急）
		return car1.at < car2.at;
	else
		return car1.f > car2.f;
}

bool EscId_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2)
{
	return ecs1.id < ecs2.id;
}

//第三步：将结果保存到本地
void SaveResult()
{
	CalcResourceUtilizationRatio();
	CalcServedCarRatio();
	CalcNormalPayCarRatio();

	ofstream ofs;

	// 根据实验要求，每次实验之前都要修改 保存路径 
	// 保存算法 OTRAP 的求解结果
	string OTRAP_SolutionFile = "../data/ExperimentalResults/exp4/5DTVC_OTRAP_Solution_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + "_" + to_string(lose_car_id) + "(" + to_string(lose_car_bid) + ").txt";
	ofs.open(OTRAP_SolutionFile, ios::out | ios::trunc);
	//数据保存格式：
	//车辆序号	车辆ID(车辆类型)	初始位置	速度	到达时间	截止时间	执行时间	任务资源需求			投标(系数)			得到分配的时间	连接的基站(基站类型)	连接情况		付款
	//SeqNum	CarId(type)			InitLoc		V		AT			DT			ET			TaskResourceDemand		Bid(Coefficient)	BAT				ConnectToECS(type)		Connectivity	Payment
	ofs << "SeqNum\t" << "Car_Id(Type)\t" << "AT\t" << "DT\t" << "ET\t" << "TaskResourceDemand\t" << "Bid(coefficient)\t" << "ConnectToECS(type)\t" << "BeAllocatedTime\t" << "Connectivity\t" << "Payment" << endl;
	int seqnum = 1;
	for (auto car = AllocResult.begin(); car != AllocResult.end(); car++, seqnum++)
	{
		ofs << seqnum << "\t";
		ofs << car->id << "(" << car->type << ")" << "\t\t";
		ofs << car->at << "\t";
		ofs << car->dt << "\t";
		ofs << car->et << "\t";
		ofs << car->TRD << "\t\t\t";
		ofs << car->bid << "(" << car->bc << ")\t";
		ofs << car->ConnectTo << "(";
		for (auto ecs = vEcsInfo.begin(); ecs != vEcsInfo.end(); ecs++)
		{
			if (ecs->id == car->ConnectTo)
			{
				ofs << ecs->type;
			}
		}
		ofs << ")\t\t";
		ofs << car->bat << "\t\t";
		ofs << car->alloc_flag << "\t\t";
		ofs << car->pay << endl;
	}
	ofs << endl;

	//输出求得的总社会福利
	ofs << "1.社会福利: SocialWelfare_max = " << TotalSocialWelfare << endl;
	ofs << endl;

	//输出算法运行时间
	ofs << "2.算法运行时间: RunTime = " << RunTime << " ms" << endl;
	//测试：找出运行时间长的原因
	ofs << "(1)DTVC_OTRAP 准备工作运行时间：PreWork_RunTime = " << PreWork_RunTime << " ms" << endl;
	//ofs << "其中，回收资源所花时间：ReturnResource_RunTime = " << ReturnResource_RunTime << " ms" << endl;
	ofs << "其中，删除过期任务所花时间：DelOvertimeTask_RunTime = " << DelOvertimeTask_RunTime << " ms" << endl;
	ofs << "其中，添加新到达车辆所花时间：AddArrivedCar_RunTime = " << AddArrivedCar_RunTime << " ms" << endl;
	ofs << "(2)DTVC_OTRAP 分配函数运行时间：ALLOC_RunTime = " << ALLOC_RunTime << " ms" << endl;
	ofs << "(3)DTVC_OTRAP 支付函数运行时间：PAY_RunTime = " << PAY_RunTime << " ms" << endl;
	ofs << endl;

	//输出资源利用率
	ofs << "3.资源利用率: Resource Utilization " << endl;
	ofs << "(1)总CPU利用率: TRU_cpu = " << TRU_cpu << endl;
	ofs << "(2)总GPU利用率: TRU_gpu = " << TRU_gpu << endl;
	ofs << "(3)总Memory利用率: TRU_memory = " << TRU_memory << endl;
	ofs << "(4)总Disk利用率: TRU_disk = " << TRU_disk << endl;
	ofs << endl;

	//输出服务车辆比例
	ofs << "4.服务车辆数(比例): T_ServedCarNum(T_ServedCarRatio) = " << T_ServedCarNum << "(" << T_ServedCarRatio << ")" << endl;
	ofs << "其中，小型车辆任务服务数(比例): S_ServedCarNum(S_ServedCarRatio) = " << S_ServedCarNum << "(" << S_ServedCarRatio << ")" << endl;
	ofs << "其中，大型车辆任务服务数(比例): L_ServedCarNum(L_ServedCarRatio) = " << L_ServedCarNum << "(" << L_ServedCarRatio << ")" << endl;
	ofs << endl;

	//输出正确付款的车辆比例
	ofs << "5.正常付款的获胜车辆数(比例): T_NormalPayedCarNum(T_NormalPayedCarRatio) = " << T_NormalPayedCarNum << "(" << T_NormalPayedCarRatio << ")" << endl;
	ofs << "其中，小型获胜车辆正常付款数(比例): S_NormalPayedCarNum(S_NormalPayedCarRatio) = " << S_NormalPayedCarNum << "(" << S_NormalPayedCarRatio << ")" << endl;
	ofs << "其中，大型获胜车辆正常付款数(比例): L_NormalPayedCarNum(L_NormalPayedCarRatio) = " << L_NormalPayedCarNum << "(" << L_NormalPayedCarRatio << ")" << endl;
	ofs.close();
}

void CalcResourceUtilizationRatio()
{
	////统计使用了的资源

	////第一种计算方式（舍弃）
	////容器 AllocResult 中的所有车辆均已得到分配
	//for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
	//{
	//	UsedResource.cpu += (*ARit).TRD.cpu * (*ARit).et;
	//	UsedResource.gpu += (*ARit).TRD.gpu * (*ARit).et;
	//	UsedResource.memory += (*ARit).TRD.memory * (*ARit).et;
	//	UsedResource.disk += (*ARit).TRD.disk * (*ARit).et;
	//}

	//// 第二种计算方式（舍弃）
	////容器 AllocResult 中的所有车辆均已得到分配
	//for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
	//{
	//	int rdm_row = 1;	//记录：遍历到 rdm 的第几行
	//	for (auto rit = (*ARit).RDM.begin(); (rit != (*ARit).RDM.end()) && (rdm_row <= 4); rit++, rdm_row++)
	//	{
	//		int rdm_column = 1;	//记录：遍历到 rdm 的每行中的第几列
	//		for (auto cit = (*rit).begin(); (cit != (*rit).end()) && (rdm_column <= (*ARit).et); cit++, rdm_column++)
	//		{
	//			if (rdm_row == 1)	//统计 cpu 的使用量 : 已分配的资源 * 执行时间 = cpu * et
	//			{
	//				UsedResource.cpu += (*cit) * ((*ARit).et - rdm_column + 1);
	//			}
	//			else if (rdm_row == 2)
	//			{
	//				UsedResource.gpu += (*cit) * ((*ARit).et - rdm_column + 1);
	//			}
	//			else if (rdm_row == 3)
	//			{
	//				UsedResource.memory += (*cit) * ((*ARit).et - rdm_column + 1);
	//			}
	//			else if (rdm_row == 4)
	//			{
	//				UsedResource.disk += (*cit) * ((*ARit).et - rdm_column + 1);
	//			}
	//		}
	//	}
	//}

	//第三种计算方式(累加资源需求向量即可)
	for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
	{
		UsedResource.cpu += (*ARit).TRD.cpu;
		UsedResource.gpu += (*ARit).TRD.gpu;
		UsedResource.memory += (*ARit).TRD.memory;
		UsedResource.disk += (*ARit).TRD.disk;
	}

	//统计所有基站的资源容量
	for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
	{
		TotalResource.cpu += (*ecsit).RC.cpu;
		TotalResource.gpu += (*ecsit).RC.gpu;
		TotalResource.memory += (*ecsit).RC.memory;
		TotalResource.disk += (*ecsit).RC.disk;
	}

	//计算资源利用率
	TRU_cpu = (double)UsedResource.cpu / ((double)TotalResource.cpu * (double)T);
	TRU_gpu = (double)UsedResource.gpu / ((double)TotalResource.gpu * (double)T);
	TRU_memory = (double)UsedResource.memory / ((double)TotalResource.memory * (double)T);
	TRU_disk = (double)UsedResource.disk / ((double)TotalResource.disk * (double)T);
}

void CalcServedCarRatio()
{
	T_ServedCarNum = AllocResult.size();		//服务车辆总数
	//统计：得到分配的车辆的任务类型数量
	for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
	{
		if ((*ARit).type == "S")
		{
			S_ServedCarNum++;
		}
		else if ((*ARit).type == "L")
		{
			L_ServedCarNum++;
		}
		else
		{
			cout << "容器 AllocResult[] 中，获胜车辆的类型统计错误！" << endl;
		}
	}

	//只要读入的车辆任务信息不为空
	if (N != 0)
	{
		T_ServedCarRatio = (double)T_ServedCarNum / (double)N;
	}
	if (RS_S_CarNum != 0)
	{
		S_ServedCarRatio = (double)S_ServedCarNum / (double)T_ServedCarNum;
	}
	if (RS_L_CarNum != 0)
	{
		L_ServedCarRatio = (double)L_ServedCarNum / (double)T_ServedCarNum;
	}
}

void CalcNormalPayCarRatio()
{
	//统计 正常付款的获胜车辆任务的 数量 和 类型
	for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
	{
		if ((*ARit).pay > epsilon)	//正常付款
		{
			T_NormalPayedCarNum++;
			if ((*ARit).type == "S")
			{
				S_NormalPayedCarNum++;
			}
			else if ((*ARit).type == "L")
			{
				L_NormalPayedCarNum++;
			}
		}
	}
	if (!AllocResult.empty())
	{
		int AllocatedCar = AllocResult.size();
		T_NormalPayedCarRatio = (double)T_NormalPayedCarNum / (double)AllocatedCar;
	}
	if (T_NormalPayedCarNum != 0)
	{
		S_NormalPayedCarRatio = (double)S_NormalPayedCarNum / (double)T_NormalPayedCarNum;
		L_NormalPayedCarRatio = (double)L_NormalPayedCarNum / (double)T_NormalPayedCarNum;
	}
}

int main()
{
	srand((unsigned)time(NULL));

	T_vCarInfo.reserve(20000);
	//BeAllocatedCar.reserve(20000), 
	UnAllocatedCar.reserve(20000), AllocResult.reserve(20000);
	cts_AllocResult.reserve(20000), tmp_X.reserve(20000);

	//for (; ExeNum <= count_DTVC_OTRAP; ExeNum++)
	if (ExeNum <= count_DTVC_OTRAP)		//检查 bug 使用
	{
		cout << "这是第 " << ExeNum << " 次运行 DTVC_OTRAP 算法 ! " << endl;

		//准备工作，重置使用过的数据和容器
		TotalSocialWelfare = 0, cts_SocialWelfare = 0;

		RunTime = 0;
		//ReturnResource_RunTime = 0, 
		PreWork_RunTime = 0, DelOvertimeTask_RunTime = 0, AddArrivedCar_RunTime = 0;
		ALLOC_RunTime = 0, PAY_RunTime = 0;

		UsedResource.cpu = 0, UsedResource.gpu = 0, UsedResource.memory = 0, UsedResource.disk = 0;
		TotalResource.cpu = 0, TotalResource.gpu = 0, TotalResource.memory = 0, TotalResource.disk = 0;
		TRU_cpu = 0, TRU_gpu = 0, TRU_memory = 0, TRU_disk = 0;

		T_ServedCarNum = 0, S_ServedCarNum = 0, L_ServedCarNum = 0;
		T_ServedCarRatio = 0, S_ServedCarRatio = 0, L_ServedCarRatio = 0;

		T_NormalPayedCarNum = 0, S_NormalPayedCarNum = 0, L_NormalPayedCarNum = 0;
		T_NormalPayedCarRatio = 0, S_NormalPayedCarRatio = 0, L_NormalPayedCarRatio = 0;

		//BeAllocatedCar.clear(), 
		vEcsInfo.clear(), T_vCarInfo.clear(), UnAllocatedCar.clear();
		AllocResult.clear(), cts_AllocResult.clear(), tmp_X.clear();

		//第一步：读入数据
		LoadEcsInfo(EcsInfoFile, EcsInfoFile_RCM);

		LoadCarInfo(RS_CarInfoFile_S, RS_CarInfoFile_S_RDM, RS_CarInfoFile_S_DCM, T_vCarInfo, RS_S_CarNum);	//读入小型车辆信息
		//LoadCarInfo(RS_CarInfoFile_L, RS_CarInfoFile_L_RDM, RS_CarInfoFile_L_DCM, T_vCarInfo, RS_L_CarNum);	//读入大型车辆信息

		int count_S_Car = 0, count_L_Car = 0;
		for (auto carit = T_vCarInfo.begin(); carit != T_vCarInfo.end(); carit++)
		{
			if (carit->type == "S")
				count_S_Car++;
			else if (carit->type == "L")
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

		//测试：是否正确读入基站和车辆信息（成功）

		/*cout << "读入的基站信息如下：" << endl;
		for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
		{
			cout << (*ecsit) << endl;
		}
		cout << "读入的车辆信息如下：" << endl;
		for (auto carit = T_vCarInfo.begin(); carit != T_vCarInfo.end(); carit++)
		{
			cout << (*carit) << endl;
		}*/

		cout << "在时间槽[1, " << T << "]，开始运行 DTVC_OTRAP 算法！" << endl;

		time_t tBegin, tEnd;
		tBegin = clock();	//开始计时

		//第二步：每个时间槽执行一次 DTVC_OTRAP 算法
		for (int t = 1; t <= T; t++)
		{
			//cout << "当前时间槽 t = " << t << ", 执行 DTVC_OTRAP 算法：" << endl;
			DTVC_OTRAP(t, vEcsInfo, T_vCarInfo);
		}

		tEnd = clock();	//计时结束

		cout << "第 " << ExeNum << " 次运行 DTVC_OTRAP 算法结束！" << endl;
		cout << endl;

		RunTime = static_cast<double>(tEnd - tBegin) / CLOCKS_PER_SEC * 1000;

		////测试：能否正确展示结果(成功)
		////社会福利
		//cout << "整个运行时间[1," << T << "]中，DTVC_OTRAP 算法求解的社会福利为：SocialWelfare = " << TotalSocialWelfare << endl;
		////运行时间
		//cout << " DTVC_OTRAP 算法的运行时间为：RunTime = " << RunTime << "ms" << endl;
		////分配结果
		//cout << "整个运行时间[1," << T << "]中，DTVC_OTRAP 算法求解的分配结果为：AllocResult = " << endl;
		//for (auto ARit = AllocResult.begin(); ARit != AllocResult.end(); ARit++)
		//{
		//	cout << (*ARit) << endl;
		//}
		////基站资源容量
		//cout << "分配后的基站资源容量如下：" << endl;
		//for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
		//{
		//	cout << (*ecsit) << endl;
		//}

		//第三步：将结果保存到本地
		SaveResult();
	}

	system("pause");
	return 0;
}
