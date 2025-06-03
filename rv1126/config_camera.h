#ifndef CONFIG_CAMERA_H
#define CONFIG_CAMERA_H

#include <QWidget>
#include <QMap>
#include<QMessageBox>
#include <QNetworkInterface>




namespace Ui {
class config_camera;
}

class config_camera : public QWidget
{
    Q_OBJECT

public:
    explicit config_camera(QWidget *parent = nullptr);
    ~config_camera();

    static QString cameraid;
    static QString Location;
    // 摄像头信息结构体
    struct CameraInfo {
        int id;
        double longitude;
        double latitude;
        QString location;
        QString up;
        QString down;
        QString left;
        QString right;
    };
    static QMap<int, CameraInfo> cameraMap;



signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息

private slots:
    void onComboBoxChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::config_camera *ui;




    void initializeCameraData();

    void showWlanIpAddress();

};

#endif // CONFIG_CAMERA_H
