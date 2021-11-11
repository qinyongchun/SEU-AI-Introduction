#include"Read.h"
#include"TreeNode.h"
#include <fstream>  
#include <sstream>  
#include <iostream> 
#include <vector> 
#include<string>
#include<map>
#include<set>
using namespace std;

//int N;
//string basic[10][4] = { { "1","0" } ,{ "1","0" } ,{ "1","0" } ,{ "1","0" } ,{ "0" ,"0.5" ,"1" }, { "0" ,"0.5" ,"1" },{ "1","0" },{ "1","0" }, { "1" ,"0.67", "0.33", "0" },{ "0" ,"0.17", "0.5" ,"1" } };
//TreeNode::TreeNode(string name)
//{
//	attribute = name;
//	if (name == "ALTERNATE")
//	{
//		number = 1;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "BAR")
//	{
//		number = 2;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "FRI/SAT")
//	{
//		number = 3;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "HUNGRY")
//	{
//		number = 4;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "PATRONS")
//	{
//		number = 5;
//		count = 3;
//		value = { "0" ,"0.5" ,"1" };
//	}
//	else if (name == "PRICE")
//	{
//		number = 6;
//		count = 3;
//		value = { "0" ,"0.5" ,"1" };
//	}
//	else if (name == "RAINING")
//	{
//		number = 7;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "RESERVATION")
//	{
//		number = 8;
//		count = 2;
//		value = { "1","0" };
//	}
//	else if (name == "TYPE")
//	{
//		number = 9;
//		count = 4;
//		value = { "1" ,"0.67", "0.33", "0" };
//	}
//	else //WAITESTIMATE
//	{
//		number = 10;
//		count = 4;
//		value = { "0" ,"0.17", "0.5" ,"1" };
//	}
//	count1.resize(count,-1);
//	count0.resize(count,-1);
//	judge.resize(count,-1);
//}
//
//Node::Node(int num)
//{
//	this->number = num;
//}
//
//void InitNode(TreeNode A,Read Data_train)
//{
//	N = Data_train.dataSet.size();
//	for (int i = 0; i < N; ++i)//第i个数据
//	{
//		if (Data_train.dataSet[10][i] == "1" )//正例
//		{
//			string temp = Data_train.dataSet[A.number-1][i];
//			for (int k = 0; k < A.count; ++k)
//			{
//				if (A.value[k] == temp)
//					A.count1[k]++;
//			}
//		}
//		else//反例
//		{
//			string temp = Data_train.dataSet[A.number-1][i];
//			for (int k = 0; k < A.count; ++k)
//			{
//				if (A.value[k] == temp)
//					A.count0[k]++;
//			}
//
//		}
//	}
//	for (int i = 0; i < A.count; ++i)
//	{
//		if (A.count1[i] == 0 && A.count0[i] != 0)
//			A.judge[i] = 0;
//		if (A.count1[i] != 0 && A.count0[i] == 0)
//			A.judge[i] = 1;
//	}
//	return;
//}
//
//double B_function(double q)
//{
//	double B = -q * (log(q) / log(2)) - (1 - q) * (log(1 - q) / log(2));
//	return B;
//}
//
////void Importance(TreeNode A)
////{
////	double importance = 0;
////	for (int i = 0; i < A.count; i++)
////	{
////		double p = A.count1[i];
////		double n = A.count0[i];
////		importance += (p + n) * B_function(p / (p + n)) / N;
////	}
////	A.importance = importance;
////	return;
////}
//
//int Number_Count(int number)
//{
//	if (number == 0 || number == 1 || number == 2 || number == 3|| number == 6|| number == 7)
//	{
//		return 2;
//	}
//	else if (number == 4|| number == 5)
//	{
//		return 3;
//	}
//	else
//	{
//		return 4;
//	}
//
//}
//
//int Name_Number(string name)
//{
//	int number = 0;
//	if (name == "ALTERNATE")
//	{
//		number = 1;
//	}
//	else if (name == "BAR")
//	{
//		number = 2;
//	}
//	else if (name == "FRI/SAT")
//	{
//		number = 3;
//	}
//	else if (name == "HUNGRY")
//	{
//		number = 4;
//	}
//	else if (name == "PATRONS")
//	{
//		number = 5;
//	}
//	else if (name == "PRICE")
//	{
//		number = 6;
//	}
//	else if (name == "RAINING")
//	{
//		number = 7;
//	}
//	else if (name == "RESERVATION")
//	{
//		number = 8;
//	}
//	else if (name == "TYPE")
//	{
//		number = 9;
//	}
//	else //WAITESTIMATE
//	{
//		number = 10;
//	}
//	return number - 1;
//}
//
//double Importance(int number, Read data)//对于第number个属性返回其信息熵减, number=0:9
//{
//	double importance = 0;
//	for (int k = 0; k < Number_Count(number); ++k)
//	{
//		int p = 0;//正例
//		int n = 0;//反例
//		for (int i = 0; i < data.dataSet.size(); i++)
//		{
//			if (data.dataSet[i][10] == "1" && data.dataSet[i][number] == basic[number][k])
//				p++;
//			if (data.dataSet[i][10] == "0" && data.dataSet[i][number] == basic[number][k])
//				n++;
//		}
//		importance += (p + n) * B_function(p / (p + n)) / N;
//	}
//
//	return 1- importance;
//
//}
//
//int plurality_value(Read example)
//{
//	int p = 0;//正例
//	int n = 0;//反例
//	for (int i = 0; i < example.dataSet.size(); ++i)
//	{
//		if (example.dataSet[i][10] == "1")
//			p++;
//		else n++;
//	}
//	if (p >= n) return 1;
//	else return 0;
//}

