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
	//�ж�
	if (inFile.fail())
	{
		cout << "��ȡ�ļ�ʧ��" << endl;
	}
	vector<string>temp(11);//���ڴ洢��ȡ����ÿ������
	dataHead = vector<string>(11);
	dataSet.resize(1000, temp);//���������С
	int row, column;
	row = 0;
	while (getline(inFile, linestr))//���ж�ȡ�ļ���ֱ��Ϊ��Ϊֹ
	{
		column = 0;
		stringstream ss(linestr);//��ɶ�ά��ṹ
		string str;//ÿ���еĵ����ַ�

		while (getline(ss, str, ','))
		{
			if (row == 0) {
				dataHead[column] = str.c_str();
			}
			else {
				dataSet[row-1][column] = str.c_str();//atof���ַ���תΪdouble����
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
	//���vector,�鿴���
	for (int i = 0; i < dataSet.size(); i++)
	{
		for (int j = 0; j < dataSet[i].size(); j++)
		{
			cout << dataSet[i][j] << " ";

		}
		cout << endl;
	}
}