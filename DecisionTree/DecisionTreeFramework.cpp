#include"Read.h"
#include<iostream>
#include <iomanip>
#include<map>
#include<set>
using namespace std;

class Tree_Node
{
public:
	//���캯������������Ϊ�����ݼ������ԣ������е����԰������ÿ�����Կ��ܵ�ȡֵ���ɵı�ʣ���δ�����������
	Tree_Node(map<string, vector<string>> temp_Table, vector<string> temp_Attribute, map<string, vector<string>> data_AttValues, vector<string> temp_remain);
	//�����ӽڵ�
	void generate_ChildNode();
	//������Ϣ���� Ѱ�����ŷ�������
	string findBestAttribute();
	//������Ϣ��
	double calc_Entropy(map<string, vector<string>> temp_Table);
	//���ýڵ������
	void set_NodeAttribute(string atttribute);
	//���Ը��������������
	int attribute_Num;
	//���ڵ������
	string node_Attribute;
	//���ݼ����ԣ������������飬��ſ��ܵ����ԣ��������
	vector<string> data_Attribute;//ALTERNATE	 BAR	 FRISAT	 HUNGRY	 PATRONS	 PRICE	 RAINING	 RESERVATION	 TYPE	 WAITESTIMATE	 WILLWAIT
	//���ڵ����������ֵ
	vector<string> node_AttValues;//���磺ALTERNATE = {0��1}
	//ʣ�����Լ��������ڵݹ�ʱȥ���ѷ�������
	vector<string>remain_Attributes;
	//����ָ�뼯�ϣ�Ϊ��˵���ýڵ�ΪҶ�ڵ㣬������Ϊ������
	map<string, Tree_Node*> childNode;
	//�������ϱ�ȫ������
	map<string, vector<string>> MyDateTable;
	//��������Զ�Ӧ������ֵ
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

//���ܺ�����ɾ��vector�����е�ĳһ��Ԫ��
vector<string> deleteElement(vector<string> data_array, string element) {
	for (vector<string>::iterator it = data_array.begin(); it != data_array.end(); )
	{
		if (*it == element)
		{
			it = data_array.erase(it); //����д��arr.erase(it);
		}
		else
		{
			++it;
		}
	}
	return data_array;
}

//������Ϣ��
double Tree_Node::calc_Entropy(map<string, vector<string>> temp_Table)
{
	map<string, vector<string>> table = temp_Table;
	//���ݼ�������������
	int sample_Num = (int)temp_Table[data_Attribute[0]].size();
	//�������ݼ��е��������
	map<string, int> class_Map;
	for (int i = 0; i < sample_Num; i++)
	{
		//data_Attribute[attribute_Num]��Ӧ�ľ������ݼ������
		string class_String = table[data_Attribute[attribute_Num]][i];
		class_Map[class_String]++;
	}

	map<string, int>::iterator it = class_Map.begin();
	//���������Ӧ������
	vector<int> n_Class;

	for (; it != class_Map.end(); it++)
	{
		//m_Class.push_back(it->first);
		n_Class.push_back(it->second);
	}
	//������Ϣ��
	double Ent = 0;
	for (int k = 0; k < class_Map.size(); k++)
	{
		//����
		double p = (double)n_Class[k] / sample_Num;
		if (p == 0)
		{
			//�涨��p=0ʱ��plogp=0
			continue;
		}
		Ent -= p * (log(p) / log(2));//��Ϣ��
	}

	return Ent;
}


//Ѱ�����ŷ���
string Tree_Node::findBestAttribute()
{
	//��������
	int N = (int)MyDateTable[data_Attribute[0]].size();
	//���ڴ����������
	string best_Attribute;
	//��Ϣ����
	double gain = 0;
	//��ÿ��ʣ������
	for (int i = 0; i < remain_Attributes.size(); i++)
	{
		//������Ϣ���棬ѡȡ�����������������༴Ϊ���ŷ���
		double temp_Gain = calc_Entropy(MyDateTable);//���ݹ�ʽ�Ƚ����ڵ����Ϣ�س�ʼ��������
		//�Ը����Ե����ݼ����з��ࣨ��ȡ������ֵ�������Ӽ���
		string temp_Att = remain_Attributes[i];//����ѡȡ������
		vector<string> remain_AttValues;//���Կ��ܵ�ȡֵ
		for (int j = 0; j < data_AttValues[temp_Att].size(); j++)
		{
			remain_AttValues.push_back(data_AttValues[temp_Att][j]);
		}

		//��ÿ������ֵ����Ϣ��
		for (int k = 0; k < remain_AttValues.size(); k++)
		{
			//����ֵ
			string temp_AttValues = remain_AttValues[k];
			int sample_Num = 0;//������ֵ��Ӧ��������
			//����map������Ÿ�����ֵ�µ������Ӽ�
			map<string, vector<string>>sub_DataTable;
			for (int l = 0; l < MyDateTable[temp_Att].size(); l++)
			{
				if (temp_AttValues == MyDateTable[temp_Att][l])
				{
					sample_Num++;
					//������������ѵ��������sub_DataTable
					for (int m = 0; m < data_Attribute.size(); m++)
					{
						sub_DataTable[data_Attribute[m]].push_back(MyDateTable[data_Attribute[m]][l]);
					}
				}
			}
			//�ۼ�ÿ����ֵ����Ϣ��
			temp_Gain -= (double)sample_Num / N * calc_Entropy(sub_DataTable);
		}
		//�Ƚ�Ѱ�����ŷ�������
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
	//���ýڵ������
	this->node_Attribute = attribute;
}

void Tree_Node::generate_ChildNode()
{
	//��������
	int N = (int)MyDateTable[data_Attribute[0]].size();

	//�����ݼ�������������������map���棬ֻ���ж����һ�м���
	map<string, int> category;
	for (int i = 0; i < N; i++)
	{
		vector<string> temp_Class;
		temp_Class = MyDateTable[data_Attribute[attribute_Num]];
		category[temp_Class[i]]++;
	}

	//��һ�����
	//ֻ��һ����𣬱��ΪҶ�ڵ�
	if (1 == category.size())
	{
		map<string, int>::iterator it = category.begin();
		node_Attribute = it->first;
		return;
	}

	//�ڶ������
	//��remian_attributes�з�����������
	string best_Attribute = findBestAttribute();
	//�����ڵ�����Ϊ��������
	node_Attribute = best_Attribute;
	//���������Ե�ÿ������ֵ
	for (int i = 0; i < data_AttValues[best_Attribute].size(); i++)
	{
		string best_AttValues = data_AttValues[best_Attribute][i];
		//����map������Ÿ�����ֵ�µ������Ӽ�
		map<string, vector<string>> sub_DataTable;
		for (int j = 0; j < MyDateTable[best_Attribute].size(); j++)
		{
			//Ѱ���������������ݼ�������ֵ��ͬ����������
			if (best_AttValues == MyDateTable[best_Attribute][j])
			{
				//�ҵ���Ӧ�����ݼ��������Ӽ���sub_DataTable����������ȫ�����Զ�Ҫ���룩
				for (int k = 0; k < data_Attribute.size(); k++)
				{
					sub_DataTable[data_Attribute[k]].push_back(MyDateTable[data_Attribute[k]][j]);
				}
			}
		}
		//���Ӽ�Ϊ�գ�����֧�ڵ㣨�ӽڵ㣩���ΪҶ�ڵ㣬���ΪMyDateTable����������
		if (sub_DataTable.empty())
		{
			//�����ӽڵ�
			Tree_Node* p = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, remain_Attributes);
			//�ҳ����������࣬��Ϊ�ӽڵ������
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
			//������Ҷ�ڵ�����
			//p->set_NodeAttribute(childNode_Attribute);
			p->set_NodeAttribute("0");
			//���ӽڵ����childNode��Ԥ��������ʱ����õ�
			childNode[best_AttValues] = p;
		}
		else//����Ϊ�գ����ʣ������ֵ�޳������ԣ�����generate_ChildNode��������ϸ��
		{
			vector<string> child_RemainAtt;
			child_RemainAtt = remain_Attributes;
			child_RemainAtt = deleteElement(child_RemainAtt, best_Attribute);

			//�����ӽڵ�
			Tree_Node* pt = new Tree_Node(sub_DataTable, data_Attribute, data_AttValues, child_RemainAtt);
			//���ӽڵ����childNode
			childNode[best_AttValues] = pt;
			//�ӽڵ��ٵ���generate_ChildNode����
			pt->generate_ChildNode();
		}
	}

}

//����Ϊ��Ԥ���������������Լ���
string findClass(vector<string> attributes, Tree_Node a)
{
	//Ѱ�ҷ������ַ���
	for (int i = 0; i < a.data_AttValues[a.data_Attribute.back()].size(); ++i)
	{
		if (a.node_Attribute == a.data_AttValues[a.data_Attribute.back()][i])
			return a.node_Attribute;
	}
	int x = 0;
	//�ҵ���ǰ�ڵ����Եı��
	for (int i = 0; i < a.data_Attribute.size(); ++i)
	{
		if (a.node_Attribute == a.data_Attribute[i])
		{
			x = i;
			break;
		}
	}
	//�ݹ�����
	return findClass(attributes, *a.childNode[attributes[x]]);
}

/*
������ �����������
���룺 ���Լ�������顢����������������
����� ����׼ȷ��
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

//��Read data��ʼ�����ڵ�
Tree_Node Input(Read data)
{
	//���������б�
	vector<string> Attributes = data.dataHead;
	//��ʱ��������б����ڴ�����ʼʣ�������б�����������
	vector<string> Temp_Attributes = Attributes;
	Temp_Attributes.pop_back();
	vector<string> remain = Temp_Attributes;
	int N = data.dataSet.size();
	//��ʼ��ȫ�����ݱ�
	map<string, vector<string>>Table;
	for (int j = 0; j < N; j++)
	{
		//��ȡһ������ֵ����
		vector<string> AttValues = data.dataSet[j];
		//��������ֵ���뵽���ݱ�Table��
		for (int i = 0; i < Attributes.size(); i++)
		{
			Table[Attributes[i]].push_back(AttValues[i]);
		}
	}
	//��ʼ�����Խ���б�
	map<string, vector<string>> AttValues;
	for (int i = 0; i < Attributes.size(); i++)
	{
		set<string> attValues;
		for (int j = 0; j < N; j++)
		{
			string temp = Table[Attributes[i]][j];
			//�����ظ�����ֵ��set�ǲ�������
			attValues.insert(temp);
		}
		for (set<string>::iterator it = attValues.begin(); it != attValues.end(); it++)
		{
			//�����п��ܵ�����ֵ����AttValues[Attributes[i]]
			AttValues[Attributes[i]].push_back(*it);
		}

	}
	return Tree_Node(Table, Attributes, AttValues, remain);

}


int main() {

	//���ݼ�������
	vector<string>  Data_attributes;
	string train_file_name = "./restaurant_willwait_dataset/restaurant_willwait/restaurant_willwait_train.csv";
	//��ȡѵ�����ݼ�����
	Read train_data = Read(train_file_name);
	//��ȡ���ݼ��������б�
	Data_attributes = train_data.dataHead;
	//��ȡ���ݼ���ѵ������
	vector<vector<string>> train_data_table = train_data.dataSet;
	//�������ݼ�
	vector<vector<string>> test_samples; 

	//�빹����������Բ������ݽ����жϣ����������ӽ�classifyRes�����С�

	Tree_Node DecisionTree = Input(train_data);
	DecisionTree.generate_ChildNode();

	//���Լ�����ȷ���������
	vector<int> test_result;
	//������
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
	cout << "׼ȷ�ʣ�" << fixed << setprecision(2) << res << endl;
	cout << "--------------------------end------------------------------" << endl;

	return 0;
}