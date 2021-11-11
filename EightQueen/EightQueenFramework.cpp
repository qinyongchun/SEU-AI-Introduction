#include<iostream>
#include<vector>
#include"EightQueenNode.h"
#include<ctime>
#include<Windows.h>
#define N 10000

using namespace std;

//����ʺ�����
const int queenNum = 8;

//��ͻ������
static int checkNum = 0;

//��Żʺ�����飬�±�Ϊ������ȡֵΪ����
static int* queenColumn = new int[queenNum];

//����������
static int maxRoundNum = 100;

/*
���룺��Żʺ�����飬�±�Ϊ������ȡֵΪ����
����ֵ�� /
������ִ�д�ӡ���ܣ���ӡ���ʺ����յİڷ�λ�á�
*/
void printResult(vector<int> eightQueenColumnResult, int checkNum);

/*
* ���룺
* �������Żʺ�����飬�±�Ϊ������ȡֵΪ����
*����ڷŰ˸��ʺ��λ�ã���֤����ͬһ��ͬһ��
*/
vector<int> randomInitQueenColumn();

/*
*���룺��Żʺ������
*�����true:��ȷ��� false:������
*/
bool checkQueenColumn(vector<int> queenColumn);

/*
*Լ����������Ļ����㷨���ҵ�һ��������ɣ�
*/
vector<int> bactrackDFS() {
	/*
	    �ڴ�ʵ�ְ˻ʺ�����Ļ����㷨�����ں�����������printResult������ӡ�����
	*/
	
	vector<int> initqueenlist = randomInitQueenColumn();
	int row = 1;//��ǰ����
	checkNum = 0;
	while(!checkQueenColumn(initqueenlist))//��δ�ﵽĿ��״̬���������
	{
		bool prior = false;//��¼ǰһλ����λ���Ƿ����
		for (int j = initqueenlist.at(row) + 1; j < queenNum; ++j)//��row�еĵ�j��λ��
		{
			bool checkupper = true;
			for (int r = 0; r < row; ++r)//�Ƿ���֮ǰ���лʺ����
			{				
				checkNum++;
				if (j == initqueenlist.at(r) || abs(row - r) == abs(j - initqueenlist.at(r)))
				{
					checkupper = false;
					break;
				}
			}
			if (checkupper)//��ǰj����
			{
				initqueenlist[row] = j;
				row++;//������һ��
				if (row == queenNum)
				{
					return initqueenlist;
				}
				prior = true;//ǰһλ������ȷ
				break;
			}
			else//��ǰj������
			{
				continue;
			}
		}
		if (!prior)//ǰһλ���Ӵ���
		{
			initqueenlist[row] = 0;
			if (row > 0)
			{
				row--;
			}
		}

	}
	initqueenlist.push_back(checkNum);
	return initqueenlist; 
}

class positionIncorrectNum
{
public:
	int position = queenNum;
	int incorrectNum = queenNum;
};
int RandomQueen(vector<int> initQueenColumn);//�����ѡ�г�ͻ�Ļʺ�
int RandomConflict(vector<int> initQueenColumn, int randomqueen);//�Իʺ�randomqueen�����ѡ�г�ͻ��λ�ã����˵�ǰ�ʺ�
/*
*Լ�������������С��ͻ����㷨���ҵ�һ��������ɣ�
*/
vector<int> minConflicts() {
	/*
		�ڴ�ʵ�ְ˻ʺ��������С��ͻ����㷨�����ں�����������printResult������ӡ�����
	*/
	srand((int)time(0));
	vector<int> initQueenColumn = randomInitQueenColumn();
	checkNum = 0;
	for (int i = 0; i < maxRoundNum; ++i)//��������ƶ�����
	{
		if (checkQueenColumn(initQueenColumn))//�ҵ�Ŀ��״̬
		{
			initQueenColumn.push_back(i);//��¼�ƶ�����
			return initQueenColumn;
		}
		else
		{
			int randomqueen = RandomQueen(initQueenColumn);//�����ѡ�г�ͻ�Ļʺ�
			int minimumQueen = RandomConflict(initQueenColumn, randomqueen);//�Իʺ�randomqueen�����ѡ�г�ͻ��λ�ã����˵�ǰ�ʺ�
			initQueenColumn[randomqueen] = minimumQueen;//�ƶ��ʺ�
		}
	}
	initQueenColumn.push_back(maxRoundNum);//�ﵽ����ƶ��������ƣ���¼�ƶ�����
	return initQueenColumn;
}

int RandomQueen(vector<int> initQueenColumn)
{
	vector<int> Queens;
	for (int j = 0; j < queenNum; ++j)//����ÿһ���ʺ�
	{
		int incorrectNum = 0;
		for (int k = 0; k < queenNum; ++k)
		{
			if (k == j)
				continue;
			checkNum++;
			if (initQueenColumn.at(k) == initQueenColumn.at(j) || abs(k - j) == abs(initQueenColumn.at(k) - initQueenColumn.at(j)))
				incorrectNum++;
		}
		if (incorrectNum != 0)//��ͻ������Ϊ0
		{
			Queens.push_back(j);
		}
	}
	int random = rand() % Queens.size();//�ӳ�ͻ�ʺ��������ѡ
	return Queens.at(random);
}

int RandomConflict(vector<int> initQueenColumn, int randomqueen)
{
	vector<positionIncorrectNum> mimimumpos;
	vector<positionIncorrectNum> list;
	int minimumConflict = queenNum;//��С��ͻ����������Ϊ���ֵ
	int minimumQueen = queenNum;//
	for (int j = 0; j < queenNum; ++j)//��ÿ��λ�ñ���
	{
		if (j == initQueenColumn.at(randomqueen))//�ܿ���ǰλ��
			continue;
		int incorrectNum = 0;
		for (int k = 0; k < queenNum; ++k)
		{
			if (k == randomqueen)
				continue;
			checkNum++;
			if (initQueenColumn.at(k) == j || abs(k - randomqueen) == abs(initQueenColumn.at(k) - j))
				incorrectNum++;
		}
		positionIncorrectNum current;
		current.position = j;
		current.incorrectNum = incorrectNum;
		list.push_back(current);//��¼ÿ��λ�ó�ͻ����
	}
	int min = queenNum;
	for (int i = 0; i < list.size(); ++i)//����Ѱ����С��ͻ
	{
		if (list.at(i).incorrectNum <= min)
		{
			min = list.at(i).incorrectNum;
		}
	}
	for (int i = 0; i < list.size(); ++i)//��ѡ����С��ͻ�Ľڵ�
	{
		if (list.at(i).incorrectNum == min)
		{
			mimimumpos.push_back(list.at(i));
		}
	}
	int randNum = rand() % mimimumpos.size();//���ѡ����С��ͻ�ڵ�
	return mimimumpos.at(randNum).position;
}

int main() 
{
	double result_right = 0;
	double steps_sum = 0;
	double check_sum = 0;
	int Maxcheck = -100;
	int Mincheck = 1000000;
	
	for (int s = 0;s<N ;s++)
	{
		int type;
		cout << "���ڴ˴�������õ��㷨��" << endl;
		cout << "-----------------------------------------------------" << endl;
		cout << "1: ���ݷ�" << endl;
		cout << "2: ��С��ͻ��ⷨ" << endl;
		cout << "0:�˳�" << endl;
		cout << "-----------------------------------------------------" << endl;
		cin >> type;
		//Sleep(500);
		//type = 2;
		while (type < 0 && type>2) {
			cout << "���������������������룺" << endl;
			cin >> type;
			break;
		}
		cout << "----------------------START--------------------------" << endl;
		cout << endl;
		if (type == 1) {
			int t1 = GetTickCount64();
			vector<int> queenColumnResult = bactrackDFS();
			int t2 = GetTickCount64();
			int Check = queenColumnResult.back();
			queenColumnResult.pop_back();
			bool result = checkQueenColumn(queenColumnResult);
			if (result) {
				cout << "��ȷ���" << endl;
				printResult(queenColumnResult,checkNum);
				cout << "��ʱ " << t2 - t1 << " ms" << endl;
				if (Check > Maxcheck) Maxcheck = Check;
				if (Check < Mincheck) Mincheck = Check;
				check_sum += Check;

			}
			else {
				cout << "������" << endl;
				printResult(queenColumnResult, checkNum);
			}
		}
		else if (type == 2) {
			int t1 = GetTickCount64();
			vector<int> queenColumnResult = minConflicts();
			int t2 = GetTickCount64();
			int steps = queenColumnResult.at(queenNum);
			queenColumnResult.pop_back();
			bool result = checkQueenColumn(queenColumnResult);
			if (result) {
				cout << "��ȷ���" << endl;
				printResult(queenColumnResult, checkNum);
				cout << "��ʱ " << t2 - t1 << " ms" << endl;

				result_right += 1;
				steps_sum += steps;
				check_sum += checkNum;
			}
			else {
				cout << "������" << endl;
				printResult(queenColumnResult, checkNum);
			}
			//cout << "��ͻ������" << checkNum << endl;
			cout << "�ƶ�����" << steps << endl;
		}
		else {
			return 0;
		}
		cout << "----------------------END ---------------------------" << endl;
		cout << endl;
		checkNum = 0;
	}

	//cout << "average:" << check_sum / N << endl << "MAX:"<< Maxcheck << endl << "MIN:"<<Mincheck<<endl;
	//cout << result_right / N << endl << steps_sum / N;

	return 0;
}

bool checkQueenColumn(vector<int> queenColumn) {
	if (queenColumn.size() != queenNum)
		return false;
	for (int i = 0; i < queenNum; i++) {
		for (int j = i + 1; j < queenNum; j++) {
			if (queenColumn[i] == queenColumn[j])
				return false;
			if (abs(i - j) == abs(queenColumn[i] - queenColumn[j]))
				return false;
		}
	}
	return true;
}

vector<int> randomInitQueenColumn() {
	vector<int> randomQueenColumn = vector<int>();
	for (int i = 0; i < queenNum; i++) {
		randomQueenColumn.push_back(i);
	}
	//��ʼ��N���ʺ��Ӧ��R����Ϊ0~N-1��һ�����У���û������ʺ�ͬ�У�Ҳû���κλʺ�ͬ��
	for (int i = 0; i <= queenNum - 2; i++) {
		//srand((int)time(0));
		int randomNum = rand() % (queenNum - i) + i;
		int temp = randomQueenColumn[i];
		randomQueenColumn[i] = randomQueenColumn[randomNum];
		randomQueenColumn[randomNum] = temp;
	}
	return randomQueenColumn;
}

void printResult(vector<int> eightQueenColumnResult, int checkNum)
{
	cout << "---------------------�ʺ�λ�ðڷŽ������-----------------" << endl;
	cout << endl;
	for (int i = 0; i < eightQueenColumnResult.size(); i++) {
		//switch (eightQueenColumnResult[i]) {
		//case 0:
		//	cout << "Q * * * * * * *" << endl;
		//	break;
		//case 1:
		//	cout << "* Q * * * * * *" << endl;
		//	break;
		//case 2:
		//	cout << "* * Q * * * * *" << endl;
		//	break;
		//case 3:
		//	cout << "* * * Q * * * *" << endl;
		//	break;
		//case 4:
		//	cout << "* * * * Q * * *" << endl;
		//	break;
		//case 5:
		//	cout << "* * * * * Q * *" << endl;
		//	break;
		//case 6:
		//	cout << "* * * * * * Q *" << endl;
		//	break;
		//case 7:
		//	cout << "* * * * * * * Q" << endl;
		//	break;
		//}
		for (int j = 0; j < eightQueenColumnResult.at(i);++j)
			cout << "��";
		cout << "��";
		for (int j = queenNum - 1; j > eightQueenColumnResult.at(i); --j)
			cout << "��";
		cout << endl;

	}
	cout << "��ͻ��������"<< checkNum << endl;
}


