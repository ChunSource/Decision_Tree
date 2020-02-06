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

	struct info //记录属性对应的纯的位置和数量
	{
		double sum = 0;
		vector<int> index;
	};

	class Tree
	{
	public:
		bool	isOvercast = false;  //表示 是否叶子节点
		int		dataIndex = -1;		 //表示哪类属性
		double		data = -1;			 //表示哪类属性的哪个数据
		double		result = -1;
		vector<Tree*> otherNode ;
	};
	Decision_Tree(vector<vector<double>> data);
	double check(vector<double>& data);
	double Recursive_Tree(Tree*, vector<double>& data);
private:
	void process_entropy(vector<vector<double>>& data,Tree* tmpNode);

	double mathlog(double a, double x);
	//计算类别熵
	double class_entropy(vector<double> nums);

	//nums是属性，source是类型，由此计算条件熵
	double property_entropy(vector<double>& nums, vector<double>& source); //!!!!!!!

	//计算信息增益  类别熵 - 条件熵
	double information_divergence(double information, double condition);

	//计算分裂信息度量
	double instrisic_information(vector<double>& nums);

	//信息增益率 ==  信息增益 /  信息分裂度量
	double Gain_rate(double divergence, double instrisic);

	//从t中删除index保存的位置的元素
	template<class T>
	void removeVectorElement(vector<T>& t, vector<int> index);

	//打印数据集
	void show(vector<vector<double>>& data);

	//给定纯属性值，获取位置
	vector<int> scan_Pure(int row, double cmp,vector<vector<double>>& data);

	//给定属性类型，获取所有纯和不存的信息
	map<double, map<double, vector<int>>> scan_Pure(int row, vector<vector<double>>& data);


	//member:
	Tree* rootNode = nullptr;   //创建一个根节点

	vector<double> Entropy;  //记录条件熵
	map<int,double> IGR;   //信息增益率

};

