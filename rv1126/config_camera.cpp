#include "config_camera.h"
#include "ui_config_camera.h"
#include <QMap>

QString config_camera::cameraid = "camera1";
QString config_camera::Location = "图书馆路口";
QMap<int, config_camera::CameraInfo> config_camera::cameraMap;


config_camera::config_camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::config_camera)
{
    ui->setupUi(this);

    // 初始化摄像头数据
    initializeCameraData();

    // 填充下拉列表
    for (int i = 1; i <= 10; ++i) {
        ui->comboBox->addItem(QString("camera%1").arg(i));
    }

    // 连接信号和槽
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &config_camera::onComboBoxChanged);

    // 默认显示第一个摄像头的信息
    if (!cameraMap.isEmpty()) {
        onComboBoxChanged(0);
    }
    showWlanIpAddress(); // 初始化时显示IP地址
}

config_camera::~config_camera()
{
    delete ui;
}


void config_camera::initializeCameraData()
{
    // 添加所有摄像头信息
    cameraMap[1] = {1, 118.717583, 30.909502, "图书馆路口","东","南","西","北"};
    cameraMap[2] = {2, 118.719124, 30.909018, "行政楼正门","东","南","西","北"};
    cameraMap[3] = {3, 118.71785, 30.910971, "敬亭学堂正门入口","东","南","西","北"};
    cameraMap[4] = {4, 118.719719, 30.91102, "新安学堂侧门入口","东","南","西","北"};
    cameraMap[5] = {5, 118.721458, 30.9110955, "体育馆正门","西","北","东","南"};
    cameraMap[6] = {6, 118.720963, 30.911787, "食堂侧门入口","南","西","东","北"};
    cameraMap[7] = {7, 118.722328, 30.911974, "操场正门","西","北","东","南"};
    cameraMap[8] = {8, 118.721977, 30.913212, "宿舍楼1、2路口","西","北","东","南"};
    cameraMap[9] = {9, 118.719971, 30.913881, "宿舍楼5路口","南","西","东","北"};
    cameraMap[10] = {10, 118.71761, 30.908051, "大学生活动中心""西","东","北","南"};
}

void config_camera::onComboBoxChanged(int index)
{
    // 获取当前选择的摄像头ID（index从0开始，所以+1）
    int cameraId = index + 1;

    // 检查是否存在该摄像头
    if (cameraMap.contains(cameraId)) {
        const CameraInfo &info = cameraMap[cameraId];

        // 更新UI显示
        ui->textEdit_2->setText(info.location);
        ui->textEdit_3->setText(QString::number(info.longitude, 'f', 6));
        ui->textEdit_4->setText(QString::number(info.latitude, 'f', 6));
    }
}


void config_camera::showWlanIpAddress()
{
    QString interfaceName = "wlan0"; // 指定要查询的接口名称
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    foreach (const QNetworkInterface &interface, interfaces) {
        if (interface.name() == interfaceName && interface.isValid()) {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach (const QNetworkAddressEntry &entry, entries) {
                if (!entry.ip().isNull() && entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ui->textEdit_5->setText(entry.ip().toString());
                    return;
                }
            }
        }
    }

    // 如果没有找到wlan0或没有有效IP地址
    ui->textEdit_5->setText("wlan0 IP address not found");
}




void config_camera::on_pushButton_2_clicked()
{
    // 确保有选中项
    if(ui->comboBox->currentIndex() >= 0) 
    {
        //传递摄像头编号、位置参数
        cameraid = ui->comboBox->currentText();
        Location = ui->textEdit_2->toPlainText();
        QMessageBox::information(this, "确认", "已经传递参数！");
    }
    else
    {
        QMessageBox::warning(this, "警告", "请先选择一个摄像头！");
    }

}



void config_camera::on_pushButton_clicked()
{
     emit sendsignal();
}
