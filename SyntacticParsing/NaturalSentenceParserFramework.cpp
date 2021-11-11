#include<iostream>
#include<vector>
#include <cstdlib>
#include <string> 
#include<map>
#include <utility>
#include <algorithm>
using namespace std;

/*
返回一个map类型的词典e0Dict，形式如下：
(Noun,[Noun,stench,breeze...])
(Verb,[Verb,is,see,smell...])
...
*/
map<string, vector<string>> constructDic();

/*
返回一个语法规则集数组，形式如下：
(
    (S, NP, VP),
    (S, S, Conjunction, S),
    ...
)

数组中每个数组的第一个元素为左侧非终结符，例如
(S, NP, VP)对应 S->NP VP 语法规则
*/
vector<vector<string>> constructGrammarRules();

//分类前缀
string Left = "[";
string Mid = ":";
string Right = "]";

//词性集合
vector<string> types = { "Noun","Verb","Adjective","Adverb","Pronoun", "Name", "Article", "Preposition", "Conjunction", "Digit", };

//结果数组
vector<vector<string>> result;


//输入语句为 the wumpus is dead
vector<string> inputSentence();

//句法解析函数
string parse(map<string, vector<string>> e0Dict, vector<vector<string>> e0GrammarRules, vector<string> sentence);
void print(vector<vector<string>> result);

int main(){
    //加载词典
    map<string, vector<string>> e0Dict = constructDic();

    //加载语法规则
    vector<vector<string>> e0GrammarRules = constructGrammarRules();

    //输入语句
    vector<string> sentence = inputSentence();

    cout << "-------------------------------start------------------------------------" << endl;
    cout << "输入语句：the wumpus is dead" << endl;
    cout << endl;
    /*
    在此实现语句的解析，并最终输出如下格式的结果。
    [NP:[Article:the][Noun:wumpus]][VP:[VP:[Verb:is]][Adjective:dead]]]
    */
	parse(e0Dict, e0GrammarRules, sentence);
    //string result = parse(e0Dict, e0GrammarRules, sentence);
    cout << "句法解析树：";
	print(result);
    //cout << result << endl;
    cout << endl;
    cout << "-------------------------------end------------------------------------" << endl;
    //system("pause");
    return 0;
}


string parse(map<string, vector<string>> e0Dict, vector<vector<string>> e0GrammarRules, vector<string> sentence)
{
	//词性分类
	for (int i = 0; i < sentence.size(); ++i)
	{
		string currentword = sentence[i];//获取当前字符串
		for (int j = 0; j < types.size(); ++j)
		{
			string currenttype = types[j];
			for (int k = 0; k < e0Dict[currenttype].size(); ++k)//遍历字典匹配字符串
			{
				if (currentword == e0Dict[currenttype][k])
				{
					result.push_back({ Left,currenttype,Mid,currentword,Right });//在result中加入词性分类
					break;
				}
			}
		}
	}

	//复写标记，记录result是否被更改过
	int ResultRewrite = 0;

	//语法分类循环
	while (1)
	{
		for (int i = 0; i < result.size(); ++i)
		{
			if (ResultRewrite == 1 && result.size() == 1) return" ";//仅剩一种语句：S，此时可以返回。

			string firsttype = result[i][1];//第一个词性

			for (int j = 0; j < e0GrammarRules.size(); ++j)
			{
				//单元语法：对应VP->Verb等
				if (e0GrammarRules[j][1] == firsttype && e0GrammarRules[j].size() == 2)
				{
					result[i].insert(result[i].begin(), Mid);//插入语法分类
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					i--;//指针回退，因为双元语法可能嵌套单元语法
					j = 0;
					ResultRewrite = 1;
					break;
				}
				//双元组语法
				if (i < result.size()-1 && e0GrammarRules[j][1] == firsttype && e0GrammarRules[j][2] == result[i + 1][1] && e0GrammarRules[j].size() == 3)
				{
					for (int s = 0; s < result[i + 1].size(); ++s)//后一位前移
					{
						result[i].push_back(result[i + 1][s]);
					}
					result[i].insert(result[i].begin(), Mid);//插入语法分类
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					for (int t = i + 1; t < result.size()-1; ++t)//每一位前移
					{
						result[t] = result[t + 1];
					}
					result.pop_back();//去除后一位
					ResultRewrite = 1;
					break;
				}
				//三元组语法：对应S Conj S
				if (i < result.size() - 2 && e0GrammarRules[j][1] == firsttype && e0GrammarRules[j][2] == result[i + 1][1] && e0GrammarRules[j][3] == result[i + 2][1] && e0GrammarRules[j].size() == 4)
				{
					for (int s = 0; s < result[i + 1].size(); ++s)//后一位前移
					{
						result[i].push_back(result[i + 1][s]);
					}
					for (int s = 0; s < result[i + 2].size(); ++s)//后两位前移
					{
						result[i].push_back(result[i + 2][s]);
					}
					result[i].insert(result[i].begin(), Mid);//插入语法分类
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					for (int t = i + 1; t < result.size() - 2; ++t)//每一位前移
					{
						result[t] = result[t + 2];
					}
					result.pop_back();//去除最后两位
					result.pop_back();
					ResultRewrite = 1;
					break;
				}
			}
		}
	}


    return "";
}

//输入result
void print(vector<vector<string>> result)
{
	for (int i = 0; i < result.size(); ++i)
	{
		for (int j = 0; j < result[i].size(); ++j)
		{
			cout << result[i][j];
		}
	}
}

 /*
 返回一个map类型的词典e0Dict，形式如下：
 (Noun,[Noun,stench,breeze...])
 (Verb,[Verb,is,see,smell...])
 ...
 */
 map<string, vector<string>> constructDic() {
     //构造名词数组
     vector<string> nounDic = vector<string>();
     nounDic.push_back("Noun");
     nounDic.push_back("stench");
     nounDic.push_back("breeze");
     nounDic.push_back("glitter");
     nounDic.push_back("nothing");
     nounDic.push_back("agent");
     nounDic.push_back("wumpus");
     nounDic.push_back("pit");
     nounDic.push_back("pits");
     nounDic.push_back("gold");
     nounDic.push_back("east");

     //构造动词数组
     vector<string> verbDic = vector<string>();
     verbDic.push_back("Verb");
     verbDic.push_back("is");
     verbDic.push_back("see");
     verbDic.push_back("smell");
     verbDic.push_back("shoot");
     verbDic.push_back("feel");
     verbDic.push_back("stinks");
     verbDic.push_back("go");
     verbDic.push_back("grab");
     verbDic.push_back("carry");
     verbDic.push_back("kill");
     verbDic.push_back("turn");

     //构造形容词数组
     vector<string> adjectiveDic = vector<string>();
     adjectiveDic.push_back("Adjective");
     adjectiveDic.push_back("right");
     adjectiveDic.push_back("left");
     adjectiveDic.push_back("east");
     adjectiveDic.push_back("dead");
     adjectiveDic.push_back("back");
     adjectiveDic.push_back("smelly");

     //构造副词数组
     vector<string> adverbDic = vector<string>();
     adverbDic.push_back("Adverb");
     adverbDic.push_back("here");
     adverbDic.push_back("there");
     adverbDic.push_back("nearby");
     adverbDic.push_back("ahead");
     adverbDic.push_back("right");
     adverbDic.push_back("left");
     adverbDic.push_back("east");
     adverbDic.push_back("south");
     adverbDic.push_back("back");

     //构造代词数组
     vector<string> pronounDic = vector<string>();
     pronounDic.push_back("Pronoun");
     pronounDic.push_back("me");
     pronounDic.push_back("you");
     pronounDic.push_back("I");
     pronounDic.push_back("it");

     //构造名字数组
     vector<string> nameDic = vector<string>();
     nameDic.push_back("Name");
     nameDic.push_back("John");
     nameDic.push_back("Mary");
     nameDic.push_back("Boston");
     nameDic.push_back("Aristotle");

     //构造冠词数组
     vector<string> articleDic = vector<string>();
     articleDic.push_back("Article");
     articleDic.push_back("the");
     articleDic.push_back("a");
     articleDic.push_back("an");

     //构造介词数组
     vector<string> prepositionDic = vector<string>();
     prepositionDic.push_back("Preposition");
     prepositionDic.push_back("to");
     prepositionDic.push_back("in");
     prepositionDic.push_back("on");
     prepositionDic.push_back("near");

     //构造连词数组
     vector<string> conjunctionDic = vector<string>();
     conjunctionDic.push_back("Conjunction");
     conjunctionDic.push_back("and");
     conjunctionDic.push_back("or");
     conjunctionDic.push_back("but");

     //构造数字数组
     vector<string> digitDic = vector<string>();
     digitDic.push_back("Digit");
     digitDic.push_back("0");
     digitDic.push_back("1");
     digitDic.push_back("2");
     digitDic.push_back("3");
     digitDic.push_back("4");
     digitDic.push_back("5");
     digitDic.push_back("6");
     digitDic.push_back("7");
     digitDic.push_back("8");
     digitDic.push_back("9");

     //根据不同类别单词数组构造词典map
     map<string, vector<string>> e0Dict = map<string, vector<string>>();
     e0Dict.insert(pair<string, vector<string>>("Noun", nounDic));
     e0Dict.insert(pair<string, vector<string>>("Verb", verbDic));
     e0Dict.insert(pair<string, vector<string>>("Adjective", adjectiveDic));
     e0Dict.insert(pair<string, vector<string>>("Adverb", adverbDic));
     e0Dict.insert(pair<string, vector<string>>("Pronoun", pronounDic));
     e0Dict.insert(pair<string, vector<string>>("Name", nameDic));
     e0Dict.insert(pair<string, vector<string>>("Article", articleDic));
     e0Dict.insert(pair<string, vector<string>>("Preposition", prepositionDic));
     e0Dict.insert(pair<string, vector<string>>("Conjunction", conjunctionDic));
     e0Dict.insert(pair<string, vector<string>>("Digit", digitDic));

     return e0Dict;
 }

 /*
返回一个语法规则集数组，形式如下：
(
    (S, NP, VP),
    (S, S, Conjunction, S),
    ...
)

数组中每个数组的第一个元素为左侧非终结符，例如
(S, NP, VP)对应 S->NP VP 语法规则
*/
 vector<vector<string>> constructGrammarRules() {

     //构造左侧非终结符为S的语法规则
     vector<string> e0GrammarRule1 = vector<string>();
     e0GrammarRule1.push_back("S");
     e0GrammarRule1.push_back("NP");
     e0GrammarRule1.push_back("VP");

     //构造左侧非终结符为S的语法规则
     vector<string> e0GrammarRule2 = vector<string>();
     e0GrammarRule2.push_back("S");
     e0GrammarRule2.push_back("S");
     e0GrammarRule2.push_back("Conjunction");
     e0GrammarRule2.push_back("S");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule3 = vector<string>();
     e0GrammarRule3.push_back("NP");
     e0GrammarRule3.push_back("Pronoun");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule4 = vector<string>();
     e0GrammarRule4.push_back("NP");
     e0GrammarRule4.push_back("Name");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule5 = vector<string>();
     e0GrammarRule5.push_back("NP");
     e0GrammarRule5.push_back("Noun");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule6 = vector<string>();
     e0GrammarRule6.push_back("NP");
     e0GrammarRule6.push_back("Article");
     e0GrammarRule6.push_back("Noun");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule7 = vector<string>();
     e0GrammarRule7.push_back("NP");
     e0GrammarRule7.push_back("Digit");
     e0GrammarRule7.push_back("Digit");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule8 = vector<string>();
     e0GrammarRule8.push_back("NP");
     e0GrammarRule8.push_back("NP");
     e0GrammarRule8.push_back("PP");

     //构造左侧非终结符为NP的语法规则
     vector<string> e0GrammarRule9 = vector<string>();
     e0GrammarRule9.push_back("NP");
     e0GrammarRule9.push_back("NP");
     e0GrammarRule9.push_back("RelClause");

     //构造左侧非终结符为VP的语法规则
     vector<string> e0GrammarRule10 = vector<string>();
     e0GrammarRule10.push_back("VP");
     e0GrammarRule10.push_back("Verb");

     //构造左侧非终结符为VP的语法规则
     vector<string> e0GrammarRule11 = vector<string>();
     e0GrammarRule11.push_back("VP");
     e0GrammarRule11.push_back("VP");
     e0GrammarRule11.push_back("NP");

     //构造左侧非终结符为VP的语法规则
     vector<string> e0GrammarRule12 = vector<string>();
     e0GrammarRule12.push_back("VP");
     e0GrammarRule12.push_back("VP");
     e0GrammarRule12.push_back("Adjective");

     //构造左侧非终结符为VP的语法规则
     vector<string> e0GrammarRule13 = vector<string>();
     e0GrammarRule13.push_back("VP");
     e0GrammarRule13.push_back("VP");
     e0GrammarRule13.push_back("PP");

     //构造左侧非终结符为VP的语法规则
     vector<string> e0GrammarRule14 = vector<string>();
     e0GrammarRule14.push_back("VP");
     e0GrammarRule14.push_back("VP");
     e0GrammarRule14.push_back("Adverb");

     //构造左侧非终结符为PP的语法规则
     vector<string> e0GrammarRule15 = vector<string>();
     e0GrammarRule15.push_back("PP");
     e0GrammarRule15.push_back("Preposition");
     e0GrammarRule15.push_back("NP");

     //构造左侧非终结符为RelClause的语法规则
     vector<string> e0GrammarRule16 = vector<string>();
     e0GrammarRule16.push_back("RelClause");
     e0GrammarRule16.push_back("that");
     e0GrammarRule16.push_back("VP");

     //构造语法规则集
     vector<vector<string>> e0GrammarRules = vector<vector<string>>();
     e0GrammarRules.push_back(e0GrammarRule1);
     e0GrammarRules.push_back(e0GrammarRule2);
     e0GrammarRules.push_back(e0GrammarRule3);
     e0GrammarRules.push_back(e0GrammarRule4);
     e0GrammarRules.push_back(e0GrammarRule5);
     e0GrammarRules.push_back(e0GrammarRule6);
     e0GrammarRules.push_back(e0GrammarRule7);
     e0GrammarRules.push_back(e0GrammarRule8);
     e0GrammarRules.push_back(e0GrammarRule9);
     e0GrammarRules.push_back(e0GrammarRule10);
     e0GrammarRules.push_back(e0GrammarRule11);
     e0GrammarRules.push_back(e0GrammarRule12);
     e0GrammarRules.push_back(e0GrammarRule13);
     e0GrammarRules.push_back(e0GrammarRule14);
     e0GrammarRules.push_back(e0GrammarRule15);
     e0GrammarRules.push_back(e0GrammarRule16);

     return e0GrammarRules;
 }

 //输入语句为 the wumpus is dead
 vector<string> inputSentence() {
     vector<string> sentence = vector<string>();
	 sentence = { "the", "wumpus" ,"is" ,"dead" };
	 return sentence;
 }