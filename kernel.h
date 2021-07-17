#ifndef MY_KERNEL_H
#define MY_KERNEL_H

#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <time.h>

//#include "mainwindow.h"

using namespace std;

typedef long long ll;

//#define mp(a,b) make_pair(a,b)

class Search;

class State
{
private:
    string st;//存储该状态的字符串，本实验中使用长度为9的字符串存储状态信息
    string pre;//存储该节点的前驱的字符串
    int f, g, h;//分别对应评估函数的f(n)，g(n)，h(n)

public:
    State() {}
    State(string st, string pre = "", int g = 0, int h = 0);//构造函数，利用默认参数实现1~4参数的构造
    void set(string st, string pre = "", int g = 0, int h = 0);//设置(更新)状态函数
    bool operator<(const State temp) const;//重载<号，根据f值比较大小，用于优先队列排序
    bool operator==(const State temp) const;//重载==号
    // friend void Search::GetSolution();

    //声明友元类
    friend class Search;
    friend class MainWindow;
};

class Search
{
private:
    static const int STATE_SIZE = 9;//状态大小
    static const int MOV_NUM = 4;//移动情况的数量，因为只有上下左右4种移动方式，因此该值为4

    bool error_state;//用于描述输入状态是否有错
    bool finded;//用于判断是否能找到解，或是否已经找到解
    string origin_state, end_state;//起始字符串和目标字符串
    int run_time;//用于记录搜索时间

    priority_queue<State> open;//优先队列类型的open表
    map<string,string> close;//map类型的close表，其中键为该状态的字符串，值为其前驱状态的字符串
    vector<State> all_path;//用于记录所有搜索中所有进入close的节点
    vector<string> path;//用于记录最佳路径
    int h_func;//用于记录选择哪个启发函数
    /*  0 1 2
        3 4 5
        6 7 8 */
    //mov数组表示每一个位置的可能的移动位置，其中-1表示不可达
    int mov[STATE_SIZE][MOV_NUM] = {
        {-1, 1, -1, 3},
        {0, 2, -1, 4},
        {1, -1, -1, 5},
        {-1, 4, 0, 6},
        {3, 5, 1, 7},
        {4, -1, 2, 8},
        {-1, 7, 3, -1},
        {6, 8, 4, -1},
        {7, -1, 5, -1}};

    static const int GET_H_NUM = 2;//记录启发函数总数量，本题共用到2种启发函数
    int (*get_h)(string st,string end_state);//启发函数的函数指针，用于简化操作
public:
    Search(){}
    Search(string origin_state, string end_state = "123456780", int h_func = 0);//构造函数，利用默认参数实现1~3参数的构造
    void set(string origin_state, string end_state = "123456780", int h_func = 0);//设置(更新)Search的函数，与上面构造函数的不同点在于需要清空(clear)open、clpse等容器
    void GetPath();//寻找并记录最佳路径
    bool CheckState();//判断输入是否合法，在前端中有类似实现，此处用于单独调试后端时使用
    void GetSolution();//搜索答案

    //声明友元类和友元函数(启发函数)
    friend class MainWindow;
    friend int get_h_0(string st,string end_state);//启发函数h_0(n)
    friend int get_h_1(string st,string end_state);//启发函数h_1(n)
};

int get_h_0(string st,string end_state);
int get_h_1(string st,string end_state);

#endif
