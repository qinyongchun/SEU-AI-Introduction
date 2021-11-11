#pragma once
#include<vector>
#include<string>
using namespace std;

class Read
{
public:
	vector<vector<string>> dataSet; //因为需要获取属性的嵌套数组
	vector<string> dataHead;
	Read(string filename);
};