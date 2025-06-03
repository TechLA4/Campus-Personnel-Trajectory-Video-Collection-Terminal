#-------------------------------------------------
#
# Project created by QtCreator 2025-05-05T23:15:52
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets sql network concurrent
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rv1126
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    config.cpp \
    detection.cpp \
    atk_yolov5_object_recognize.cpp \
    face_database.cpp \
    drm_func.c \
    rga_func.c \
    postprocess.cc \
    database_inf.cpp \
    database_log.cpp \
    database_video.cpp \
    detect_postprocess.cpp \
    detect.cpp \
    facedetect.cpp \
    config_camera.cpp

HEADERS += \
        mainwindow.h \
    config.h \
    detection.h \
    atk_yolov5_object_recognize.h \
    face_database.h \
    include/drm_func.h \
    include/postprocess.h \
    include/rga_func.h \
    include/rknn_api.h \
    include/3rdparty/CImg/CImg.h \
    include/3rdparty/drm/include/demo.h \
    include/3rdparty/drm/include/demo_define.h \
    include/3rdparty/drm/include/tool.h \
    include/3rdparty/drm/include/xf86drm.h \
    include/3rdparty/drm/include/xf86drmMode.h \
    include/3rdparty/drm/include/libdrm/amdgpu_drm.h \
    include/3rdparty/drm/include/libdrm/drm.h \
    include/3rdparty/drm/include/libdrm/drm_fourcc.h \
    include/3rdparty/drm/include/libdrm/drm_mode.h \
    include/3rdparty/drm/include/libdrm/drm_sarea.h \
    include/3rdparty/drm/include/libdrm/i915_drm.h \
    include/3rdparty/drm/include/libdrm/mach64_drm.h \
    include/3rdparty/drm/include/libdrm/mga_drm.h \
    include/3rdparty/drm/include/libdrm/nouveau_drm.h \
    include/3rdparty/drm/include/libdrm/qxl_drm.h \
    include/3rdparty/drm/include/libdrm/r128_drm.h \
    include/3rdparty/drm/include/libdrm/radeon_drm.h \
    include/3rdparty/drm/include/libdrm/savage_drm.h \
    include/3rdparty/drm/include/libdrm/sis_drm.h \
    include/3rdparty/drm/include/libdrm/tegra_drm.h \
    include/3rdparty/drm/include/libdrm/vc4_drm.h \
    include/3rdparty/drm/include/libdrm/via_drm.h \
    include/3rdparty/drm/include/libdrm/virtgpu_drm.h \
    include/3rdparty/rga/include/drmrga.h \
    include/3rdparty/rga/include/rga.h \
    include/3rdparty/rga/include/RgaApi.h \
    include/3rdparty/rga/include/RockchipFileOps.h \
    include/3rdparty/rga/include/RockchipRga.h \
    include/3rdparty/rga/include/RockchipRgaMacro.h \
    include/3rdparty/stb/stb_image.h \
    include/3rdparty/stb/stb_image_resize.h \
    include/3rdparty/stb/stb_image_write.h \
    database_inf.h \
    database_log.h \
    database_video.h \
    detect_postprocess.h \
    detect.h \
    facedetect.h \
    config_camera.h

FORMS += \
        mainwindow.ui \
    config.ui \
    detection.ui \
    database_inf.ui \
    database_log.ui \
    database_video.ui \
    config_camera.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    SOURCE/images.qrc

INCLUDEPATH += \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rga \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/easymedia \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/uAPI \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/xcore \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/algos \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/common \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rkaiq/iq_parser \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rknn \
    /opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot/usr/include/rockx \
    ./include \
    ./include/3rdparty \
    ./include/3rdparty/drm/include/libdrm \
    ./include/3rdparty/rga/include
    ./include/seeta \

LIBS += -L$$PWD/lib
LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_freetype -lopencv_highgui -lopencv_videoio
LIBS += -leasymedia -lrga -lrkaiq -lrknn_api -lsample_common_isp -lsqlite3 -lrockx
LIBS += -lpthread -ldl
LIBS += -lSeetaFaceDetector -lSeetaFaceLandmarker -lSeetaFaceRecognizer -lSeetaQualityAssessor -lSeetaNet
LIBS += -lopencv_video


QMAKE_CXXFLAGS += -g -O0 -ggdb -gdwarf -funwind-tables -rdynamic -Wno-write-strings -Wno-return-type
DEFINES += RKAIQ

DISTFILES += \
    1.mp4

