#include "2RandomSampling.h"

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

//1.读入 1GenerateData 生成的大数据库
//(1)读入基站信息
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile)
{
	ifstream ifs_ECS(EcsInfofile);		//读入基站信息 (除 资源容量矩阵 外)
	ifstream ifs_ECS_RCM(EcsRCMfile);	//读入基站的资源容量矩阵
	for (int j = 0; j < M; j++)
	{
		EcsInfo* ecs = new EcsInfo;	//临时 EcsInfo 对象
		ifs_ECS >> ecs->id;		//读入 ECS 编号
		ifs_ECS >> ecs->type;	//读入 ECS 类型
		ifs_ECS >> ecs->SCR;		//读入 ECS 信号覆盖半径
		ifs_ECS >> ecs->DL.x;	//读入 ECS 部署位置 x 坐标
		ifs_ECS >> ecs->DL.y;	//读入 ECS 部署位置 y 坐标
		ifs_ECS >> ecs->RC.cpu;	//读入 ECS 的资源容量
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
//(2)读入车辆信息
void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, vector<CarInfo>& Car, int CarNum)
{
	ifstream ifs_Car(CarInfofile);		//读入车辆任务信息
	ifstream ifs_Car_RDM(CarRDMfile);	//读入车辆资源需求矩阵
	ifstream ifs_Car_DCM(CarDCMfile);	//读入车辆部署约束矩阵
	for (int i = 0; i < CarNum; i++)	//读入车辆信息
	{
		//提示栈空间不足的原因是，每辆车的CM占用的栈空间(10*1440)过大
		// 1.使用 vector<vector<bool>> 存储 CM，可避免该问题
		// 2.动态指针也可解决该问题(采用)
		CarInfo* car = new CarInfo;
		ifs_Car >> car->id;			//读入车辆任务编号
		ifs_Car >> car->type;		//读入车辆任务类型
		ifs_Car >> car->at;			//读入任务到达时间
		ifs_Car >> car->dt;			//读入任务截止时间
		ifs_Car >> car->et;			//读入任务执行时间
		ifs_Car >> car->TRD.cpu;	//读入任务资源容量
		ifs_Car >> car->TRD.gpu;
		ifs_Car >> car->TRD.memory;
		ifs_Car >> car->TRD.disk;
		ifs_Car >> car->bid;		//读入投标
		ifs_Car >> car->bc;			//读入投标系数

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(car->et, false)));
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

		Car.push_back(*car);		//读入的对应类型的车辆容器中
		T_vCarInfo.push_back(*car);	//读入的所有类型的车辆容器中
		delete car;
	}
	ifs_Car.close();
	ifs_Car_DCM.close();

	int count_S_Car = 0, count_L_Car = 0;
	for (int i = 0; i < CarNum; i++)
	{
		if (Car[i].type == "S")
			count_S_Car++;
		else if (Car[i].type == "L")
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

//2.随机抽取五组车辆数据，每组车辆数据个数为 100, 500, 5000, 10000, 20000。且每组数据要抽取10次。
// 供算法 VCG-TRAP 、OVMAP* 和 OTRAP 调入使用。
void RandomSampling(vector<CarInfo>& Car, vector<CarInfo>& rsCar, string type, int SamplingNum)
{
	if (SamplingNum <= Car.size())
	{
		//打乱车辆在容器中的排列顺序
		random_shuffle(Car.begin(), Car.end());
		for (vector<CarInfo>::iterator carit = Car.begin(); carit != Car.begin() + SamplingNum; carit++)
		{
			rsCar.push_back(*carit);			//随机抽取的对应类型的车辆任务
			RS_T_vCarInfo.push_back(*carit);	//随机抽取的所有类型的车辆任务
		}
	}
	else
	{
		cout << "随机抽取的车辆数大于生成的车辆数( " << SamplingNum << " > " << Car.size() << " )，无法正确抽取！ " << endl;
	}

	cout << "已经随机抽取 " << SamplingNum << " 条 " << type << " 型车辆任务信息！" << endl;
}

//3.将信息保存到本地
//(1)将基站信息保存到本地(原封不动)
void SaveEcsInfo()
{
	ofstream ofs;
	//(1)展示给用户浏览的基站信息(简化版)
	//string EcsInfoFile_show = "../data/2RandomSampling/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//保存示例ECS数据
	//string EcsInfoFile_show = "../data/2RandomSampling/exp1/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验一数据
	//string EcsInfoFile_show = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验二数据
	//string EcsInfoFile_show = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验三数据
	string EcsInfoFile_show = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验三数据(不带投标系数)
	ofs.open(EcsInfoFile_show, ios::out | ios::trunc);
	ofs << "ECS_id\t" << "Type\t" << "SignalCoverageRadius(m)\t" << "DeploymentLocation(coord)\t" << "ResourceCapacity\t" << endl;
	for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
	{
		ofs << ecsit->id << "\t";			//基站编号
		ofs << ecsit->type << "\t";			//基站类型
		ofs << ecsit->SCR << "\t\t\t";		//基站信号覆盖半径
		ofs << ecsit->DL << "\t\t\t";		//基站部署位置（坐标点）
		ofs << ecsit->RC << "\t";           //基站资源容量
		ofs << endl;
	}
	ofs.close();

	cout << "已成功将 " << tEcsNum << "条基站信息写入 EcsInfo.txt 中，供用户浏览。" << endl;
	cout << "其中，小型基站数量为：" << S_EcsNum << "，"
		<< "大型基站数量为：" << L_EcsNum << "。" << endl;

	//(2)供算法输入使用的基站信息：除资源容量矩阵 RCM (带前缀 data_ )
	//string EcsInfoFile_input = "../data/2RandomSampling/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//保存示例ECS数据
	//string EcsInfoFile_input = "../data/2RandomSampling/exp1/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验一数据
	//string EcsInfoFile_input = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验二数据
	//string EcsInfoFile_input = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(M_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验三数据
	string EcsInfoFile_input = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验三数据(不带投标系数)
	ofs.open(EcsInfoFile_input, ios::out | ios::trunc);
	for (vector<EcsInfo>::iterator ecs = vEcsInfo.begin(); ecs != vEcsInfo.end(); ecs++)
	{
		ofs << ecs->id << " ";							//基站编号
		ofs << ecs->type << " ";						//基站类型 S/M/L
		ofs << ecs->SCR << " ";							//基站工作半径 CoverageRadius
		ofs << ecs->DL.x << " " << ecs->DL.y << " ";	//基站位置 Location	
		ofs << ecs->RC.cpu << " " << ecs->RC.gpu << " " << ecs->RC.memory << " " << ecs->RC.disk << " ";	//基站资源容量 ResourceCapacity
		ofs << endl;
	}
	ofs.close();

	//(3)供算法输入使用的基站信息：资源容量矩阵 RCM (带前缀 data_ )
	//string EcsRCMFile_input = "../data/2RandomSampling/exp1/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验一数据
	//string EcsRCMFile_input = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验二数据
	string EcsRCMFile_input = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";	//抽取实验二数据
	ofs.open(EcsRCMFile_input, ios::out | ios::trunc);
	for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
	{
		for (auto rit = (*ecsit).RCM.begin(); rit != (*ecsit).RCM.end(); rit++)
		{
			for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
			{
				ofs << (*cit) << " ";
			}
			ofs << endl;
		}
		ofs << endl;
	}
	ofs.close();

	cout << "已成功将 " << tEcsNum << "条基站信息写入 data_EcsInfo.txt 中，供算法输入使用。" << endl;
	cout << "其中，小型基站数量为：" << S_EcsNum << "，"
		<< "大型基站数量为：" << L_EcsNum << "。" << endl;

}

//(2)将随机抽取的车辆数据保存到本地
void SaveCarInfo()
{
	//1. 保存“供用户查看”的随机抽取的车辆信息
	ofstream ofs;
	//(1)保存随机抽取的车辆任务信息(除时变部署约束矩阵 DCM)
	//string CarInfoFile_show = "../data/2RandomSampling/RS_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//保存示例数据
	//string CarInfoFile_show = "../data/2RandomSampling/exp1/RS_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验一数据
	//string CarInfoFile_show = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/RS_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验二数据
	//string CarInfoFile_show = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/RS_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据
	string CarInfoFile_show = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/RS_CarInfo_" + to_string(RS_S_CarNum) + "_" + to_string(RS_L_CarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据（不带投标系数）
	ofs.open(CarInfoFile_show, ios::out | ios::trunc);	//如果文件存在，先删除，再创建
	
	//第一种形式：当车辆数量较多时，可用该种行驶保存“供用户浏览的车辆信息”
	ofs << "Car_id\t" << "Type\t" << "ArrivalTime\t" << "DeadlineTime\t" << "ExecutionTime\t" << "TaskResourceDemand\t" << "Bid(Coefficient)\t" << endl;
	for (auto rscar = RS_T_vCarInfo.begin(); rscar != RS_T_vCarInfo.end(); rscar++)
	{
		//数据格式：编号       类型   初始位置(坐标)   行驶方向            速度  到达时间    截止时间        执行时间        任务资源需求          投标(投标系数)	
		//数据格式：Car_id     Type   InitLoc          DrivingDirection    v     ArrivalTime DeadlineTime    ExecutionTime   TaskResourceDemand    Bid(Coefficient)
		ofs << rscar->id << "\t";			//车辆编号
		ofs << rscar->type << "\t";			//车辆类型
		ofs << rscar->at << "\t\t";        //车辆任务到达时间
		ofs << rscar->dt << "\t\t";        //车辆任务截止时间
		ofs << rscar->et << "\t\t";        //车辆任务执行时间
		ofs << rscar->TRD << "\t\t\t";		//车辆任务资源需求
		ofs << rscar->bid << "(" << rscar->bc << ")" << endl;     //车辆投标(投标系数)
	}
	ofs.close();

	////第二种形式（示例用）：详细展示了所有车辆的信息，当车辆数量较少时，可用该种行驶保存“供用户浏览的车辆信息”
	//for (auto rscar = RS_T_vCarInfo.begin(); rscar != RS_T_vCarInfo.end(); rscar++)
	//{
	//	//数据格式：编号       类型   初始位置(坐标)   行驶方向            速度  到达时间    截止时间        执行时间        任务资源需求          投标(投标系数)	
	//	//数据格式：Car_id     Type   InitLoc          DrivingDirection    v     ArrivalTime DeadlineTime    ExecutionTime   TaskResourceDemand    Bid(Coefficient)
	//	ofs << "Car_id = " << rscar->id << ",    ";                //车辆编号
	//	ofs << "Type = " << rscar->type << ",    ";                //车辆类型
	//	ofs << "ArrivalTime = " << rscar->at << ",    ";           //车辆任务到达时间
	//	ofs << "DeadlineTime = " << rscar->dt << ",    ";          //车辆任务截止时间
	//	ofs << "ExecutionTime = " << rscar->et << ",    ";         //车辆任务执行时间
	//	ofs << "TaskResourceDemand = " << rscar->TRD << ",    ";   //车辆任务资源需求
	//	ofs << "Bid(Coefficient) = " << rscar->bid << "(" << rscar->bc << ")" << endl;     //车辆投标(投标系数)
	//	ofs << "ResourceDemandMatrix = " << endl;   //车辆的时变资源需求矩阵
	//	for (auto rit = (*rscar).RDM.begin(); rit != (*rscar).RDM.end(); rit++)
	//	{
	//		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
	//		{
	//			ofs << (*cit) << " ";
	//		}
	//		ofs << endl;
	//	}
	//	ofs << "DeploymentConstraintMatrix = " << endl;     //车辆的部署约束矩阵
	//	for (auto rit = (*rscar).DCM.begin(); rit != (*rscar).DCM.end(); rit++)
	//	{
	//		for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
	//		{
	//			ofs << (*cit) << " ";
	//		}
	//		ofs << endl;
	//	}
	//	ofs << endl;
	//}
	//ofs.close();

	cout << "已成功将 " << RS_T_CarNum << "条车辆信息写入 CarInfo.txt 中，供用户浏览。" << endl;
	cout << "其中，小型车辆数量为：" << RS_S_CarNum << "，"
		<< "大型车辆数量为：" << RS_L_CarNum << "。" << endl;

	//2. 保存“供算法调入使用”的随机抽取的车辆信息
	SaveCarInfoAsInput(RS_S_vCarInfo, "S", RS_S_vCarInfo.size());
	SaveCarInfoAsInput(RS_L_vCarInfo, "L", RS_L_vCarInfo.size());

	cout << "已成功将 " << RS_T_CarNum << "条车辆任务写入 data_CarInfo.txt 中，供算法输入使用。" << endl;
	cout << "其中，小型车辆任务数目为：" << RS_S_CarNum << ","
		<< "大型车辆任务数目为：" << RS_L_CarNum << "." << endl;

}
//封装一个函数，用来 保存“供算法输入使用”的车辆任务信息
void SaveCarInfoAsInput(vector<CarInfo>& rsCar, string type, int rsCarNum)
{
	ofstream ofs;
	//(1)保存随机抽取的车辆信息（不包括车辆的 时变资源请求矩阵RDM 和 时变部署约束矩阵DCM）
	//string CarInfoFile_Input = "../data/2RandomSampling/data_RS_CarInfo_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//保存示例数据
	//string CarInfoFile_Input = "../data/2RandomSampling/exp1/data_RS_CarInfo_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验一数据使用
	//string CarInfoFile_Input = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/data_RS_CarInfo_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验二数据使用
	//string CarInfoFile_Input = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/data_RS_CarInfo_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用
	string CarInfoFile_Input = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/data_RS_CarInfo_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用（不带投标系数）
	ofs.open(CarInfoFile_Input, ios::out | ios::trunc);
	for (vector<CarInfo>::iterator rscar = rsCar.begin(); rscar != rsCar.end(); rscar++)
	{
		//数据格式：编号   类型   出发点   速度 到达时间 截止时间 执行时间 任务资源需求			投标	投标系数		
		//数据格式：CarId  Type   InitLoc  v    at       dt       et       TaskResourceDemand   b		BidCoefficient   
		ofs << rscar->id << " ";		//车辆编号：表示这是第几辆车子 
		ofs << rscar->type << " ";		//车辆/任务类型：S/M/L 
		ofs << rscar->at << " ";		//车辆到达时间/产生任务时间：Arrival Time
		ofs << rscar->dt << " ";		//任务截止时间：Deadline Time
		ofs << rscar->et << " ";		//执行任务时间：Execution Time			
		ofs << rscar->TRD.cpu << " " << rscar->TRD.gpu << " " << rscar->TRD.memory << " " << rscar->TRD.disk << " ";	//任务资源需求：TaskResourceDemand
		ofs << rscar->bid << " ";		//投标: Bid
		ofs << rscar->bc << endl;		//投标系数: BidCoefficient
	}
	ofs.close();

	//(2)保存随机抽取的车辆资源需求矩阵 RDM
	//string CarRDMFile_Input = "../data/2RandomSampling/data_RS_Car_RDM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取示例数据使用
	//string CarRDMFile_Input = "../data/2RandomSampling/exp1/data_RS_Car_RDM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验一数据使用
	//string CarRDMFile_Input = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/data_RS_Car_RDM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验二数据使用
	//string CarRDMFile_Input = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/data_RS_Car_RDM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用
	string CarRDMFile_Input = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/data_RS_Car_RDM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用(不带投标系数)
	ofs.open(CarRDMFile_Input, ios::out | ios::trunc);
	for (vector<CarInfo>::iterator rscar = rsCar.begin(); rscar != rsCar.end(); rscar++)
	{
		for (vector<vector<int>>::iterator rit = rscar->RDM.begin(); rit != rscar->RDM.end(); rit++)	//遍历矩阵 DCM 的行：M
		{
			for (vector<int>::iterator cit = (*rit).begin(); cit != (*rit).end(); cit++)	//遍历矩阵 DCM 每行中的每列：T
			{
				ofs << (*cit) << " ";
			}
			ofs << endl;
		}
		ofs << endl;
	}
	ofs.close();

	//(3)保存随机抽取的车辆部署约束矩阵 DCM
	//string CarDCMFile_Input = "../data/2RandomSampling/data_RS_Car_DCM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取示例数据使用
	//string CarDCMFile_Input = "../data/2RandomSampling/exp1/data_RS_Car_DCM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验一数据使用
	//string CarDCMFile_Input = "../data/2RandomSampling/exp2/data_CarInfo_50%_5000_5000/data_RS_Car_DCM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验二数据使用
	//string CarDCMFile_Input = "../data/2RandomSampling/exp3/EcsInfo_9_0_1/data_RS_Car_DCM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用
	string CarDCMFile_Input = "../data/2RandomSampling/exp3_no_bc/EcsInfo_8_0/data_RS_Car_DCM_" + type + "_" + to_string(rsCarNum) + "-" + to_string(ExeNum) + ".txt";	//抽取实验三数据使用(不带投标系数)
	ofs.open(CarDCMFile_Input, ios::out | ios::trunc);
	for (vector<CarInfo>::iterator rscar = rsCar.begin(); rscar != rsCar.end(); rscar++)
	{
		for (vector<vector<bool>>::iterator rit = rscar->DCM.begin(); rit != rscar->DCM.end(); rit++)	//遍历矩阵 DCM 的行：M
		{
			for (vector<bool>::iterator cit = (*rit).begin(); cit != (*rit).end(); cit++)	//遍历矩阵 DCM 每行中的每列：T
			{
				ofs << (*cit) << " ";
			}
			ofs << endl;
		}
		ofs << endl;
	}
	ofs.close();

}

int main()
{
	srand((unsigned)time(NULL));

	//1.读入 1GenerateData 生成的大数据库
	time_t tLoadData_begin, tLoadData_end;
	tLoadData_begin = clock();
	//(1)读入基站信息
	LoadEcsInfo(EcsInfoFile, EcsInfoFile_RCM);
	//(2)读入车辆信息
	//预留空间 : 
	T_vCarInfo.reserve(81000);
	S_vCarInfo.reserve(41000);
	L_vCarInfo.reserve(41000);

	RS_T_vCarInfo.reserve(41000);
	RS_S_vCarInfo.reserve(21000);
	RS_L_vCarInfo.reserve(21000);
	
	//读入数据 
	//exp1: S_tCarNum = 1000(100), 5000(500), 10000(5000), 20000(10000), 40000(20000)
	//exp2: S_tCarNum = 20000(9500) L_tCarNum = 20000(500)
	LoadCarInfo(CarInfoFile_S, CarInfoFile_S_RDM, CarInfoFile_S_DCM, S_vCarInfo, S_tCarNum);	//小型车辆任务信息
	LoadCarInfo(CarInfoFile_L, CarInfoFile_L_RDM, CarInfoFile_L_DCM, L_vCarInfo, 20000);	//大型车辆任务信息

	tLoadData_end = clock();
	LoadData_RunTime = static_cast<double>(tLoadData_end - tLoadData_begin) / CLOCKS_PER_SEC;

	cout << "读入数据花费时间：LoadData_Runtime = " << LoadData_RunTime << " s" << endl;
	cout << endl;

	//测试：是否成功读入基站和车辆任务信息(成功)

	/*cout << "读入的基站信息如下：" << endl;
	for (auto ecs = vEcsInfo.begin(); ecs != vEcsInfo.end(); ecs++)
	{
		cout << "基站：id = " << ecs->id << ",    ";
		cout << "type = " << ecs->type << ",    ";
		cout << "SCR = " << ecs->SCR << ",    ";
		cout << "DL = " << ecs->DL << ",    ";
		cout << "RC = " << ecs->RC;
		cout << endl;
	}
	cout << endl;
	cout << "读入的车辆信息如下：" << endl;
	for (auto car = T_vCarInfo.begin(); car != T_vCarInfo.end(); car++)
	{
		cout << "车辆：id = " << car->id << ",    ";
		cout << "type = " << car->type << ",    ";
		cout << "at = " << car->at << ",    ";
		cout << "dt = " << car->dt << ",    ";
		cout << "et = " << car->et << ",    ";
		cout << "TRD = " << car->TRD << ",    ";
		cout << "bid = " << car->bid << ",    ";
		cout << "bc = " << car->bc << endl;
		cout << "RDM = " << endl;
		for (int r = 0; r < R; r++)
		{
			for (int t = 0; t < car->et; t++)
			{
				cout << car->RDM[r][t] << " ";
			}
			cout << endl;
		}
		cout << "DCM = " << endl;
		for (int j = 0; j < M; j++)
		{
			for (int t = 0; t < T; t++)
			{
				cout << car->DCM[j][t] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}*/

	//2.随机抽取五组车辆数据，每组车辆数据个数为 100, 500, 5000, 10000, 20000。且每组数据要抽取10次。
	// 供算法 OPT-TRAP 、OVMAP* 和 OTRAP 调入使用。
	for (; ExeNum <= SamplingNum; ExeNum++)
	//if (ExeNum <= SamplingNum)
	{
		cout << "这是第 " << ExeNum << " 次随机抽取（RandomSampling） " << RS_S_CarNum << " _ " << RS_L_CarNum << " 条车辆任务信息！" << endl;

		time_t tSampling_begin, tSampling_end;
		tSampling_begin = clock();

		RandomSampling(S_vCarInfo, RS_S_vCarInfo, "S", RS_S_CarNum);		//随机抽取小型车辆任务信息
		RandomSampling(L_vCarInfo, RS_L_vCarInfo, "L", RS_L_CarNum);		//随机抽取大型车辆任务信息

		tSampling_end = clock();
		Sampling_RunTime += static_cast<double>(tSampling_end - tSampling_begin) / CLOCKS_PER_SEC;

		time_t tSaveData_begin, tSaveData_end;
		tSaveData_begin = clock();

		//3.将信息保存到本地
		//(1)将基站信息保存到本地
		//SaveEcsInfo();
		//(2)将随机抽取的车辆数据保存到本地
		SaveCarInfo();

		tSaveData_end = clock();
		SaveData_RunTime += static_cast<double>(tSaveData_end - tSaveData_begin) / CLOCKS_PER_SEC;

		cout << "第 " << ExeNum << " 次随机采样花费时间：Sampling_RunTime = " << Sampling_RunTime << " s" << endl;
		cout << "第 " << ExeNum << " 次保存数据花费时间：SaveData_Runtime = " << SaveData_RunTime << " s" << endl;
		cout << endl;

		//保存好车辆信息后要及时清空容器，方便下次存取车辆任务信息
		RS_T_vCarInfo.clear();
		RS_S_vCarInfo.clear();
		RS_L_vCarInfo.clear();

		Sampling_RunTime = 0, SaveData_RunTime = 0;

	}

	//3.将信息保存到本地
	//(1)将基站信息保存到本地（只需要保存一次）
	SaveEcsInfo();

	system("pause");

	return 0;
}
