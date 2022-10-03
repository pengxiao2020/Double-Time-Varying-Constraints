#pragma once
#include "GlobalDefinition.h"
#include "EcsInfo.h"
#include "CarInfo.h"

//����ȫ�ֺ������� << �����(C++���ı�� p40, 4.5.2), ʹ���������������
ostream& operator<<(ostream& cout, Coordinate& coord);

//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ�����������������Դ���� [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);

//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ���������վ��Ϣ���������
ostream& operator<<(ostream& cout, EcsInfo& ecs);

//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ�������������Ϣ���������
ostream& operator<<(ostream& cout, CarInfo& car);

//��һ������������
vector<EcsInfo> vEcsInfo;	//������л�վ��Ϣ������

vector<CarInfo> T_vCarInfo;	//������г�����Ϣ������

//vector<CarInfo> S_vCarInfo;	//���С�ͳ�����Ϣ������
//vector<CarInfo> M_vCarInfo;	//������ͳ�����Ϣ������
//vector<CarInfo> L_vCarInfo;	//��Ŵ��ͳ�����Ϣ������

//�����վ��Ϣ��һ���Զ��ꡣ
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile);
//���복����Ϣ�������Ͷ���
void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, vector<CarInfo>& Car, int CarNum);

//�ڶ�����ÿ��ʱ���ִ��һ��OTRAP�㷨
//vector<CarInfo> BeAllocatedCar;	//��÷���������ִ������ĳ�������
vector<CarInfo> UnAllocatedCar;	//δ��÷����Ҵ���������Ч�ڵĳ�������

vector<CarInfo> AllocResult;		//��ֹ��[1,t]����÷�������г�������
vector<CarInfo> cts_AllocResult;	//��ǰʱ��۵ķ����� cts = current tims slot

vector<CarInfo> tmp_X;			//���ÿ������ OTRAP_ALLOC ��õ�����ĳ������ϣ��´����� OTRAP_ALLOC ǰ���

//1.DTVC_OTRAP �㷨���
void DTVC_OTRAP(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& Car);		//DTVC_OTRAP �㷨���

//2.DTVC_OTRAP ���亯��
void DTVC_OTRAP_ALLOC(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar);	// DTVC_OTRAP ���亯��
bool EcsInDegree_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2);	//����վ�������Ի�վ����
void CalcCarValueDensity(EcsInfo& ecs, int cts);	//���㣺�ڵ�ǰʱ��� t ���ܹ����� ecs �����г�������Դ�ܶ� Car[i].f
bool CarValueDensity_Descending(const CarInfo& car1, const CarInfo& car2);	//�� ������Դ�ܶ�Car[i].f �ǵ���˳��Գ�������
bool EscId_Ascending(const EcsInfo& ecs1, const EcsInfo& ecs2);	//���� ECS_id ������������ vEcsInfo �� ECS ��˳��

//3.DTVC_OTRAP ֧�����������ַ���
void DTVC_OTRAP_PAY(int cts, vector<EcsInfo>& ECS, vector<CarInfo>& UACar, vector<CarInfo>& cts_AR);


//��������������,���浽����
void SaveResult();
void CalcResourceUtilizationRatio();	//������Դ������
void CalcServedCarRatio();		//�������������
void CalcNormalPayCarRatio();		//������������ĳ�������
