#ifndef DETECT_H
#define DETECT_H

#include "detect_postprocess.h"
#include "rknn_api.h"
#include <opencv2/opencv.hpp>
#include <unistd.h>
static cv::Mat algorithm_image;

// fmt: BGRA8888
static cv::Scalar colorArray[10] = {
    cv::Scalar(0,   0,   255, 255),
    cv::Scalar(0,   255, 0,   255),
    cv::Scalar(139, 0,   0,   255),
    cv::Scalar(0,   100, 0,   255),
    cv::Scalar(0,   139, 139, 255),
    cv::Scalar(0,   206, 209, 255),
    cv::Scalar(255, 127, 0,   255),
    cv::Scalar(72,  61,  139, 255),
    cv::Scalar(0,   255, 0,   255),
    cv::Scalar(255, 0,   0,   255),
};


/*
 * yolov5检测初始化函数
 * ctx:输入参数,rknn_context句柄
 * path:输入参数,算法模型路径
 */
int yolov5_detect_init(rknn_context *ctx, const char *path);


/*
 * yolov5检测执行函数
 * ctx:输入参数,rknn_context句柄
 * input_image:输入参数,图像数据输入(cv::Mat是Opencv的类型)
 * output_dets:输出参数，目标检测框输出
 */
int yolov5_detect_run(rknn_context ctx, cv::Mat input_image, yolov5_detect_result_group_t *detect_result_group);


/*
 * yolov5检测释放函数
 * ctx:输入参数,rknn_context句柄
 */
int yolov5_detect_release(rknn_context ctx);



static void printRKNNTensor(rknn_tensor_attr *attr);
static int letter_box(cv::Mat input_image, cv::Mat *output_image, int model_input_size);

int plot_one_box(cv::Mat src, int x1, int x2, int y1, int y2, char *label, char colour);

#endif // DETECT_H
