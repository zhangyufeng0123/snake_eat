#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<time.h>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<QDebug>
#include<queue>
#include<vector>
#include<QKeyEvent>
#include<QTime>
#include<QMessageBox>

using namespace std;

struct node
{
    int x,y;
};

int fx[4]={0,0,-1,1},fy[4]={-1,1,0,0};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->grabKeyboard();
    this->setFixedSize(1000,600);
    this->setWindowTitle("贪吃蛇");
    this->frame=new QFrame[75*45]();

    for( int y=0;y<75;y++){
        frame[y].setStyleSheet("background: rgb(0,0,0)");
        frame[44*75+y].setStyleSheet("background: rgb(0,0,0)");
    }
    for(int i=1;i<45;i++){
        frame[i*75].setStyleSheet("background: rgb(0,0,0)");
        frame[i*75+74].setStyleSheet("background: rgb(0,0,0)");
    }
    for(int x=0;x<45;x++){
        for(int y=0;y<75;y++){
            ui->triangle->addWidget(&frame[x*75+y],x,y);
        }
    }
    for(int i=0;i<75*45;i++){
        frame[i].hide();
    }
    this->judge_move=0;
    ui->score->hide();
    ui->score_show->hide();
    ui->label_2->hide();
    ui->tips->hide();
    this->dir=4;
    this->snake_x=2;
    this->snake_y=8;
    connect(ui->easy,SIGNAL(clicked()),this,SLOT(on_pushbutton_easy()));
    connect(ui->normal,SIGNAL(clicked()),this,SLOT(on_pushbutton_normal()));
    connect(ui->difficult,SIGNAL(clicked()),this,SLOT(on_pushbutton_difficult()));
    connect(ui->hard,SIGNAL(clicked()),this,SLOT(on_pushbutton_hard()));
    connect(ui->action_P,SIGNAL(triggered()),this,SLOT(on_action_return_mainwindow()));
    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(on_pushbutton_easy()));
    connect(ui->action_3,SIGNAL(triggered()),this,SLOT(on_pushbutton_normal()));
    connect(ui->action_4,SIGNAL(triggered()),this,SLOT(on_pushbutton_difficult()));
    connect(ui->action_5,SIGNAL(triggered()),this,SLOT(on_pushbutton_hard()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete []this->frame;
}

//元素都显现出来
void MainWindow::ui_into_show(){
    ui->label_2->show();
    ui->score_show->show();
    ui->score->show();
    ui->tips->show();
}

//元素隐藏
void MainWindow::ui_into_hide(){
    ui->frame_2->hide();
    ui->frame->hide();
    ui->easy->hide();
    ui->normal->hide();
    ui->difficult->hide();
    ui->hard->hide();
    ui->label->hide();
    ui->label_try->hide();
}

//数据的初始化
void MainWindow::into_number_init(){
    this->dir=4;
    this->snake_x=2;
    this->snake_y=8;
    this->grade=-1;
}

//map的初始化
void MainWindow::map_init(){
    memset(this->map,0,sizeof(this->map));
    for( int y=0;y<75;y++){
        this->map[y]=1;
        this->map[44*75+y]=1;
    }
    for(int i=1;i<45;i++){
        this->map[i*75]=1;
        this->map[i*75+74]=1;
    }
}

//简单的模式
void MainWindow::on_pushbutton_easy(){

    this->level=1;
    while(!this->snake_pos.empty()){
        this->snake_pos.pop();
    }

    this->ui_into_hide();
    this->ui_into_show();
    this->into_number_init();
    this->map_init();

    this->window_create();
}

//普通的模式
void MainWindow::on_pushbutton_normal(){
    this->level=2;
    while(!this->snake_pos.empty()){
        this->snake_pos.pop();
    }

    this->ui_into_hide();
    this->ui_into_show();
    this->into_number_init();
    this->map_init();

    this->window_create();
}

//困难的模式
void MainWindow::on_pushbutton_difficult(){
    this->level=3;

    while(!this->snake_pos.empty()){
        this->snake_pos.pop();
    }

    this->ui_into_hide();
    this->ui_into_show();
    this->into_number_init();
    this->map_init();

    this->window_create();
}

//炼狱的模式
void MainWindow::on_pushbutton_hard(){
    this->level=4;

    while(!this->snake_pos.empty()){
        this->snake_pos.pop();
    }

    this->ui_into_hide();
    this->ui_into_show();
    this->map_init();
    this->into_number_init();

    this->window_create();
}

//返回到主界面
void MainWindow::on_action_return_mainwindow(){
    this->ui_out_hide();
    this->ui_out_show();

    this->judge_move=0;
}

//返回到主界面的时候隐藏元素
void MainWindow::ui_out_hide(){
    ui->score->hide();
    ui->score_show->hide();
    ui->label_2->hide();
    ui->tips->hide();
    for(int x=0;x<45;x++){
        for(int y=0;y<75;y++){
            this->frame[x*75+y].hide();
        }
    }
}

void MainWindow::ui_out_show(){
    ui->easy->show();
    ui->normal->show();
    ui->difficult->show();
    ui->hard->show();
    ui->label->show();
    ui->label_2->show();
    ui->frame->show();
    ui->label_try->show();
    ui->frame_2->show();
}

//先将地图上的点标记
void MainWindow::rand_create_map(int m){
    memset(this->map,0,sizeof(this->map));
    srand(time(NULL));
    for( int y=0;y<75;y++){
        this->map[y]=1;
        this->map[44*75+y]=1;
    }
    for(int i=1;i<45;i++){
        this->map[i*75]=1;
        this->map[i*75+74]=1;
    }
    for(int i=0;i<m;i++){
        int tmp=rand()%3150+225;
        this->map[tmp]=1;
        this->visit[tmp%75][tmp/75]=1;
    }
}

//键盘上下左右键控制
void MainWindow::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        if(this->dir==2||this->judge_move==0){
            break;
        }
        if(this->dir==1&&this->judge_move==1){//向上移动

            //移动的头
            this->snake_pos.push((this->snake_x-1)*75+this->snake_y);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_x=this->snake_x-1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                break;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_x=this->snake_x-1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            //this->grade=this->grade+1;
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();
            this->update();
        }else{
            this->snake_pos.push((this->snake_x-1)*75+this->snake_y);

            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_x=this->snake_x-1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                break;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_x=this->snake_x-1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            //this->grade=this->grade+1;
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();
            this->update();
        }
        this->dir=1;
        this->update();
        break;
    case Qt::Key_Down:
        if(this->dir==1||this->judge_move==0){
            break;
        }
        if(this->dir==2&&this->judge_move==1){//向下移动
                    //移动的头
                    this->snake_pos.push((this->snake_x+1)*75+this->snake_y);
                    if(this->map[this->snake_pos.back()]==1){
                        int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                        this->on_action_return_mainwindow();
                        break;
                    }else if(this->map[this->snake_pos.back()]==2){
                        this->map[this->snake_pos.back()]=1;
                        this->snake_x=this->snake_x+1;
                        this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                        //this->grade=this->grade+1;
                        //QString str=QString::number(this->grade,10);
                        //ui->score_show->setText(str);
                        this->fruit_feed();
                        break;
                    }
                    this->map[this->snake_pos.back()]=1;
                    this->snake_x=this->snake_x+1;
                    this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


                    //后面的身子尾随着头
                    this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
                    this->map[this->snake_pos.front()]=0;
                    this->snake_pos.pop();
                    this->update();
                }
        else{
            this->snake_pos.push((this->snake_x+1)*75+this->snake_y);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_x=this->snake_x+1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                break;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_x=this->snake_x+1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();
            this->update();
        }
        this->dir=2;
        this->update();
        break;
    case Qt::Key_Left:
        if(this->dir==4||this->judge_move==0){
            break;
        }
        else if(this->dir==3&&this->judge_move==1){//向左移动
                    //移动的头
                    this->snake_pos.push(this->snake_x*75+this->snake_y-1);
                    if(this->map[this->snake_pos.back()]==1){
                        int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                        this->on_action_return_mainwindow();
                        break;
                    }else if(this->map[this->snake_pos.back()]==2){
                        this->map[this->snake_pos.back()]=1;
                        this->snake_y=this->snake_y-1;
                        this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                        //this->grade=this->grade+1;
                        //QString str=QString::number(this->grade,10);
                        //ui->score_show->setText(str);
                        this->fruit_feed();
                        break;
                    }
                    this->map[this->snake_pos.back()]=1;
                    this->snake_y=this->snake_y-1;
                    this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


                    //后面的身子尾随着头
                    this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
                    this->map[this->snake_pos.front()]=0;
                    this->snake_pos.pop();
                    this->update();
                }
        else{
            //移动的头
            this->snake_pos.push(this->snake_x*75+this->snake_y-1);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_y=this->snake_y-1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                break;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_y=this->snake_y-1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();
            this->update();
        }
        this->dir=3;
        this->update();
        break;
    case Qt::Key_Right:
        if(this->dir==3||this->judge_move==0){
            break;
        }
        if(this->dir==4&&this->judge_move==1){//向右移动
                    //移动的头
                    this->snake_pos.push(this->snake_x*75+this->snake_y+1);
                    if(this->map[this->snake_pos.back()]==1){
                        int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                        this->on_action_return_mainwindow();
                        break;
                    }else if(this->map[this->snake_pos.back()]==2){
                        this->map[this->snake_pos.back()]=1;
                        this->snake_y=this->snake_y+1;
                        this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                        //this->grade=this->grade+1;
                        //QString str=QString::number(this->grade,10);
                        //ui->score_show->setText(str);
                        this->fruit_feed();
                        break;
                    }
                    this->map[this->snake_pos.back()]=1;
                    this->snake_y=this->snake_y+1;
                    this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


                    //后面的身子尾随着头
                    this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
                    this->map[this->snake_pos.front()]=0;
                    this->snake_pos.pop();
                    this->update();

                }
        else{
            //移动的头
            this->snake_pos.push(this->snake_x*75+this->snake_y+1);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_y=this->snake_y+1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                break;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_y=this->snake_y+1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();
            this->update();
        }
        this->dir=4;
        this->update();
        break;
    case Qt::Key_P:
        this->judge_move=0;;
        break;
    case Qt::Key_O:
        this->judge_move=1;
    default:
        break;
    }
}

//搜索地图判断是否可以从一个点到任意一个其他的点
void MainWindow::bfs(int x, int y){
    this->visit[x][y]=2;
    queue<node>q;
    node o;
    o.x=x;
    o.y=y;
    q.push(o);
    while(!q.empty()){
        o=q.front();
        q.pop();
        for(int i=0;i<4;i++){
            int xx=o.x+fx[i];
            int yy=o.y+fy[i];
            if(xx>=0&&yy>=0&&xx<45&&yy<75&&this->visit[xx][yy]==0){
                node w;
                this->visit[xx][yy]=2;
                w.x=xx;
                w.y=yy;
                q.push(w);
            }
        }
    }
}

//蛇动，间隔时间随着难度控制
void MainWindow::snake_move(){
    while(1){
        if(this->dir==1&&this->judge_move==1){//向上移动

            //移动的头
            this->snake_pos.push((this->snake_x-1)*75+this->snake_y);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_x=this->snake_x-1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                continue;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_x=this->snake_x-1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            //this->grade=this->grade+1;
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();

            this->sleep(this->snake_time);
        }else if(this->dir==2&&this->judge_move==1){//向下移动
            //移动的头
            this->snake_pos.push((this->snake_x+1)*75+this->snake_y);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_x=this->snake_x+1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                continue;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_x=this->snake_x+1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();

            this->sleep(this->snake_time);
        }else if(this->dir==3&&this->judge_move==1){//向左移动
            //移动的头
            this->snake_pos.push(this->snake_x*75+this->snake_y-1);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_y=this->snake_y-1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                continue;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_y=this->snake_y-1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();

            this->sleep(this->snake_time);
        }else if(this->dir==4&&this->judge_move==1){//向右移动
            //移动的头
            this->snake_pos.push(this->snake_x*75+this->snake_y+1);
            if(this->map[this->snake_pos.back()]==1){
                int ret=QMessageBox::warning(this,tr("小提示"),tr("Your snake died"),QMessageBox::Ok);
                this->on_action_return_mainwindow();
                break;
            }else if(this->map[this->snake_pos.back()]==2){
                this->map[this->snake_pos.back()]=1;
                this->snake_y=this->snake_y+1;
                this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");
                //this->grade=this->grade+1;
                //QString str=QString::number(this->grade,10);
                //ui->score_show->setText(str);
                this->fruit_feed();
                continue;
            }
            this->map[this->snake_pos.back()]=1;
            this->snake_y=this->snake_y+1;
            this->frame[this->snake_pos.back()].setStyleSheet("background:rgb(255,0,0)");


            //后面的身子尾随着头
            this->frame[this->snake_pos.front()].setStyleSheet("background:rgb(255,255,255)");
            this->map[this->snake_pos.front()]=0;
            this->snake_pos.pop();

            this->sleep(this->snake_time);
        }
        else{
            this->sleep(1000);
        }
    }
}

//判断随机生成的地图是否为通路
bool MainWindow::judge_geteverywhere(int m){
    this->bfs(1,1);
    for(int i=0;i<45;i++){
        for(int j=0;j<75;j++){
            if(this->visit[i][j]==0){
                return false;
            }
        }
    }
    return true;
}

//生成地图
void MainWindow::window_create(){
    int m;
    if(this->level==1){
        m=0;
        this->snake_time=250;
    }else if(this->level==2){
        m=50;
        this->snake_time=225;
    }else if(this->level==3){
        m=100;
        this->snake_time=200;
    }else if(this->level==4){
        m=200;
        this->snake_time=150;
    }

    while(1){
        memset(this->visit,0,sizeof(this->visit));
        for( int y=0;y<75;y++){
            this->visit[0][y]=1;
            this->visit[44][y]=1;
        }
        for(int i=1;i<44;i++){
            this->visit[i][0]=1;
            this->visit[i][74]=1;
        }
        this->rand_create_map(m);
        if(this->judge_geteverywhere(m)){
            break;
        }
    }
    for(int i=3;i<=8;i++){
        this->snake_pos.push(2*75+i);
    }
    for(int x=1;x<44;x++){
        for(int y=1;y<74;y++){
            if(x==2&&(y>=3&&y<=8)){
                this->map[x*75+y]=1;
                this->frame[x*75+y].setStyleSheet("background:rgb(255,0,0)");
                continue;
            }
            if(this->map[x*75+y]==1){
                this->frame[x*75+y].setStyleSheet("background:rgb(0,0,0)");
            }else{
                this->frame[x*75+y].setStyleSheet("background:rgb(255,255,255)");
            }
        }
    }
    for(int i=0;i<45*75;i++){
        this->frame[i].show();
    }
    this->sleep(3000);
    this->judge_move=1;
    this->dir=4;
    this->fruit_feed();
    this->snake_move();
}

void MainWindow::fruit_feed(){
    this->grade=this->grade+1;
    ui->score_show->display(this->grade);
    srand(time(NULL));
    int m=rand()%3300+75;
    while(this->map[m]!=0){
        m=rand()%3300+75;
    }
    this->map[m]=2;
    this->frame[m].setStyleSheet("background:rgb(0,255,0)");
}

void MainWindow::sleep(int msec){
    QTime dieTime=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
