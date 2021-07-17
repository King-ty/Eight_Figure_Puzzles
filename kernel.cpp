#include "kernel.h"
using namespace std;

State::State(string st, string pre, int g, int h)
{
    this->st = st;
    this->pre = pre;
    this->g = g;
    this->h = h;
    this->f = g + h;
}

void State::set(string st, string pre, int g, int h)
{
    this->st = st;
    this->pre = pre;
    this->g = g;
    this->h = h;
    this->f = g + h;
}

bool State::operator<(const State temp) const
{
    return f > temp.f;
}

bool State::operator==(const State temp) const
{
    return st == temp.st;
}

Search::Search(string origin_state, string end_state, int search_func)
{
    this->origin_state = origin_state;
    this->end_state = end_state;
//    this->sleep_time = sleep_time;
//    this->h_func = search_func;
    if(search_func==1)
    {
        this->get_h=get_h_1;
    }
    else
    {
        this->get_h=get_h_0;
    }
    this->error_state = false;
    this->finded = false;
}

void Search::set(string origin_state, string end_state, int search_func)
{
    this->origin_state = origin_state;
    this->end_state = end_state;
    if(search_func==1)
    {
        this->get_h=get_h_1;
    }
    else
    {
        this->get_h=get_h_0;
    }
    this->error_state = false;
    this->finded = false;

    //相比于构造函数，set时还要将容器清空
    all_path.clear();
    path.clear();
    close.clear();
    while(!open.empty())
        open.pop();
}

void Search::GetPath()
{
//    path.clear();
    for(string temp=end_state;temp!=" ";temp=close[temp])
    {
        path.push_back(temp);//从后向前寻找路径
    }
    reverse(path.begin(), path.end());//将数组转置
}

bool Search::CheckState()//判断输入正确性，仅用于后端单独测试，不详细介绍
{
    if (origin_state.length() != STATE_SIZE || end_state.length() != STATE_SIZE)
        return false;
    bool hd[10][2] = {0};
    for (int i = 0; i < STATE_SIZE; ++i)
    {
        if (origin_state[i] > '9' || origin_state[i] < '0' || end_state[i] > '9' || end_state[i] < '0')
        {
            return false;
        }
        hd[origin_state[i] - '0'][0] = 1;
        hd[end_state[i] - '0'][1] = 1;
    }
    for (int i = 0; i < STATE_SIZE; ++i)
    {
        if (!hd[i][0] || !hd[i][1])
            return false;
    }
    return true;
}

void Search::GetSolution()
{
    if (!CheckState())//首先判断输入正确性，仅用于后端单独测试
    {
        error_state = true;
        return;
    }
    run_time=clock();//开始计时
    finded = false;
    open.push(State(origin_state, " ", 0, 0));//open放入初始状态
    string temp_str;
    State state;
    while (!open.empty() && !finded)//还没有找到且open不为空
    {
        state = open.top();//获得当前节点
        open.pop();
        temp_str = state.st;
        all_path.push_back(state);
        close[temp_str]=state.pre;//当前节点放入close

        int pos_0 = temp_str.find('0');
        for (int i = 0; i < MOV_NUM; ++i)
        {
            if (mov[pos_0][i] >= 0)
            {
                swap(temp_str[pos_0], temp_str[mov[pos_0][i]]);//获得新状态
                if (temp_str == end_state)//搜索到目标状态了
                {
//                    all_path.push_back(temp_str);
                    close[temp_str]=state.st;
                    run_time=clock()-run_time;//计算搜索用时
                    GetPath();//寻找路径
                    finded = true;//置已找到
                    while (!open.empty())
                        open.pop();//清空open表
                    return;
                }
                if (!close.count(temp_str))
                {
                    //扩展新节点，放入open
                    State new_st(temp_str, state.st, state.g + 1, get_h(temp_str,end_state));
                    open.push(new_st);
                }
                swap(temp_str[pos_0], temp_str[mov[pos_0][i]]);
            }
        }
        while(!open.empty()&&close.count(open.top().st))
            open.pop();//清除已在close表中但同时也在open中的重复节点
    }
}

int get_h_0(string st,string end_state)//曼哈顿距离
{
    int ret = 0;
    for (int i = 0; i < Search::STATE_SIZE; ++i)
    {
        for (int j = 0; j < Search::STATE_SIZE; ++j)
        {
            if (end_state[j] == st[i])
            {
                ret += abs(j % 3 - i % 3) + abs(j / 3 - i / 3);//计算曼哈顿距离
                break;
            }
        }
    }
    return ret;
}

int get_h_1(string st,string end_state)//位置不正确的格子数
{
    int ret = 0;
    for (int i = 0; i < Search::STATE_SIZE; ++i)
    {
        if (end_state[i] != st[i])
        {
            ret++;
        }
    }
    return ret;
}


//测试后端用主函数，已注释
//int main()
//{//int main()
//{
//    string origin, end;
//    for (;;)
//    {
//        cin >> origin >> end;
//        Search my_search(origin, end, 0, 500);
//        my_search.GetSolution();
//        if (my_search.error_state)
//        {
//            cout << "state error!" << endl;
//            continue;
//        }
//        else if (!my_search.finded)
//        {
//            cout << "cannot find solution!" << endl;
//            continue;
//        }
//        for (int i = 0; i <= my_search.path.size(); ++i)
//        {
//            cout << my_search.path[i] << endl;
//        }
//        return 0;
//    }
//}
//    string origin, end;
//    for (;;)
//    {
//        cin >> origin >> end;
//        Search my_search(origin, end, 0, 500);
//        my_search.GetSolution();
//        if (my_search.error_state)
//        {
//            cout << "state error!" << endl;
//            continue;
//        }
//        else if (!my_search.finded)
//        {
//            cout << "cannot find solution!" << endl;
//            continue;
//        }
//        for (int i = 0; i <= my_search.path.size(); ++i)
//        {
//            cout << my_search.path[i] << endl;
//        }
//        return 0;
//    }
//}
