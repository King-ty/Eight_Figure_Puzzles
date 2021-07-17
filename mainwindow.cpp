#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(GetTickCount());//设置随机数种子
    InitMembers();
    InitConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitMembers()
{
    setWindowTitle("实验1-8数码");//设置窗口标题

    //设置默认所用启发函数为h(n)_0
    ui->cbox_h0->setChecked(true);
    ui->cbox_h1->setChecked(false);
    h_func=0;

    //将三个状态存入数组方便后续操作
    origin_state[0]=ui->origin_0;
    origin_state[1]=ui->origin_1;
    origin_state[2]=ui->origin_2;
    origin_state[3]=ui->origin_3;
    origin_state[4]=ui->origin_4;
    origin_state[5]=ui->origin_5;
    origin_state[6]=ui->origin_6;
    origin_state[7]=ui->origin_7;
    origin_state[8]=ui->origin_8;

    now_state[0]=ui->now_0;
    now_state[1]=ui->now_1;
    now_state[2]=ui->now_2;
    now_state[3]=ui->now_3;
    now_state[4]=ui->now_4;
    now_state[5]=ui->now_5;
    now_state[6]=ui->now_6;
    now_state[7]=ui->now_7;
    now_state[8]=ui->now_8;

    end_state[0]=ui->end_0;
    end_state[1]=ui->end_1;
    end_state[2]=ui->end_2;
    end_state[3]=ui->end_3;
    end_state[4]=ui->end_4;
    end_state[5]=ui->end_5;
    end_state[6]=ui->end_6;
    end_state[7]=ui->end_7;
    end_state[8]=ui->end_8;

    //设置按钮的初始状态
    ui->btn_auto_run->setDisabled(true);
    ui->btn_step_run->setDisabled(true);
    ui->btn_tree->setDisabled(true);

    //设置状态now_state为只读，其他可写
    SetStateReadonly(origin_state,false);
    SetStateReadonly(now_state,true);
    SetStateReadonly(end_state,false);
}

void MainWindow::InitConnections()
{
    //创建连接，其中btn的连接由于其槽函数命名的特殊性，可以省略
    connect(ui->action_about,SIGNAL(triggered()),this,SLOT(MsgAbout()));
    connect(ui->action_use,SIGNAL(triggered()),this,SLOT(MsgManual()));

    connect(ui->path_text, SIGNAL(cursorPositionChanged()), this, SLOT(autoScroll()));
}

void MainWindow::MsgAbout()
{
    QString title="关于8数码应用";
    QString info=
            "人工智能大作业1，八数码展示程序\n"
            "开发者 : 田宇\n"
            "Email : king-ty@foxmail.com\n";
    QMessageBox::information(this,title,info,QMessageBox::Ok,QMessageBox::Ok);//给出提示信息窗口
}

void MainWindow::MsgManual()
{
    QString title="使用说明";
    QString info="请按照按钮及标签的提示进行操作，如有疑问欢迎联系开发者";
    QMessageBox::information(this,title,info,QMessageBox::Ok,QMessageBox::Ok);//给出提示信息窗口
}

void MainWindow::SetStateReadonly(QLineEdit *state[STATE_SIZE],bool flag)
{
    for(int i=0;i<STATE_SIZE;++i)
    {
        state[i]->setReadOnly(flag);//设置单个QLineEdit为只读
    }
}

QString MainWindow::GetInput(QLineEdit *input[STATE_SIZE])\
{
    QString ret="";
    for(int i=0;i<STATE_SIZE;++i)
    {
        if(input[i]->text()>='0'&&input[i]->text()<='9')
        {
            ret+=input[i]->text();
        }
        else
        {
            ret+='0';//非数字直接处理为'0'
        }
    }
    return ret;
}

bool MainWindow::CheckInput(string input)
{
    if(input.size()!=STATE_SIZE)
        return false;//输入长度不合法的情况
    bool ys[STATE_SIZE+1]={0};
//    string str=input.toStdString();
    for(int i=0;i<STATE_SIZE;++i)
    {
        if(input[i]>'8'||input[i]<'0'||ys[input[i]-'0'])
            return false;//数字超过合理范围或出现重复数字
        ys[input[i]-'0']=1;
    }
    return true;
}

void MainWindow::WriteState(QString str,QLineEdit *state[STATE_SIZE])
{
    for(int i=0;i<STATE_SIZE;++i)
    {
        if(str[i]=='0')
        {
            state[i]->setText(" ");//数字'0'处理为" "，使显示美观
        }
        else
        {
            state[i]->setText(str.mid(i,1));
        }
    }
}

string MainWindow::GetRandState()
{
    string ret="";
    bool us[STATE_SIZE+1]={0};
    for(int i=0;i<STATE_SIZE;++i)
    {
        int temp=rand()%STATE_SIZE;
        while(us[temp])
        {
            temp=rand()%STATE_SIZE;//循环处理随机数字出现重复的情况，直到不出现重复数字
        }
        us[temp]=1;
        ret+=temp+'0';
    }
    return ret;
}

void MainWindow::ClearState(QLineEdit *state[STATE_SIZE])
{
    for(int i=0;i<STATE_SIZE;++i)
    {
        state[i]->clear();//清空单个QLineEdit
    }
}

void MainWindow::MySleep(int slp_tm)
{
    QTime slp;
    slp.start();
    while(slp.elapsed() < slp_tm) {
        QCoreApplication::processEvents();//让程序处理那些还没有处理的事件，然后再把使用权返回给调用者
    }
}

bool MainWindow::HaveSolu(string origin_str,string end_str)
{
    //把两个状态除了'0'之外的数字从上到下、从左到右分别组成2个数组，判断其逆序对的数量，如果逆序对数量奇偶性相同则必有解，否则必无解。
    int num0=0,num1=0;
    for(int i=1;i<STATE_SIZE;++i)
    {
        for(int j=0;j<i;++j)
        {
            if(origin_str[i]!='0'&&origin_str[j]>origin_str[i])
                num0++;
            if(end_str[i]!='0'&&end_str[j]>end_str[i])
                num1++;
        }
    }
    return num0%2==num1%2;
}

void MainWindow::GetTree()
{
    string dot_name="tree.dot";
    ofstream dot_out(dot_name);
    dot_out<<"digraph tree {\n";
    dot_out<<"node [shape = record];\n";
    string temp_str,from,to;
    int node_cnt=0;
    const int NODE_MAX=300;
    for(int i=0;i<all_path.size()&&node_cnt<NODE_MAX;++i)
    {
        temp_str=all_path[i].st;
        from=temp_str.substr(0,3)+"\\n"+temp_str.substr(3,3)+"\\n"+temp_str.substr(6,3)+"\\n"+"f(n)="+to_string(all_path[i].f);
        int pos_0 = temp_str.find('0');
        for (int j = 0; j < Search::MOV_NUM&&node_cnt<NODE_MAX; ++j)
        {
            if (my_search.mov[pos_0][j] >= 0)
            {
                swap(temp_str[pos_0], temp_str[my_search.mov[pos_0][j]]);
                to=temp_str.substr(0,3)+"\\n"+temp_str.substr(3,3)+"\\n"+temp_str.substr(6,3)+"\\n"+"f(n)="+to_string(all_path[i].g+1+my_search.get_h(temp_str,my_search.end_state));//
                if(count(path.begin(),path.end(),temp_str))
                {
                    to+="[color=red]";
                }
                dot_out<<"\t\""<<from<<"\"->\""<<to<<"\";\n";
                node_cnt++;
                swap(temp_str[pos_0], temp_str[my_search.mov[pos_0][j]]);
            }
        }
    }
    dot_out<<"}\n";
}

void MainWindow::on_btn_randstr_clicked()
{
//    ui->btn_randstr->setDisabled(true);
    ClearState(now_state);//首先将当前状态(now_state)清空
    string origin_str=GetRandState();//获得随机的初始状态字符串
    string end_str=GetRandState();//获得随机的结束状态字符串

    //如果初始状态与结束状态无解则循环生成新的结束状态字符串，直到有解
    while(!HaveSolu(origin_str,end_str))
    {
        end_str=GetRandState();
    }

    //将起始和结束状态写入相应的字符串
    QString os_for_write=QString::fromStdString(origin_str);
    QString es_for_write=QString::fromStdString(end_str);
    WriteState(os_for_write,origin_state);
    WriteState(es_for_write,end_state);
//    ui->btn_randstr->setDisabled(false);
}

void MainWindow::on_btn_start_clicked()
{
    path_cnt=0;
    string info;

    //搜索过程中设置所有按钮均为不可使用状态
    ui->btn_start->setDisabled(true);
    ui->btn_randstr->setDisabled(true);
    ui->btn_tree->setDisabled(true);
    ui->btn_clear->setDisabled(true);

    //搜索过程中设置起始和结束状态为只读
    SetStateReadonly(origin_state,true);
    SetStateReadonly(end_state,true);

    //获得起始和结束状态字符串
    origin_state_str=GetInput(origin_state).toStdString();
    end_state_str=GetInput(end_state).toStdString();

    //若输入有误，给出输入有误的提示
    if(!CheckInput(origin_state_str)||!CheckInput(end_state_str))
    {
        QMessageBox::warning(this,"警告","输入错误！请检查输入",QMessageBox::Ok,QMessageBox::Ok);
        goto QUIT;
    }

    //若输入无解，给出相应的提示
    if(!HaveSolu(origin_state_str,end_state_str))
    {
        QMessageBox::information(this,"提示","当前情况无解！请重新输入",QMessageBox::Ok,QMessageBox::Ok);
        goto QUIT;
    }

    my_search.set(origin_state_str,end_state_str,h_func);//设置搜索类的状态
    my_search.GetSolution();//搜索过程

    //获得路径
    all_path=my_search.all_path;
    path=my_search.path;

    //展示搜索完成信息
    info="已搜索完成，共"+to_string(path.size())+"步";
    QMessageBox::information(this,"提示",QString::fromStdString(info),QMessageBox::Ok,QMessageBox::Ok);
    ui->label_time->setText("搜索时间 : "+QString::number(my_search.run_time)+"ms");//展示搜索时间
    ui->label_steps->setText("总步数 : "+QString::number(path.size()));//展示搜索步数

    //搜索完成后设置所有按钮可用
    ui->btn_start->setDisabled(false);
    ui->btn_auto_run->setDisabled(false);
    ui->btn_step_run->setDisabled(false);
    ui->btn_tree->setDisabled(false);
QUIT://退出标签，供goto使用
    ui->btn_randstr->setDisabled(false);
    ui->btn_clear->setDisabled(false);

    SetStateReadonly(origin_state,false);
    SetStateReadonly(end_state,false);
}

void MainWindow::on_btn_step_run_clicked()
{
    if(path_cnt==0)
        ui->path_text->clear();//清空文本框

    WriteState(QString::fromStdString(path[path_cnt]),now_state);//更新当前状态

    //更新搜索过程框中的文字说明
    ui->path_text->insertPlainText(QString::fromStdString("第"+to_string(path_cnt+1)+"步 : \n"));
    for(int j=0;j<STATE_SIZE;++j)
    {
        ui->path_text->insertPlainText(QString::fromStdString(path[path_cnt].substr(j,1)+" "));
        if(j%3==2)
            ui->path_text->insertPlainText("\n");
    }
    ui->path_text->insertPlainText("\n");

    path_cnt++;//更新路径计数器
    if(path_cnt==path.size())
    {
        ui->path_text->insertPlainText("执行完毕！");
        path_cnt=0;//重置路径计数器
    }
}

void MainWindow::on_btn_auto_run_clicked()
{
    //自动展示过程中设置除看搜索树外的所有按钮为不可用
    ui->btn_auto_run->setDisabled(true);
    ui->btn_step_run->setDisabled(true);
    ui->btn_randstr->setDisabled(true);
    ui->btn_clear->setDisabled(true);
    ui->btn_start->setDisabled(true);

    if(path_cnt==0)
        ui->path_text->clear();//清空文本框

    for(int i=path_cnt;i<path.size();++i)
    {
        WriteState(QString::fromStdString(path[i]),now_state);//更新当前状态

        //更新搜索过程框中的文字说明
        ui->path_text->insertPlainText(QString::fromStdString("第"+to_string(i+1)+"步 : \n"));
        for(int j=0;j<STATE_SIZE;++j)
        {
            ui->path_text->insertPlainText(QString::fromStdString(path[i].substr(j,1)+" "));
            if(j%3==2)
                ui->path_text->insertPlainText("\n");
        }
        ui->path_text->insertPlainText("\n");

        MySleep(SLEEP_TM);//延时，增强演示效果
    }

    ui->path_text->insertPlainText("执行完毕！");
    path_cnt=0;//重置路径计数器

    //展示完成后设置所以按钮为可用
    ui->btn_auto_run->setDisabled(false);
    ui->btn_step_run->setDisabled(false);
    ui->btn_randstr->setDisabled(false);
    ui->btn_clear->setDisabled(false);
    ui->btn_start->setDisabled(false);
}

void MainWindow::on_btn_clear_clicked()
{
    //清除所有状态
    ClearState(origin_state);
    ClearState(now_state);
    ClearState(end_state);

    //设置按钮为初始状态
    ui->btn_auto_run->setDisabled(true);
    ui->btn_step_run->setDisabled(true);
    ui->btn_randstr->setDisabled(false);
    ui->btn_start->setDisabled(false);
    ui->btn_tree->setDisabled(true);

    //设置启发函数为初始状态
    ui->cbox_h0->setChecked(true);
    ui->cbox_h1->setChecked(false);
    h_func=0;

    ui->path_text->clear();
    ui->label_steps->setText("总步数 : ");
    ui->label_time->setText("搜索时间 : ");

    SetStateReadonly(origin_state,false);
    SetStateReadonly(now_state,true);
    SetStateReadonly(end_state,false);
}

void MainWindow::on_btn_tree_clicked()
{
    ui->btn_auto_run->setDisabled(true);
    ui->btn_step_run->setDisabled(true);
    ui->btn_randstr->setDisabled(true);
    ui->btn_start->setDisabled(true);
    ui->btn_tree->setDisabled(true);
    ui->btn_clear->setDisabled(true);
    GetTree();
    system("del tree.png");
    system("dot -Tpng tree.dot -o tree.png");
    TreeWindow *tree=new TreeWindow();
    tree->show();
    ui->btn_auto_run->setDisabled(false);
    ui->btn_step_run->setDisabled(false);
    ui->btn_randstr->setDisabled(false);
    ui->btn_start->setDisabled(false);
    ui->btn_tree->setDisabled(false);
    ui->btn_clear->setDisabled(false);
}

void MainWindow::on_cbox_h0_clicked()
{
    ui->cbox_h1->setChecked(false);//另一个复选框自动清空
    h_func=0;//设置启发函数为h(n)_0
}

void MainWindow::on_cbox_h1_clicked()
{
    ui->cbox_h0->setChecked(false);//另一个复选框自动清空
    h_func=1;//设置启发函数为h(n)_1
}

void MainWindow::autoScroll()
{
    //用于设置文本框自动滚动
    QTextCursor cursor=ui->path_text->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->path_text->setTextCursor(cursor);
}
