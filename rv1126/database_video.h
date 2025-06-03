#ifndef DATABASE_VIDEO_H
#define DATABASE_VIDEO_H

#include <QWidget>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QLabel>
#include <opencv2/opencv.hpp>
#include<QTimer>
#include<QProcess>

namespace Ui {
class database_video;
}

class database_video : public QWidget
{
    Q_OBJECT

public:
    explicit database_video(QWidget *parent = nullptr);
    ~database_video();

private:
    Ui::database_video *ui;
    bool openFile();
    QMediaPlayer *mediaPlayer;
    QVideoWidget *videoWidget;
    QLabel *videoPlaceholder; // 用于占位显示的Label
    QTimer *timer;
    cv::VideoCapture cap;
    void nextFrame();
    QProcess process;

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

};

#endif // DATABASE_VIDEO_H
