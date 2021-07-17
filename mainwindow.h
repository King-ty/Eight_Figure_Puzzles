#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextBrowser>
#include <QTextCursor>
#include <QMessageBox>
#include <QString>
#include <QTime>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include "kernel.h"
#include "treewindow.h"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    //帮助菜单的“关于”选项的槽函数
    void MsgAbout();

    //帮助菜单的“使用说明”选项的槽函数
    void MsgManual();

private:
    Ui::MainWindow *ui;

    static const int STATE_SIZE=9;//状态大小为9
    static const int SLEEP_TM=500;//默认自动执行间隔为500ms

    Search my_search;//实例化查找对象
    vector<State> all_path;
    vector<string> path;//存储路径
    QLineEdit *origin_state[STATE_SIZE],*now_state[STATE_SIZE],*end_state[STATE_SIZE];//分别与ui的起始、当前、结束状态相对应
    int h_func;//记录所使用的启发函数
    int path_cnt;//路径计数器，用于展示路径
    TreeWindow *pTreeWindow;
    string origin_state_str,end_state_str;

    //初始化ui对象的参数
    void InitMembers();

    //初始化信号槽连接情况
    void InitConnections();

    //设置一个状态中所有的QLineEdit的可读状态
    void SetStateReadonly(QLineEdit *state[STATE_SIZE],bool flag=true);

    //检查某一状态的输入是否合法
    bool CheckInput(string input);

    //将一个状态的所有输入转为一个QString返回
    QString GetInput(QLineEdit *input[STATE_SIZE]);

    //将一个状态写入一个字符串
    void WriteState(QString str,QLineEdit *state[STATE_SIZE]);

    //获得一个随机状态
    string GetRandState();

    //清空一个状态
    void ClearState(QLineEdit *state[STATE_SIZE]);

    //自己封装的Sleep函数，因为Qt中Sleep函数不能正常工作
    void MySleep(int slp_tm);

    //判断是否有解
    bool HaveSolu(string ,string);

    //获得搜索树
    void GetTree();

private slots://ui中按钮等的槽函数
    //“随机生成”按钮的槽函数
    void on_btn_randstr_clicked();

    //“开始搜索”按钮的槽函数
    void on_btn_start_clicked();

    //“单步执行”按钮的槽函数
    void on_btn_step_run_clicked();

    //“自动执行”按钮的槽函数
    void on_btn_auto_run_clicked();

    //“初始化”按钮的槽函数
    void on_btn_clear_clicked();

    //“搜索树”按钮的槽函数
    void on_btn_tree_clicked();

    //h(n)_0的checkbox的槽函数
    void on_cbox_h0_clicked();

    //h(n)_1的checkbox的槽函数
    void on_cbox_h1_clicked();

    //“输出路径”文本框的槽函数
    void autoScroll();
};

#endif // MAINWINDOW_H
