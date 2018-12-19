#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFrame>
#include<QLabel>
#include<QWidget>
#include<QPoint>
#include<QColor>
#include<QPalette>
#include<QtCore>
#include<queue>
#include<iostream>
#include<vector>
#include<QTime>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sleep(int msec);
protected:
     void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    QFrame *frame;
    int dir;//来判断方向
    int map[5000];//判断蛇是否可以移动
    int sum;
    int snake_x,snake_y;//来记录蛇头的坐标
    int grade;//分数
    int judge_move;//判断是否可以移动
    int level;//等级，你选择的难度
    int snake_time;
    int visit[100][100];//用于生成地图并且判断是否为连通图
    queue<int> snake_pos;//记录蛇的身子
public slots:
    void on_pushbutton_easy();//简单难度
    void on_pushbutton_normal();//普通难度
    void on_pushbutton_difficult();//困难难度
    void on_pushbutton_hard();//炼狱难度
    void on_action_return_mainwindow();//
    void window_create();//
    void rand_create_map(int);//
    bool judge_geteverywhere(int);//
    void bfs(int x, int y);//
    void snake_move();//
    void fruit_feed();//
    void ui_into_show();//
    void ui_into_hide();//
    void ui_out_show();
    void ui_out_hide();
    void into_number_init();//
    void map_init();//
};

#endif // MAINWINDOW_H
