#ifndef APRIORI_H
#define APRIORI_H

#include <vector>
#include <string>
#include <map>
using namespace std;

struct apriorNode
{
    vector<int> candidate; // 候选项
    int cnt;               // 候选项出现次数
};
class Apriori
{
private:
    vector<vector<int>> dataBase;  // 数据集
    int minSupport;                // 最小支持度
    map<int, int> C0;              // 用一组对应关系代替C0
    vector<vector<vector<int>>> C; // 候选项集，C[0]为空，用C0表示
    vector<vector<vector<int>>> L; // 频繁项集

public:
    void read_data_base(string fileName); // 读取数据集
    void set_min_support(int minSupport); // 设置最小支持度，即频繁项集最少出现的次数
    void run_apriori();                   // 运行Apriori算法
    void print_apriori_res();             // 输出频繁项集
};

#endif