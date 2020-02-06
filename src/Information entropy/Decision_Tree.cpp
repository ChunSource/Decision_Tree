#include "Decision_Tree.h"

Decision_Tree::Decision_Tree(vector<vector<double>> data)
{

	this->rootNode = new Tree;
	process_entropy(data, rootNode);
	//Ŀǰλ�õ�һ����Ϣ�ؼ�������������Ѿ�û���κ������ˣ�2020.2.3-2020.2.4
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

		if (it->data == data[index])  //�ж������Ƿ��Ӧ����
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
	cout << "\n�ݹ�";
	show(data);
	vector<double> tmpClass = data.at(0);
	double classEntropy = class_entropy(tmpClass);

	Entropy.clear();
	IGR.clear();
	Entropy.push_back(classEntropy);


	for (int i = 1; i < data.size(); i++)
	{
		vector<double> tmp = data.at(i);//ȡ������
		if (tmp.empty())
		{
			continue;
		}

		double tmpentropy = property_entropy(tmp, tmpClass);  //����������,��¼��������

	//	Entropy.push_back(tmpentropy);   //�����ؼ�����ɣ�����

		double divergence = classEntropy - tmpentropy;  //������Ϣ����

		double instrisic = instrisic_information(tmp);  //����������Զ���

		double igr = Gain_rate(divergence, instrisic); //����������

		IGR[i] = igr;
	}
	//������

		//��ѡ��������ߵ�һ������
	double max = -1;
	int index = -1;  //��ǰ����������
	map<int, double>::iterator IGRiter;
	for (IGRiter = IGR.begin(); IGRiter != IGR.end(); IGRiter++)
	{
		if (IGRiter->second > max)
		{
			max = IGRiter->second;
			index = IGRiter->first;
		}
	}

	//������ڵ�



	set<double> cprc;
	map<double, map<double, vector<int>>> puremap = scan_Pure(index, data);  //��¼�˵�ǰ�еĴ�ֵ���
	map<double, map<double, vector<int>>>::iterator iter;
	int i = 0;
	for (iter = puremap.begin(); iter != puremap.end(); iter++, i++) //������ǰ���͵Ĵ�����
	{

		double currentdata = iter->first; //��ǰ����ֵ

		if (iter->second.size() == 1)  //ֻ��һ��map,Ҳ����ֻ��һ�����ͣ�����������
		{

			Tree* left = new Tree;
			left->dataIndex = index; //��ʾ��������
			left->data = currentdata;  //��Ϊֻ��һ�����ݣ�����ֱ�ӻ�ȡ��һ������
			left->isOvercast = true; //��ʾҶ�ӽڵ�
			left->result = iter->second.begin()->first;

			tmpNode->otherNode.push_back(left);

			switch (i)
			{
				cout << "\n";
			case 1:
			{
				cout << "���� ";
				if (currentdata == 0)
				{
					cout << "���� ";
				}
				else if (currentdata == 2)
				{
					cout << "���� ";
				}
				else
				{
					cout << "���� ";
				}
				break;
			}
			case 2:
			{
				cout << "�¶� ";
				if (currentdata == 0)
				{
					cout << "���� ";
				}
				else if (currentdata == 1)
				{
					cout << "���� ";
				}
				else
				{
					cout << "���� ";
				}
				break;
			}
			case 3:
			{
				cout << "ʪ��";
				if (currentdata == 1)
				{
					cout << "���� ";
				}
				else if (currentdata == 2)
				{
					cout << "�� ";
				}
				break;
			}
			case 4:
			{
				cout << "����";
				if (currentdata == 0)
				{
					cout << "�� ";
				}
				else if (currentdata == 2)
				{
					cout << "ǿ ";
				}
				break;
			}
			}
			if (left->result == 0)
			{
				cout << "ȡ��\n";
			}
			else
			{
				cout << "����\n";
			}
			vector<int> tmpPureIndex = scan_Pure(index, currentdata, data);

			for (int i = 0; i < data.size(); i++) //���������У�ɾ�������ݣ�ʣ�µľ�����Ҫ���½�����Ϣ�ؼ��������
			{
				if (data.at(i).empty())
				{
					continue;
				}
				removeVectorElement(data.at(i), tmpPureIndex);
			}
		}
		else //������Ǵ�����
		{

			//��ѡҪ�ָ������
			cprc.insert(currentdata);
			cout << "\n �Ǵ�����" << currentdata;
		}
	}

	//�ָ����ݣ��ݹ鴦��������
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
	//��������������
	auto copyClass = nums;
	copyClass.clear(); //����һ��ͬ���Ľṹ

	map<double, double> tempClass;
	map<double, double>::iterator tempClassIter;
	for (auto it : nums)
	{
		tempClassIter = tempClass.find(it);
		if (tempClassIter != tempClass.end()) //�ҵ�֮ǰ��ŵ�
		{
			tempClass[it] = tempClassIter->second + 1;
		}
		else  //�Ҳ����½�һ��
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
	//��������������
	auto copyClass = nums;
	copyClass.clear(); //����һ��ͬ���Ľṹ

	//���ԣ�����
	map<double, double> tempproperty;  //���ԣ�����
	map<double, map<double, double>>     tempproperty_RelatedClass;  //���ԣ�<��Ӧ�������ԣ�����>

	map<double, double>::iterator temppropertyIter;
	map<double, map<double, double>>::iterator tempproperty_RelatedClassIter;

	for (int i = 0; i < nums.size(); i++)  //�������������
	{

		double it = nums[i];
		double src = source[i];
		temppropertyIter = tempproperty.find(it);  //�����ж���������
		if (temppropertyIter != tempproperty.end()) //�ҵ�֮ǰ��ŵ�
		{
			tempproperty[it] = temppropertyIter->second + 1;  //��������


			map<double, double> tmp = tempproperty_RelatedClass[it];    //�Ȼ�ȡ�ɶ���

			double srctmp = tmp[src];
			if (srctmp < 0 || srctmp >= source.size()) //��û��ʼ��
			{
				tmp[src] = 1;
			}
			else   //�Ѿ���ʼ����
			{
				tmp[src] = tmp[src] + 1;
			}
			//ԭ������+1
			tempproperty_RelatedClass[it] = tmp;   //�ٸ���ȥ
		//	pure[it] 
		}
		else  //�Ҳ����½�һ��
		{
			map<double, double> map;
			map[src] = 1;
			tempproperty[it] = 1;
			tempproperty_RelatedClass[it] = map;  //��
		}
	}

	double result = 0;
	for (auto it : tempproperty) //����Ԫ��
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
	vector<double> root = data[0];  //��Ϣ
	vector<double> rule = data[row]; //����
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

	vector<double> root = data[0];  //��Ϣ
	vector<double> rule = data[row]; //����
	map<double, map<double, vector<int>>> mps;  //�ĸ����ԣ���Ӧ�ĸ����ͣ�λ��

	for (int i = 0; i < rule.size(); i++)
	{
		mps[rule[i]][root[i]].push_back(i);
	}

	return mps;
}

double Decision_Tree::instrisic_information(vector<double>& nums)
{
	auto copyClass = nums;
	copyClass.clear(); //����һ��ͬ���Ľṹ

	map<double, double> tempClass;
	map<double, double>::iterator tempClassIter;
	for (auto it : nums)
	{
		tempClassIter = tempClass.find(it);
		if (tempClassIter != tempClass.end()) //�ҵ�֮ǰ��ŵ�
		{
			tempClass[it] = tempClassIter->second + 1;
		}
		else  //�Ҳ����½�һ��
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
	sort(index.begin(), index.end());  //��С��������
	for (int i = 0; i < index.size(); i++)
	{
		int pos = index.at(i) - i;
		t.erase(t.begin() + pos);
	}
	cout << "\n ";
}
