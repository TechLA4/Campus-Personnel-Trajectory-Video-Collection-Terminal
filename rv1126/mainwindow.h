#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <config.h>
#include <QRadioButton>
#include<config_camera.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void reshow();  // 添加这个声


    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
