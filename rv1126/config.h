#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>


namespace Ui {
class config;
}

class config : public QWidget
{
    Q_OBJECT

public:
    explicit config(QWidget *parent = nullptr);
    ~config();


    //--------视频裁剪相关参数-------
    //中心点位移阈值
    static double movementThreshold;  // 单位像素，中心点移动超过这个距离视为“移动”
    // 光流法参数
    static double pyrScale ; // pyramidScale：金字塔图像每层缩放因子（越小层数越多，计算更稳定但慢）
    static int levels; // levels：图像金字塔层数（越多能检测越大位移）
    static int winsize; // winsize：窗口大小，越大越平滑但可能丢细节（通常为15~25）
    static int iterations ; // iterations：每层迭代次数，影响精度与速度
    static int polyN ; // polyN：每个像素用于拟合多项式的邻域大小（建议为5或7）
    static double polySigma; // polySigma：高斯滤波的σ，影响平滑程度（推荐为1.1 ~ 1.5）
    static int flags ;
    static int WINDOW_SIZE; // 滑动窗口帧数


private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();

signals:
    void sendsignal();//这个函数用户向主界面通知关闭的消息




private:
    Ui::config *ui;
};

#endif
