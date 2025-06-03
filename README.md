**Note**: This project is my undergraduate graduation thesis work. The final product is a functional demo, but still requires further improvements and polish.

# 🎯 Campus Surveillance Terminal Based on YOLOv5 and Face Recognition (RV1126 + Qt)



This project is developed on a **Ubuntu 20.04 host** and cross-compiled to run on the **Rockchip RV1126 embedded platform**.
![RV1126](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/image-20250603100246175.png)
It consists of **two major components**:

------

## 📦 Project Structure

### 🔹 1. YOLOv5 + ROCKX Integration (RV1126 ATK-SDK Based)

- Combines **YOLOv5 object detection** with **ROCKX face recognition**.
- Achieves **real-time detection at 30 FPS** on the RV1126 platform.
- Runs directly on the framebuffer with **automatic fullscreen output**.

> **Limitations**: Lacks extended UI display and limits information output.

------

### 🔹 2. Complete Qt-Based Surveillance Terminal

A full-featured embedded terminal with a multi-window Qt GUI.

### 🏗️ System Architecture

![Architecture](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/image-20250603094233177.png)

------

## 🚀 Development Workflow

- Development and cross-compilation are done on a **Ubuntu host**.
- Deployment and execution are performed on the **RV1126 board**.

![Development Workflow](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps3.jpg)

------

## 🧹 Qt GUI Structure

The system is composed of **7 custom Qt Widgets**, displayed in a user-friendly navigation flow:

1. **Main Window** – Homepage (MainWindow)
2. **Pre-launch Configuration**:
   - Camera Settings (`Config_camera`)
   - Video Cropping Parameters (`Config`)
   - Personnel Database Info (`Database_inf`)
3. **Runtime Detection** – Real-time Object & Face Detection (`Detection`)
4. **Post-processing Display**:
   - Keyframe Annotation Result (`Database_log`)
   - Cropped Video Management (`Database_video`)

![GUI Flow](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps5.jpg)

------

## 📸 Camera Configuration Module

Allows intuitive **camera selection, preview, and parameter setup** for different locations.

- Automatically loads available devices
- Displays camera coordinates and position
- Supports IP auto-recognition
- Parameters can be confirmed and passed to backend

![Camera Setup](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps6.jpg)

------

## 👥 People Recognition Pipeline

The detection system integrates:

- **YOLOv5** for **pedestrian detection**
- **SeetaFace** for **facial recognition**

![Recognition Modules](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps7.jpg)

------

## 🔖 Keyframe Annotation Features

For each detected person:

- Extracts attributes such as:
  - **Backpack detection**
  - **Clothing color**
  - **Motion direction prediction**

------

## ✂️ Video Frame Cropping System

Two levels of intelligent frame pruning:

1. **Non-person frame removal**:
    Filters out frames without any detected people.
2. **Static person frame removal**:
    For detected persons remaining static across multiple frames:
   - Uses **optical flow** and **central displacement** analysis for pruning.

![Cropping Algorithm](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps8.jpg)

### 📊 Cropping Result Display

- **Detection GUI** shows cropped frame count in real time
- **Database Video Page** compares saved vs cropped video data

![Cropping Display](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps9.jpg)

------

## 🌐 Network Transmission

Supports both real-time and historical data transmission:

- **RTSP streaming** to backend surveillance system
- **Aliyun OSS integration**:
  - Uploads cropped video and annotated keyframes for backend trajectory reconstruction

![RTSP + OSS](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps10.jpg)
 ![OSS Upload](https://techla-img.oss-cn-hangzhou.aliyuncs.com/CODE/WEB/wps11.jpg)

## 

------

## 🛠️ Environment & Toolchain

- **Host**: Ubuntu 20.04 LTS
- **Target**: RV1126 (ATK SDK, Rockchip Linux)
- **Cross Toolchain**: `atk-dlrv1126-toolchain`
- **Frameworks**: Qt 5, OpenCV, YOLOv5, SeetaFace, ROCKX SDK
- **Cloud**: Aliyun OSS for data sync
