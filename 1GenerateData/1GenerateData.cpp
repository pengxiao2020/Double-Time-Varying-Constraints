// 1GenerateData.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "1GenerateData.h"

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


// 1.建模
void Modeling()
{
    //(1)道路建模(vector)
    int x = 1;   //标记第几行
    int y = 1;   //标记第几列
    for (vector<vector <bool>>::iterator xit = Road.begin(); xit != Road.end() && x <= X; xit++)
    {
        y = 1;
        for (vector <bool>::iterator yit = (*xit).begin(); yit != (*xit).end() && y <= Y; yit++)
        {
            if (x == 300 || x == 900 || y == 300 || y == 900)
            //if (x == 30 || x == 90 || y == 30 || y == 90) // 测试用小矩阵
            {
                (*yit) = true;
                //cout << "第 " << x << "行第 " << y << "列初始化为 1 " << endl;
            }
            else
            {
                (*yit) = false;
                //cout << "第 " << x << "行第 " << y << "列初始化为 0 " << endl;
            }
            y++;
        }
        x++;
    }

    ////测试: 输出生成的道路矩阵(已成功)
    //cout << "生成的二维道路矩阵为：" << endl;
    //for (auto xit = Road.begin(); xit != Road.end(); xit++)
    //{
    //    for (auto yit = (*xit).begin(); yit != (*xit).end(); yit++)
    //    {
    //        cout << (*yit) ;
    //    }
    //    cout << endl;
    //}
    //cout << endl;

    ////(1)道路建模(二维数组,下标从0开始) 弃用

    //for (int x = 0; x < X; x++)
    //{
    //    for (int y = 0; y < Y; y++)
    //    {
    //        if(x == 299 || x == 899 || y == 299 || y == 899)
    //        //if (x == 29 || x == 89 || y == 29 || y == 89)
    //        {
    //            Road[x][y] = 1;
    //        }
    //        else
    //        {
    //            Road[x][y] = 0;
    //        }
    //    }
    //}
    //////测试：输出生成的井字道路二维矩阵
    ////cout << "生成的二维道路矩阵如下: " << endl;
    ////for (int x = 0; x < X; x++)
    ////{
    ////    for (int y = 0; y < Y; y++)
    ////    {
    ////        cout << Road[x][y];
    ////    }
    ////    cout << endl;
    ////}
    ////cout << endl;

    //(2)时间槽设置(在头文件中已定义, 见1GenerateData.h)
    cout << "系统运行时间槽: [1,T] = [1," << T << "]" << endl;

    //(3)资源种类设置
    cout << "系统一共提供 R 种资源: R = " << R << ", 分别指代 CPU, GPU, Memory 和 Disk." << endl;
    cout << endl;
}

// 2.生成基站信息
void GenerateEcsInfo()
{
    for (int j = 1; j <= M; j++)
    {
        //生成具体的基站对象
        EcsInfo ecs;

        //生成基站编号(唯一)
        ecs.id = j;

        //生成基站类型
        CalcEcsType(ecs);

        //生成小型基站覆盖半径
        ecs.SCR = ECS_SCR;

        //生成基站位置(唯一): 每隔 ECS_SCR m，建成一座基站
        CalcEcsDL(ecs);
        ////测试: 是否正确接收生成的部署位置(成功)
        //cout << "id = " << s_ecs.id << " 的小型ECS s_ecs 的部署位置 DL = (" << s_ecs.DL.x << "," << s_ecs.DL.y << ")." << endl;

        //计算基站的信号覆盖范围
        CalcEcsSCA(ecs);

        
        //小型基站资源容量为 (20,10,40,150)
        if (ecs.type == "S")
        {
            //生成基站资源容量 RC，共包含 4 种资源: CPU, GPU, Memory, Disk, 即 R = 4
            for (int r = 0; r < R; r++)
            {
                switch (r)
                {
                case 0:		//生成 CPU 的资源容量，即c_j1
                    ecs.RC.cpu = S_ECS_RC_CPU;
                    break;
                case 1:		//生成 GPU 的资源容量，即c_j2
                    ecs.RC.gpu = S_ECS_RC_GPU;
                    break;
                case 2:		//生成 Memory 的资源容量，即c_j3
                    ecs.RC.memory = S_ECS_RC_MEMORY;
                    break;
                case 3:		//生成 Disk 的资源容量，即c_j4
                    ecs.RC.disk = S_ECS_RC_DISK;
                    break;
                default:
                    break;
                }
            }
            //生成资源容量矩阵 RCM : R × T 矩阵
            //每一行的矩阵容量都是满载的，也就是说，每过一个时间槽，ECS的资源容量都会刷新且满载
            int count_RCM_row = 1;  //记录资源容量矩阵的行数(当前是第几行)
            for (auto rit = ecs.RCM.begin(); rit != ecs.RCM.end() && count_RCM_row <= 4; rit++, count_RCM_row++)
            {
                if (count_RCM_row == 1)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = S_ECS_RC_CPU;  // RCM 第 1 行全都是满载的 CPU 资源
                    }
                }
                else if (count_RCM_row == 2)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = S_ECS_RC_GPU;  // RCM 第 2 行全都是满载的 GPU 资源
                    }
                }
                else if (count_RCM_row == 3)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = S_ECS_RC_MEMORY;  // RCM 第 3 行都是满载的 Memory 资源
                    }
                }
                else if (count_RCM_row == 4)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = S_ECS_RC_DISK;  // RCM 第 4 行都是满载的 Disk 资源
                    }
                }
                else
                {
                    cout << "RCM 行数错误，不进行赋值！" << endl;
                }
            }
        }
        //大型基站资源容量为 (40,20,80,300)
        else //ecs.type == "L",        
        {
            for (int r = 0; r < R; r++)
            {
                switch (r)
                {
                case 0:		//生成 CPU 的资源容量，即c_j1
                    ecs.RC.cpu = L_ECS_RC_CPU;
                    break;
                case 1:		//生成 GPU 的资源容量，即c_j2
                    ecs.RC.gpu = L_ECS_RC_GPU;
                    break;
                case 2:		//生成 Memory 的资源容量，即c_j3
                    ecs.RC.memory = L_ECS_RC_MEMORY;
                    break;
                case 3:		//生成 Disk 的资源容量，即c_j4
                    ecs.RC.disk = L_ECS_RC_DISK;
                    break;
                default:
                    break;
                }
            }
            //生成资源容量矩阵 RCM : R × T 矩阵
            //每一行的矩阵容量都是满载的，也就是说，每过一个时间槽，ECS的资源容量都会刷新且满载
            int count_RCM_row = 1;  //记录资源容量矩阵的行数(当前是第几行)
            for (auto rit = ecs.RCM.begin(); rit != ecs.RCM.end() && count_RCM_row <= 4; rit++, count_RCM_row++)
            {
                if (count_RCM_row == 1)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = L_ECS_RC_CPU;  // RCM 第 1 行全都是满载的 CPU 资源
                    }
                }
                else if (count_RCM_row == 2)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = L_ECS_RC_GPU;  // RCM 第 2 行全都是满载的 GPU 资源
                    }
                }
                else if (count_RCM_row == 3)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = L_ECS_RC_MEMORY;  // RCM 第 3 行都是满载的 Memory 资源
                    }
                }
                else if (count_RCM_row == 4)
                {
                    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
                    {
                        (*cit) = L_ECS_RC_DISK;  // RCM 第 4 行都是满载的 Disk 资源
                    }
                }
                else
                {
                    cout << "RCM 行数错误，不进行赋值！" << endl;
                }
            }
        }

        //把生成的基站信息放入vector容器 vEcsInfo 中保存
        vEcsInfo.push_back(ecs);
    }

    if (vEcsInfo.size() == tEcsNum)
    {
        cout << "已成功生成 " << tEcsNum << "条基站信息数据,其中包含：" << endl;
        cout << "小型基站数据 " << S_EcsNum << " 条；大型基站数据 " << L_EcsNum << " 条." << endl;
        cout << endl;
    }
    else
        cout << "基站数据生成未完成，因为“存放基站信息的容器大小 vEcsInfo.size()”不等于“基站总数 tEcsNum”，即: " << vEcsInfo.size() << " 不等于 " << tEcsNum << " ！" << endl;

}
void CalcEcsType(EcsInfo& ecs)
{
    if (S_EcsNum != 8)
    {
        //(1)部署 2 个大型基站
        if ((L_EcsNum == 2) && (ver == 1) && ((ecs.id == 1) || (ecs.id == 8)))
        {
            ecs.type = "L";
        }
        else if ((L_EcsNum == 2) && (ver == 2) && ((ecs.id == 3) || (ecs.id == 6)))
        {
            ecs.type = "L";
        }
        //(2)部署 4 个大型基站
        else if ((L_EcsNum == 4) && ((ecs.id == 1) || (ecs.id == 3) || (ecs.id == 6) || (ecs.id == 8)))
        {
            ecs.type = "L";
        }
        //(3)部署 8 个大型基站
        else if (L_EcsNum == 8)
        {
            ecs.type = "L";
        }
        else
        {
            ecs.type = "S";
        }
    }
    else    // S_EcsNum == 8 : 全部都是小型基站
    {
        ecs.type = "S";
    }
}
void CalcEcsDL(EcsInfo& ecs)
{
    int x_coord = 0, y_coord = 0;   //临时变量，用来记录 当前 ecs 的 x 坐标值 和 y 的坐标值
    //根据 ecs的编号 决定部署位置
    //按照 横坐标 x 递增的顺序设置ECS部署位置
    if (ecs.id == 1)   //坐标为：Road[299][299]
    {
        x_coord = 299 + 1;
        y_coord = 299 + 1;
    }
    else if (ecs.id == 2)   //坐标为：Road[299][599]
    {
        x_coord = 299 + 1;
        y_coord = 599 + 1;
    }
    else if (ecs.id == 3)   //坐标为：Road[299][899]
    {
        x_coord = 299 + 1;
        y_coord = 899 + 1;
    }
    else if (ecs.id == 4)   //坐标为：Road[599][299]
    {
        x_coord = 599 + 1;
        y_coord = 299 + 1;
    }
    else if (ecs.id == 5)   //坐标为：Road[599][899]   
    {
        x_coord = 599 + 1;
        y_coord = 899 + 1;
    }
    else if (ecs.id == 6)  //坐标为：Road[899][299] 
    {
        x_coord = 899 + 1;
        y_coord = 299 + 1;
    }
    else if (ecs.id == 7)  //坐标为：Road[899][599]
    {
        x_coord = 899 + 1;
        y_coord = 599 + 1;
    }
    else if (ecs.id == 8)  //坐标为：Road[899][899]
    {
        x_coord = 899 + 1;
        y_coord = 899 + 1;
    }

    ecs.DL.x = x_coord;     //记录 x 坐标
    ecs.DL.y = y_coord;     //记录 y 坐标 
}
void CalcEcsSCA(EcsInfo& ecs)
{
    Coordinate temp_DL = ecs.DL;    //暂时记录 ecs 的部署位置
    Coordinate temp_SCA;            //暂时存放 计算出的信号覆盖范围

    double ecs_sca = SCR;           //记录 基站覆盖半径
    double d = 0;                   //记录 计算出的信号覆盖范围 与 ECS部署位置 之间的直线距离

    for (int x = temp_DL.x - SCR; x <= temp_DL.x + SCR; x++)
    {
        if (x >= 1 && x <= X)   // 保证基站的信号覆盖范围在井子道路的 X 坐标轴之内
        {
            for (int y = temp_DL.y - SCR; y <= temp_DL.y + SCR; y++)
            {
                if (y >= 1 && y <= Y) // 保证基站的信号覆盖范围在井子道路的 Y 坐标轴之内
                {
                    //d = sqrt(pow(abs(x - temp_DL.x), 2) + pow(abs(y - temp_DL.y), 2));  //有绝对值
                    d = sqrt(pow((x - temp_DL.x), 2) + pow((y - temp_DL.y), 2));  //无绝对值
                    if (d < ecs_sca && Road[x - 1][y - 1] == 1) //保证当前信号覆盖范围是车辆可行的
                    {
                        temp_SCA.x = x;
                        temp_SCA.y = y;
                        ecs.SCA.push_back(temp_SCA);
                    }
                    //清空 temp_SCA 的赋值
                    temp_SCA.x = 0;
                    temp_SCA.y = 0;
                    d = 0;
                }
            }
        }
    }

    ////测试：基站的信号覆盖范围是否计算正确(成功)
    //cout << "id = " << ecs.id << " 的基站部署位置为：(" << ecs.DL.x << "," <<ecs.DL.y <<"), 它的信号覆盖范围如下：" << endl;
    //for (auto coordit = ecs.SCA.begin(); coordit != ecs.SCA.end(); coordit++)
    //{
    //    cout << *coordit << ", ";
    //}
    //cout << endl;

}

// 3.生成车辆信息
void GenerateCarInfo()
{
    //(0)初始化车辆到达时间(服从正态分布),初始位置,行驶方向,投标系数(均匀分布)
    InitialCarArrivalTime();        //往容器 vCarAT 中存放所有车辆的到达时间(正态分布)
    vector<int>::iterator ATit = vCarAT.begin();    //迭代器

    InitialCarLocation();           //往容器 vCarIL 中存放所有可行驶的坐标点
    int CarILNum = vCarIL.size();           //统计一共有几个位置点

    InitialCarDrivingDirection();   //往容器 vCarDD 存放四种行驶方向

    InitialCarBidCoefficient();     //往容器 vCarBC 存放所有车辆的投标系数(均匀分布)
    vector<double>::iterator BCit = vCarBC.begin();

    //生成车辆数据
    for (int i = 0; i < N; i++)
    {
        //生成具体的车辆对象
        CarInfo car;

        //生成车辆编号
        car.id = i + 1;

        //设置车辆类型;
        SetCarType(car);

        //生成车辆初始位置 (只有4796个位置点，迭代结束之后则会报错front() called on empty vector)
        int random_IL = rand() % CarILNum;      //取一个随机值做容器 CarIL 的下标
        car.RTL.push_back(vCarIL[random_IL]);

        //设置车辆的初始行驶方向
        SetCarInitialDrivingDirection(car);

        ////测试: 根据车辆的初始位置，判断他的初始行驶方向是否设置正确
        //cout << "车辆的初始行驶方向设置如下：" << endl;
        //for ()
        //    cout << "id = " << car.id << "的车辆的初始位置为 " << car.RTL.front() << ", 其初始行驶方向为：" << car.DD.front() << endl;

        //设置车辆初始行驶速度(车辆的行驶速度随时间变化)
        car.RTV.push_back(5 + rand() % 6);     //测试时 rtv取值范围是 [5,10]

        //设置车辆任务请求信息
        if (ATit != vCarAT.end())   //设置车辆任务的到达时间
        {
            car.at = *(ATit);
            ATit++;
        }
        SetCarExecutionTime(car);                       //设置车辆任务的执行时间
        SetCarDeadlineTime(car);                        //设置车辆任务的截止时间
        SetCarTaskResourceDemand(car);                  //设置车辆任务的资源需求

        if (BCit != vCarBC.end())                       //设置车辆任务的投标系数
        {
            car.bc = *(BCit);
            BCit++;
        }
        SetCarBid(car);                                 //设置车辆投标

        CalcCarResourceDemandsMatrix(car);              //计算车辆的时变资源需求矩阵(et = 1时为向量)

        CalcCarEntireDrivingInformation(car);           //计算车辆的全程实时速度,行驶方向和实时位置

        CalcCarDeploymentConstraintsMatrix(car);        //计算车辆的时变部署约束矩阵

        if (car.type == "S")
        {
            S_vCarInfo.push_back(car);
        }
        else    //car.type = "L"
        {
            L_vCarInfo.push_back(car);
        }

        //把生成的车辆信息放入车辆容器 vCarInfo 中
        vCarInfo.push_back(car);

    }

}
//(1)生成车辆到达时间的函数(正态分布)
void InitialCarArrivalTime()
{
    /* 实现思路：
     * ①初始化一个vector容器 vCarAT，存储符合 Normal Distribution 的随机数;
     * ②打乱 存放车辆到达时间的容器 vCarAT 中的元素顺序
     */
    int n = N;	//车辆总数 N
    int t = T;	//时间槽 T

    //(1)T = 2160时，创建一个 mu(mean) =  T/2 , sigma(sd) = T/6 的正态分布, 68% 的车在 6 - 8 点, 95% 的车在 5 - 9 点
    //(2)T = 1440时，创建一个 mu(mean) =  T/2 , sigma(sd) = T/8 的正态分布, 68% 的车在 6:30 - 7:30 点, 95% 的车 在 6 - 8点。
    double m = (double)t / (double)2;		//设置均值 mean
    double sd = (double)t / (double)8;			//设置标准差 standard deviation
    double mu{ m }, sigma{ sd };
    std::default_random_engine rn((unsigned int)time(NULL));	//随机数生成器 rn = random number
    //使用新标准生成随机数
    //rn.seed((unsigned int)time(NULL));
    std::normal_distribution<> normAT{ mu, sigma };

    //生成N辆车的到达时间 ci.at
    //并把生成的车辆到达时间装入 vCarAT 中
    for (int i = 0; i < n;)
    {
        int at = int(normAT(rn) + 0.5);	//对产生的随机到达时间进行四舍五入
        if (at >= 1 && at <= t)	//生成的到达时间 at 应当满足 1 <= at <= T
        {
            vCarAT.push_back(at);
            i++;
        }
    }

    ////测试：打印vCarAT中生成的所有随机数（成功）
    //cout << "正态分布normAT : X ~ N(mu,sigma) = X ~ N(" << mu << "," << sigma << ")" << endl;
    //for (vector<int>::iterator ATit = vCarAT.begin(); ATit != vCarAT.end(); ATit++)
    //{
    //	cout << (*ATit) << endl;
    //}
    //
    ////计算 at = [mu - sigma, mu + sigma] 的占比，是否在68%左右(成功)
    ////计算 at = [mu - 2*sigma, mu + 2*sigma] 的占比，是否在95%左右(成功)
    //int count = 0;
    //int atsize = vCarAT.size();
    //cout << "atsize = " << atsize << endl;
    //for (vector<int>::iterator ATit = vCarAT.begin(); ATit != vCarAT.end(); ATit++)
    //{
    //	if (((*ATit) > (int)(m - 2*sd)) && ((*ATit) < (int)(m + 2*sd)))
    //	{
    //		count++;
    //	}
    //}
    //cout << "count = " << count << endl;
    //double p = (double)count / (double)atsize;
    //cout << "随机生成的到达时间 at 的范围在 ["<< mu << " - 2*" << sigma << ", "
    //	<< mu << " + 2*"<< sigma << "] 的比例为：" << p << endl;

    //打乱vCarAT中的元素顺序，实现随机排序
    random_shuffle(vCarAT.begin(), vCarAT.end());
}
//(2)生成车辆随机初始位置的函数
void InitialCarLocation()
{
    /* 实现思路:
     * ①将车辆可行驶道路坐标化,并把所有坐标点都放入 vector 容器 vCarIL 中
     * ②打乱 vector 容器 vCarIL 中的元素排序
     */

    Coordinate Road_coord;  //初始化一个临时坐标点
    for (int x = 1; x <= X; x++)
    {
        for (int y = 1; y <= Y; y++)
        {
            if (Road[x - 1][y - 1] == 1)
            {
                Road_coord.x = x;   //记录当前可行驶位置的 x 坐标
                Road_coord.y = y;   //记录当前可行驶位置的 y 坐标
                vCarIL.push_back(Road_coord);
                //重置坐标点(Road_coord)
                Road_coord.x = 0;
                Road_coord.y = 0;
            }
        }
    }

    ////测试: 是否成功记录可行驶道路的所有坐标点(成功)
    //cout << "vCarIL 中所有可行驶的坐标点如下:" << endl;
    //for (auto ILit = vCarIL.begin(); ILit != vCarIL.end(); ILit++)
    //{
    //    cout << "(" << ILit->x << "," << ILit->y << "), ";
    //}
    //cout << endl;

    //打乱vCarIL中的元素顺序，实现随机排序
    random_shuffle(vCarIL.begin(), vCarIL.end());


}
//(3)生成车辆随机初始行驶方向的函数
void InitialCarDrivingDirection()
{
    vCarDD.push_back("U");   // U = up, 表示在井字道路中向上行驶
    vCarDD.push_back("D");   // D = down, 表示在井字道路中向下行驶
    vCarDD.push_back("L");   // L = left, 表示在井字道路中向左行驶
    vCarDD.push_back("R");   // R = right, 表示在井字道路中向右行驶
}
//(4)生成车辆随机投标系数的函数
void InitialCarBidCoefficient()
{
    int n = N;  // 暂存车辆总数 N
    double rand_bc = 0;
    for (int i = 1; i <= n; i++)
    {
        int a = 0, b = 0;
        if (i <= (n / 2))		//一半车辆的投标系数取值在[0.25,1]之间：迫切程度低
        {
            a = 25, b = 100;
        }
        else
        {
            a = 100; b = 400;	//另一半车辆的投标系数取值在[1,4]之间：迫切程度高
        }
        rand_bc = (double)((double)(a + rand() % (b - a + 1)) / 100);
        vCarBC.push_back(rand_bc);
        rand_bc = 0;
    }

    //打乱vCarBC中的元素顺序，实现随机排序
    random_shuffle(vCarBC.begin(), vCarBC.end());
}
//(5)设置车辆类型的函数
void SetCarType(CarInfo& car)
{
    int car_id = car.id;
    if (car_id <= S_CarNum)     //小型车辆任务
    {
        car.type = "S";
    }
    else    // car_id > S_CarNum : 大型车辆任务
    {
        car.type = "L";
    }
}
//(6)设置车辆初始行驶方向的函数
void SetCarInitialDrivingDirection(CarInfo& car)
{
    //读取车辆的初始位置坐标点
    Coordinate initloc = car.RTL.front();

    //设车辆仅在十字路口(300,300),(300,900),(900,300),(900,900)才能随机选择四个行驶方向
    if ((initloc.x == 300 && initloc.y == 300) || (initloc.x == 300 && initloc.y == 900) ||
        (initloc.x == 900 && initloc.y == 300) || (initloc.x == 900 && initloc.y == 900))
    {
        //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
        car.DD.push_back((vCarDD[rand() % 4]));  //在十字路口时可自由选择 4 种行驶方向
    }
    else if ((initloc.x == 300 && (initloc.y != 300 && initloc.y != 900))  //初始位置在第一条横向道路上(非十字路口)
        || (initloc.x == 900 && (initloc.y != 300 && initloc.y != 900)))    //初始位置在第二条横向道路上(非十字路口)
    {
        car.DD.push_back((vCarDD[2 + rand() % 2])); //初始位置在横向道路时，行驶方向只能向左或向右
    }
    else if ((initloc.y == 300 && (initloc.x != 300 && initloc.x != 900))  //初始位置在第一条纵向道路上(非十字路口)
        || (initloc.y == 900 && (initloc.x != 300 && initloc.x != 900)))    //初始位置在第二条纵向道路上(非十字路口)
    {
        car.DD.push_back((vCarDD[rand() % 2]));  //初始位置在横向道路时，行驶方向只能向上或向下
    }
    else
    {
        cout << "id = " << car.id << ", 初始位置为 " << car.RTL.front() << " 的车辆的 初始行驶方向 设置错误！" << endl;
    }
}
//(7)设置车辆任务执行时间的函数
void SetCarExecutionTime(CarInfo& car)
{
    string car_type = car.type;
    int car_et = 0;
    if (car_type == "S") //小型车辆任务的执行时间
    {
        car_et = 4 + rand() % 5;  //小型车辆的执行时间取 [4,8] 的随机值
    }
    else    // car_type == "L"
    {
        car_et = 8 + rand() % 9;    //大型车辆任务的执行时间取 [8,16] 的随机值
    }
    car.et = car_et;
}
//(8)设置车辆任务截止时间的函数
void SetCarDeadlineTime(CarInfo& car)
{
    int car_at = car.at;    //读取车辆的到达时间
    int car_et = car.et;    //读取车辆的执行时间
    int car_dt = 0;         //暂存车辆的任务截止时间
    car_dt = car_at + (1 + rand() % 8) * car_et;    //设置车辆任务的截止时间为：执行时间的 1 - 8倍 (随机值) 
    if (car_dt > T)     //如果截止时间超过系统运行时间槽的范围，将其限定在最大的系统运行时间槽
    {
        car_dt = T;
    }
    car.dt = car_dt;
}
//(9)设置车辆任务任务资源需求(静态)的函数
void SetCarTaskResourceDemand(CarInfo& car)
{
    if (car.type == "S")
    {
        car.TRD.cpu = 1 + rand() % 8;  // 小型车辆任务的 CPU 资源需求量为 [1,8] 的随机值
        car.TRD.gpu = 1 + rand() % 4;  // 小型车辆任务的 GPU 资源需求量为 [1,4] 的随机值
        car.TRD.memory = 8 + rand() % 9;  // 小型车辆任务的 Memory 资源需求量为 [8,16] 的随机值
        car.TRD.disk = 64 + rand() % 65;  // 小型车辆任务的 Disk 资源需求量为 [64,128] 的随机值
    }
    else    // car.type == "L"
    {
        car.TRD.cpu = 8 + rand() % 9;  // 中型车辆任务的 CPU 资源需求量为 [8,16] 的随机值
        car.TRD.gpu = 4 + rand() % 5;  // 中型车辆任务的 GPU 资源需求量为 [4,8] 的随机值
        car.TRD.memory = 16 + rand() % 17;  // 中型车辆任务的 Memory 资源需求量为 [16,32] 的随机值
        car.TRD.disk = 128 + rand() % 129;  // 中型车辆任务的 Disk 资源需求量为 [128,256] 的随机值
    }
}
//(10)设置车辆任务投标函数的函数
void SetCarBid(CarInfo& car)
{
    //投标计算公式: bid =  bc[i] * e[i] * [sigma_(r=1~4) (gamma_r * s_i_r)]
    double car_bc = double(car.bc);	//读入投标系数
    double car_et = double(car.et);	//读入执行时间
    double temp = 0;			//单位时间内的所有资源的使用成本
    Resource car_trd = car.TRD;	//使用 trd 记录车辆的任务资源需求向量

    temp = car_trd.cpu * gamma.unitcost_cpu + car_trd.gpu * gamma.unitcost_gpu
        + car_trd.memory * gamma.unitcost_memory + car_trd.disk * gamma.unitcost_disk;

    car.bid = (double)(car_bc * car_et * temp); //带投标系数的投标
    //car.bid = (double)(car_et * temp); //带投标系数的投标
}
//(11)计算车辆的全程实时驾驶信息:包括行驶方向, 实时速度和实时位置
void CalcCarEntireDrivingInformation(CarInfo& car)
{
    Coordinate car_cl = car.RTL.front();      //读取车辆的初始位置（当前位置）cl = current location
    string car_cdd = car.DD.front();           //读取车辆的初始行驶方向 cdd = current driving direction
    int car_cv = car.RTV.front();            //读取车辆的初始行驶速度(当前速度) cv = current velocity

    int car_at = car.at, car_dt = car.dt, car_et = car.et;      //读取车辆相关时间信息
    for (int t = car_at; t <= car_dt - car_et + 1; t++)
    {
        //根据当前时间槽的行驶方向, 实时位置 和 实时速度, 计算出下一个时间槽的 车辆实时位置
        //车辆在当前时间槽的移动路径分作三种情况:
        // (1)车辆移动后越过十字路口
        // (2)车辆移动后刚好抵达十字路口坐标点
        // (3)车辆移动后没有抵达十字路口 
        //现在的 car_cl 是当前时间槽移动前的位置
        if (car_cdd == "U")   //车辆向上行驶(以矩阵下标为判断基础: x减小, y不变)
        {
            //(1)车辆在当前时间槽移动后会越过十字路口
            if ((car_cl.x > 900 && car_cl.x - car_cv < 900) || (car_cl.x > 300 && car_cl.x - car_cv < 300))
            {
                //车辆在当前时间槽移动,越过十字路口时,会改变其行驶方向
                //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
                //将产生改变的坐标赋值放在前面，然后再赋值固定的坐标，否则会产生 bug。例如，要写作：
                //1.car_cl.y = car_cl.y - (900 - (car_cl.x - car_cv));
                //2.car_cl.x = 900;
                //而不能写成：
                //1.car_cl.x = 900;
                //2.car_cl.y = car_cl.y - (900 - (car_cl.x - car_cv));
                car_cdd = vCarDD[rand() % 4];   //在十字路口时可自由选择 4 种行驶方向
                if (car_cdd == "U")
                {
                    car_cl.x = car_cl.x - car_cv;   //计算本时间槽行驶后的位置,也即下一个时间槽的实时位置
                }
                else if (car_cdd == "D" && (car_cl.x > 900 && car_cl.x - car_cv < 900))
                {
                    car_cl.x = 900 + (900 - (car_cl.x - car_cv));
                }
                else if (car_cdd == "D" && (car_cl.x > 300 && car_cl.x - car_cv < 300))
                {
                    car_cl.x = 300 + (300 - (car_cl.x - car_cv));
                }
                else if (car_cdd == "L" && (car_cl.x > 900 && car_cl.x - car_cv < 900))
                {
                    car_cl.y = car_cl.y - (900 - (car_cl.x - car_cv));
                    car_cl.x = 900;
                }
                else if (car_cdd == "L" && (car_cl.x > 300 && car_cl.x - car_cv < 300))
                {
                    car_cl.y = car_cl.y - (300 - (car_cl.x - car_cv));
                    car_cl.x = 300;
                }
                else if (car_cdd == "R" && (car_cl.x > 900 && car_cl.x - car_cv < 900))
                {
                    car_cl.y = car_cl.y + (900 - (car_cl.x - car_cv));
                    car_cl.x = 900;
                }
                else if (car_cdd == "R" && (car_cl.x > 300 && car_cl.x - car_cv < 300))
                {
                    car_cl.y = car_cl.y + (300 - (car_cl.x - car_cv));
                    car_cl.x = 300;
                }
                //1.更新在时间槽中改变的行驶方向
                car.DD.push_back(car_cdd);  //将改变的行驶方向置入 vector 容器 car.DD 中
            }
            else    //车辆在当前时间槽移动后不越过十字路口(可能刚好抵达十字路口(2),也可能没有到达十字路口处(3))
            {
                car_cl.x = car_cl.x - car_cv;   //计算本时间槽行驶后的位置,也即下一个时间槽的实时位置
            }
        }
        else if (car_cdd == "D")    //车辆向下行驶(以矩阵下标为判断基础: x增大, y不变)
        {
            //(1)车辆在当前时间槽移动后会越过十字路口
            if ((car_cl.x < 900 && car_cl.x + car_cv > 900) || (car_cl.x < 300 && car_cl.x + car_cv > 300))
            {
                //车辆在当前时间槽移动,越过十字路口时,会改变其行驶方向
                //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
                car_cdd = vCarDD[rand() % 4];   //在十字路口时可自由选择 4 种行驶方向
                if (car_cdd == "U" && (car_cl.x < 900 && car_cl.x + car_cv > 900))
                {
                    car_cl.x = 900 - (car_cl.x + car_cv - 900);   //计算本时间槽行驶后的位置,也即下一个时间槽的实时位置
                }
                else if (car_cdd == "U" && (car_cl.x < 300 && car_cl.x + car_cv > 300))
                {
                    car_cl.x = 300 - (car_cl.x + car_cv - 300);
                }
                else if (car_cdd == "D")
                {
                    car_cl.x = car_cl.x + car_cv;
                }
                else if (car_cdd == "L" && (car_cl.x < 900 && car_cl.x + car_cv > 900))
                {
                    car_cl.y = car_cl.y - (car_cl.x + car_cv - 900);
                    car_cl.x = 900;
                }
                else if (car_cdd == "L" && (car_cl.x < 300 && car_cl.x + car_cv > 300))
                {
                    car_cl.y = car_cl.y - (car_cl.x + car_cv - 300);
                    car_cl.x = 300;
                }
                else if (car_cdd == "R" && (car_cl.x < 900 && car_cl.x + car_cv > 900))
                {
                    car_cl.y = car_cl.y + (car_cl.x + car_cv - 900);
                    car_cl.x = 900;
                }
                else if (car_cdd == "R" && (car_cl.x < 300 && car_cl.x + car_cv > 300))
                {
                    car_cl.y = car_cl.y + (car_cl.x + car_cv - 300);
                    car_cl.x = 300;
                }
                //1.更新在时间槽中改变的行驶方向
                car.DD.push_back(car_cdd);  //将改变的行驶方向置入 vector 容器 car.DD 中  可能有 bug
            }
            else    //车辆在当前时间槽移动后不越过十字路口(可能刚好抵达十字路口(2),也可能没有到达十字路口处(3))
            {
                car_cl.x = car_cl.x + car_cv;
            }
        }
        else if (car_cdd == "L")    //车辆向左行驶(以矩阵下标为判断基础: x不变, y减小)
        {
            //(1)车辆在当前时间槽移动后会越过十字路口
            if ((car_cl.y > 900 && car_cl.y - car_cv < 900) || (car_cl.y > 300 && car_cl.y - car_cv < 300))
            {
                //车辆在当前时间槽移动,越过十字路口时,会改变其行驶方向
                //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
                car_cdd = vCarDD[rand() % 4];   //在十字路口时可自由选择 4 种行驶方向
                if (car_cdd == "U" && (car_cl.y > 900 && car_cl.y - car_cv < 900))
                {
                    car_cl.x = car_cl.x - (900 - (car_cl.y - car_cv));
                    car_cl.y = 900;
                }
                else if (car_cdd == "U" && (car_cl.y > 300 && car_cl.y - car_cv < 300))
                {
                    car_cl.x = car_cl.x - (300 - (car_cl.y - car_cv));
                    car_cl.y = 300;
                }
                else if (car_cdd == "D" && (car_cl.y > 900 && car_cl.y - car_cv < 900))
                {
                    car_cl.x = car_cl.x + (900 - (car_cl.y - car_cv));
                    car_cl.y = 900;
                }
                else if (car_cdd == "D" && (car_cl.y > 300 && car_cl.y - car_cv < 300))
                {
                    car_cl.x = car_cl.x + (300 - (car_cl.y - car_cv));
                    car_cl.y = 300;
                }
                else if (car_cdd == "L")
                {
                    car_cl.y = car_cl.y - car_cv;   // x不变
                }
                else if (car_cdd == "R" && (car_cl.y > 900 && car_cl.y - car_cv < 900))  //调头，即折返
                {
                    car_cl.y = 900 + (900 - (car_cl.y - car_cv));
                }
                else if (car_cdd == "R" && (car_cl.y > 300 && car_cl.y - car_cv < 300))
                {
                    car_cl.y = 300 + (300 - (car_cl.y - car_cv));
                }
                //1.更新在时间槽中改变的行驶方向
                car.DD.push_back(car_cdd);  //将改变的行驶方向置入 vector 容器 car.DD 中  可能有 bug
            }
            else    //车辆在当前时间槽移动后不越过十字路口(可能刚好抵达十字路口(2),也可能没有到达十字路口处(3))
            {
                car_cl.y = car_cl.y - car_cv;
            }
        }
        else if (car_cdd == "R")     //车辆向右行驶(以矩阵下标为判断基础: x不变, y增大)
        {
            //(1)车辆在当前时间槽移动后会越过十字路口
            if ((car_cl.y < 900 && car_cl.y + car_cv > 900) || (car_cl.y < 300 && car_cl.y + car_cv > 300))
            {
                //车辆在当前时间槽移动,越过十字路口时,会改变其行驶方向
                //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
                car_cdd = vCarDD[rand() % 4];   //在十字路口时可自由选择 4 种行驶方向
                if (car_cdd == "U" && (car_cl.y < 900 && car_cl.y + car_cv > 900))
                {
                    car_cl.x = car_cl.x - (car_cl.y + car_cv - 900);
                    car_cl.y = 900;
                }
                else if (car_cdd == "U" && (car_cl.y < 300 && car_cl.y + car_cv > 300))
                {
                    car_cl.x = car_cl.x - (car_cl.y + car_cv - 300);
                    car_cl.y = 300;
                }
                else if (car_cdd == "D" && (car_cl.y < 900 && car_cl.y + car_cv > 900))
                {
                    car_cl.x = car_cl.x + (car_cl.y + car_cv - 900);
                    car_cl.y = 900;
                }
                else if (car_cdd == "D" && (car_cl.y < 300 && car_cl.y + car_cv > 300))
                {
                    car_cl.x = car_cl.x + (car_cl.y + car_cv - 300);
                    car_cl.y = 300;
                }
                else if (car_cdd == "L" && (car_cl.y < 900 && car_cl.y + car_cv > 900))
                {
                    car_cl.y = 900 - (car_cl.y + car_cv - 900);
                }
                else if (car_cdd == "L" && (car_cl.y < 300 && car_cl.y + car_cv > 300))
                {
                    car_cl.y = 300 - (car_cl.y + car_cv - 300);
                }
                else if (car_cdd == "R")
                {
                    car_cl.y = car_cl.y + car_cv;
                }
                //1.更新在时间槽中改变的行驶方向
                car.DD.push_back(car_cdd);  //将改变的行驶方向置入 vector 容器 car.DD 中  可能有 bug
            }
            else    //车辆在当前时间槽移动后不越过十字路口(可能刚好抵达十字路口(2),也可能没有到达十字路口处(3))
            {
                car_cl.y = car_cl.y + car_cv;
            }
        }
        //现在的 car_cl 是当前时间槽移动后的位置，即下一个时间槽移动前的位置
        //仅当 下一个时间槽的车辆实时位置 合法, 才记录车辆的实时位置并更新下一个时间槽的 行驶方向 和 行驶速度
        if (car_cl.x >= 1 && car_cl.x <= X && car_cl.y >= 1 && car_cl.y <= Y)      //保证下一个坐标点位置在坐标轴范围内
        {
            if (Road[car_cl.x - 1][car_cl.y - 1] == 1)   // 保证下一个坐标点位置是可行驶道路
            {
                //2.记录本时间槽行驶后的位置，也即下一个时间槽的实时位置，置入容器 vector<Coordinate> car.RTL
                car.RTL.push_back(car_cl);

                // 如果车辆在当前时间槽的行驶路径没有越过十字路口，那么，要根据他移动后的位置来判断其行驶方向
                // (2)车辆移动后刚好抵达十字路口坐标点：需要更新在下一个时间槽时，车辆的行驶方向
                //    将新的行驶方向置入容器 car.DD 中,否则不改变行驶方向.
                if ((car_cl.x == 300 && car_cl.y == 300) || (car_cl.x == 300 && car_cl.y == 900) ||
                    (car_cl.x == 900 && car_cl.y == 300) || (car_cl.x == 900 && car_cl.y == 900))
                {
                    //从容器Direction 中随机选取一个方向置入容器 car.DD 中, car.DD.front() 存放的是车辆的初始行驶方向
                    car_cdd = vCarDD[rand() % 4];   //在十字路口时可自由选择 4 种行驶方向
                    car.DD.push_back(car_cdd);
                }
                // (3)车辆移动后没有到达十字路口坐标点, 行驶方向不改变(不写代码)

                //3.更新下一个时间槽时的车辆行驶速度(时变): [50,100] 的随机值
                car_cv = 50 + rand() % 51;
                car.RTV.push_back(car_cv);
            }
            else
            {
                //cout << "id = " << car.id << " 在下一时间槽 " << t + 1 << " 的实时位置为 " << car_cl << ", 不合法(不是可行驶道路)！且不更新其行驶方向与行驶速度！" << endl;
                break;
            }
        }
        else
        {
            //cout << "id = " << car.id << " 在下一时间槽 " << t + 1 << " 的实时位置为 " << car_cl << ", 不合法(不在坐标范围内)！且不更新其行驶方向与行驶速度！" << endl;
            break;
        }
    }

    ////测试 : 车辆的全程行驶信息是否计算正确(成功)
    ////当 dt = at + 1 且 et = 1 时，RTV 和 RTL 中有 3 个元素（原本只应有 2 个）的原因是：
    ////程序会在当前时间槽中 计算出下一个时间槽的 速度 和 位置 并将其放入 RTV 和 RTL 容器中，所以，会多放一个元素，即多出时间槽 dt+1 的位置和速度
    //cout << "car[" << car.id << "] 的车辆的全程行驶信息如下 : at = " << car.at << ", dt = " << car.dt << ", et = " << car.et << endl;
    //cout << "car.DD = {";        //行驶方向
    //for (auto ddit = car.DD.begin(); ddit != car.DD.end(); ddit++)
    //{
    //    cout << (*ddit) << ", ";
    //}
    //cout << "}, " << endl;
    //cout << "car.RTV = {";      //实时速度
    //for (auto vit = car.RTV.begin(); vit != car.RTV.end(); vit++)
    //{
    //    cout << (*vit) << ", ";
    //}
    //cout << "}. " << endl;
    //cout << "car.RTL = {";      //实时位置
    //for (auto locit = car.RTL.begin(); locit != car.RTL.end(); locit++)
    //{
    //    cout << (*locit) << ", ";
    //}
    //cout << "}. " << endl;
    //cout << endl;


}
//(12)计算车辆的时变部署约束矩阵 Deployment Constraint Matrix
void CalcCarDeploymentConstraintsMatrix(CarInfo& car)
{
    int m = M, t = T;    //暂存基站数量(部署约束矩阵总行数), 系统运行时间槽(部署约束矩阵总列数)

    //vector<vector<bool>> dcm(m, vector<bool>(t, false));    //暂存部署约束矩阵的容器

    vector<EcsInfo>::iterator ecsit = vEcsInfo.begin();    //遍历 vEcsInfo 容器的迭代器, vEcsInfo 容器存放所有ECS的信息

    //(1)获取当前车辆相关信息
    int car_at = car.at - 1;    //到达时间，矩阵下标从 0 开始，故 - 1
    int car_dt = car.dt;        //任务截止时间，在截止时间点时，也可以执行任务，故不 - 1 
    int car_et = car.et;        //记录执行时间
    int car_lst = car_dt - car_et + 1;      // lst = last submit time 最晚提交时间

    vector<Coordinate>::iterator car_cl = car.RTL.begin();    //指向当前车辆的实时位置的迭代器

    //(2)逐个检查ECS的信号覆盖范围容器
    //   如果当前车辆位置在第 j 个(下标从0开始)ECS的信号覆盖范围内，则置当前时间槽（列）的第 j 行的元素为 1;
    //为防止程序出现bug，必须保证 car_at <= car_lst
    //例如，假设 at = 1436, dt = 1440, et = 6(数据合理), 但会出现 dt - et + 1 = 1435 < at. 从而导致部署约束矩阵生成错误
    if (car_at <= car_lst)
    {
        for (vector<vector<bool>>::iterator rit = car.DCM.begin(); rit != car.DCM.end() && ecsit != vEcsInfo.end(); rit++, ecsit++)  //遍历部署约束矩阵行
        {
            for (vector<bool>::iterator cit = (*rit).begin() + car_at; (cit != (*rit).begin() + car_lst) && (car_cl != car.RTL.end()); cit++, car_cl++)    //遍历部署约束矩阵单行中的列
            {
                //(3)遍历 vector<Coordinate> 容器 ECS.SCA 中的覆盖地址, 如果 车辆当前位置 在信号覆盖范围内，赋值为 true
                for (vector<Coordinate>::iterator coordit = (*ecsit).SCA.begin(); coordit != (*ecsit).SCA.end(); coordit++)
                {
                    if ((*coordit) == (*car_cl))  //在结构体Coordinate定义中 重载 == 运算符,即可用来判断坐标点是否相等
                    {
                        (*cit) = true;
                        break;
                    }
                }
            }
            car_cl = car.RTL.begin();   //检查车辆 与 下一个基站的连接关系，实时位置迭代器要重置
            car_at = car.at - 1;
        }
    }

    ////测试：是否正确生成车辆的时变部署约束矩阵(成功)

    ////当 dt = at + 1 且 et = 1 时，RTV 和 RTL 中有 3 个元素（原本只应有 2 个）的原因是：
    ////程序会在当前时间槽中 计算出下一个时间槽的 速度 和 位置 并将其放入 RTV 和 RTL 容器中，所以，会多放一个元素，即多出时间槽 dt+1 的位置和速度
    //cout << "car[" << car.id << "] 的车辆的全程行驶信息如下 : at = " << car.at << ", dt = " << car.dt << ", et = " << car.et << endl;
    //cout << "car.DD = {";        //行驶方向
    //for (auto ddit = car.DD.begin(); ddit != car.DD.end(); ddit++)
    //{
    //    cout << (*ddit) << ", ";
    //}
    //cout << "}, " << endl;
    //cout << "car.RTV = {";      //实时速度
    //for (auto vit = car.RTV.begin(); vit != car.RTV.end(); vit++)
    //{
    //    cout << (*vit) << ", ";
    //}
    //cout << "}. " << endl;
    //cout << "car.RTL = {";      //实时位置
    //for (auto locit = car.RTL.begin(); locit != car.RTL.end(); locit++)
    //{
    //    cout << (*locit) << ", ";
    //}
    //cout << "}. " << endl;
    //cout << "car.DCM 如下： " << endl;
    //for (auto rit = car.DCM.begin(); rit != car.DCM.end(); rit++)
    //{
    //    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
    //    {
    //        cout << *cit << " ";
    //    }
    //    cout << endl;
    //}
    //cout << endl;

}
//(13)计算车辆的时变资源需求矩阵 Resource Demands Matrix
void CalcCarResourceDemandsMatrix(CarInfo& car)
{
    //读取车辆的静态资源需求，用以约束车辆的时变资源需求矩阵
    double trd_cpu = car.TRD.cpu, trd_gpu = car.TRD.gpu, trd_memory = car.TRD.memory, trd_disk = car.TRD.disk;
    double et = car.et;    //读取执行时间
    //cout << "car[" << car.id << "].TRD = " << car.TRD << ", et = " << car.et << endl;

    //初始化一个临时矩阵 rdm = resource demands matrix，暂存车辆的时变资源需求矩阵 R×et，
    vector<vector<int>> rdm = vector<vector<int>>(R, vector<int>(et, false));

    int count_row = 1;  //矩阵行的计数，表示对 rdm 的第几行赋值
    //共有R行，每行代表：车辆在执行时假期间对第 r 种资源的所有需求量，每行都要受到第 r 种静态资源需求向量的约束，即每行的和必须‘等于’第 r 种资源的静态需求量 
    for (auto rit = rdm.begin(); rit != rdm.end() && count_row <= R; rit++, count_row++)
    {
        //记录已经请求的资源量，只有当 已经请求的资源量 < 静态资源请求量，才能继续请求，
        //当 已经请求的资源量 = 静态资源请求量 时，后续的执行时间槽中不能再继续申请该种资源
        int requested_cpu = 0, requested_gpu = 0, requested_memory = 0, requested_disk = 0;
        int count_column = 1;   //矩阵列的计数，表示对 rdm 的第几列赋值
        //共有 car.et 列，即在执行时间期间，车辆在每个时间槽中对资源的需求量都是随机的
        for (auto cit = (*rit).begin(); cit != (*rit).end() && count_column <= et; cit++, count_column++)
        {
            if ((count_row == 1) && (requested_cpu <= trd_cpu))         //第一行的元素总和要等于 trd_cpu
            {
                int ct_trd_cpu = 0;     //ct_trd_gpu 代表：在当前时间槽对CPU的资源需求量(四舍五入)
                double coef = (double)(1 + rand() % 40) / 100;  // 生成 [0.01, 0.40] 的随机数值作系数
                if (count_column == et) //最后一列
                {
                    ct_trd_cpu = trd_cpu - requested_cpu;
                }
                else
                {
                    ct_trd_cpu = trd_cpu * coef + 0.5;  //四舍五入
                }
                requested_cpu += ct_trd_cpu;

                //测试（成功）
                //cout << "随机生成 cpu 的时变资源需求, 在第 " << count_column << " 个执行时间槽时, coef = " << coef;  //检验系数是否在 [0.1, 0.5] 之间
                //cout << ", 修正前：ct_trd_cpu = " << ct_trd_cpu << ", requested_cpu = " << requested_cpu << endl;

                if (requested_cpu > trd_cpu)    //如果得到的资源量大于总的需求量，则要修正
                {
                    ct_trd_cpu = trd_cpu - (requested_cpu - ct_trd_cpu);
                }
                //cout << ", 修正后：ct_trd_cpu = " << ct_trd_cpu << ", requested_cpu = " << requested_cpu << endl << endl;

                (*cit) = ct_trd_cpu;
            }
            else if ((count_row == 2) && (requested_gpu <= trd_gpu))     //第二行的元素总和要等于 trd_gpu
            {
                int ct_trd_gpu = 0;     //ct_trd_gpu 代表：在当前时间槽对GPU的资源需求量(四舍五入)
                double coef = (double)(1 + rand() % 50) / 100;  // 生成 [0.01, 0.50] 的随机数值作系数
                if (count_column == et) //最后一列
                {
                    ct_trd_gpu = trd_gpu - requested_gpu;
                }
                else
                {
                    ct_trd_gpu = trd_gpu * coef + 0.5;  //四舍五入
                }
                requested_gpu += ct_trd_gpu;

                //测试（成功）
                //cout << "随机生成 gpu 的时变资源需求, 在第 " << count_column << " 个执行时间槽时, coef = " << coef;  //检验系数是否在 [0.1, 0.5] 之间
                //cout << ", 修正前：ct_trd_gpu = " << ct_trd_gpu << ", requested_gpu = " << requested_gpu << endl;
                if (requested_gpu > trd_gpu)    //如果得到的资源量大于总的需求量，则要修正
                {
                    ct_trd_gpu = trd_gpu - (requested_gpu - ct_trd_gpu);
                }
                //cout << ", 修正后：ct_trd_gpu = " << ct_trd_gpu << ", requested_gpu = " << requested_gpu << endl << endl;

                (*cit) = ct_trd_gpu;
            }
            else if ((count_row == 3) && (requested_memory <= trd_memory))    //第三行的元素总和要等于 trd_memory
            {
                int ct_trd_memory = 0;     //ct_trd_memory 代表：在当前时间槽对 Memory 的资源需求量(四舍五入)
                double coef = (double)(1 + rand() % 30) / 100;  // 生成 [0.01, 0.30] 的随机数值作系数
                if (count_column == et) //最后一列
                {
                    ct_trd_memory = trd_memory - requested_memory;
                }
                else
                {
                    ct_trd_memory = trd_memory * coef + 0.5;  //四舍五入
                }
                requested_memory += ct_trd_memory;

                //测试（成功）
                //cout << "随机生成 memory 的时变资源需求, 在第 " << count_column << " 个执行时间槽时, coef = " << coef;  //检验系数是否在 [0.1, 0.5] 之间
                //cout << ", 修正前：ct_trd_memory = " << ct_trd_memory << ", requested_memory = " << requested_memory << endl;

                if (requested_memory > trd_memory)    //如果得到的资源量大于总的需求量，则要修正
                {
                    ct_trd_memory = trd_memory - (requested_memory - ct_trd_memory);
                }
                //cout << ", 修正后：ct_trd_memory = " << ct_trd_memory << ", requested_memory = " << requested_memory << endl <<endl;

                (*cit) = ct_trd_memory;
            }
            else if ((count_row == 4) && (requested_disk <= trd_disk))    //第四行的元素总和要等于 trd_disk
            {
                int ct_trd_disk = 0;     //ct_trd_disk 代表：在当前时间槽对 Disk 的资源需求量(四舍五入)
                double coef = (double)(1 + rand() % 20) / 100;  // 生成 [0.01, 0.20] 的随机数值作系数
                if (count_column == et) //最后一列
                {
                    ct_trd_disk = trd_disk - requested_disk;
                }
                else
                {
                    ct_trd_disk = trd_disk * coef + 0.5;  //四舍五入
                }
                requested_disk += ct_trd_disk;

                //测试（成功）
                //cout << "随机生成 disk 的时变资源需求, 在第 " << count_column << " 个执行时间槽时, coef = " << coef;  //检验系数是否在 [0.1, 0.5] 之间
                //cout << ", 修正前：ct_trd_disk = " << ct_trd_disk << ", requested_disk = " << requested_disk << endl;

                if (requested_disk > trd_disk)    //如果得到的资源量大于总的需求量，则要修正
                {
                    ct_trd_disk = trd_disk - (requested_disk - ct_trd_disk);
                }
                //cout << ", 修正后：ct_trd_disk = " << ct_trd_disk << ", requested_disk = " << requested_disk << endl <<endl;

                (*cit) = ct_trd_disk;
            }
        }
    }
    ////测试：是否正确生成 时变资源请求矩阵（成功）
    //cout << "赋值前, car[" << car.id << "].TRD = " << car.TRD << ", et = " << car.et << ", 其临时的时变资源请求矩阵rdm如下：" << endl;
    //for (auto rit = rdm.begin(); rit != rdm.end(); rit++)
    //{
    //    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
    //    {
    //        cout << (*cit) << " ";
    //    }
    //    cout << endl;
    //}
    //cout << endl;

    //为车辆的时变资源需求矩阵赋值
    car.RDM = rdm;

    ////测试：是否正确生成 时变资源请求矩阵（成功）
    //cout << "赋值后, car[" << car.id << "].TRD = " << car.TRD << ", et = " << car.et << ", 其时变资源请求矩阵如下：" << endl;
    //for (auto rit = car.RDM.begin(); rit != car.RDM.end(); rit++)
    //{
    //    for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
    //    {
    //        cout << (*cit) << " ";
    //    }
    //    cout << endl;
    //}
    //cout << endl;
}

//4.保存数据到本地
//(1)保存基站数据
void SaveEcsInfo()
{
    ofstream ofs;

    //(1)展示给用户浏览的基站信息
    string EcsInfoFile_show = "../data/1GenerateData/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
    //string EcsInfoFile_show = "../data/1GenerateData/exp3_no_bc/EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
    ofs.open(EcsInfoFile_show, ios::out | ios::trunc);     //如果文件存在，先删除，再创建
    for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
    {
        ofs << "ECS_id = " << ecsit->id << ",    ";                        //基站编号
        ofs << "Type = " << ecsit->type << ",    ";                        //基站类型
        ofs << "SignalCoverageRadius(m) = " << ecsit->SCR << ",    ";      //基站信号覆盖半径
        ofs << "DeploymentLocation(coord) = " << ecsit->DL << ",    ";     //基站部署位置（坐标点）
        ofs << "ResourceCapacity = " << ecsit->RC << ",    ";                  //基站资源容量

        ofs << "SignalCoverageArea(coord) = " << endl;                  //基站信号覆盖范围（坐标点）
        for (auto scait = (*ecsit).SCA.begin(); scait != (*ecsit).SCA.end(); scait++)
        {
            ofs << (*scait) << ", ";
        }
        ofs << endl;

        ofs << "ResourceCapacityMatrix = " << endl;     //基站的资源容量矩阵
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

    cout << "已成功将 " << tEcsNum << "条基站信息写入 EcsInfo.txt 中，供用户浏览。" << endl;
    cout << "其中，小型基站数量为：" << S_EcsNum << "，"
        << "大型基站数量为：" << L_EcsNum << "。" << endl;
    cout << endl;

    //(2)供算法输入使用的基站信息(除资源容量矩阵，带前缀 data_ )
    string EcsInfoFile_input = "../data/1GenerateData/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
    //string EcsInfoFile_input = "../data/1GenerateData/exp3_no_bc/data_EcsInfo_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
    ofs.open(EcsInfoFile_input, ios::out | ios::trunc);
    for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
    {
        ofs << ecsit->id << " ";                            //基站编号
        ofs << ecsit->type << " ";                          //基站类型
        ofs << ecsit->SCR << " ";                           //基站信号覆盖半径
        ofs << ecsit->DL.x << " " << ecsit->DL.y << " ";    //基站信号部署位置（坐标点）
        ofs << ecsit->RC.cpu << " " << ecsit->RC.gpu << " " << ecsit->RC.memory << " " << ecsit->RC.disk << " ";    //基站资源容量
        ////注意：车辆的 时变部署约束矩阵 是 ECS的部署位置，信号覆盖范围，以及 车辆的全程行驶方向，时变行驶速度，实时位置的综合体现。
        ////因此，为节省磁盘空间，仅保存车辆的时变部署约束矩阵，不再保存 ECS 的信号覆盖范围 
        //for (auto scait = (*ecsit).SCA.begin(); scait != (*ecsit).SCA.end(); scait++)   //基站信号覆盖范围（坐标点）
        //{
        //    ofs << scait->x << " " << scait->y << " ";
        //}
        ofs << endl;
    }
    ofs.close();

    //(3)供算法输入使用的基站信息：资源容量矩阵(带前缀 data_ )
    string EcsRCMFile_input = "../data/1GenerateData/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
    //string EcsRCMFile_input = "../data/1GenerateData/exp3_no_bc/data_Ecs_RCM_" + to_string(S_EcsNum) + "_" + to_string(L_EcsNum) + ".txt";
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
    cout << endl;

}
//(2)保存车辆数据
void SaveCarInfo()
{
    ofstream ofs;

    //1.展示给用户浏览的车辆信息
    string CarInfoFile_show = "../data/1GenerateData/CarInfo_" + to_string(S_CarNum) + "_" + to_string(L_CarNum) + ".txt";
    //string CarInfoFile_show = "../data/1GenerateData/exp3_no_bc/CarInfo_" + to_string(S_CarNum) + "_" + to_string(L_CarNum) + ".txt";
    ofs.open(CarInfoFile_show, ios::out | ios::trunc);

    //第一种形式：当车辆数量较多时，可用该种行驶保存“供用户浏览的车辆信息”
    ofs << "Car_id\t" << "Type\t" << "InitLoc(coord)\t" << "DrivingDirection\t" << "Velocity(m/t)\t" << "ArrivalTime\t" << "DeadlineTime\t" << "ExecutionTime\t" << "TaskResourceDemand\t" << "Bid(Coefficient)\t" << endl;
    for (auto carit = vCarInfo.begin(); carit != vCarInfo.end(); carit++)
    {
        //数据格式：编号       类型   初始位置(坐标)   行驶方向            速度  到达时间    截止时间        执行时间        任务资源需求          投标(投标系数)	
        //数据格式：Car_id     Type   InitLoc          DrivingDirection    v     ArrivalTime DeadlineTime    ExecutionTime   TaskResourceDemand    Bid(Coefficient)
        ofs << carit->id << "\t";
        ofs << carit->type << "\t";
        ofs << carit->RTL.front() << "\t\t";
        ofs << "{";
        for (auto ddit = (*carit).DD.begin(); ddit != (*carit).DD.end(); ddit++)
        {
            ofs << *ddit << ",";
        }
        ofs << "}\t\t";
        ofs << "[5,10]" << "\t\t";      //车辆行驶速度
        ofs << carit->at << "\t\t";     //车辆任务到达时间
        ofs << carit->dt << "\t\t";     //车辆任务截止时间
        ofs << carit->et << "\t\t";     //车辆任务执行时间
        ofs << carit->TRD << "\t\t\t";  //车辆任务资源需求
        ofs << carit->bid << "(" << carit->bc << ")\t";     //车辆投标(投标系数)
        ofs << endl;
    }
    ofs.close();

    ////第二种形式（示例用）：详细展示了所有车辆的信息，当车辆数量较少时，可用该种行驶保存“供用户浏览的车辆信息”
    //for (auto carit = vCarInfo.begin(); carit != vCarInfo.end(); carit++)
    //{
    //    //数据格式：编号       类型   初始位置(坐标)   行驶方向            速度  到达时间    截止时间        执行时间        任务资源需求          投标(投标系数)	
    //    //数据格式：Car_id     Type   InitLoc          DrivingDirection    v     ArrivalTime DeadlineTime    ExecutionTime   TaskResourceDemand    Bid(Coefficient)
    //    ofs << "Car_id = " << carit->id << ",    ";                //车辆编号
    //    ofs << "Type = " << carit->type << ",    ";                //车辆类型
    //    ofs << "ArrivalTime = " << carit->at << ",    ";           //车辆任务到达时间
    //    ofs << "DeadlineTime = " << carit->dt << ",    ";          //车辆任务截止时间
    //    ofs << "ExecutionTime = " << carit->et << ",    ";         //车辆任务执行时间
    //    ofs << "TaskResourceDemand = " << carit->TRD << ",    ";   //车辆任务资源需求
    //    ofs << "Bid(Coefficient) = " << carit->bid << "(" << carit->bc << ")" << endl;     //车辆投标(投标系数)
    //    ofs << "ResourceDemandMatrix = " << endl;   //车辆的时变资源需求矩阵
    //    for (auto rit = (*carit).RDM.begin(); rit != (*carit).RDM.end(); rit++)
    //    {
    //        for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
    //        {
    //            ofs << (*cit) << " ";
    //        }
    //        ofs << endl;
    //    }
    //    ofs << "DrivingDirection = {";  //车辆全程的行驶方向
    //    for (auto ddit = (*carit).DD.begin(); ddit != (*carit).DD.end(); ddit++)
    //    {
    //        ofs << (*ddit) << ",";
    //    }
    //    ofs << "},    ";
    //    ofs << "RealTimeVelocity = {";  //车辆的实时速度
    //    for (auto rtvit = (*carit).RTV.begin(); rtvit != (*carit).RTV.end(); rtvit++)
    //    {
    //        ofs << (*rtvit) << ",";
    //    }
    //    ofs << "},    ";
    //    ofs << "RealTimeLocation = {";  //车辆的实时位置
    //    for (auto rtlit = (*carit).RTL.begin(); rtlit != (*carit).RTL.end(); rtlit++)
    //    {
    //        ofs << (*rtlit) << ",";
    //    }
    //    ofs << "},    ";
    //    ofs << "DeploymentConstraintMatrix = " << endl;     //车辆的部署约束矩阵
    //    for (auto rit = (*carit).DCM.begin(); rit != (*carit).DCM.end(); rit++)
    //    {
    //        for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)
    //        {
    //            ofs << (*cit) << " ";
    //        }
    //        ofs << endl;
    //    }
    //    ofs << endl;
    //}
    //ofs.close();

    cout << "已成功将 " << tCarNum << "条车辆信息写入 CarInfo.txt 中，供用户浏览。" << endl;
    cout << "其中，小型车辆数量为：" << S_CarNum << "，"
        << "大型车辆数量为：" << L_CarNum << "。" << endl;
    cout << endl;

    //2.供算法输入使用的车辆信息(带前缀 data_ )
    SaveCarInfoAsInput(S_vCarInfo, "S", S_vCarInfo.size());	//保存小型车辆任务信息
    SaveCarInfoAsInput(L_vCarInfo, "L", L_vCarInfo.size());	//保存大型车辆任务信息

    cout << "已成功将 " << tCarNum << "条车辆任务写入 data_CarInfo.txt 中，供算法输入使用。" << endl;
    cout << "其中，小型车辆任务数目为：" << S_CarNum << ","
        << "大型车辆任务数目为：" << L_CarNum << "." << endl;
    cout << endl;

}
void SaveCarInfoAsInput(vector<CarInfo>& vCar, string type, int carnum)
{
    ofstream ofs;
    //(1)保存车辆任务信息(除时变部署约束矩阵)，供算法输入使用
    string CarInfoFile_input = "../data/1GenerateData/data_CarInfo_" + type + "_" + to_string(carnum) + ".txt";
    //string CarInfoFile_input = "../data/1GenerateData/exp3_no_bc/data_CarInfo_" + type + "_" + to_string(carnum) + ".txt";
    ofs.open(CarInfoFile_input, ios::out | ios::trunc);
    //注意：车辆的 时变部署约束矩阵 是 ECS的部署位置，信号覆盖范围，以及 车辆的全程行驶方向，时变行驶速度，实时位置的综合体现。
    //因此，为节省磁盘空间，仅保存车辆的时变部署约束矩阵，不再保存车辆的行驶方向，行驶速度和实时位置
    for (auto carit = vCar.begin(); carit != vCar.end(); carit++)
    {
        //数据格式：编号       类型  到达时间    截止时间        执行时间        任务资源需求          投标(投标系数)	
        //数据格式：Car_id     Type  ArrivalTime DeadlineTime    ExecutionTime   TaskResourceDemand    Bid(Coefficient)
        ofs << carit->id << " ";        //车辆任务编号
        ofs << carit->type << " ";      //车辆任务类型
        ofs << carit->at << " ";        //车辆任务到达时间
        ofs << carit->dt << " ";        //车辆任务截止时间
        ofs << carit->et << " ";        //车辆任务执行时间
        ofs << carit->TRD.cpu << " " << carit->TRD.gpu << " " << carit->TRD.memory << " " << carit->TRD.disk << " ";    //车辆任务资源需求
        ofs << carit->bid << " " << carit->bc << " ";   //车辆投标 以及 车辆投标系数
        ofs << endl;
    }
    ofs.close();

    //(2)保存车辆的时变资源请求矩阵，供算法输入使用
    string CarRDMFile_input = "../data/1GenerateData/data_Car_RDM_" + type + "_" + to_string(carnum) + ".txt";
    //string CarRDMFile_input = "../data/1GenerateData/exp3_no_bc/data_Car_RDM_" + type + "_" + to_string(carnum) + ".txt";
    ofs.open(CarRDMFile_input, ios::out | ios::trunc);
    for (auto carit = vCar.begin(); carit != vCar.end(); carit++)     //遍历存放车辆的容器
    {
        for (auto rit = (*carit).RDM.begin(); rit != (*carit).RDM.end(); rit++) //遍历特定车辆的部署矩阵的“行”
        {
            for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)   //遍历特定车辆的部署矩阵的“每行”的“列元素”
            {
                ofs << (*cit) << " ";
            }
            ofs << endl;
        }
        ofs << endl;
    }
    ofs.close();

    //(3)保存车辆的时变部署约束矩阵，供算法输入使用
    string CarDCMFile_input = "../data/1GenerateData/data_Car_DCM_" + type + "_" + to_string(carnum) + ".txt";
    //string CarDCMFile_input = "../data/1GenerateData/exp3_no_bc/data_Car_DCM_" + type + "_" + to_string(carnum) + ".txt";
    ofs.open(CarDCMFile_input, ios::out | ios::trunc);
    for (auto carit = vCar.begin(); carit != vCar.end(); carit++)     //遍历存放车辆的容器
    {
        for (auto rit = (*carit).DCM.begin(); rit != (*carit).DCM.end(); rit++) //遍历特定车辆的部署矩阵的“行”
        {
            for (auto cit = (*rit).begin(); cit != (*rit).end(); cit++)   //遍历特定车辆的部署矩阵的“每行”的“列元素”
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

    // 1.建模
    Modeling();
    //测试: 检查部署 ECS 的关键道路位置是否为 1 （成功）

    //cout << "检查部署 ECS 的关键道路位置，如下所示:" << endl;
    //// 直接访问时，默认下标从0开始
    //cout << "第 1 个ECS部署位置: Road[0][29] = " << Road[0][29] << endl;
    //cout << "第 2 个ECS部署位置: Road[0][89] = " << Road[0][89] << endl;
    //cout << "第 3 个ECS部署位置: Road[29][0] = " << Road[29][0] << endl;
    //cout << "第 4 个ECS部署位置: Road[29][29] = " << Road[29][29] << endl;
    //cout << "第 5 个ECS部署位置: Road[29][59] = " << Road[29][59] << endl;
    //cout << "第 6 个ECS部署位置: Road[29][89] = " << Road[29][89] << endl;
    //cout << "第 7 个ECS部署位置: Road[29][119] = " << Road[29][119] << endl;
    //cout << "第 8 个ECS部署位置: Road[59][29] = " << Road[59][29] << endl;
    //cout << "第 9 个ECS部署位置: Road[59][89] = " << Road[59][89] << endl;
    //cout << "第 10 个ECS部署位置: Road[89][0] = " << Road[89][0] << endl;
    //cout << "第 11 个ECS部署位置: Road[89][29] = " << Road[89][29] << endl;
    //cout << "第 12 个ECS部署位置: Road[89][59] = " << Road[89][59] << endl;
    //cout << "第 13 个ECS部署位置: Road[89][89] = " << Road[89][89] << endl;
    //cout << "第 14 个ECS部署位置: Road[89][119] = " << Road[89][119] << endl;
    //cout << "第 15 个ECS部署位置: Road[119][29] = " << Road[119][29] << endl;
    //cout << "第 16 个ECS部署位置: Road[119][89] = " << Road[119][89] << endl;

    // 2.生成基站信息
    GenerateEcsInfo();

    ////测试: 是否成功生成基站信息数据(成功)
    //cout << "本次生成的基站总数 tEcsNum = " << tEcsNum
    //    << " ,其中包含小型基站信息 S_EcsNum = " << S_EcsNum << " 条, 大型基站信息数据 L_EcsNum = " << L_EcsNum << " 条. 具体如下：" << endl;
    //cout << "基站编号(id)  基站类型(type)  信号覆盖半径(SCR)  部署位置(DL)  资源容量(RC)" << endl;
    //for (auto ecsit = vEcsInfo.begin(); ecsit != vEcsInfo.end(); ecsit++)
    //{
    //    cout << ecsit->id << "\t\t";
    //    cout << ecsit->type << "\t\t";
    //    cout << ecsit->SCR << "\t\t";
    //    cout << "(" << ecsit->DL.x << "," << ecsit->DL.y << ")\t\t";
    //    cout << "[" << ecsit->RC->cpu << "," << ecsit->RC->gpu << "," << ecsit->RC->memory << "," << ecsit->RC->disk << "]";
    //    cout << endl;
    //    cout << "信号覆盖范围(SCA)：";
    //    for (auto scait = (*ecsit).SCA.begin(); scait != (*ecsit).SCA.end(); scait++)
    //    {
    //        cout << "(" << scait->x << "," << scait->y << "), ";
    //    }
    //    cout << endl;
    //    cout << endl;
    //}
    //cout << endl;

    // 3.生成车辆信息
    vCarInfo.reserve(81000);
    S_vCarInfo.reserve(41000);
    L_vCarInfo.reserve(41000);
    //GenerateCarInfo();     //生成小型车辆数据

    // 4.保存信息到本地
    // (1)保存基站数据
    SaveEcsInfo();

    // (2)保存车辆数据
    //SaveCarInfo();

    system("pause");
    return 0;
}
