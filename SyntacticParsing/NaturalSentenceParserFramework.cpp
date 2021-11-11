#include<iostream>
#include<vector>
#include <cstdlib>
#include <string> 
#include<map>
#include <utility>
#include <algorithm>
using namespace std;

/*
����һ��map���͵Ĵʵ�e0Dict����ʽ���£�
(Noun,[Noun,stench,breeze...])
(Verb,[Verb,is,see,smell...])
...
*/
map<string, vector<string>> constructDic();

/*
����һ���﷨�������飬��ʽ���£�
(
    (S, NP, VP),
    (S, S, Conjunction, S),
    ...
)

������ÿ������ĵ�һ��Ԫ��Ϊ�����ս��������
(S, NP, VP)��Ӧ S->NP VP �﷨����
*/
vector<vector<string>> constructGrammarRules();

//����ǰ׺
string Left = "[";
string Mid = ":";
string Right = "]";

//���Լ���
vector<string> types = { "Noun","Verb","Adjective","Adverb","Pronoun", "Name", "Article", "Preposition", "Conjunction", "Digit", };

//�������
vector<vector<string>> result;


//�������Ϊ the wumpus is dead
vector<string> inputSentence();

//�䷨��������
string parse(map<string, vector<string>> e0Dict, vector<vector<string>> e0GrammarRules, vector<string> sentence);
void print(vector<vector<string>> result);

int main(){
    //���شʵ�
    map<string, vector<string>> e0Dict = constructDic();

    //�����﷨����
    vector<vector<string>> e0GrammarRules = constructGrammarRules();

    //�������
    vector<string> sentence = inputSentence();

    cout << "-------------------------------start------------------------------------" << endl;
    cout << "������䣺the wumpus is dead" << endl;
    cout << endl;
    /*
    �ڴ�ʵ�����Ľ�����������������¸�ʽ�Ľ����
    [NP:[Article:the][Noun:wumpus]][VP:[VP:[Verb:is]][Adjective:dead]]]
    */
	parse(e0Dict, e0GrammarRules, sentence);
    //string result = parse(e0Dict, e0GrammarRules, sentence);
    cout << "�䷨��������";
	print(result);
    //cout << result << endl;
    cout << endl;
    cout << "-------------------------------end------------------------------------" << endl;
    //system("pause");
    return 0;
}


string parse(map<string, vector<string>> e0Dict, vector<vector<string>> e0GrammarRules, vector<string> sentence)
{
	//���Է���
	for (int i = 0; i < sentence.size(); ++i)
	{
		string currentword = sentence[i];//��ȡ��ǰ�ַ���
		for (int j = 0; j < types.size(); ++j)
		{
			string currenttype = types[j];
			for (int k = 0; k < e0Dict[currenttype].size(); ++k)//�����ֵ�ƥ���ַ���
			{
				if (currentword == e0Dict[currenttype][k])
				{
					result.push_back({ Left,currenttype,Mid,currentword,Right });//��result�м�����Է���
					break;
				}
			}
		}
	}

	//��д��ǣ���¼result�Ƿ񱻸��Ĺ�
	int ResultRewrite = 0;

	//�﷨����ѭ��
	while (1)
	{
		for (int i = 0; i < result.size(); ++i)
		{
			if (ResultRewrite == 1 && result.size() == 1) return" ";//��ʣһ����䣺S����ʱ���Է��ء�

			string firsttype = result[i][1];//��һ������

			for (int j = 0; j < e0GrammarRules.size(); ++j)
			{
				//��Ԫ�﷨����ӦVP->Verb��
				if (e0GrammarRules[j][1] == firsttype && e0GrammarRules[j].size() == 2)
				{
					result[i].insert(result[i].begin(), Mid);//�����﷨����
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					i--;//ָ����ˣ���Ϊ˫Ԫ�﷨����Ƕ�׵�Ԫ�﷨
					j = 0;
					ResultRewrite = 1;
					break;
				}
				//˫Ԫ���﷨
				if (i < result.size()-1 && e0GrammarRules[j][1] == firsttype && e0GrammarRules[j][2] == result[i + 1][1] && e0GrammarRules[j].size() == 3)
				{
					for (int s = 0; s < result[i + 1].size(); ++s)//��һλǰ��
					{
						result[i].push_back(result[i + 1][s]);
					}
					result[i].insert(result[i].begin(), Mid);//�����﷨����
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					for (int t = i + 1; t < result.size()-1; ++t)//ÿһλǰ��
					{
						result[t] = result[t + 1];
					}
					result.pop_back();//ȥ����һλ
					ResultRewrite = 1;
					break;
				}
				//��Ԫ���﷨����ӦS Conj S
				if (i < result.size() - 2 && e0GrammarRules[j][1] == firsttype && e0GrammarRules[j][2] == result[i + 1][1] && e0GrammarRules[j][3] == result[i + 2][1] && e0GrammarRules[j].size() == 4)
				{
					for (int s = 0; s < result[i + 1].size(); ++s)//��һλǰ��
					{
						result[i].push_back(result[i + 1][s]);
					}
					for (int s = 0; s < result[i + 2].size(); ++s)//����λǰ��
					{
						result[i].push_back(result[i + 2][s]);
					}
					result[i].insert(result[i].begin(), Mid);//�����﷨����
					result[i].insert(result[i].begin(), e0GrammarRules[j][0]);
					result[i].insert(result[i].begin(), Left);
					result[i].push_back(Right);
					for (int t = i + 1; t < result.size() - 2; ++t)//ÿһλǰ��
					{
						result[t] = result[t + 2];
					}
					result.pop_back();//ȥ�������λ
					result.pop_back();
					ResultRewrite = 1;
					break;
				}
			}
		}
	}


    return "";
}

//����result
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
 ����һ��map���͵Ĵʵ�e0Dict����ʽ���£�
 (Noun,[Noun,stench,breeze...])
 (Verb,[Verb,is,see,smell...])
 ...
 */
 map<string, vector<string>> constructDic() {
     //������������
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

     //���춯������
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

     //�������ݴ�����
     vector<string> adjectiveDic = vector<string>();
     adjectiveDic.push_back("Adjective");
     adjectiveDic.push_back("right");
     adjectiveDic.push_back("left");
     adjectiveDic.push_back("east");
     adjectiveDic.push_back("dead");
     adjectiveDic.push_back("back");
     adjectiveDic.push_back("smelly");

     //���츱������
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

     //�����������
     vector<string> pronounDic = vector<string>();
     pronounDic.push_back("Pronoun");
     pronounDic.push_back("me");
     pronounDic.push_back("you");
     pronounDic.push_back("I");
     pronounDic.push_back("it");

     //������������
     vector<string> nameDic = vector<string>();
     nameDic.push_back("Name");
     nameDic.push_back("John");
     nameDic.push_back("Mary");
     nameDic.push_back("Boston");
     nameDic.push_back("Aristotle");

     //����ڴ�����
     vector<string> articleDic = vector<string>();
     articleDic.push_back("Article");
     articleDic.push_back("the");
     articleDic.push_back("a");
     articleDic.push_back("an");

     //����������
     vector<string> prepositionDic = vector<string>();
     prepositionDic.push_back("Preposition");
     prepositionDic.push_back("to");
     prepositionDic.push_back("in");
     prepositionDic.push_back("on");
     prepositionDic.push_back("near");

     //������������
     vector<string> conjunctionDic = vector<string>();
     conjunctionDic.push_back("Conjunction");
     conjunctionDic.push_back("and");
     conjunctionDic.push_back("or");
     conjunctionDic.push_back("but");

     //������������
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

     //���ݲ�ͬ��𵥴����鹹��ʵ�map
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
����һ���﷨�������飬��ʽ���£�
(
    (S, NP, VP),
    (S, S, Conjunction, S),
    ...
)

������ÿ������ĵ�һ��Ԫ��Ϊ�����ս��������
(S, NP, VP)��Ӧ S->NP VP �﷨����
*/
 vector<vector<string>> constructGrammarRules() {

     //���������ս��ΪS���﷨����
     vector<string> e0GrammarRule1 = vector<string>();
     e0GrammarRule1.push_back("S");
     e0GrammarRule1.push_back("NP");
     e0GrammarRule1.push_back("VP");

     //���������ս��ΪS���﷨����
     vector<string> e0GrammarRule2 = vector<string>();
     e0GrammarRule2.push_back("S");
     e0GrammarRule2.push_back("S");
     e0GrammarRule2.push_back("Conjunction");
     e0GrammarRule2.push_back("S");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule3 = vector<string>();
     e0GrammarRule3.push_back("NP");
     e0GrammarRule3.push_back("Pronoun");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule4 = vector<string>();
     e0GrammarRule4.push_back("NP");
     e0GrammarRule4.push_back("Name");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule5 = vector<string>();
     e0GrammarRule5.push_back("NP");
     e0GrammarRule5.push_back("Noun");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule6 = vector<string>();
     e0GrammarRule6.push_back("NP");
     e0GrammarRule6.push_back("Article");
     e0GrammarRule6.push_back("Noun");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule7 = vector<string>();
     e0GrammarRule7.push_back("NP");
     e0GrammarRule7.push_back("Digit");
     e0GrammarRule7.push_back("Digit");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule8 = vector<string>();
     e0GrammarRule8.push_back("NP");
     e0GrammarRule8.push_back("NP");
     e0GrammarRule8.push_back("PP");

     //���������ս��ΪNP���﷨����
     vector<string> e0GrammarRule9 = vector<string>();
     e0GrammarRule9.push_back("NP");
     e0GrammarRule9.push_back("NP");
     e0GrammarRule9.push_back("RelClause");

     //���������ս��ΪVP���﷨����
     vector<string> e0GrammarRule10 = vector<string>();
     e0GrammarRule10.push_back("VP");
     e0GrammarRule10.push_back("Verb");

     //���������ս��ΪVP���﷨����
     vector<string> e0GrammarRule11 = vector<string>();
     e0GrammarRule11.push_back("VP");
     e0GrammarRule11.push_back("VP");
     e0GrammarRule11.push_back("NP");

     //���������ս��ΪVP���﷨����
     vector<string> e0GrammarRule12 = vector<string>();
     e0GrammarRule12.push_back("VP");
     e0GrammarRule12.push_back("VP");
     e0GrammarRule12.push_back("Adjective");

     //���������ս��ΪVP���﷨����
     vector<string> e0GrammarRule13 = vector<string>();
     e0GrammarRule13.push_back("VP");
     e0GrammarRule13.push_back("VP");
     e0GrammarRule13.push_back("PP");

     //���������ս��ΪVP���﷨����
     vector<string> e0GrammarRule14 = vector<string>();
     e0GrammarRule14.push_back("VP");
     e0GrammarRule14.push_back("VP");
     e0GrammarRule14.push_back("Adverb");

     //���������ս��ΪPP���﷨����
     vector<string> e0GrammarRule15 = vector<string>();
     e0GrammarRule15.push_back("PP");
     e0GrammarRule15.push_back("Preposition");
     e0GrammarRule15.push_back("NP");

     //���������ս��ΪRelClause���﷨����
     vector<string> e0GrammarRule16 = vector<string>();
     e0GrammarRule16.push_back("RelClause");
     e0GrammarRule16.push_back("that");
     e0GrammarRule16.push_back("VP");

     //�����﷨����
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

 //�������Ϊ the wumpus is dead
 vector<string> inputSentence() {
     vector<string> sentence = vector<string>();
	 sentence = { "the", "wumpus" ,"is" ,"dead" };
	 return sentence;
 }