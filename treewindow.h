#ifndef TREEWINDOW_H
#define TREEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>
//#include <algorithm>
#include <direct.h>

namespace Ui {
class TreeWindow;
}

class TreeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TreeWindow(QWidget *parent = 0);
    ~TreeWindow();

private:
    Ui::TreeWindow *ui;
};

#endif // TREEWINDOW_H
