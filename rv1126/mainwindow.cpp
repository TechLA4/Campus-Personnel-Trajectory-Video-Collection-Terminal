#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <config.h>
#include <detection.h>
#include <logout.h>
#include<database_inf.h>
#include<database_log.h>
#include<database_video.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reshow()
{
    this->show();  // 重新显示主窗口
}

void MainWindow::on_pushButton_clicked()
{

    this->hide();
    config *con = new config() ;
    connect(con,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击config界面时，调用主界面的reshow()函数
    con->show();//子界面出现

}

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    detection *det = new detection();
    connect(det,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击start子界面时，调用主界面的reshow()函数
    det->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    database_inf *data_inf = new database_inf();
    connect(data_inf ,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击start子界面时，调用主界面的reshow()函数
    data_inf->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    this->hide();
    database_log *data_log = new  database_log() ;
    connect(data_log,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击log子界面时，调用主界面的reshow()函数
    data_log->show();//子界面出现
}



void MainWindow::on_pushButton_5_clicked()
{
    this->hide();
    database_video *data_v = new  database_video() ;
    connect(data_v,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击video子界面时，调用主界面的reshow()函数
    data_v->show();//子界面出现
}

void MainWindow::on_pushButton_6_clicked()
{
    this->hide();
    config_camera *camera = new  config_camera() ;
    connect(camera,SIGNAL(sendsignal()),this,SLOT(reshow()));//当点击camera子界面时，调用主界面的reshow()函数
    camera->show();//子界面出现
}
