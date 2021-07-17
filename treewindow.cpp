#include "treewindow.h"
#include "ui_treewindow.h"

TreeWindow::TreeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeWindow)
{
    ui->setupUi(this);
    setWindowTitle("搜索树");//设置窗口标题

    ui->tree_img->setScaledContents(true);

    QImage tree_image;
    char nw_path[300];
    getcwd(nw_path,300);
    QString filename=(nw_path);
    filename.append("\\tree.png");
    if(!tree_image.load(filename))
    {
        QMessageBox::information(this,"错误","打开图像失败！");
        return;
    }
    ui->tree_img->setPixmap(QPixmap::fromImage(tree_image));
}

TreeWindow::~TreeWindow()
{
    delete ui;
}
