文件夹：1GenerateData, 2RandomSampling, 3DTVC_OPT_TRAP, 4DTVC_OVMAP, 5DTVC_OTRAP 和 6TruthfulnessVerification 存放的是C++代码，建议使用 VS 2022 打开
按照实验顺序，各解决方案的功能如下：
1. 1GenerateData 生成实验用的大数据库，并保存到本地。
2. 2RandomSampling 读入 1GenerateData 生成的大数据库中的ECS和车辆信息，进行“随机抽取”工作，并保存到本地。
3. 3DTVC_OPT_TRAP, 4DTVC_OVMAP, 5DTVC_OTRAP 分别读入 2RandomSampling 抽取的相同数据，进行实验，并把实验结果保存到本地。

文件夹：data 中存放了各种机制的实验结果。例如：
1. 文件夹 data\exmaple 存放 调试代码初期 使用的小示例，一共生成了 20 个车辆数据 和 16 个 ECS 数据
    （1）文件夹 data\exmaple\1GenerateData\ 存放生成的 20 个车辆数据（10个小任务，10个大任务） 和 16 个 ECS 数据
    （2）文件夹 data\exmaple\2RandomSampling\ 存放 从 data\exmaple\1GenerateData\ 读入的 20个车辆数据中抽取的 10 个车辆数据（其中5个小任务，5个大任务）
    （3）文件夹 data\exmaple\ 中的 3 个 .txt 文件分别是 DTVC-OPT-TRAP机制，DTVC-OVMAP机制 和 DTVC-OTRAP机制 对小示例 的实验结果
      
2. 文件夹 data\ExperimentalResults 存放 论文 第6节 的实验结果
    （1）文件夹 data\ExperimentalResults\exp1 存放 6.2.1 小节的实验结果，并附有 EXCEL 表以统计实验结果
    （2）文件夹 data\ExperimentalResults\exp2 存放 6.2.2 小节的实验结果，并附有 EXCEL 表以统计实验结果
    （3）文件夹 data\ExperimentalResults\exp3 存放 6.2.3 小节的实验结果，exp3 研究 ECS 类型对 资源分配的影响（没有写在论文中）
    （4）文件夹 data\ExperimentalResults\exp4 存放 6.2.4 小节的实验结果，exp4 验证了 DTVC-OTRAP 的真实性（没有写在论文中）（胜者6823 和 败者7703）
NOTE：
          ① 举例：DTVC_OPT_TRAP_Solution_CarInfo_100_0-1.txt
              DTVC_OPT_TRAP_Solution : 表示这是 DTVC_OPT_TRAP 机制的求解结果
              CarInfo_100_0 : 表示本次输入的车辆信息为：100个小型车辆任务请求，和 0 个大型车辆任务请求
              -1 : 表示这是第 1 次 随机抽取的车辆信息作为输入 而 得出的求解方案
              那么，DTVC_OPT_TRAP_Solution_CarInfo_100_0-1.txt 就是：DTVC_OPT_TRAP 机制以“第 1 次随机抽取的 100个小型车辆任务请求信息”作为输入，得到的求解方案
          ② DTVC_OVMAP_Solution_CarInfo_100_0-1.txt 就是：DTVC_OVMAP 机制以“第 1 次随机抽取的 100个小型车辆任务请求信息”作为输入，得到的求解方案
          ③ DTVC_OTRAP_Solution_CarInfo_100_0-1.txt 就是：DTVC_OTRAP 机制以“第 1 次随机抽取的 100个小型车辆任务请求信息”作为输入，得到的求解方案

3. 格式说明：以 exmaple 文件夹中的数据为例：
    （1）在文件夹 data\exmaple\1GenerateData\ 和 文件夹 data\exmaple\2RandomSampling\ 中：
            ①带前缀“data_”的 .txt 文件都用作算法读入，如“data_CarInfo_L_10-1.txt”，“data_RS_CarInfo_S_5-1.txt”.
            ②不带前缀“data_”的 .txt 文件可供读者查看生成的数据信息，如“CarInfo_10_10-1.txt”，“RS_CarInfo_5_5-1.txt”.
    （2）文件名介绍，例如：①“EcsInfo_16_0.txt”，②“CarInfo_10_10-1.txt”，③“data_RS_Car_DCM_S_10-1.txt”，④“data_RS_Car_RDM_S_10-1.txt”
            ① EcsInfo_ 表示这是基站(ECS)信息数据，
                10_0 = S_L 表示本实验中的基站有 16 个 小型基站 和 0个 大型基站，
                并且，同文件夹下的其他车辆信息文件（特指时变部署约束矩阵）也都是基于这 16 个小型基站信息（尤其是信号覆盖范围）来生成的。
                那么，EcsInfo_16_0.txt 就是：生成的“供用户查看的”基站信息数据，其中，包含 16 个小型基站 和 0个 大型基站 的信息
            ② RS_ = RandomSampling，表示数据已经过项目 2Randomsampling 的随机抽取；
                CarInfo_ 表示这是车辆信息数据，
                10_10  = S_L 表示该文件中含有 10个小型车辆任务请求信息，10个大型车辆任务请求信息
                -1 表示这是第 1 次随机抽取的 车辆信息。
                那么，RS_CarInfo_10_0-1.txt 就是：“第 1 次随机抽取”的“供用户查看”的车辆信息数据，其中，包含 10个小型车辆任务请求信息，0个中型车辆任务请求信息和 0个大型车辆任务请求信息
            ③ 车辆的部署约束矩阵需要单独存放，而且，因为数据量过大，无法正常打开查看（文件中只包含 0 或 1 ）。example文件夹中的 RDM 和 DCM 都可以打开
                data_ : 表示这是供算法输入使用
                RS_ = RandomSampling，表示数据已经过项目 2Randomsampling 的随机抽取；
                Car_ : 表示这是车辆数据
                DCM_ : 这是部署约束矩阵(Deployment Constraint Matrix)的缩写
                S_10 = Small_10 : 表示该文件中是 10 个小型车辆任务请求的时变部署约束矩阵
                -1 ：这是 第 1 次 随机抽取的车辆信息的时变部署约束矩阵
                那么，data_RS_Car_DCM_S_10-1.txt 就是：“第 1 次随机抽取”的“供算法输入使用”的 10 个 小型 车辆任务请求的 时变部署约束矩阵，
            ④车辆的时变资源请求矩阵 RDM 需要单独存放，其中 RDM 每一行的总和 收到 资源请求向量的约束。但是，因为数据量过大，无法正常打开查看。example文件夹中的 RDM 和 DCM 都可以打开
                data_ : 表示这是供算法输入使用
                RS_ = RandomSampling，表示数据已经过项目 2Randomsampling 的随机抽取；
                Car_ : 表示这是车辆数据
                RDM_ : 这是 资源请求矩阵 (Resource Demand Matrix)的缩写
                S_10 = Small_10 : 表示该文件中是 10 个小型车辆任务请求的 资源请求矩阵
                -1 ：这是 第 1 次 随机抽取的车辆信息的资源请求矩阵
                那么，data_RS_Car_RDM_S_10-1.txt 就是：“第 1 次随机抽取”的“供算法输入使用”的 10 个 小型 车辆任务请求的 资源请求矩阵，

如有其他问题，可联系 通讯作者：张骥先老师，邮箱：denonji@163.com
                         或者  第一作者： 彭晓 的QQ：632282329，或者邮箱：pengxiao2017@qq.com.

                
                
      
          