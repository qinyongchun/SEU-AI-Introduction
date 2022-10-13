#include <iostream>
#include <vector>
#include <queue>
#include<stack>
#include<map>
#include <time.h>
#include "eightFigurePuzzles.h"
#include <windows.h>
using namespace std;

//用于记录当前状态是否被访问过。
map<int, int> visited;

//深度有限搜索，用于限制深度。由于最大深度不超过20，且重复情况较多，因此此处设置为15
#define MAX_DEPTH 20


//广度优先搜索
int* binaryFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
//result[1] 步数 steps 
	int result[2] = { 0,0 };

	/*
		请在该位置完成广度优先搜索。
	*/

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	PUZZLE_NODE puzzleNode = initialNode;
	queue<PUZZLE_NODE> puzzleQueue;//广度优先栈
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleQueue.push(puzzleNode);//初始节点入队
	while (puzzleQueue.size()) {
		PUZZLE_NODE currentNode = puzzleQueue.front();
		if (checkObject(currentNode, objPuzzleNode)) //找到目标
		{

			for (int i = 0; i < currentNode.precedeActionList.size(); i++) {
				outputAction(currentNode.precedeActionList[i], i + 1);
			}
			cout << "找到正确结果:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentNode.puzzle[i * 3 + 0].puzzleId << "  " << currentNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentNode.depth;
			return result;
		}
		else//没找到目标
		{
			visited[visitedNum(currentNode)] = 1;//添加当前节点的访问记录
			if (currentNode.nextActionList.size() == 0) {
				currentNode = updatePuzzleNodeActionList(currentNode);
			}
			puzzleQueue.pop();//当前节点出队
			for (int i = 0; i < currentNode.nextActionList.size(); i++) //依次访问每一个可能后继
			{
				PUZZLE_NODE nextNode = moveToPuzzleNode(currentNode.nextActionList[i], currentNode);
				if (!currentNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentNode.precedeActionList.size(); actionIndex++)//依次添加路径记录
					{
						nextNode.precedeActionList.push_back(currentNode.precedeActionList[actionIndex]);
					}
				}
				nextNode.precedeActionList.push_back(currentNode.nextActionList[i]);
				if (visited[visitedNum(nextNode)] == 1) {
					continue;
				}
				nextNode.depth = currentNode.depth + 1;
				puzzleQueue.push(nextNode);
			}
		}

	}



	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;

}

//深度有限搜索
int* depthFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {

	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };
	/*
		请在该位置完成深度有限搜索，最大深度限度为25。
	*/

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;

	PUZZLE_NODE puzzleNode = initialNode;
	stack<PUZZLE_NODE> puzzleNodeStack;//记录搜索顺序的栈
	stack<PUZZLE_NODE> reverseStack;//将某一结点搜索顺序倒置，完成深度优先遍历
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleNodeStack.push(puzzleNode);//初始位置入栈
	PUZZLE_NODE temp;//临时节点，用于交换
	int nextsize;//某一节点可行走法个数
	while (!puzzleNodeStack.empty())//栈非空：未完成遍历
	{
		PUZZLE_NODE currentPuzzleNode = puzzleNodeStack.top();
		if (checkObject(currentPuzzleNode, objPuzzleNode)) //找到目标，返回目标节点
		{

			for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
				outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
			}
			cout << "找到正确结果:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentPuzzleNode.depth;
			return result;
		}
		else
		{
			visited[visitedNum(currentPuzzleNode)] = 1;//记录已到达节点
				
			while (currentPuzzleNode.depth >= MAX_DEPTH)//深度限制
			{
				puzzleNodeStack.pop();
				currentPuzzleNode = puzzleNodeStack.top();
			}

			currentPuzzleNode = puzzleNodeStack.top();

			if (!puzzleNodeStack.empty())
			{
				if (currentPuzzleNode.nextActionList.size() == 0) {
					currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
				}

				puzzleNodeStack.pop();

				nextsize = currentPuzzleNode.nextActionList.size();
				int* randomexplore = new int[nextsize];

				for (int i = 0; i < nextsize; i++)
				{
					randomexplore[i] = i;
				}
				for (int i = 0; i<10; i++)//随机重置遍历顺序
				{
					//要取得 [a,b) 的随机整数，使用 (rand() % (b-a))+ a;
					int random1 = (rand() % (nextsize - 0)) + 0;
					int random2 = (rand() % (nextsize - 0)) + 0;
					int temp = randomexplore[random1];
					randomexplore[random1] = randomexplore[random2];
					randomexplore[random2] = temp;
				}

				int i = 0;
				//随机顺序遍历
				//for (int j = 0; j < currentPuzzleNode.nextActionList.size();++j) {
				//	i = randomexplore[j];

				//按固定顺序遍历（上下左右）
				for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {

					PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
					if (!currentPuzzleNode.precedeActionList.empty()) 
					{
						for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
						{
							nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);//添加访问记录
						}
					}
					nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
					if (visited[visitedNum(nextPuzzleNode)] == 1) //已访问节点不再访问
					{
						continue;
					}
					else
					{
						nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
						puzzleNodeStack.push(nextPuzzleNode);//节点入栈
					}
				}
				//while (!reverseStack.empty())
				//{
				//	temp = reverseStack.top();
				//	puzzleNodeStack.push(temp);
				//	reverseStack.pop();
				//}
			}
			else
				return result;
		}
	}
}


//启发式搜索1
int* heuristicSearchInformedByIncorrectNum(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;

	/*
		请在该位置完成启发式搜索，启发式函数使用不正确位置的数码个数。
	*/
	vector<PUZZLE_NODE> puzzleNodeVector;//记录所有可能后继（A*）
	PUZZLE_NODE currentPuzzleNode = initialNode;
	while (IncorrectNum(currentPuzzleNode, objPuzzleNode) != 0)//未达到目标节点
	{

		visited[visitedNum(currentPuzzleNode)] = 1;//添加访问记录

		for (int i = 0; i < currentPuzzleNode.nextActionList.size(); ++i)//依次访问所有后继
		{
			PUZZLE_NODE nextpuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
			nextpuzzleNode.depth = currentPuzzleNode.depth;
			nextpuzzleNode.precedeActionList = currentPuzzleNode.precedeActionList;
			nextpuzzleNode = updatePuzzleNodeActionList(nextpuzzleNode);
			if (visited[visitedNum(nextpuzzleNode)] == 1) //该节点已被访问过
			{
				continue;
			}
			nextpuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
			nextpuzzleNode.depth++;
			puzzleNodeVector.push_back(nextpuzzleNode);//将该后继节点如数组，即作为下次选择的备选方案之一
		}
		int min_cost = 1000;
		int min_location = 0;

		for (int i = 0; i < puzzleNodeVector.size(); ++i)//选出vector中启发式函数值最小的节点，将其作为下一次的节点
		{
			if ((puzzleNodeVector[i].depth + 10*IncorrectNum(puzzleNodeVector[i], objPuzzleNode)) <= min_cost)
			{
				min_location = i;
				min_cost = puzzleNodeVector[i].depth + 10*IncorrectNum(puzzleNodeVector[i], objPuzzleNode);
				currentPuzzleNode = puzzleNodeVector[i];
			}
		}

		ExchangePuzzleNode(puzzleNodeVector[min_location], puzzleNodeVector.back());//将最优节点与最后一个节点交换
		puzzleNodeVector.pop_back();//在vector中删除最优解，以进行下一步迭代

		if (currentPuzzleNode.nextActionList.size() == 0) {
			currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
		}

	}

	if (checkObject(currentPuzzleNode, objPuzzleNode))//找到目标结果
	{

		for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
			outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
		}
		cout << "找到正确结果:" << endl;
		for (int i = 0; i < 3; i++) {
			cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
				"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
		}
		cout << endl;

		result[0] = 1;
		result[1] = currentPuzzleNode.depth;
		return result;
	}

	return result;
}

//启发式搜素2
int* heuristicSearchInformedByManhattonDis(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;

	/*
		请在该位置完成启发式搜索，启发式函数使用不正确位置的数码个数。
	*/
	vector<PUZZLE_NODE> puzzleNodeVector;
	PUZZLE_NODE currentPuzzleNode = initialNode;
	while (Manhattan(currentPuzzleNode, objPuzzleNode) != 0)
	{

		visited[visitedNum(currentPuzzleNode)] = 1;


		int x = Manhattan(currentPuzzleNode, objPuzzleNode);
		for (int i = 0; i < currentPuzzleNode.nextActionList.size(); ++i)
		{
			PUZZLE_NODE nextpuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
			nextpuzzleNode.depth = currentPuzzleNode.depth;
			nextpuzzleNode.precedeActionList = currentPuzzleNode.precedeActionList;
			nextpuzzleNode = updatePuzzleNodeActionList(nextpuzzleNode);
			if (visited[visitedNum(nextpuzzleNode)] == 1) {
				continue;
			}
			nextpuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
			nextpuzzleNode.depth++;
			puzzleNodeVector.push_back(nextpuzzleNode);
		}
		int min_cost = 1000;
		int min_location = 0;
		for (int i = 0; i < puzzleNodeVector.size(); ++i)
		{
			if ((puzzleNodeVector[i].depth + Manhattan(puzzleNodeVector[i], objPuzzleNode)) <= min_cost)
			{
				min_location = i;
				min_cost = puzzleNodeVector[i].depth + Manhattan(puzzleNodeVector[i], objPuzzleNode);
				currentPuzzleNode = puzzleNodeVector[i];
			}
		}
		ExchangePuzzleNode(puzzleNodeVector[min_location],puzzleNodeVector.back());
			puzzleNodeVector.pop_back();

		if (currentPuzzleNode.nextActionList.size() == 0) {
			currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
		}

	}

	if (checkObject(currentPuzzleNode, objPuzzleNode)) {

		for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
			outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
		}
		cout << "找到正确结果:" << endl;
		for (int i = 0; i < 3; i++) {
			cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
				"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
		}
		cout << endl;

		result[0] = 1;
		result[1] = currentPuzzleNode.depth;
		return result;
	}

	return result;
}

//广度优先搜索
int* binaryFirstSearchDemo(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	/*
		请在该位置完成广度优先搜索函数。
	*/
	PUZZLE_NODE puzzleNode = initialNode;
	queue<PUZZLE_NODE> puzzleNodeQueue;
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleNodeQueue.push(puzzleNode);
	while (puzzleNodeQueue.size()) {
		PUZZLE_NODE currentPuzzleNode = puzzleNodeQueue.front();
		if (checkObject(currentPuzzleNode, objPuzzleNode)) {

			for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
				outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
			}
			cout << "找到正确结果:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentPuzzleNode.depth;
			return result;
		}
		else {
			visited[visitedNum(currentPuzzleNode)] = 1;
			if (currentPuzzleNode.nextActionList.size() == 0) {
				currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
			}
			puzzleNodeQueue.pop();
			for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
				PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
				if (!currentPuzzleNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
					{
						nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
					}
				}
				nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
				if (visited[visitedNum(nextPuzzleNode)] == 1) {
					continue;
				}
				nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
				puzzleNodeQueue.push(nextPuzzleNode);
			}
		}

	}
	
	return result;
}

int main()
{
	PUZZLE_NODE objPuzzleNode;
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			objPuzzleNode.puzzle[i * 3 + j].puzzleId = i * 3 + j;
			objPuzzleNode.puzzle[i * 3 + j].xPosition = i;
			objPuzzleNode.puzzle[i * 3 + j].yPosition = j;
		}
	}
	objPuzzleNode = updatePuzzleNodeActionList(objPuzzleNode);

	int setup = 0;
	while (setup != -1) 
	{


		visited.clear();
		cout << "-------------------------------------------------" << endl;
		cout << "请输入调试设置:" << endl;
		cout<< " -1:退出; " << endl;
		cout << "0:广度优先搜索示例;" << endl;
		cout << "1:广度优先搜索; " << endl;
		cout << "2:深度有限搜索; " << endl;
		cout << "3:启发式搜索1; " << endl;
		cout<<  "4:启发式搜索2:" << endl; 
		
		cin >> setup;

		if (setup == -1)
		{
			cout << "结束运行" << endl;
			cout << endl;
			return 0;

		}
		else
		{
			cout << "请输入大于等于5小于等于20的回退步数" << endl;
			int backwardSteps;
			cin >> backwardSteps;
			while (backwardSteps < 5 || backwardSteps >20)
			{
				cout << "输入错误，请输入大于等于5小于等于20的回退步数" << endl;
				cin >> backwardSteps;
			}
			PUZZLE_NODE initialNode = initialPuzzleNode(backwardSteps);

			int* result;
			if (setup == 1)
			{
				result = binaryFirstSearch(initialNode, objPuzzleNode);
			}
			else if (setup == 2) 
			{
				result = depthFirstSearch(initialNode, objPuzzleNode);
			}
			else if (setup == 3) 
			{
				result = heuristicSearchInformedByIncorrectNum(initialNode, objPuzzleNode);
			}
			else if (setup == 4) 
			{
				result = heuristicSearchInformedByManhattonDis(initialNode, objPuzzleNode);
			}
			else if (setup == 0) 
			{
				cout << "广度优先搜索示例:" << endl;
				result = binaryFirstSearchDemo(initialNode, objPuzzleNode);
			}
			else 
			{
				cout << "输入设置有误，请重新运行" << endl;
				cout << endl;
				return 0;
			}

			if (result[0] == 1) 
			{
				cout << "结果为correct,步数为" << result[1] << endl;
			}
			else 
			{
				cout << "结果为wrong" << endl;
			}
			cout << "-------------------------------------------------" << endl;

			cout << endl;
		}
	}
	return 0;

}

