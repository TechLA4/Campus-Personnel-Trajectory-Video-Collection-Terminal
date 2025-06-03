#include "facedetect.h"
#include "qdebug.h"
#include <FaceEngine.h>
#include<FaceDetector.h>
#include <Struct.h>
#include <Struct_cv.h>
#include <FaceDetector.h>
#include <FaceRecognizer.h>


FaceDetect::FaceDetect(QObject *parent)
    : QObject{parent}
{
    seeta::ModelSetting FDmode("/demo/bin/fd_2_00.dat", seeta::ModelSetting::CPU, 0);
    seeta::ModelSetting PDmode("/demo/bin/pd_2_00_pts5.dat", seeta::ModelSetting::CPU, 0);
    seeta::ModelSetting FRmode("/demo/bin/fr_2_10.dat", seeta::ModelSetting::CPU, 0);

    this->fenginerptr = new seeta::FaceEngine(FDmode, PDmode, FRmode);




    //导入已有的人脸数据库
    this->fenginerptr->Load("./face.db");

}

FaceDetect::~FaceDetect()
{
    delete fenginerptr;
}

//人脸注册
int64_t FaceDetect::face_register(Mat &faceimage)
{
    //mat数据转为SeetaImageData数据
    SeetaImageData simage;
    simage.data = faceimage.data;
    simage.width = faceimage.cols;
    simage.height = faceimage.rows;
    simage.channels = faceimage.channels();
    //返回人脸id
    int64_t faceid = this->fenginerptr->Register(simage);
    qDebug()<<"faceid:"<<faceid;
    if(faceid >= 0){
        fenginerptr->Save("./face.db");
    }
    return faceid;
}

//人脸查询
int FaceDetect::face_query(Mat &faceimage)
{
    //mat数据转为SeetaImageData数据
    SeetaImageData simage;
    simage.data = faceimage.data;
    simage.width = faceimage.cols;
    simage.height = faceimage.rows;
    simage.channels = faceimage.channels();
    float similarity = 0;
    int64_t faceid = fenginerptr->Query(simage,&similarity);
    qDebug()<<"similarity:"<<similarity;
    qDebug()<<"faceid:"<<faceid;
    if(similarity > 0.5){
        emit send_faceid(faceid);
    }else{
//        emit send_faceid(-1);
    }
    return faceid;
}

