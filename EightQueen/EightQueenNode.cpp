#include "EightQueenNode.h"
#include<vector>
using namespace std;


EightQueenNode::EightQueenNode(vector<int>  queenColumns, int currentRow) {
    this->currentRow = currentRow;
    for (int i = 0; i < queenColumns.size(); i++) {
        queenColumn.push_back(queenColumns.at(i));
    }
}


vector<int> EightQueenNode::getQueenColumn()
{
    return queenColumn;
}

void EightQueenNode::setQueenColumn(vector<int>  queenColumns)
{
    queenColumn = queenColumns;
}

int EightQueenNode::getCurrentRow()
{
    return this->currentRow;
}

void EightQueenNode::setCurrentRow(int currentRow)
{
    this->currentRow = currentRow;
}

EightQueenNode::EightQueenNode()
{
}
