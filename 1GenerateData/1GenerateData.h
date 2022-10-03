#pragma once
#include "GlobalDefinition.h"
#include "CarInfo.h"
#include "EcsInfo.h"

//����ȫ�ֺ������� << �����(C++���ı�� p40, 4.5.2), ʹ���������������
ostream& operator<<(ostream& cout, Coordinate& coord);

//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ�����������������Դ���� [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);

// 1.��ģ
void Modeling();

// 2.���ɻ�վ��Ϣ
//����vector����������������ɵĻ�վ��Ϣ
vector<EcsInfo> vEcsInfo;

//1.���ɻ�վ��Ϣ
void GenerateEcsInfo();
//ȷ�ϻ�վ����  CalcEcsType = Calculate Type of ECS 
void CalcEcsType(EcsInfo& ecs);
//�����վ�Ķ�ά����λ��  CalcEcsDL = Calculate Deployment Location of ECS 
void CalcEcsDL(EcsInfo& ecs);
//���� ECS ���źŸ��Ƿ�Χ: ʹ�� vector ������������ܱ��źŸ��ǵ������(x,y)
void CalcEcsSCA(EcsInfo& ecs);

//�ڶ����֣��������
//��ų�����Ϣ��������vector<CarInfo>
vector<CarInfo> vCarInfo;
//����vector�������ֱ�������Ų�ͬ���͵ĳ�����Ϣ
vector<CarInfo> S_vCarInfo;	//���С�ͳ��������� S_vCarInfo = Small vCarInfo
vector<CarInfo> L_vCarInfo;	//��Ŵ��ͳ��������� L_vCarInfo = Large vCarInfo

vector<int> vCarAT;			//��ų�������ʱ�� ArrivalTime ������
vector<Coordinate> vCarIL;	//��ų�����ʼλ�� InitLoc ������
vector<string> vCarDD;		//��ų����� 4 ����ʻ���� DrivingDirection �����ϣ����£����У�����
							//��ʻ�����ǻ���������·��ģ�ĽǶȶ��Եģ����ǳ��������ֱ��,����,��ת����ת
vector<double>vCarBC;		//��ų���Ͷ��ϵ�� BidCoefficient ������ 
							//һ�복����Ͷ��ϵ��Ϊ[0.25,1],��һ�복����Ͷ��ϵ��Ϊ[1,4]
//1.���ɳ�����Ϣ
void GenerateCarInfo();
//(1)���ɳ�������ʱ��ĺ���
//�������� Normal Distribution ����������������ɳ�������ʱ��
void InitialCarArrivalTime();
//(2)���ɳ��������ʼλ�õĺ���
void InitialCarLocation();
//(3)���ɳ��������ʼ��ʻ����ĺ���
void InitialCarDrivingDirection();
//(4)���ɳ������Ͷ��ϵ���ĺ���
void InitialCarBidCoefficient();
//(5)���ó������͵ĺ��� 
void SetCarType(CarInfo& car);
//(6)���ó�����ʼ��ʻ����ĺ���
void SetCarInitialDrivingDirection(CarInfo& car);
//(7)���ó�������ִ��ʱ��ĺ���
void SetCarExecutionTime(CarInfo& car);
//(8)���ó��������ֹʱ��ĺ���
void SetCarDeadlineTime(CarInfo& car);
//(9)���ó�������������Դ����(��̬)�ĺ���
void SetCarTaskResourceDemand(CarInfo& car);
//(10)���ó�������Ͷ��ĺ���
void SetCarBid(CarInfo& car);
//(11)���㳵����ȫ��ʵʱ��ʻ��Ϣ:������ʻ����,ʵʱ�ٶȺ�ʵʱλ��
void CalcCarEntireDrivingInformation(CarInfo& car);
//(12)���㳵����ʱ�䲿��Լ������ Deployment Constraints Matrix
void CalcCarDeploymentConstraintsMatrix(CarInfo& car);
//(13)���㳵����ʱ����Դ������� Resource Demands Matrix
void CalcCarResourceDemandsMatrix(CarInfo& car);

//�������֣������ɵ����ݱ��浽����
//1.����վ��Ϣ���浽���ع��û��鿴
void SaveEcsInfo();

//2.��������Ϣ(�������Ӿ�����)���浽����
void SaveCarInfo();
//��װһ���������ѳ����ĸ������Ա��浽��ͬ�ļ��У����㷨����ʹ��
void SaveCarInfoAsInput(vector<CarInfo>& Car, string type, int carnum);