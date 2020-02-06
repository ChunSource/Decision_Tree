#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include<algorithm>
using namespace std;

class Decision_Tree
{
public:
	struct gain_H
	{
		double GainRate = 0;
		double InstrisicInformation = 0;
	};

	struct info //��¼���Զ�Ӧ�Ĵ���λ�ú�����
	{
		double sum = 0;
		vector<int> index;
	};

	class Tree
	{
	public:
		bool	isOvercast = false;  //��ʾ �Ƿ�Ҷ�ӽڵ�
		int		dataIndex = -1;		 //��ʾ��������
		double		data = -1;			 //��ʾ�������Ե��ĸ�����
		double		result = -1;
		vector<Tree*> otherNode ;
	};
	Decision_Tree(vector<vector<double>> data);
	double check(vector<double>& data);
	double Recursive_Tree(Tree*, vector<double>& data);
private:
	void process_entropy(vector<vector<double>>& data,Tree* tmpNode);

	double mathlog(double a, double x);
	//���������
	double class_entropy(vector<double> nums);

	//nums�����ԣ�source�����ͣ��ɴ˼���������
	double property_entropy(vector<double>& nums, vector<double>& source); //!!!!!!!

	//������Ϣ����  ����� - ������
	double information_divergence(double information, double condition);

	//���������Ϣ����
	double instrisic_information(vector<double>& nums);

	//��Ϣ������ ==  ��Ϣ���� /  ��Ϣ���Ѷ���
	double Gain_rate(double divergence, double instrisic);

	//��t��ɾ��index�����λ�õ�Ԫ��
	template<class T>
	void removeVectorElement(vector<T>& t, vector<int> index);

	//��ӡ���ݼ�
	void show(vector<vector<double>>& data);

	//����������ֵ����ȡλ��
	vector<int> scan_Pure(int row, double cmp,vector<vector<double>>& data);

	//�����������ͣ���ȡ���д��Ͳ������Ϣ
	map<double, map<double, vector<int>>> scan_Pure(int row, vector<vector<double>>& data);


	//member:
	Tree* rootNode = nullptr;   //����һ�����ڵ�

	vector<double> Entropy;  //��¼������
	map<int,double> IGR;   //��Ϣ������

};

