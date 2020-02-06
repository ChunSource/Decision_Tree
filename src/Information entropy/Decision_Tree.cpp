#include "Decision_Tree.h"

Decision_Tree::Decision_Tree(vector<vector<double>> data)
{

	this->rootNode = new Tree;
	process_entropy(data, rootNode);
	//目前位置第一层信息熵计算和数据重整已经没有任何问题了，2020.2.3-2020.2.4
}


double Decision_Tree::check(vector<double>& data)
{
	return Recursive_Tree(rootNode, data);
}

double Decision_Tree::Recursive_Tree(Tree* tmp, vector<double>& data)
{
	for (auto it : tmp->otherNode)
	{
		int index = it->dataIndex;

		if (it->data == data[index])  //判断属性是否对应数据
		{
			if (it->isOvercast)
			{
				return it->result;
			}
			else
			{
				return Recursive_Tree(it, data);
			}

		}
	}
	return -1;
}

void Decision_Tree::process_entropy(vector<vector<double>>& data, Tree* tmpNode)
{
	cout << "\n递归";
	show(data);
	vector<double> tmpClass = data.at(0);
	double classEntropy = class_entropy(tmpClass);

	Entropy.clear();
	IGR.clear();
	Entropy.push_back(classEntropy);


	for (int i = 1; i < data.size(); i++)
	{
		vector<double> tmp = data.at(i);//取出属性
		if (tmp.empty())
		{
			continue;
		}

		double tmpentropy = property_entropy(tmp, tmpClass);  //计算条件熵,记录纯的属性

	//	Entropy.push_back(tmpentropy);   //条件熵计算完成，保存

		double divergence = classEntropy - tmpentropy;  //计算信息增益

		double instrisic = instrisic_information(tmp);  //计算分裂属性度量

		double igr = Gain_rate(divergence, instrisic); //计算增益率

		IGR[i] = igr;
	}
	//构建树

		//挑选增益率最高的一类数据
	double max = -1;
	int index = -1;  //当前的属性类型
	map<int, double>::iterator IGRiter;
	for (IGRiter = IGR.begin(); IGRiter != IGR.end(); IGRiter++)
	{
		if (IGRiter->second > max)
		{
			max = IGRiter->second;
			index = IGRiter->first;
		}
	}

	//设置左节点



	set<double> cprc;
	map<double, map<double, vector<int>>> puremap = scan_Pure(index, data);  //记录了当前行的纯值情况
	map<double, map<double, vector<int>>>::iterator iter;
	int i = 0;
	for (iter = puremap.begin(); iter != puremap.end(); iter++, i++) //遍历当前类型的纯数据
	{

		double currentdata = iter->first; //当前属性值

		if (iter->second.size() == 1)  //只有一种map,也就是只有一种类型，纯类型数据
		{

			Tree* left = new Tree;
			left->dataIndex = index; //表示数据类型
			left->data = currentdata;  //因为只有一个数据，所以直接获取第一个数据
			left->isOvercast = true; //表示叶子节点
			left->result = iter->second.begin()->first;

			tmpNode->otherNode.push_back(left);

			switch (i)
			{
				cout << "\n";
			case 1:
			{
				cout << "天气 ";
				if (currentdata == 0)
				{
					cout << "晴天 ";
				}
				else if (currentdata == 2)
				{
					cout << "雨天 ";
				}
				else
				{
					cout << "阴天 ";
				}
				break;
			}
			case 2:
			{
				cout << "温度 ";
				if (currentdata == 0)
				{
					cout << "寒冷 ";
				}
				else if (currentdata == 1)
				{
					cout << "适中 ";
				}
				else
				{
					cout << "炎热 ";
				}
				break;
			}
			case 3:
			{
				cout << "湿度";
				if (currentdata == 1)
				{
					cout << "正常 ";
				}
				else if (currentdata == 2)
				{
					cout << "高 ";
				}
				break;
			}
			case 4:
			{
				cout << "风速";
				if (currentdata == 0)
				{
					cout << "弱 ";
				}
				else if (currentdata == 2)
				{
					cout << "强 ";
				}
				break;
			}
			}
			if (left->result == 0)
			{
				cout << "取消\n";
			}
			else
			{
				cout << "进行\n";
			}
			vector<int> tmpPureIndex = scan_Pure(index, currentdata, data);

			for (int i = 0; i < data.size(); i++) //所有数据中，删除纯数据，剩下的就是需要重新进行信息熵计算的数据
			{
				if (data.at(i).empty())
				{
					continue;
				}
				removeVectorElement(data.at(i), tmpPureIndex);
			}
		}
		else //如果不是纯数据
		{

			//挑选要分割的数据
			cprc.insert(currentdata);
			cout << "\n 非纯数据" << currentdata;
		}
	}

	//分割数据，递归处理不纯数据
	set<double>::iterator setiter;
	for (setiter = cprc.begin(); setiter != cprc.end(); setiter++)
	{
		Tree* node = new Tree;
		node->isOvercast = false;
		node->dataIndex = index;
		node->data = *setiter;
		tmpNode->otherNode.push_back(node);


		//	tmpNode->otherNode.push_back()
		vector<int> pos;
		for (int i = 0; i < data[index].size(); i++)
		{
			double cmp = data[index][i];
			if (cmp == *setiter)
			{
				pos.push_back(i);
			}
		}


		vector<vector<double>> cpdata;
		for (int i = 0; i < data.size(); i++)
		{
			vector<double> soncpdata;
			for (int j = 0; j < pos.size(); j++)
			{
				soncpdata.push_back(data[i][pos[j]]);
			}
			cpdata.push_back(soncpdata);
		}
		cpdata.at(index).clear();
		show(data);
		process_entropy(cpdata, node);
	}




	//	show(data);
}

double Decision_Tree::mathlog(double a, double x)
{
	return log(x) / log(a);
}

double Decision_Tree::class_entropy(vector<double> nums)
{
	//遍历出所有类型
	auto copyClass = nums;
	copyClass.clear(); //复制一个同样的结构

	map<double, double> tempClass;
	map<double, double>::iterator tempClassIter;
	for (auto it : nums)
	{
		tempClassIter = tempClass.find(it);
		if (tempClassIter != tempClass.end()) //找到之前存放的
		{
			tempClass[it] = tempClassIter->second + 1;
		}
		else  //找不到新建一个
		{
			tempClass[it] = 1;
		}
	}

	double result = 0;
	for (auto it : tempClass)
	{
		double left = it.second;
		double head = left / nums.size();
		head = head * mathlog(2, head);
		result += head;
	}
	return -result;
}

double Decision_Tree::property_entropy(vector<double>& nums, vector<double>& source)
{
	//遍历出所有类型
	auto copyClass = nums;
	copyClass.clear(); //复制一个同样的结构

	//属性，数量
	map<double, double> tempproperty;  //属性，数量
	map<double, map<double, double>>     tempproperty_RelatedClass;  //属性，<对应类别的属性，数量>

	map<double, double>::iterator temppropertyIter;
	map<double, map<double, double>>::iterator tempproperty_RelatedClassIter;

	for (int i = 0; i < nums.size(); i++)  //计算出所有数量
	{

		double it = nums[i];
		double src = source[i];
		temppropertyIter = tempproperty.find(it);  //计算有多少种数据
		if (temppropertyIter != tempproperty.end()) //找到之前存放的
		{
			tempproperty[it] = temppropertyIter->second + 1;  //数量递增


			map<double, double> tmp = tempproperty_RelatedClass[it];    //先获取旧对象

			double srctmp = tmp[src];
			if (srctmp < 0 || srctmp >= source.size()) //还没初始化
			{
				tmp[src] = 1;
			}
			else   //已经初始化了
			{
				tmp[src] = tmp[src] + 1;
			}
			//原基础上+1
			tempproperty_RelatedClass[it] = tmp;   //再赋回去
		//	pure[it] 
		}
		else  //找不到新建一个
		{
			map<double, double> map;
			map[src] = 1;
			tempproperty[it] = 1;
			tempproperty_RelatedClass[it] = map;  //纯
		}
	}

	double result = 0;
	for (auto it : tempproperty) //遍历元素
	{
		double left = it.second;
		double head = left / nums.size();

		double last = 0;
		map<double, double> map = tempproperty_RelatedClass[it.first];
		for (auto cl : map)
		{
			double mid = cl.second / left;
			mid = mid * mathlog(2, mid);

			last = last - (mid);
		}

		result += head * last;
	}

	return result;
}

double Decision_Tree::information_divergence(double information, double condition)
{
	return information - condition;
}

double Decision_Tree::Gain_rate(double divergence, double instrisic)
{
	return divergence / instrisic;
}

void Decision_Tree::show(vector<vector<double>>& data)
{
	cout << "\n";
	cout << "\n";
	for (auto it : data)
	{
		for (auto sonit : it)
		{
			cout << " " << sonit;
		}
		cout << "\n";
	}
}

vector<int> Decision_Tree::scan_Pure(int row, double cmp, vector<vector<double>>& data)
{
	vector<double> root = data[0];  //信息
	vector<double> rule = data[row]; //条件
	vector<int> nums;
	for (int i = 0; i < rule.size(); i++)
	{
		if (rule.empty())
		{
			continue;
		}
		if (rule[i] == cmp)
		{
			nums.push_back(i);
		}
	}

	return nums;
}

map<double, map<double, vector<int>>> Decision_Tree::scan_Pure(int row, vector<vector<double>>& data)
{

	vector<double> root = data[0];  //信息
	vector<double> rule = data[row]; //条件
	map<double, map<double, vector<int>>> mps;  //哪个属性，对应哪个类型，位置

	for (int i = 0; i < rule.size(); i++)
	{
		mps[rule[i]][root[i]].push_back(i);
	}

	return mps;
}

double Decision_Tree::instrisic_information(vector<double>& nums)
{
	auto copyClass = nums;
	copyClass.clear(); //复制一个同样的结构

	map<double, double> tempClass;
	map<double, double>::iterator tempClassIter;
	for (auto it : nums)
	{
		tempClassIter = tempClass.find(it);
		if (tempClassIter != tempClass.end()) //找到之前存放的
		{
			tempClass[it] = tempClassIter->second + 1;
		}
		else  //找不到新建一个
		{
			tempClass[it] = 1;
		}
	}

	double result = 0;
	for (auto it : tempClass)
	{
		double left = it.second;
		double head = left / nums.size();
		head = head * mathlog(2, head);
		result -= head;
	}
	return result;
}

template<class T>
inline void Decision_Tree::removeVectorElement(vector<T>& t, vector<int> index)
{
	sort(index.begin(), index.end());  //从小到大排序
	for (int i = 0; i < index.size(); i++)
	{
		int pos = index.at(i) - i;
		t.erase(t.begin() + pos);
	}
	cout << "\n ";
}
