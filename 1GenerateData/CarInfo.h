#pragma once
#include "GlobalDefinition.h"

struct CarInfo
{
    //����
    int id;         //�������:1~tCarNum��
    string type;    //��������: S / M / L
    vector<int> RTV;        //����ʵʱ�ٶ�(ʱ��): ��m/sΪ��λ����Χ: v = [5,10]m/s = [50,100]m/t, RTV = Real-Time Velocity
    vector<string> DD;      //���������ʻ����: ֱ��,��ͷ,��ת,��ת straight / back / left / right, DD = DrivingDirection
                            //����(30,30),(30,90),(90,30),(90,90)���ı���ʻ���� 
    vector<Coordinate> RTL;      //����ʵʱλ��, ���Ӿ��ȷֲ� U[0,L],  cl = Current Location

    int at = 0;         //����ʱ��a[i], ������̬�ֲ� N[T/2,(T/8)^2], at = ArriveTime
    int dt = 0;         //������ɽ�ֹʱ�� d[i], dt = DeadlineTime
    int et = 0;         //ִ��ʱ�� e[i], et = ExecutionTime
    struct Resource TRD;    //������Դ���� s[i]������������Դ��(CPU, GPU, Memory, Disk)
    double bc = 0.00;   //Ͷ��ϵ�� mu[i], ���Ӿ��ȷֲ� U[0.25, 4], bc = BidCoefficient
    double bid = 0.00;      //Ͷ��b[i], b = Bid

    //������ʱ����Դ�������(Task Resource Demand Matrix)
    vector<vector<int>> RDM;

    //������ʱ�䲿��Լ������(Deployment Constraint Matrix)
    vector<vector<bool>> DCM = vector<vector<bool>>(M, vector<bool>(T, false));

    //���� Payment
    double pay = 0.00;       // p = Payment

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

		vector<vector<int>> rdm = vector<vector<int>>(R, (vector<int>(rhs.et, 0)));		//�ݴ� rhs.RDM
		rdm = rhs.RDM;
		this->RDM = rdm;

		vector<vector<bool>> dcm = vector<vector<bool>>(M, (vector<bool>(T, false)));	//�ݴ� rhs.DCM
		dcm = rhs.DCM;
		this->DCM = dcm;

		return *this;
	}

};