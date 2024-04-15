#include "apriori.h"
#include <fstream>
#include <iostream>
using namespace std;

void Apriori::read_data_base(string fileName)
{
    vector<unordered_set<int>> dataBase;
    ifstream infile;
    infile.open(fileName);
    string line;

    int i = 0;
    map<int, int> C;              // 用一组对应关系代替C0
    while (getline(infile, line)) // 读取文件中的每一行
    {
        dataBase.push_back(unordered_set<int>());
        int num = 0;
        for (int j = 0; j < line.size(); j++)
            if (line[j] != ' ')
                num = num * 10 + line[j] - '0';
            else
            {
                dataBase[i].insert(num);    // 遇到空格就将数字加入dataBase
                if (C.find(num) == C.end()) // 第一次出现该数字
                    C.insert(make_pair(num, 1));
                else
                    C[num]++;
                num = 0;
            }
        i++;
    }
    infile.close();

    this->dataBase = dataBase;
    this->C0 = C;
}
void Apriori::set_min_support(int minSupport)
{
    int minCnt = (dataBase.size() * minSupport % 100 == 0) ? (dataBase.size() * minSupport / 100) : (dataBase.size() * minSupport / 100 + 1); // 频繁项集的最小出现次数，向上取整
    this->minSupport = minCnt;
}
void Apriori::run_apriori()
{
    // 由C[0]生成L[0]
    L.push_back(vector<set<int>>());
    for (auto it : C0)
        if (it.second >= minSupport)
            L[0].push_back(set<int>({it.first}));

    int k = 0;
    C.push_back(vector<set<int>>());
    while (L[k].size() > 1) // 如果L[k]中只有一个频繁项集，则无法生成C[k+1]
    {
        // 生成C[k+1]
        C.push_back(vector<set<int>>());
        for (int i = 0; i < L[k].size(); i++)
        {
            for (int j = i + 1; j < L[k].size(); j++)
            {
                // 连接
                bool isEqual = 1;
                for (auto it1 = L[k][i].begin(), it2 = L[k][j].begin(); it1 != prev(L[k][i].end()); it1++, it2++)
                    if (*it1 != *it2)
                    {
                        isEqual = 0;
                        break;
                    }
                if (isEqual) // 除最后一个元素外都相等
                {
                    set<int> l;
                    for (auto it1 = L[k][i].begin(); it1 != L[k][i].end(); it1++)
                        l.insert(*it1);
                    l.insert(*prev(L[k][j].end())); // l可以进入C[k+1]

                    // 剪枝
                    bool Flag = 1;     // 判断l是否满足条件
                    for (auto it1 : l) // 排除一个元素
                    {
                        set<int> subSet; // l的子集
                        for (auto it2 : l)
                            if (it2 != it1)
                                subSet.insert(it2);
                        bool flag = 0; // 判断子集是否在L[k]中
                        for (auto it3 : L[k])
                            if (subSet == it3)
                            {
                                flag = 1;
                                break;
                            }
                        if (!flag)
                        {
                            Flag = 0;
                            break;
                        }
                    }
                    if (Flag)
                        C[k + 1].push_back(l);
                }
                else
                    break; // 后续也不会有相等的了
            }
        }

        // 由C[k+1]生成L[k+1]
        L.push_back(vector<set<int>>());
        vector<apriorNode> newTable;
        for (auto it : C[k + 1])
        {
            apriorNode temp;
            temp.candidate = it, temp.cnt = 0;
            newTable.push_back(temp);
        }
        for (int i = 0; i < dataBase.size(); i++)
        {
            for (int ii = 0; ii < newTable.size(); ii++)
            {
                bool flag = 1;
                for (auto it : newTable[ii].candidate)
                    if (dataBase[i].find(it) == dataBase[i].end()) // 候选项的每个元素在数据集中不存在
                    {
                        flag = 0;
                        break;
                    }
                if (flag) // 数据集的该行包含该候选项
                    newTable[ii].cnt++;
            }
        }
        for (int i = 0; i < newTable.size(); i++)
            if (newTable[i].cnt >= minSupport)
                L[k + 1].push_back(newTable[i].candidate);
        k++;
    }
}
void Apriori::print_apriori_res()
{
    // 测试数据集是否正确读入
    /* cout << "数据集: " << endl;
    for (int i = 0; i < dataBase.size(); i++)
    {
        for (auto it : dataBase[i])
            cout << it << " ";
        cout << endl;
    }
    cout << endl; */

    // 测试最小支持度是否正确
    /* cout << "最小支持度: " << minSupport << endl;
    cout << endl; */

    // 测试C是否正确生成
    /* cout << "候选项集C: " << endl;
    for (int i = 1; i < C.size(); i++)
    {
        cout << "C[" << i + 1 << "]: " << endl;
        for (auto &it : C[i])
        {
            cout << "{ ";
            for (auto &it2 : it)
                cout << it2 << " ";
            cout << "}" << endl;
        }
    }
    cout << endl; */

    // 测试L是否正确生成
    /* cout << "频繁项集L: " << endl;
    for (int i = 0; i < L.size(); i++)
    {
        cout << "L[" << i + 1 << "]: " << endl;
        for (auto &it : L[i])
        {
            cout << "{ ";
            for (auto &it2 : it)
                cout << it2 << " ";
            cout << "}" << endl;
        }
    }
    cout << endl; */

    cout << "频繁项集L的元素个数: " << endl;
    for (int i = 0; i < L.size(); i++)
        if (L[i].size() > 0)
            cout << "L[" << i + 1 << "]: " << L[i].size() << endl;
}