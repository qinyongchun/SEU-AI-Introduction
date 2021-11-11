#include"Read.h"
#include<iostream>
#include <iomanip>
#include<map>
#include<set>
using namespace std;

class Tree_Node
{
public:
	//构造函数，参数依次为：数据集表（属性）、所有的属性包括类别、每个属性可能的取值构成的表、剩余的未被分类的属性
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain);
	//生成子节点
	void generate_ChildNode();
	//计算信息增益 寻找最优分类属性
	string findBestAttribute();
	//计算信息熵
	double calc_Entropy(map<string, vector<string>> temp_Table);
	//设置节点的属性
	void set_NodeAttribute(string atttribute);
	//属性个数，不包括类别
	int attribute_Num;
	//本节点的属性
	string node_Attribute;
	//数据集属性，定义属性数组，存放可能的属性，包括类别
	vector<string> data_Attribute;//ALTERNATE	 BAR	 FRISAT	 HUNGRY	 PATRONS	 PRICE	 RAINING	 RESERVATION	 TYPE	 WAITESTIMATE	 WILLWAIT
	//本节点的所有属性值
	vector<string> node_AttValues;//比如：ALTERNATE = {0，1}
	//剩余属性集，用于在递归时去除已分类属性
	vector<string>remain_Attributes;
	//子树指针集合，为空说明该节点为叶节点，其属性为分类结果
	map<string, Tree_Node*> childNode;
	//样本集合表，全体数据
	map<string, vector<string>> MyDateTable;
	//定义各属性对应的属性值
	map<string, vector<string>> data_AttValues;
};


Tree_Node::Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain)
{
	data_Attribute = temp_Attribute;
	attribute_Num = (int)temp_Attribute.size() - 1;
	this->data_AttValues = data_AttValues;
	MyDateTable = temp_Table;
	remain_Attributes = temp_remain;
}

//功能函数：删除vector数组中的某一个元素
vector<string> deleteElement(vector<string> data_array, string element) {
	for (vector<string>::iterator it = data_array.begin(); it != data_array.end(); )
	{
		if (*it == element)
		{
			it = data_array.erase(it); //不能写成arr.erase(it);
		}
		else
		{
			++it;
		}
	}
	return data_array;
}

//计算信息熵
double Tree_Node::calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//数据集中样本的数量
	int sample_Num = (int)temp_Table[data_Attribute[0]].size();
	//计算数据集中的类别数量
	map<string, int> class_Map;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]对应的就是数据集的类别
		string class_String = table[data_Attribute[attribute_Num]][i];
		class_Map[class_String]++;
	}

	map<string, int>::iterator it = class_Map.begin();
	//存放类别及其对应的数量
	vector<int> n_Class;

	for (; it != class_Map.end(); it++)
	{
		//m_Class.push_back(it->first);
		n_Class.push_back(it->second);
	}
	//计算信息熵
	double Ent = 0;
	for (int k = 0; k < class_Map.size(); k++)
	{
		//比例
		double p = (double)n_Class[k] / sample_Num;
		if (p == 0)
		{
			//规定了p=0时，plogp=0
			continue;
		}
		Ent -= p * (log(p) / log(2));//信息熵
	}

	return Ent;
}


//寻找最优分类
string Tree_Node::findBestAttribute()
{
	//样本个数
	int N = (int)MyDateTable[data_Attribute[0]].size();
	//用于存放最优属性
	string best_Attribute;
	//信息增益
	double gain = 0;
	//对每个剩余属性
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		//定义信息增益，选取增益最大的属性来分类即为最优分类
		double temp_Gain = calc_Entropy(MyDateTable);//根据公式先将本节点的信息熵初始化给增益
		//对该属性的数据集进行分类（获取各属性值的数据子集）
		string temp_Att = remain_Attributes[i];//假设选取的属性
		vector<string> remain_AttValues;//属性可能的取值
		for (int j = 0; j < data_AttValues[temp_Att].size(); j++)
		{
			remain_AttValues.push_back(data_AttValues[temp_Att][j]);
		}

		//对每个属性值求信息熵
		for (int k = 0; k < remain_AttValues.size(); k++)
		{
			//属性值
			string temp_AttValues = remain_AttValues[k];
			int sample_Num = 0;//该属性值对应样本数量
			//定义map用来存放该属性值下的数据子集
			map<string, vector<string>>sub_DataTable;
			for (int l = 0; l < MyDateTable[temp_Att].size(); l++)
			{
				if (temp_AttValues == MyDateTable[temp_Att][l])
				{
					sample_Num++;
					//将符合条件的训练集存入sub_DataTable
					for (int m = 0; m < data_Attribute.size(); m++)
					{
						sub_DataTable[data_Attribute[m]].push_back(MyDateTable[data_Attribute[m]][l]);
					}
				}
			}
			//累加每个属值的信息熵
			temp_Gain -= (double)sample_Num / N * calc_Entropy(sub_DataTable);
		}
		//比较寻找最优分类属性
		if (temp_Gain > gain)
		{
			gain = temp_Gain;
			best_Attribute = temp_Att;
		}
	}

	return best_Attribute;
}

void Tree_Node::set_NodeAttribute(string attribute)
{
	//设置节点的属性
	this->node_Attribute = attribute;
}

void Tree_Node::generate_ChildNode()
{
	//样本个数
	int N = (int)MyDateTable[data_Attribute[0]].size();

	//将数据集中类别种类和数量放入map里面，只需判断最后一列即可
	map<string, int> category;
	for (int i = 0; i < N; i++)
	{
		vector<string> temp_Class;
		temp_Class = MyDateTable[data_Attribute[attribute_Num]];
		category[temp_Class[i]]++;
	}

	//第一种情况
	//只有一个类别，标记为叶节点
	if (1 == category.size())
	{
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		return;
	}

	//第二种情况
	//从remian_attributes中分类最优属性
	string best_Attribute = findBestAttribute();
	//将本节点设置为最优属性
	node_Attribute = best_Attribute;
	//对最优属性的每个属性值
	for (int i = 0; i < data_AttValues[best_Attribute].size(); i++)
	{
		string best_AttValues = data_AttValues[best_Attribute][i];
		//定义map用来存放该属性值下的数据子集
		map<string, vector<string>> sub_DataTable;
		for (int j = 0; j < MyDateTable[best_Attribute].size(); j++)
		{
			//寻找最优属性在数据集中属性值相同的数据样本
			if (best_AttValues == MyDateTable[best_Attribute][j])
			{
				//找到对应的数据集，存入子集中sub_DataTable（该样本的全部属性都要存入）
				for (int k = 0; k < data_Attribute.size(); k++)
				{
					sub_DataTable[data_Attribute[k]].push_back(MyDateTable[data_Attribute[k]][j]);
				}
			}
		}
		//若子集为空，将分支节点（子节点）标记为叶节点，类别为MyDateTable样本最多的类
		if (sub_DataTable.empty())
		{
			//生成子节点
			Tree_Node* p = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, remain_Attributes);
			//找出样本最多的类，作为子节点的属性
			map<string, int>::iterator it = category.begin();
			string childNode_Attribute = it->first;
			int max_Num = it->second;
			it++;
			for (; it != category.end(); it++)
			{
				if (it->second > max_Num)
				{
					max_Num = it->second;
					childNode_Attribute = it->first;
				}
			}
			//设置子叶节点属性
			//p->set_NodeAttribute(childNode_Attribute);
			p->set_NodeAttribute("0");
			//将子节点存入childNode，预测样本的时候会用到
			childNode[best_AttValues] = p;
		}
		else//若不为空，则从剩余属性值剔除该属性，调用generate_ChildNode继续往下细分
		{
			vector<string> child_RemainAtt;
			child_RemainAtt = remain_Attributes;
			child_RemainAtt = deleteElement(child_RemainAtt, best_Attribute);

			//生成子节点
			Tree_Node* pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//将子节点存入childNode
			childNode[best_AttValues] = pt;
			//子节点再调用generate_ChildNode函数
			pt->generate_ChildNode();
		}
	}

}

//输入为待预测样本的所有属性集合
string findClass(vector<string> attributes, Tree_Node a)
{
	//寻找分类结果字符串
	for (int i = 0; i < a.data_AttValues[a.data_Attribute.back()].size(); ++i)
	{
		if (a.node_Attribute == a.data_AttValues[a.data_Attribute.back()][i])
			return a.node_Attribute;
	}
	int x = 0;
	//找到当前节点属性的编号
	for (int i = 0; i < a.data_Attribute.size(); ++i)
	{
		if (a.node_Attribute == a.data_Attribute[i])
		{
			x = i;
			break;
		}
	}
	//递归搜索
	return findClass(attributes, *a.childNode[attributes[x]]);
}

/*
描述： 计算评测分数
输入： 测试集类别数组、决策树分类结果数组
输出： 分类准确率
*/
double evaluatScore(vector<int> test_result, vector<int> classify_res) {
	int correct_num = 0;
	for (int i = 0; i < test_result.size(); i++)
	{

		if (classify_res[i] == test_result[i]) {
			correct_num++;
		}
	}
	double sample_size = test_result.size();
	double res = correct_num / sample_size;
	return res;
}

//从Read data初始化根节点
Tree_Node Input(Read data)
{
	//所有属性列表
	vector<string> Attributes = data.dataHead;
	//临时存放属性列表，用于创建初始剩余属性列表（不包含结果项）
	vector<string> Temp_Attributes = Attributes;
	Temp_Attributes.pop_back();
	vector<string> remain = Temp_Attributes;
	int N = data.dataSet.size();
	//初始化全体数据表
	map<string, vector<string>>Table;
	for (int j = 0; j < N; j++)
	{
		//获取一行属性值输入
		vector<string> AttValues = data.dataSet[j];
		//将各属性值输入到数据表Table中
		for (int i = 0; i < Attributes.size(); i++)
		{
			Table[Attributes[i]].push_back(AttValues[i]);
		}
	}
	//初始化属性结果列表
	map<string, vector<string>> AttValues;
	for (int i = 0; i < Attributes.size(); i++)
	{
		set<string> attValues;
		for (int j = 0; j < N; j++)
		{
			string temp = Table[Attributes[i]][j];
			//若有重复属性值，set是不会插入的
			attValues.insert(temp);
		}
		for (set<string>::iterator it = attValues.begin(); it != attValues.end(); it++)
		{
			//将所有可能的属性值存入AttValues[Attributes[i]]
			AttValues[Attributes[i]].push_back(*it);
		}

	}
	return Tree_Node(Table, Attributes, AttValues, remain);

}


int main() {

	//数据集的属性
	vector<string>  Data_attributes;
	string train_file_name = "./restaurant_willwait_dataset/restaurant_willwait/restaurant_willwait_train.csv";
	//获取训练数据集对象
	Read train_data = Read(train_file_name);
	//获取数据集的属性列表
	Data_attributes = train_data.dataHead;
	//获取数据集的训练数据
	vector<vector<string>> train_data_table = train_data.dataSet;
	//测试数据集
	vector<vector<string>> test_samples; 

	//请构造决策树并对测试数据进行判断，并将结果添加进classifyRes数组中。

	Tree_Node DecisionTree = Input(train_data);
	DecisionTree.generate_ChildNode();

	//测试集合正确的类别数组
	vector<int> test_result;
	//分类结果
	vector<int> classify_res;

	string test_file_name = "./restaurant_willwait_dataset/restaurant_willwait/restaurant_willwait_test.csv";
	Read test_data = Read(test_file_name);
	int N = test_data.dataSet.size();
	for (int i = 0; i < N; ++i)
	{
		vector<string> Test = test_data.dataSet[i];
		string last = Test.back();
		test_result.push_back(atoi(last.c_str()));
		Test.pop_back();
		string temp = findClass(Test, DecisionTree);
		int result = atoi(temp.c_str());
		classify_res.push_back(result);
	}

	
	cout << "--------------------------result------------------------------" << endl;
	double res = evaluatScore(test_result, classify_res);
	cout << "准确率：" << fixed << setprecision(2) << res << endl;
	cout << "--------------------------end------------------------------" << endl;

	return 0;
}