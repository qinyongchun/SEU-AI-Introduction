#include"Read.h"
#include <fstream>  
#include <sstream>  
#include <iostream> 
#include <vector> 
using namespace std;

Read::Read(string filename)
{
	ifstream inFile(filename);
	string linestr;
	//判断
	if (inFile.fail())
	{
		cout << "读取文件失败" << endl;
	}
	vector<string>temp(11);//用于存储读取到的每行数据
	dataHead = vector<string>(11);
	dataSet.resize(1000, temp);//重置数组大小
	int row, column;
	row = 0;
	while (getline(inFile, linestr))//逐行读取文件，直到为空为止
	{
		column = 0;
		stringstream ss(linestr);//存成二维表结构
		string str;//每行中的单个字符

		while (getline(ss, str, ','))
		{
			if (row == 0) {
				dataHead[column] = str.c_str();
			}
			else {
				dataSet[row-1][column] = str.c_str();//atof将字符串转为double类型
			}
			column++;

		}
		row++;
	}

	for (int i = 0; i < dataHead.size(); i++)
	{
		cout << dataHead[i] << "\t";
	}
	cout << endl;
	vector<string>resizeData(column);
	dataSet.resize(row-1, resizeData);
	//输出vector,查看结果
	for (int i = 0; i < dataSet.size(); i++)
	{
		for (int j = 0; j < dataSet[i].size(); j++)
		{
			cout << dataSet[i][j] << " ";

		}
		cout << endl;
	}
}