# @author        Liang Huanyan
# @email         lianghuanyan@alientek.com
# http://www.openedv.com/forum.php

cmake_minimum_required(VERSION 3.8)
message(STATUS "cmake version ${CMAKE_VERSION}")

set(TOOLCHAIN_DIR /opt/atk-dlrv1126-toolchain)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/usr/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/usr/bin/arm-linux-gnueabihf-gcc)
set(SYSROOT ${TOOLCHAIN_DIR}/arm-buildroot-linux-gnueabihf/sysroot/usr/include)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/arm-buildroot-linux-gnueabihf/sysroot)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-g -O0 -ggdb -gdwarf -funwind-tables -rdynamic)
add_definitions(-Wno-write-strings -Wno-return-type)

set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_imgproc opencv_features2d opencv_flann opencv_highgui opencv_freetype)
set(RKNN_LIBS easymedia rga rkaiq rknn_api sample_common_isp sqlite3 rockx)
link_directories(include lib)
	
include_directories(${SYSROOT})
include_directories(${SYSROOT}/rga)
include_directories(${SYSROOT}/easymedia)
include_directories(${SYSROOT}/rkaiq/uAPI)
include_directories(${SYSROOT}/rkaiq/xcore)
include_directories(${SYSROOT}/rkaiq/algos)
include_directories(${SYSROOT}/rkaiq/common)
include_directories(${SYSROOT}/rkaiq/iq_parser)
include_directories(${SYSROOT}/rknn)
include_directories(${SYSROOT}/rockx)
include_directories(.)
include_directories(./include)
include_directories(./include/3rdparty)
include_directories(./include/3rdparty/drm/include/libdrm)
include_directories(./include/3rdparty/rga/include)

add_definitions(-DRKAIQ)

add_executable(atk_yolov5_object_recognize atk_yolov5_object_recognize.cpp drm_func.c postprocess.cc rga_func.c face_database.cpp)
target_link_libraries(atk_yolov5_object_recognize ${OPENCV_LIBS} ${RKNN_LIBS} pthread dl)