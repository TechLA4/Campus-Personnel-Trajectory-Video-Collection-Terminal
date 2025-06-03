#include "database_video.h"
#include "ui_database_video.h"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include<QTimer>
#include"config_camera.h"
#include<QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include <QProcess>


database_video::database_video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::database_video)
{
    ui->setupUi(this);

    cv::VideoCapture cap;
    QTimer *timer;

    // 创建并启动一个 QProcess 来执行 ossutil 命令
        QProcess *process = new QProcess(this);

        // 设置 ossutil 命令及参数
        QString command = "ossutil";
        QStringList arguments;
        arguments << "cp" << "/test/logs/011358_1.json" << "oss://qingwu-oss/oss_test/011358_1.json";

        // 启动 QProcess 执行命令
        process->start(command, arguments);

        printf("yes");

}

database_video::~database_video()
{
    delete ui;
}

void database_video::on_pushButton_clicked()
{
    emit sendsignal();
}

bool database_video::openFile() {
    // 创建文件对话框实例
    QFileDialog dialog(this);

    // 设置对话框属性
    dialog.setWindowTitle("Open File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setOption(QFileDialog::DontUseNativeDialog); // 关键选项
    dialog.setGeometry(QRect(-280, 150, 600, 800));     // 设置合适尺寸
    dialog.setStyleSheet(
        "QScrollBar:vertical { width: 50px; }"
        "QScrollBar:horizontal { height: 50px; }"
    );

    // 执行对话框
    if (!dialog.exec())
        return false;

    // 获取选中文件
    QStringList files = dialog.selectedFiles();
    if (files.isEmpty())
        return false;

    QString fileName = files.first();

    // 打开视频文件
    cap.open(fileName.toStdString());
    if (!cap.isOpened()) {
        QMessageBox::information(this, "Information", "Failed to open video file.");
        return false;
    }

    // 启动定时器播放视频
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &database_video::nextFrame);
    timer->start(33); // ~30fps

    // 获取视频文件所在目录
    QFileInfo fileInfo(fileName);
    QString videoDir = fileInfo.absolutePath();

    // 构造目标txt文件路径
    QString txtFileName = fileInfo.baseName() + ".txt"; // 去掉视频文件扩展名，添加.txt
    QString txtFilePath = videoDir + "/sim_log/" + txtFileName;

    // 检查txt文件是否存在
    if (QFile::exists(txtFilePath)) {
        // 读取txt文件内容
        QFile txtFile(txtFilePath);
        if (txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&txtFile);
            in.setCodec("UTF-8");
            QString content = in.readAll();
            txtFile.close();

            // 显示内容到ui->textBrowser
            ui->textEdit_2->setPlainText(content);
        } else {
            QMessageBox::warning(this, "Warning", "Failed to open text file.");
        }
    } else {
        QMessageBox::warning(this, "Warning", "Text file not found.");
    }

    return true;
}

void database_video::on_pushButton_2_clicked()
{
    openFile();
}

// 显示下一帧
void database_video::nextFrame() {
    cv::Mat frame;
    if (!cap.read(frame)) {
        timer->stop();
        cap.release();
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio));
}















