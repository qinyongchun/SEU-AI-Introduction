#include<iostream>
#include<vector>
#include"EightQueenNode.h"
#include<ctime>
#include<Windows.h>
#define N 10000

using namespace std;

//定义皇后数量
const int queenNum = 8;

//冲突检测次数
static int checkNum = 0;

//存放皇后的数组，下标为行数，取值为列数
static int* queenColumn = new int[queenNum];

//最大随机次数
static int maxRoundNum = 100;

/*
输入：存放皇后的数组，下标为行数，取值为列数
返回值： /
描述：执行打印功能，打印出皇后最终的摆放位置。
*/
void printResult(vector<int> eightQueenColumnResult, int checkNum);

/*
* 输入：
* 输出：存放皇后的数组，下标为行数，取值为列数
*随机摆放八个皇后的位置，保证不在同一行同一列
*/
vector<int> randomInitQueenColumn();

/*
*输入：存放皇后的数组
*输出：true:正确结果 false:错误结果
*/
bool checkQueenColumn(vector<int> queenColumn);

/*
*约束满足问题的回溯算法（找到一个结果即可）
*/
vector<int> bactrackDFS() {
	/*
	    在此实现八皇后问题的回溯算法，请在函数体最后调用printResult函数打印结果。
	*/
	
	vector<int> initqueenlist = randomInitQueenColumn();
	int row = 1;//当前行数
	checkNum = 0;
	while(!checkQueenColumn(initqueenlist))//若未达到目标状态则继续回溯
	{
		bool prior = false;//记录前一位棋子位置是否可行
		for (int j = initqueenlist.at(row) + 1; j < queenNum; ++j)//第row行的第j个位置
		{
			bool checkupper = true;
			for (int r = 0; r < row; ++r)//是否与之前所有皇后兼容
			{				
				checkNum++;
				if (j == initqueenlist.at(r) || abs(row - r) == abs(j - initqueenlist.at(r)))
				{
					checkupper = false;
					break;
				}
			}
			if (checkupper)//当前j可行
			{
				initqueenlist[row] = j;
				row++;//进入下一行
				if (row == queenNum)
				{
					return initqueenlist;
				}
				prior = true;//前一位棋子正确
				break;
			}
			else//当前j不可行
			{
				continue;
			}
		}
		if (!prior)//前一位棋子错误
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
int RandomQueen(vector<int> initQueenColumn);//随机挑选有冲突的皇后
int RandomConflict(vector<int> initQueenColumn, int randomqueen);//对皇后randomqueen随机挑选有冲突的位置（除了当前皇后）
/*
*约束满足问题的最小冲突检测算法（找到一个结果即可）
*/
vector<int> minConflicts() {
	/*
		在此实现八皇后问题的最小冲突检测算法，请在函数体最后调用printResult函数打印结果。
	*/
	srand((int)time(0));
	vector<int> initQueenColumn = randomInitQueenColumn();
	checkNum = 0;
	for (int i = 0; i < maxRoundNum; ++i)//设置最大移动次数
	{
		if (checkQueenColumn(initQueenColumn))//找到目标状态
		{
			initQueenColumn.push_back(i);//记录移动次数
			return initQueenColumn;
		}
		else
		{
			int randomqueen = RandomQueen(initQueenColumn);//随机挑选有冲突的皇后
			int minimumQueen = RandomConflict(initQueenColumn, randomqueen);//对皇后randomqueen随机挑选有冲突的位置（除了当前皇后）
			initQueenColumn[randomqueen] = minimumQueen;//移动皇后
		}
	}
	initQueenColumn.push_back(maxRoundNum);//达到最大移动次数限制，记录移动次数
	return initQueenColumn;
}

int RandomQueen(vector<int> initQueenColumn)
{
	vector<int> Queens;
	for (int j = 0; j < queenNum; ++j)//遍历每一个皇后
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
		if (incorrectNum != 0)//冲突次数不为0
		{
			Queens.push_back(j);
		}
	}
	int random = rand() % Queens.size();//从冲突皇后中随机挑选
	return Queens.at(random);
}

int RandomConflict(vector<int> initQueenColumn, int randomqueen)
{
	vector<positionIncorrectNum> mimimumpos;
	vector<positionIncorrectNum> list;
	int minimumConflict = queenNum;//最小冲突次数，设置为最大值
	int minimumQueen = queenNum;//
	for (int j = 0; j < queenNum; ++j)//对每个位置遍历
	{
		if (j == initQueenColumn.at(randomqueen))//避开当前位置
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
		list.push_back(current);//记录每个位置冲突次数
	}
	int min = queenNum;
	for (int i = 0; i < list.size(); ++i)//遍历寻找最小冲突
	{
		if (list.at(i).incorrectNum <= min)
		{
			min = list.at(i).incorrectNum;
		}
	}
	for (int i = 0; i < list.size(); ++i)//挑选出最小冲突的节点
	{
		if (list.at(i).incorrectNum == min)
		{
			mimimumpos.push_back(list.at(i));
		}
	}
	int randNum = rand() % mimimumpos.size();//随机选择最小冲突节点
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
		cout << "请在此处输入调用的算法：" << endl;
		cout << "-----------------------------------------------------" << endl;
		cout << "1: 回溯法" << endl;
		cout << "2: 最小冲突检测法" << endl;
		cout << "0:退出" << endl;
		cout << "-----------------------------------------------------" << endl;
		cin >> type;
		//Sleep(500);
		//type = 2;
		while (type < 0 && type>2) {
			cout << "输入类型有误，请重新输入：" << endl;
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
				cout << "正确结果" << endl;
				printResult(queenColumnResult,checkNum);
				cout << "耗时 " << t2 - t1 << " ms" << endl;
				if (Check > Maxcheck) Maxcheck = Check;
				if (Check < Mincheck) Mincheck = Check;
				check_sum += Check;

			}
			else {
				cout << "错误结果" << endl;
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
				cout << "正确结果" << endl;
				printResult(queenColumnResult, checkNum);
				cout << "耗时 " << t2 - t1 << " ms" << endl;

				result_right += 1;
				steps_sum += steps;
				check_sum += checkNum;
			}
			else {
				cout << "错误结果" << endl;
				printResult(queenColumnResult, checkNum);
			}
			//cout << "冲突检测次数" << checkNum << endl;
			cout << "移动步数" << steps << endl;
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
	//初始化N个皇后对应的R数组为0~N-1的一个排列，即没有任意皇后同列，也没有任何皇后同行
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
	cout << "---------------------皇后位置摆放结果如下-----------------" << endl;
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
			cout << "□";
		cout << "■";
		for (int j = queenNum - 1; j > eightQueenColumnResult.at(i); --j)
			cout << "□";
		cout << endl;

	}
	cout << "冲突检测次数："<< checkNum << endl;
}


