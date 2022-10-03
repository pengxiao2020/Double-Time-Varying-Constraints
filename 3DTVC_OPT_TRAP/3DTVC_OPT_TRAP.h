#pragma once
#include "GlobalDefinition.h"
#include "EcsInfo.h"
#include "CarInfo.h"

//����ȫ�ֺ������� << �����(C++���ı�� p40, 4.5.2), ʹ���������������
ostream& operator<<(ostream& cout, Coordinate& coord);

//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ�����������������Դ���� [CPU, GPU, Memory, Disk]
ostream& operator<<(ostream& cout, Resource& trd);
//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ������� ��վ ��Ϣ���������
ostream& operator<<(ostream& cout, EcsInfo& ecs);
//����ȫ�ֺ������� << �����(C++���ı�� p40), ʹ������� ���� ��Ϣ���������
ostream& operator<<(ostream& cout, CarInfo& car);

//��һ������������
//�����վ��Ϣ��һ���Զ��ꡣ
void LoadEcsInfo(string EcsInfofile, string EcsRCMfile);
//��ȡ������Ϣ�������Ͷ���
void LoadCarInfo(string CarInfofile, string CarRDMfile, string CarDCMfile, int CarNum, CarInfo* carp);


//�ڶ��������亯�����������ŷ�����
int* TRAP_IP(CarInfo* carp);
int LessthanOrEqual(int a, int b);		//����С�ڵ��ڵ�������ֵ
int GreaterthanOrEqual(int a, int b);	//���ش��ڵ��ڵ�������ֵ

//��������֧���������������������
void TRAP_PAY(int* X);
//��װһ���������� Car[i] �߳���������Car[]��
//������env, model, cplex, ��������[], Ҫ�߳��ĳ���
void RemoveCar_i(CarInfo* carp, int id);
//��װһ���������ָ� Car[i] ����Ϣ
void RecoverCar_i(CarInfo* carp, int id);

//���Ĳ���������,���浽����
void SaveResult();

void CalcResourceUtilizationRatio();	//������Դ������

void CalcServedCarRatio();	//�������������

