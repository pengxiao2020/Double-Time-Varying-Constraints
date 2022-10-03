#pragma once
#include "GlobalDefinition.h"

//EcsInfo �����������վ��Ϣ

struct EcsInfo
{
	//����
	int id;		//��վ��� 1~tEcsNum

	string type;	//��վ���� S/M/L 

	int SCR = 0;	//��վ���źŸ��ǰ뾶 ͳһΪ 300 m�����۴�С��

	struct Coordinate DL;	//��վ����λ��(DeploymentLocation)�����ֵ�·�е�λ��,ʹ�ö�ά�����ʾ(x,y)

	vector<Coordinate> SCA;	//��վ�źŸ��Ƿ�Χ SignalCoverageArea����������ܹ����ǵ������

	struct Resource RC;	//��Դ��������������������Դ��(CPU, GPU, Memory, Disk)

	vector<vector<int>> RCM = vector<vector<int>>(R, vector<int>(T, 0));	//��Դ��������

	EcsInfo& operator= (const EcsInfo& rhs)
	{
		this->id = rhs.id;
		this->type = rhs.type;
		this->SCR = rhs.SCR;

		this->DL.x = rhs.DL.x;
		this->DL.y = rhs.DL.y;

		this->RC.cpu = rhs.RC.cpu;
		this->RC.gpu = rhs.RC.gpu;
		this->RC.memory = rhs.RC.memory;
		this->RC.disk = rhs.RC.disk;

		vector<vector<int>> rcm = vector<vector<int>>(R, vector<int>(T, 0));	//��Դ��������
		rcm = rhs.RCM;
		this->RCM = rcm;

		return *this;
	}
};
