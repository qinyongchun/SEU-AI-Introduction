#pragma once
#include<vector>
#include<string>
using namespace std;

class Read
{
public:
	vector<vector<string>> dataSet; //��Ϊ��Ҫ��ȡ���Ե�Ƕ������
	vector<string> dataHead;
	Read(string filename);
};