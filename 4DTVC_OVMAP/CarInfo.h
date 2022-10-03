#pragma once
#include "GlobalDefinition.h"

struct CarInfo
{
	//����
	int id;			//�������
	string type;	//������������
	vector<int> RTV;        //����ʵʱ�ٶ�(ʱ��): ��m/sΪ��λ����Χ: v = [5,10]m/s = [50,100]m/t, RTV = Real-Time Velocity
	vector<string> DD;      //���������ʻ����: ֱ��,��ͷ,��ת,��ת straight / back / left / right, DD = DrivingDirection
							//����(30,30),(30,90),(90,30),(90,90)���ı���ʻ���� 
	vector<Coordinate> RTL;      //����ʵʱλ��, ���Ӿ��ȷֲ� U[0,L],  cl = Current Location

	//������������ theta_i
	int at = 0;		//����ʱ��
	int dt = 0;		//�����ֹʱ��
	int et = 0;		//����ִ��ʱ��
	struct Resource TRD;	//������Դ���� TaskResourceDemand = TRD[4] = {s1, s2, s3, s4} = {CPU, GPU, Memory, Disk}
	double bid = 0.00;	//Ͷ��
	double bc = 0.00;	//Ͷ��ϵ��
	double pay = 0.00;	//��������

	//������������Դ�������(Task Resource Demand Matrix)
	//vector<vector<int>> RDM = vector<vector<int>>(R, vector<int>(et, false));
	vector<vector<int>> RDM;

	//������ʱ�䲿��Լ������(Deployment Constraint Matrix)
	vector<vector<bool>> DCM = vector<vector<bool>>(M, vector<bool>(T, false));

	//����״̬
	int bat = -1;				//�����õ������ʱ�� BeAllocatedTime����ֵΪʱ���
	bool alloc_flag = false;	//��� ���� �Ƿ�õ���վ����
	bool accom_flag = false;	//��� ���� �Ƿ�������� accom = accomplish 
	int ConnectTo = -1;			//��� �������ĸ���վ��ECS[j].id��ʵ�ʽ�������
	double dr = 0.00;		//������������Դ����
	double f = 0.00;		//�����ļ�ֵ�ܶ�

	///���ظ�ֵ�������ʹ������������Ϣ�����໥����
	//lhs = left hand side, ��ֵ������Ŀ��
	//rhs = right hand side, ��ֵ������Դͷ
	CarInfo& operator= (const CarInfo& rhs)
	{
		this->id = rhs.id;
		this->type = rhs.type;

		this->at = rhs.at;
		this->dt = rhs.dt;
		this->et = rhs.et;

		this->TRD.cpu = rhs.TRD.cpu;	//��Դ����ֵ
		this->TRD.gpu = rhs.TRD.gpu;
		this->TRD.memory = rhs.TRD.memory;
		this->TRD.disk = rhs.TRD.disk;

		this->bid = rhs.bid;
		this->bc = rhs.bc;
		this->pay = rhs.pay;

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(rhs.et, 0)));	//�ݴ� rhs.RDM
		rdm = rhs.RDM;
		this->RDM = rdm;

		vector<vector<bool>> dcm = vector < vector<bool>>(M, (vector<bool>(T, false)));	//�ݴ� rhs.DCM
		dcm = rhs.DCM;
		this->DCM = dcm;

		//for (int j = 0; j < M; j++)
		//{
		//	for (int t = 0; t < T; t++)
		//	{
		//		this->DCM[j][t] = rhs.DCM[j][t];	//ʱ�䲿��Լ���������ֵ
		//	}
		//}

		this->bat = rhs.bat;
		this->alloc_flag = rhs.alloc_flag;
		//this->accom_flag = rhs.accom_flag;
		this->ConnectTo = rhs.ConnectTo;
		this->dr = rhs.dr;
		this->f = rhs.f;

		return *this;
	}

};



