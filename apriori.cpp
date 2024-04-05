#include "apriori.h"
#include <fstream>
#include <iostream>
using namespace std;

void Apriori::read_data_base(string fileName)
{
    vector<unordered_map<int, bool>> dataBase;
    ifstream infile;
    infile.open(fileName);
    string line;
    int i = 0;
    C.push_back(set<set<int>>());
    while (getline(infile, line)) // 读取文件中的每一行
    {
        dataBase.push_back(unordered_map<int, bool>());
        int num = 0;
        for (int j = 0; j < line.size(); j++)
            if (line[j] != ' ')
                num = num * 10 + line[j] - '0';
            else
            {
                dataBase[i].insert(make_pair(num, true)); // 遇到空格就将数字加入dataBase
                set<int> temp;
                temp.insert(num);
                C[0].insert(temp); // 加入C
                num = 0;
            }
        i++;
    }
    infile.close();
    this->dataBase = dataBase;
}
void Apriori::set_min_support(int minSupport)
{
    int minCnt = (dataBase.size() * minSupport % 100 == 0) ? (dataBase.size() * minSupport / 100) : (dataBase.size() * minSupport / 100 + 1); // 频繁项集的最小出现次数，向上取整
    this->minSupport = minCnt;
}
void Apriori::run_apriori()
{
    // 由C[0]生成L[0]
    L.push_back(set<set<int>>());
    vector<apriorNode> table;
    for (auto it : C[0])
    {
        apriorNode temp;
        temp.candidate = it, temp.cnt = 0;
        table.push_back(temp);
    }
    for (int i = 0; i < dataBase.size(); i++)
    {
        for (int ii = 0; ii < table.size(); ii++)
        {
            bool flag = 1;
            for (auto it : table[ii].candidate)
                if (dataBase[i].find(it) == dataBase[i].end()) // 候选项的每个元素在数据集中不存在
                {
                    flag = 0;
                    break;
                }
            if (flag) // 数据集的该行包含该候选项
                table[ii].cnt++;
        }
    }
    for (int i = 0; i < table.size(); i++)
        if (table[i].cnt >= minSupport)
            L[0].insert(table[i].candidate);

    int k = 0;
    while (L[k].size() > 1)
    {
        // 生成C[k+1]
        C.push_back(set<set<int>>());
        for (auto it = L[k].begin(); it != L[k].end(); it++)
        {
            for (auto it2 = it; it2 != L[k].end(); it2++)
            {
                set<int> temp = *it;
                temp.insert((*it2).begin(), (*it2).end());
                if (temp.size() == k + 2)
                    C[k + 1].insert(temp);
            }
        }

        // 由C[k+1]生成L[k+1]
        L.push_back(set<set<int>>());
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
                L[k + 1].insert(newTable[i].candidate);
        k++;
    }
}
void Apriori::print_apriori_res()
{
    // 测试数据集是否正确读入
    cout << "数据集: " << endl;
    for (int i = 0; i < dataBase.size(); i++)
    {
        for (auto it : dataBase[i])
            cout << it.first << " ";
        cout << endl;
    }
    cout << endl;

    // 测试最小支持度是否正确
    cout << "最小支持度: " << minSupport << endl;
    cout << endl;

    // 测试C是否正确生成
    cout << "候选项集C: " << endl;
    for (int i = 0; i < C.size(); i++)
    {
        for (auto &it : C[i])
        {
            for (auto &it2 : it)
                cout << it2 << " ";
            cout << endl;
        }
    }
    cout << endl;

    // 测试L是否正确生成
    cout << "频繁项集L: " << endl;
    for (int i = 0; i < L.size(); i++)
    {
        for (auto &it : L[i])
        {
            for (auto &it2 : it)
                cout << it2 << " ";
            cout << endl;
        }
    }

    cout << "频繁项集L的元素个数: " << endl;
    for (int i = 0; i < L.size(); i++)
        cout << "L[" << i + 1 << "]: " << L[i].size() << endl;
}