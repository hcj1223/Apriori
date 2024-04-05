#ifndef APRIORI_H
#define APRIORI_H
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
using namespace std;

struct apriorNode
{
    set<int> candidate; // 候选项
    int cnt;            // 候选项出现次数
};
class Apriori
{
private:
    vector<unordered_map<int, bool>> dataBase; // 数据集
    int minSupport;                            // 最小支持度
    vector<set<set<int>>> C;                   // 候选项集
    vector<set<set<int>>> L;                   // 频繁项集

public:
    void read_data_base(string fileName); // 读取数据集
    void set_min_support(int minSupport); // 设置最小支持度
    void run_apriori();                   // 运行Apriori算法
    void print_apriori_res();             // 输出频繁项集
};

#endif