#ifndef DETECTION_H
#define DETECTION_H

#include<atk_yolov5_object_recognize.h>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include<QTimer>
#include "detect.h"
#include <FaceEngine.h>
#include<FaceEngine.h>
#include <opencv2/opencv.hpp>
#include<QualityAssessor.h>
#include<QDateTime>
#include<QFont>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QMediaRecorder>
#include <QCamera>
#include<QThread>
#include <QtConcurrent>
#include<QProcess>
#include <deque>

#include"config.h"
#include "database_inf.h"
#include"config_camera.h"



class VideoSimplifyWorker;

namespace Ui {
class detection;
}

class detection : public QWidget
{
    Q_OBJECT

public:
    explicit detection(QWidget *parent = nullptr);


    ~detection();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();
    void updateFrame();
    void splitVideoFile();

    void on_pushButton_6_clicked();
    void handleSimplifyResult(QString simplifiedPath, QString fileName, QString resultText);

private:
    Ui::detection *ui;

    cv::VideoCapture capture;
    cv::VideoCapture cap1;
    QTimer *timer;


    void video_show();
//    void updateFrame();
    rknn_context ctx;


    float similarity_threshold = 0.2f;

    seeta::FaceEngine *fenginerptr;
    seeta::v2::QualityAssessor QA;
    std::map<int64_t, std::string> GalleryIndexMap;
    std::vector<std::string> GalleryImageFilename;
    std::vector<int64_t> GalleryIndex;



    struct LogEntry {
        QString cameraid;      // 摄像头
        QString time;          // 时间
        QString location;      // 位置
        QString person;        // 人员
        QString bag;           //是否背包
        QString clothes_color; //衣服颜色
        QString name;          // 检测到的名称（如ID）
        float similarity;      // 相似度（百分比）
        QString direction;     // 方向

        // 正确的默认构造函数
        LogEntry() :
            cameraid(config_camera::cameraid),
            time(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")),
            location(config_camera::Location),
            person("false"),
            bag("false"),
            clothes_color("unknown"),
            name("unkown"),
            similarity(0.0f),
            direction("unknown")
        {}
    };


    //Json Log相关函数
    QString getTimestampSuffix();
    QJsonObject logToJson(const LogEntry &log);
    void writeLogToJson(const LogEntry &log);

    // 视频录制相关
    cv::VideoWriter videoWriter;
    bool isRecording = false;
    QString currentVideoFilename;
    QTimer *videoSplitTimer;  // 专门用于视频分割的定时器

    QString getVideoFilename();
    void nextFrame1();


    // 原有成员变量...
    QThread* workerThread;
    VideoSimplifyWorker* videoWorker;

    //视频处理相关函数
    void simplifyVideo(const QString &videoPath, const QString &outputPath);


     QString oss= "oss://qingwu-oss/detection_rv1126/";


     struct PersonBox {
         int x1, y1, x2, y2;
     };

     struct FaceBox {
         int x1, y1, x2, y2;
     };

     std::vector<PersonBox> person_boxes;
     std::vector<cv::Rect> backpack_boxes;
     std::vector<FaceBox> face_boxes;


     // 头文件中添加
     QProcess* splitProcess;


     std::map<QString, std::deque<QPoint>> person_center_history;
     const int HISTORY_LENGTH = 5; // 保留最近5帧

     void appendLog();
     void appendLog1();





signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息
    void appendsimLogToUI(const QString &text);
    void simplifyFinished(QString simplifiedPath, QString uploadName, QString resultText);

};







#endif // DETECTION_H
