// Information entropy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include "Decision_Tree.h"
using namespace std;


int main()
{
	//
	vector<double> clas = { 1,1,0,0,0,1,1,1,1,1,1,1,0,0 }; //决策
	vector<double> weat = { 0,0,0,0,0,1,1,1,1,2,2,2,2,2 }; //天气    0晴天，1阴天，2雨天
	vector<double> cold = { 0,1,2,2,1,2,0,1,2,1,0,1,0,1 }; //温度	 0寒冷，1适中，2炎热
	vector<double> humi = { 1,1,2,2,2,2,1,2,1,2,1,1,1,2 }; //湿度    1正常，2高
	vector<double> wind = { 0,2,0,2,0,0,2,2,0,0,0,0,2,2 }; //风速	 0弱，2强
	
	vector<vector<double>> data;   //合并数据到一个集合
	data.push_back(clas);
	data.push_back(weat);
	data.push_back(cold);
	data.push_back(humi);
	data.push_back(wind);
	Decision_Tree a(data);
	
	//测试数据    五位数据  保留，天气，温度，湿度，风速
	vector<double> test = { 0,2,0,1,2 };   //雨天，寒冷，正常，弱
	cout << "\n\n\ndouble " << a.check(test);
	system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
