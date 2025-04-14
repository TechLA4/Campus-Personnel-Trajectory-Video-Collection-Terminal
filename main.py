# main.py
import sys
import cv2
import os
import time
import numpy as np
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QHBoxLayout, QTextEdit
from PyQt5.QtCore import QTimer, Qt
from PyQt5.QtGui import QImage, QPixmap

from utils.yolo_detector import YoloFaceDetector
from utils.face_recog import FaceRecognizer
from utils.logger import log_recognition

class FaceRecognitionApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("实时人脸识别系统")
        self.setGeometry(100, 100, 1000, 600)

        self.initUI()

        # 加载YOLO模型与人脸识别器
        self.detector = YoloFaceDetector('yolov5_face.pt')
        self.recognizer = FaceRecognizer('known_faces/')

        # 打开摄像头
        self.cap = cv2.VideoCapture(0)

        # 定时器，每30ms读取一次帧
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(30)

    def initUI(self):
        self.video_label = QLabel()
        self.video_label.setFixedSize(640, 480)

        self.log_text = QTextEdit()
        self.log_text.setReadOnly(True)

        layout = QHBoxLayout()
        layout.addWidget(self.video_label)
        layout.addWidget(self.log_text)

        self.setLayout(layout)

    def update_frame(self):
        ret, frame = self.cap.read()
        if not ret:
            return

        # 检测人脸并识别身份
        results = self.detector.detect(frame)
        identities = self.recognizer.identify_faces(frame, results)

        # 在图像上绘制检测框和ID
        for (x1, y1, x2, y2), (name, conf) in zip(results, identities):
            label = f'{name} ({conf:.2f})' if name != "Unknown" else "Unknown"
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 255, 0), 2)
            log_recognition(name, conf)

            # 展示在右侧日志区
            now = time.strftime('%Y-%m-%d %H:%M:%S')
            self.log_text.append(f"[{now}] {label}")

        # 转为Qt图像显示
        rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        qt_img = QImage(rgb_image.data, w, h, ch * w, QImage.Format_RGB888)
        self.video_label.setPixmap(QPixmap.fromImage(qt_img))

    def closeEvent(self, event):
        self.cap.release()
        self.timer.stop()
        super().closeEvent(event)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = FaceRecognitionApp()
    window.show()
    sys.exit(app.exec_())
