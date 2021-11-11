#ifndef EIGHTQUEENNODE_H
#define EIGHTQUEENNODE_H
#include<vector>
using namespace std;

class EightQueenNode {

private: 
	vector<int> queenColumn;//皇后的列数
	int currentRow;//当前行数

public:
	vector<int> getQueenColumn();
	void setQueenColumn(vector<int>  queenColumn);
	int getCurrentRow();
	void setCurrentRow(int currentRow);
	EightQueenNode();
	EightQueenNode(vector<int>  queenColumn,int currentDepth);
};



#endif // !EIGHTQUEENNODE_H

