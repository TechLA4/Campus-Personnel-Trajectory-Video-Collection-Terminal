#ifndef DATABASE_INF_H
#define DATABASE_INF_H

#include <QWidget>
#include <QDebug>
#include <QHeaderView>
#include <QSqlError>
#include <QApplication>
#include <QSqlRecord>
#include<QSqlDatabase>
#include<QLabel>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include<QTimer>
#include "detect.h"
#include "facedetect.h"
#include "qdebug.h"
#include <FaceEngine.h>
#include<FaceDetector.h>
#include <Struct.h>
#include <Struct_cv.h>
#include <FaceDetector.h>
#include <FaceRecognizer.h>
#include <QualityAssessor.h>
#include<QFont>
#include <QMediaRecorder>
#include <QCamera>


namespace Ui {
class database_inf;
}

class database_inf : public QWidget
{
    Q_OBJECT

public:
    explicit database_inf(QWidget *parent = nullptr);
    ~database_inf();



private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_tableView_activated(const QModelIndex &index);

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private:
    Ui::database_inf *ui;
    /* 打开文本文件 */
    void openDB();
    QSqlDatabase db;
    void displayImage(const QString &url, QLabel *label);
};

#endif // DATABASE_INF_H
