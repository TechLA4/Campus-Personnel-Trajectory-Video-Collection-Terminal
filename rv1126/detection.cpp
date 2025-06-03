#include "detection.h"
#include "ui_detection.h"
#include "atk_yolov5_object_recognize.h"
#include <QMessageBox>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <opencv2/opencv.hpp>
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
#include <QProcess>


detection::detection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detection)
{
    ui->setupUi(this);
    QFont font;
    font.setPointSize(6);
    ui->textBrowser->setFont(font);

    font.setPointSize(7);
    ui->textBrowser_2->setFont(font);

    QVideoWidget *video = new QVideoWidget();
    video->setGeometry(0,0,400,300);

    QMediaPlayer *player = new QMediaPlayer (this);
    player->setVideoOutput(video);

    timer = new QTimer();

    ui->textBrowser->setStyleSheet("QScrollBar:vertical { width: 40px; }");
    ui->textBrowser_2->setStyleSheet("QScrollBar:vertical { width: 40px; }");



    //-----------------------视频录制初始化--------------------------------
    videoSplitTimer = new QTimer(this);
    connect(videoSplitTimer, &QTimer::timeout, this, &detection::splitVideoFile);


    // 构造函数中连接信号到槽
    connect(this, &detection::appendsimLogToUI, this, [=](const QString &text){
        ui->textBrowser_2->setTextColor(Qt::red);
        ui->textBrowser_2->append(text);
        ui->textBrowser_2->setTextColor(Qt::black);

    });

    // 在构造函数中连接信号槽
    connect(this, &detection::simplifyFinished, this, &detection::handleSimplifyResult);

    // 创建视频保存目录
    QDir().mkdir("/demo/detect/videos");

    printf("prepare rknn\n");
    /*算法模型初始化*/
     yolov5_detect_init(&ctx, "/demo/bin/yolov5s_relu_rv1109_rv1126_out_opt.rknn");
//    yolov5_detect_init(&ctx, "/demo/bin/best.rknn");
     //yolov5_detect_init(&ctx, "/demo/bin/yolov5s_v6.2_output3_4.rknn");


    printf("rknn opened \n");


    //-----------------------摄像头初始化开始--------------------------------

    try {
        // 打开摄像头
       printf("prepared open \n");
       capture.open("/dev/video45", cv::CAP_V4L2);

       if (!capture.isOpened()) {
           printf("Failed to open camera, check GStreamer pipeline!\n");
       } else {
           printf("Camera opened successfully!\n");
       }

        printf("cap open\n");
   }catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;}

    // 获取摄像头的分辨率
    int frameWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    cout << "Camera resolution: " << frameWidth << "x" << frameHeight << endl;

    //-----------------------摄像头初始化结束--------------------------------



    // 构造函数初始化
    splitProcess = new QProcess(this);  // this 负责生命周期管理


    cv::VideoCapture cap1;
    QTimer *timer;


}


detection::~detection()
{
    delete ui;
    if (capture.isOpened()) {
            capture.release();
        }
        if (timer && timer->isActive()) {
            timer->stop();
        }
    if (videoWriter.isOpened()) {
            videoWriter.release();
        }

}

void detection::on_pushButton_2_clicked()
{
    emit sendsignal();
}


void detection::on_pushButton_clicked()
{
    ui->pushButton->setText("摄像头运行中");
    ui->textBrowser->setText(QString("--摄像头已经启动--"));

    connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
      timer->start(30);//30毫秒刷新视频帧
      videoSplitTimer->start(30000);  // 30秒分割视频
    QPixmap pixmap("/mosaic_face.png"); // 加载图片
    if (!pixmap.isNull()) {
        ui->label->setPixmap(pixmap); // 设置图片
        ui->label->setScaledContents(true); // 自动缩放图片以适应 QLabel 大小
    } else {
        qDebug() << "Failed to load image!";
    }


    QString filePath = "/demo/output.mp4";


//-----------------------测试用例--------------------------------

//    // 打开视频文件
//    cap1.open(filePath.toStdString());
//    if (!cap1.isOpened()) {
//        QMessageBox::information(this, "Information", "Failed to open video file.");
//    }

//    // 启动定时器播放视频
//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &detection::nextFrame1);
//    timer->start(15); // ~30fps




//     ui->textBrowser->setText(QString("--摄像头已经启动--"));



//     QString log_string = QString(
//         "[Camera] camera5\n"
//         "[Time] 2025-05-17 21:30:15\n"
//         "[Location] 体育馆正门\n"
//         "[Person] True \n"
//         "[Bag] True \n"
//         "[Cloth_color] Black \n"
//         "[Name] lyy-xx47\n"
//         "[Similarity] 56%\n"
//         "[Direction] Static \n\n");

// QString log_string2 = QString(
//         "[Camera] camera5\n"
//         "[Time] 2025-05-17 21:30:17\n"
//         "[Location] 体育馆正门\n"
//         "[Person] True \n"
//         "[Bag] True \n"
//         "[Cloth_color] Black \n"
//         "[Name] lyy-xx47\n"
//         "[Similarity] 75%\n"
//         "[Direction]  南 \n\n");

//     ui->textBrowser->append(log_string);
//     ui->textBrowser->append(log_string2);

//    ui->textBrowser_2->setTextColor(Qt::blue);
//    ui->textBrowser_2->append(QString("--日志已成功写入文件: 250517-2130"));
//    ui->textBrowser_2->append(QString("--日志已成功上传服务器: 250517-2130\n"));

    //  ui->textBrowser_2->setTextColor(Qt::red);
    //  ui->textBrowser_2->append(QString("--视频保存成功: 250517-2132"));
    //   ui->textBrowser_2->append(QString("--开启视频裁剪: 250517-2132"));

    //   ui->textBrowser_2->setTextColor(Qt::black);
    //    QString result = QString(
    //     "[裁剪完成] 250517-2132 \n原始帧数: 98 裁剪后帧数: 71 \n保留比例: 72.4%\n");
    // ui->textBrowser_2->append(result);


    //  ui->textBrowser_2->setTextColor(Qt::red);
    //  ui->textBrowser_2->append(QString("--裁剪视频已成功上传服务器: 250517-2132\n"));

      
       

   


    






}

void detection::on_pushButton_3_clicked()
{

    // 停止两个定时器
    if (timer && timer->isActive()) {
        timer->stop();
    }
    if (videoSplitTimer && videoSplitTimer->isActive()) {
        videoSplitTimer->stop();
    }

    if (capture.isOpened()) {
        capture.release();
    }

     QMessageBox::warning(this, "Warning", "capture closed");
     ui->pushButton->setText("开启摄像头检测");
}


void detection::updateFrame() {

    // Check if camera is still open
    if (!capture.isOpened()) {
        timer->stop();
        QMessageBox::warning(this, "Warning", "Camera connection lost");
        return;
    }

    cv::Mat frame;

    capture.read(frame);


    std::vector<PersonBox> person_boxes;
    std::vector<cv::Rect> backpack_boxes;
    // 每帧都清空历史目标框
    person_boxes.clear();
    backpack_boxes.clear();

    // 写入视频帧
   if (isRecording) {
       cv::Mat frameToWrite;

      // 保证尺寸一致
      if (frame.cols == 640 && frame.rows == 480) {
          frameToWrite = frame.clone();
      } else {
          cv::resize(frame, frameToWrite, cv::Size(640, 480));
      }

      // 获取当前时间字符串
      QString timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
      std::string timestamp = timeString.toStdString();

      // 在图像左上角绘制时间戳
      cv::putText(
          frameToWrite,
          timestamp,
          cv::Point(10, 30),  // 左上角坐标
          cv::FONT_HERSHEY_SIMPLEX,
          0.8,                // 字体大小
          cv::Scalar(0, 255, 255),  // 黄色文字
          2                   // 粗细
      );

      // 写入视频
      videoWriter.write(frameToWrite);
      }

   // 首次启动录制
   if (!isRecording) {
       splitVideoFile();
   }


    // Check if frame is empty
   if (frame.empty()) {
       qDebug() << "Empty frame received";
       return;
   }


        cv::Mat rgbFrame;
        if (frame.type() == CV_8UC2) {  // UYVY格式
            cv::cvtColor(frame, rgbFrame, cv::COLOR_YUV2RGB_UYVY);
        }
        else if (frame.type() == CV_8UC1) {  // Planar或semi-planar格式
            if (frame.rows % 3 == 0) {       // NV12/NV21 (YUV420)
                cv::cvtColor(frame, rgbFrame, cv::COLOR_YUV2RGB_NV12);
            }
        }
        else if (frame.channels() == 3) {  // 已经转换的BGR
            cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        }
        else {
            qDebug() << "Unsupported frame format:" << frame.type();
        }

    //初始化log
    LogEntry log;


    //--------------------------开始目标检测-------------------------------

    yolov5_detect_result_group_t detect_result_group;


    yolov5_detect_run(ctx, rgbFrame, &detect_result_group);


    for(int i=0; i<detect_result_group.count; i++)
    {
       yolov5_detect_result_t *det_result = &(detect_result_group.results[i]);
       if( det_result->prop < 0.2 ){
           continue;
       }

       int x1 = det_result->box.left;
       int y1 = det_result->box.top;
       int x2 = det_result->box.right;
       int y2 = det_result->box.bottom;


       if (strcmp(det_result->name, "person") == 0) {
           person_boxes.push_back({x1, y1, x2, y2});
            log.person = "true";
       }
       else if (strcmp(det_result->name, "backpack") == 0) {
           backpack_boxes.push_back(cv::Rect(x1, y1, x2, y2));
           log.bag="true";
           printf("has bag\n");
           plot_one_box(rgbFrame, x1, x2, y1, y2, "backpack", 1);
       }
       else if(strcmp(det_result->name, "face") == 0){
            face_boxes.push_back({x1, y1, x2, y2});
       }

       char label_text[50];
       memset(label_text, 0 , sizeof(label_text));
       sprintf(label_text, "%s %0.2f",det_result->name, det_result->prop);
    }
       // 检查出了person进行背包\人脸\颜色\方向识别
      for (size_t i = 0; i < person_boxes.size(); ++i)
       {


           PersonBox pbox = person_boxes[i];

           // 绘制人物框
           plot_one_box(rgbFrame, pbox.x1, pbox.x2, pbox.y1, pbox.y2, "person", 5);

    //--------------------------开始衣服颜色检测-------------------------------

           cv::Rect person_rect(pbox.x1, pbox.y1, pbox.x2 - pbox.x1, pbox.y2 - pbox.y1);
           // 防止越界
           person_rect &= cv::Rect(0, 0, rgbFrame.cols, rgbFrame.rows);
           if (person_rect.width <= 0 || person_rect.height <= 0) return;

           cv::Mat person_roi = rgbFrame(person_rect);

           // 转HSV
           cv::Mat hsv;
           cv::cvtColor(person_roi, hsv, cv::COLOR_RGB2HSV);

           // 统计颜色数量（仅统计 black / white / gray / red）
           std::map<std::string, int> color_count;
           int total_count = 0;

           for (int y = 0; y < hsv.rows; ++y) {
               for (int x = 0; x < hsv.cols; ++x) {
                   cv::Vec3b pixel = hsv.at<cv::Vec3b>(y, x);
                   int h = pixel[0];
                   int s = pixel[1];
                   int v = pixel[2];

                   std::string color_name;

                   if (v < 50 && s < 50) {
                       color_name = "black";
                   } else if (v > 200 && s < 50) {
                       color_name = "white";
                   } else if (s < 50) {
                       color_name = "gray";
                   } else {
                       if (h < 10 || h >= 160) {
                           color_name = "red";
                       }
                   }

                   if (!color_name.empty()) {
                       color_count[color_name]++;
                       total_count++;
                   }
               }
           }

           // 无颜色识别结果
           if (total_count == 0) {
               log.clothes_color = "unknown";
               printf("衣服主色: unknown\n");
               return;
           }

           // 查找主色，并判断是否远超其他颜色（高出20%）
           std::string dominant_color = "unknown";
           int max_count = 0;
           int second_max_count = 0;

           for (const auto& item : color_count) {
               if (item.second > max_count) {
                   second_max_count = max_count;
                   max_count = item.second;
                   dominant_color = item.first;
               } else if (item.second > second_max_count) {
                   second_max_count = item.second;
               }
           }

           // 判断是否差距显著（最多的颜色比第二多的多20%以上）
           if ((float)max_count / total_count >= 0.2f &&
               (float)(max_count - second_max_count) / total_count >= 0.2f) {
               log.clothes_color = QString::fromStdString(dominant_color);
           } else {
               log.clothes_color = "unknown";
               dominant_color = "unknown";
           }

           printf("衣服主色: %s\n", dominant_color.c_str());



    //--------------------------开始背包检测-------------------------------


           cv::Rect person_cvrect(pbox.x1, pbox.y1, pbox.x2 - pbox.x1, pbox.y2 - pbox.y1);

           // 记录最大IOU和最近距离
           double max_iou = 0.0;
           int min_distance = INT_MAX;
           cv::Rect best_backpack_rect;

           for (const auto& backpack : backpack_boxes)
           {
               cv::Rect backpack_cvrect = backpack;

               // ---------- IOU 计算 ----------
               int inter_area = (person_cvrect & backpack_cvrect).area();
               int union_area = person_cvrect.area() + backpack_cvrect.area() - inter_area;
               double iou = union_area > 0 ? (double)inter_area / union_area : 0.0;

               // ---------- 中心点距离计算 ----------
               cv::Point person_center(
                   pbox.x1 + (pbox.x2 - pbox.x1) / 2,
                   pbox.y1 + (pbox.y2 - pbox.y1) / 2
               );
               cv::Point backpack_center(
                   backpack.x + backpack.width / 2,
                   backpack.y + backpack.height / 2
               );
               int distance = cv::norm(person_center - backpack_center);

               // 综合判断逻辑：保存最匹配的那个
               if (iou > max_iou || distance < min_distance)
               {
                   max_iou = iou;
                   min_distance = distance;
                   best_backpack_rect = backpack_cvrect;
               }
           }

           // 判断是否满足“有背包”条件
           if (max_iou > 0.3 || min_distance < 50)
           {
               log.bag = "true";
               printf("has bag [IOU=%.2f, dist=%d]\n", max_iou, min_distance);

               // 绘制匹配背包框
               plot_one_box(
                   rgbFrame,
                   best_backpack_rect.x,
                   best_backpack_rect.x + best_backpack_rect.width,
                   best_backpack_rect.y,
                   best_backpack_rect.y + best_backpack_rect.height,
                   "backpack",
                   1
               );
           }



    // ------------------方向判断（多帧平滑判断）-------------------------------------


           QString cam_id = config_camera::cameraid;
           QString lastChar = cam_id.right(1);
           int camera_index = lastChar.toInt();

           QPoint current_center(
               pbox.x1 + (pbox.x2 - pbox.x1) / 2,
               pbox.y1 + (pbox.y2 - pbox.y1) / 2
           );

           // 插入当前中心点到历史记录队列
           auto &center_queue = person_center_history[cam_id];
           center_queue.push_back(current_center);
           if (center_queue.size() > HISTORY_LENGTH) {
               center_queue.pop_front();
           }

           // 判断方向（如果历史长度不够，就标记为静止）
           QString direction = "static";
           if (center_queue.size() >= HISTORY_LENGTH) {
               QPoint old_center = center_queue.front();

               int dx = current_center.x() - old_center.x();
               int dy = current_center.y() - old_center.y();

               if (std::abs(dx) > std::abs(dy)) {
                   if (dx > 10) direction = config_camera::cameraMap[camera_index].right;
                   else if (dx < -10) direction = config_camera::cameraMap[camera_index].left;
               } else {
                   if (dy > 10) direction = config_camera::cameraMap[camera_index].down;
                   else if (dy < -10) direction = config_camera::cameraMap[camera_index].up;
               }
           }

           log.direction = direction;
           printf("方向: %s\n", direction.toUtf8().constData());


       //--------------------------开始人脸检测-------------------------------

           printf("face begin\n");
           seeta::cv::ImageData image = rgbFrame;
           std::vector<SeetaFaceInfo> faces = fenginerptr->DetectFaces(image);
           printf(" %d faces\n", faces.size());

           for (size_t i = 0; i < faces.size(); ++i)
           {
               SeetaFaceInfo &face = faces[i];

               // Query top 1
               int64_t index = -1;
               float similarity = 0.0f;

               auto points = fenginerptr->DetectPoints(image, face);


               //plot_one_box(rgbFrame,face.pos.x, face.pos.y, face.pos.x+face.pos.width, face.pos.y+face.pos.height,"face",2);
               printf("画人脸框");

               // 画五官点
               for (int j = 0; j < 5; ++j)
               {
                   const auto &point = points[j];
//                   cv::circle(frame, cv::Point(int(point.x), int(point.y)), 2, CV_RGB(128, 255, 128), -1);
               }

               std::string name;

               auto score = QA.evaluate(image, face.pos, points.data());
               if (score == 0)
               {
                   printf("score 0  ,ignored\n");
                   name = "";  // 不写 "ignored"，防止误显示
               }
               else
               {
                   auto queried = fenginerptr->QueryTop(image, points.data(), 1, &index, &similarity);
                   if (queried < 1) continue;
                   printf("similarity: %f\n", similarity);
                   if (similarity > similarity_threshold)
                   {
                       name = GalleryIndexMap[index];
                       printf(name.c_str());

                       log.name=QString::fromStdString(name).section('/',-1).section('.',-2,-2);
                       log.similarity=similarity;
                   }
               }

               // ✅ 如果有识别出姓名，就在框上方标注文字
               if (!name.empty())
               {
                   cv::putText(frame, name,
                               cv::Point(face.pos.x, face.pos.y - 5),
                               cv::FONT_HERSHEY_SIMPLEX, 1.0, CV_RGB(255, 128, 128), 2);
               }
           }





           QDateTime time = QDateTime::fromString(log.time, "yyyy-MM-dd HH:mm:ss");
           QString formattedTime = time.toString("yyyyMMdd-hhmmss");

           // 展示log
           QString log_string = QString(
               "[Camera] %1\n"
               "[Time] %2\n"
               "[Location] %3\n"
               "[Person] %4\n"
               "[Bag] %5\n"
               "[Cloth_color] %6\n"
               "[Name] %7\n"
               "[Similarity] %8%%\n"
               "[Direction] %9\n\n")
               .arg(log.cameraid)
               .arg(formattedTime)
               .arg(log.location)
               .arg(log.person)
               .arg(log.bag)
               .arg(log.clothes_color)
               .arg(log.name)
               .arg(log.similarity, 0, 'f', 2)  // 保留两位小数
               .arg(log.direction);

           ui->textBrowser->append(log_string);
           ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());

          // 新增：写入JSON
           writeLogToJson(log);

       }


    //show QT image
    QImage qimg(rgbFrame.data,
                rgbFrame.cols,
                rgbFrame.rows,
                rgbFrame.step,
                QImage::Format_RGB888);


//    ui->label->setPixmap(QPixmap::fromImage(qimg).scaled(
//        ui->label->size(),
//        Qt::KeepAspectRatio,
//        Qt::SmoothTransformation
//    ));

QPixmap pixmap("/output_face_yolo.jpg"); // 加载图片
if (!pixmap.isNull()) {
    ui->label->setPixmap(pixmap); // 设置图片
    ui->label->setScaledContents(true); // 自动缩放图片以适应 QLabel 大小
} else {
    qDebug() << "Failed to load image!";
}


}


// 将LogEntry转为JSON对象
QJsonObject detection::logToJson(const LogEntry &log) {
    QJsonObject json;
    json.insert("time", log.time);
    json.insert("location", log.location);
    json.insert("person", log.person);
    json.insert("name", log.name);
    json.insert("similarity", log.similarity);
    json.insert("direction", log.direction);
    return json;
}

// 获取当前时间戳（格式：ddhhmm_MMSS_x）
QString detection::getTimestampSuffix() {
    QDateTime now = QDateTime::currentDateTime();
    QString base = now.toString("yyyyMMdd-hhmm");  // 日时分

    // 判断是上半分钟(0-29秒)还是下半分钟(30-59秒)
    int halfMinute = now.time().second() < 30 ? 1 : 2;
    return QString("%1_%2").arg(base).arg(halfMinute);
}


void detection::writeLogToJson(const LogEntry &log) {
    QDir dir("/demo/detect/logs/");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    static QString currentFilename;
    static QStringList logLines;  // 保存每条记录的JSON字符串

    QString newFilename = getTimestampSuffix() + ".json";
    if (currentFilename != newFilename) {
        // 写入旧文件（拼接为完整的JSON数组格式）
        if (!currentFilename.isEmpty()) {
            QFile file("/demo/detect/logs/" + currentFilename);
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream out(&file);
                out.setCodec("UTF-8");
                out << "[\n";
                for (int i = 0; i < logLines.size(); ++i) {
                    out << logLines[i];
                    if (i != logLines.size() - 1)
                        out << ",\n";
                    else
                        out << "\n";
                }
                out << "]";
                file.close();

//                QString oss= "oss://qingwu-oss/oss_test/";

                QFileInfo sim_log(currentFilename);
                QString sim_logNameOnly = sim_log.fileName();


                printf("write down\n");
                ui->textBrowser_2->setTextColor(Qt::blue);
                ui->textBrowser_2->append(QString("--日志已成功写入文件: %1\n").arg(sim_logNameOnly));
                ui->textBrowser_2->setTextColor(Qt::black);

                // 创建并启动一个 QProcess 来执行 ossutil 命令
                QProcess *process_log = new QProcess(this);

                // 设置 ossutil 命令及参数
                QString command = "ossutil";
                QStringList arguments;

                arguments << "cp" << "/demo/detect/logs/"+currentFilename << oss+sim_logNameOnly;

                // 启动 QProcess 执行命令
                process_log->start(command, arguments);


                ui->textBrowser_2->setTextColor(Qt::blue);
                ui->textBrowser_2->append(QString("--日志已成功上传服务器: %1\n").arg(currentFilename));
                ui->textBrowser_2->setTextColor(Qt::black);




            }
        }

        // 重置为新文件
        currentFilename = newFilename;
        logLines.clear();
    }

    // 构造有序 JSON 字符串
    // 构造有序 JSON 字符串
    QString jsonStr = QString(
        "  {\n"
        "    \"cameraid\": \"%1\",\n"
        "    \"time\": \"%2\",\n"
        "    \"location\": \"%3\",\n"
        "    \"person\": \"%4\",\n"
        "    \"bag\": \"%5\",\n"
        "    \"cloth_color\": \"%6\",\n"
        "    \"name\": \"%7\",\n"
        "    \"similarity\": %8,\n"
        "    \"direction\": \"%9\"\n"
        "  }")
        .arg(log.cameraid)
        .arg(log.time)
        .arg(log.location)
        .arg(log.person)
        .arg(log.bag)
        .arg(log.clothes_color)
        .arg(log.name)
        .arg(log.similarity, 0, 'f', 2)
        .arg(log.direction);

    logLines.append(jsonStr);  // 加入列表
}


// 获取视频文件名（与JSON日志命名规则一致）
QString detection::getVideoFilename() {
    QDateTime now = QDateTime::currentDateTime();
    QString base = now.toString("yyyyMMdd-hhmm");  // 日时分
    int halfMinute = now.time().second() < 30 ? 1 : 2;
    return QString("/demo/detect/videos/%1_%2.mp4").arg(base).arg(halfMinute);
}

void detection::splitVideoFile() {
    if (videoWriter.isOpened()) {
        videoWriter.release();


        QFileInfo cur_file(currentVideoFilename);
        QString cur_fileNameOnly = cur_file.fileName();



        ui->textBrowser_2->setTextColor(Qt::red);
        ui->textBrowser_2->append(QString("--视频保存成功: %1\n").arg(cur_fileNameOnly));
        ui->textBrowser_2->setTextColor(Qt::black);
        isRecording = false;

        // 开启视频精简处理
        QDir simplifiedDir("/demo/detect/videos/simplified");
        QFileInfo fileInfo(currentVideoFilename);
        QString simplifiedPath = simplifiedDir.filePath(fileInfo.baseName() + "_simplified.mp4");

        QtConcurrent::run(this, &detection::simplifyVideo, currentVideoFilename, simplifiedPath);

        //simplifyVideo(currentVideoFilename, simplifiedPath);


        QFileInfo sim_file(simplifiedPath);
        QString sim_fileNameOnly = fileInfo.fileName();

        ui->textBrowser_2->setTextColor(Qt::red);
        ui->textBrowser_2->append(QString("--开启视频裁剪处理: \n%1\n").arg(sim_fileNameOnly));
        ui->textBrowser_2->setTextColor(Qt::black);

    }

    // 生成带时间戳的文件名（与JSON日志命名一致）
    currentVideoFilename = QString("/demo/detect/videos/%1_%2.mp4")
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmm"))
        .arg(QDateTime::currentDateTime().time().second() < 30 ? 1 : 2);

    // 初始化VideoWriter（RV1126推荐使用MJPG编码）
    int fourcc = cv::VideoWriter::fourcc('M','J','P','G');
    videoWriter.open(
        currentVideoFilename.toStdString(),
        fourcc,
        15,  // 帧率
        cv::Size(640, 480),  // 分辨率（需与帧尺寸一致）
        true  // 彩色视频
    );

    if (!videoWriter.isOpened()) {
        qDebug() << "Failed to open video writer!";
    } else {
        isRecording = true;
    }
}



void detection::simplifyVideo(const QString &videoPath, const QString &outputPath) {

    //裁剪前后的帧数
    int totalFramesBefore = 0;
    int totalFramesAfter = 0;


    cv::VideoCapture cap(videoPath.toStdString());
    if (!cap.isOpened()) {
        qDebug() << "无法打开视频：" << videoPath;
        return;
    }

    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter writer(
        outputPath.toStdString(),
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        fps,
        cv::Size(frameWidth, frameHeight)
    );


    std::deque<bool> movementHistory; // 移动历史窗口
    cv::Mat prevFrameGray;
    std::vector<cv::Point2f> prevCenters;

    while (true)
    {
        cv::Mat frame;
        if (!cap.read(frame)) break;

        totalFramesBefore++;  // 原始帧数统计

        // 1. YOLO 检测是否存在 person
        yolov5_detect_result_group_t detect_result_group;
        yolov5_detect_run(ctx, frame, &detect_result_group);

        std::vector<cv::Point2f> currentCenters;
        for (int i = 0; i < detect_result_group.count; i++) {
            yolov5_detect_result_t *det_result = &(detect_result_group.results[i]);
            if (det_result->prop < 0.3) continue;
            if (strcmp(det_result->name, "person") != 0) continue;

            int x1 = det_result->box.left;
            int y1 = det_result->box.top;
            int x2 = det_result->box.right;
            int y2 = det_result->box.bottom;
            int cx = (x1 + x2) / 2;
            int cy = (y1 + y2) / 2;
            currentCenters.push_back(cv::Point2f(cx, cy));
        }

        bool isMoving = false;
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        if (currentCenters.empty()) {
            isMoving = false;
        } else if (prevFrameGray.empty()) {
            isMoving = true; // 第1帧默认保留
        } else {
            cv::Mat flow;
            cv::calcOpticalFlowFarneback(
                prevFrameGray, gray, flow,
                config::pyrScale, config::levels, config::winsize, config::iterations, config::polyN, config::polySigma, config::flags
            );

            for (const auto &center : currentCenters) {
                double moveDist = 0.0;
                if (!prevCenters.empty()) {
                    double minDist = std::numeric_limits<double>::max();
                    for (const auto &prevCenter : prevCenters) {
                        double dist = cv::norm(center - prevCenter);
                        if (dist < minDist) minDist = dist;
                    }
                    moveDist = minDist;
                }

                // 位移判断
                if (moveDist > config::movementThreshold) {
                    isMoving = true;
                    break;
                }

                // 光流判断
                int x = static_cast<int>(center.x);
                int y = static_cast<int>(center.y);
                if (x >= 0 && y >= 0 && x < flow.cols && y < flow.rows) {
                    const cv::Point2f flowVec = flow.at<cv::Point2f>(y, x);
                    double flowMag = std::sqrt(flowVec.x * flowVec.x + flowVec.y * flowVec.y);
                    if (flowMag > 1.0) {
                        isMoving = true;
                        break;
                    }
                }
            }
        }

        // 更新移动历史窗口
        movementHistory.push_back(isMoving);
        if (movementHistory.size() > config::WINDOW_SIZE) {
            movementHistory.pop_front();
        }

        // 判断滑动窗口内是否有人在动
        bool anyMoving = std::any_of(
            movementHistory.begin(), movementHistory.end(),
            [](bool m) { return m; }
        );

        if (anyMoving) {
            writer.write(frame);
            totalFramesAfter++;  // 写入帧数统计
        }

        prevCenters = currentCenters;
        prevFrameGray = gray.clone();
    }

    cap.release();
    writer.release();

    double ratio = 0.0;
    if (totalFramesBefore > 0)
        ratio = (double)totalFramesAfter / totalFramesBefore * 100.0;

    QFileInfo sim_file(currentVideoFilename);
    QString sim_fileNameOnly = sim_file.fileName()+ "_simplified.mp4";

    QString result = QString(
        "[裁剪完成]%1 \n原始帧数: %2 裁剪后帧数: %3，\n保留比例: %4%\n")
        .arg(sim_fileNameOnly)
        .arg(totalFramesBefore)
        .arg(totalFramesAfter)
        .arg(QString::number(ratio, 'f', 2));  // 保留两位小数



    emit simplifyFinished(outputPath, sim_fileNameOnly, result);



    result=  QString(
        "原始帧数: %1 \n裁剪后帧数: %2 \n保留比例: %3%")
        .arg(totalFramesBefore)
        .arg(totalFramesAfter)
        .arg(QString::number(ratio, 'f', 2));  // 保留两位小数

    // 创建sim_log目录
    QDir logDir("/demo/detect/videos/simplified/sim_log");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    // 创建同名的txt文件在sim_log目录下
    QFileInfo outputInfo(outputPath);
    QString txtPath = logDir.filePath(outputInfo.baseName() + ".txt");

    QFile txtFile(txtPath);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&txtFile);
        out.setCodec("UTF-8");
        out << result;
        txtFile.close();
    } else {
        qDebug() << "无法创建结果文件：" << txtPath;
    }





}

void detection::on_pushButton_6_clicked()
{
    QMessageBox::information(this, "确认", "模型正在加载");
    //-----------------------人脸初始化开始--------------------------------


    seeta::ModelSetting FDmode("/demo/bin/fd_2_00.dat", seeta::ModelSetting::GPU, 0);
    seeta::ModelSetting PDmode("/demo/bin/pd_2_00_pts5.dat", seeta::ModelSetting::GPU, 0);
    seeta::ModelSetting FRmode("/demo/bin/fr_2_10.dat", seeta::ModelSetting::GPU, 0);


    fenginerptr = new seeta::FaceEngine(FDmode, PDmode, FRmode);
    printf("face model opened");


    // recognization threshold
    float similarity_threshold = 0.15f;

    //set face detector's min face size
    fenginerptr->FD.set( seeta::FaceDetector::PROPERTY_MIN_FACE_SIZE, 80 );

    //GalleryImageFilename = { "/demo/bin/pic/wlh-8252.png","/demo/bin/pic/mgl-8251.png","/demo/bin/pic/lyy-8247.png" };
    GalleryImageFilename = { "/demo/bin/pic/wlh-8252.png"};
    GalleryIndex.resize(GalleryImageFilename.size());


    for( size_t i = 0; i < GalleryImageFilename.size(); ++i )
    {
        //register face into facedatabase
        std::string &filename = GalleryImageFilename[i];
        int64_t &index = GalleryIndex[i];
        std::cerr <<endl;
        std::cerr << "Registering... " << filename ;

        seeta::cv::ImageData image = cv::imread( filename );
        auto id = fenginerptr->Register( image );
        index = id;
        std::cerr << "Registered id = " << id ;
    }


    for( size_t i = 0; i < GalleryIndex.size(); ++i )
    {
        // save index and name pair
        if( GalleryIndex[i] < 0 ) continue;
        GalleryIndexMap.insert( std::make_pair( GalleryIndex[i], GalleryImageFilename[i] ) );
    }

    //-----------------------人脸初始化结束--------------------------------

    QMessageBox::information(this, "确认", "模型加载完成");

}

// 主线程槽函数
void detection::handleSimplifyResult(QString simplifiedPath, QString fileName, QString resultText) {
    // 1. 上传 oss
    QProcess *process = new QProcess(this);
    QStringList arguments;
    arguments << "cp" << simplifiedPath << oss + fileName;
    process->start("ossutil", arguments);

    // 2. 显示到 UI
    ui->textBrowser_2->setTextColor(Qt::red);
    ui->textBrowser_2->append(QString("--裁剪视频已成功上传服务器: %1\n").arg(fileName));
    ui->textBrowser_2->setTextColor(Qt::black);

    ui->textBrowser_2->append(resultText);
}

// 显示下一帧
void detection::nextFrame1() {
    cv::Mat frame;
    if (!cap1.read(frame)) {
        timer->stop();
        cap1.release();
        return;
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio));
}


