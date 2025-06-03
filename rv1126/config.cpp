#include "config.h"
#include "ui_config.h"
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include<QMessageBox>


//--------视频裁剪相关参数-------
//中心点位移阈值
double config::movementThreshold = 5;  // 单位像素，中心点移动超过这个距离视为“移动”
// 光流法参数
double config::pyrScale = 0.5; // pyramidScale：金字塔图像每层缩放因子（越小层数越多，计算更稳定但慢）
int config::levels = 2; // levels：图像金字塔层数（越多能检测越大位移）
int config::winsize = 10; // winsize：窗口大小，越大越平滑但可能丢细节（通常为15~25）
int config::iterations = 1; // iterations：每层迭代次数，影响精度与速度
int config::polyN = 5; // polyN：每个像素用于拟合多项式的邻域大小（建议为5或7）
double config::polySigma = 1.1; // polySigma：高斯滤波的σ，影响平滑程度（推荐为1.1 ~ 1.5）
int config::flags = 0;
int config::WINDOW_SIZE = 10; // 滑动窗口帧数





config::config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);
    //windowsize
    ui->doubleSpinBox->setValue(10); // 将默认值设置为 10
    ui->doubleSpinBox->setRange(0, 20); // 设置数值范围
    ui->doubleSpinBox->setSingleStep(5);   // 设置步长
    ui->doubleSpinBox->setDecimals(0);       // 设置小数位数

    //levels
    ui->doubleSpinBox_2->setValue(1);
    ui->doubleSpinBox_2->setRange(1, 3); // 设置数值范围
    ui->doubleSpinBox_2->setSingleStep(1);   // 设置步长
    ui->doubleSpinBox_2->setDecimals(0);       // 设置小数位数

    //iteration
    ui->doubleSpinBox_3->setValue(1);
    ui->doubleSpinBox_3->setRange(1, 3); // 设置数值范围
    ui->doubleSpinBox_3->setSingleStep(1);   // 设置步长
    ui->doubleSpinBox_3->setDecimals(0);       // 设置小数位数

    //move
    ui->doubleSpinBox_4->setValue(5);
    ui->doubleSpinBox_4->setRange(1, 20); // 设置数值范围
    ui->doubleSpinBox_4->setSingleStep(5);   // 设置步长
    ui->doubleSpinBox_4->setDecimals(0);       // 设置小数位数





}

config::~config()
{
    delete ui;
}

void config::on_pushButton_3_clicked()
{
    emit sendsignal();
    //this->close();
}


void config::on_pushButton_clicked()
{

   // 从 UI 控件读取值并赋给静态变量
   WINDOW_SIZE = static_cast<int>(ui->doubleSpinBox->value());  // 转为 int
   levels = static_cast<int>(ui->doubleSpinBox_2->value());    // 转为 int
   iterations = static_cast<int>(ui->doubleSpinBox_3->value()); // 转为 int
   movementThreshold = ui->doubleSpinBox_4->value();           // 直接 double

   QMessageBox::information(this, "确认", "已经传递参数！");

}
