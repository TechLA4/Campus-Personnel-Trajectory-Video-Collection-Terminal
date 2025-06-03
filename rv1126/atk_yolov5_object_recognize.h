#ifndef _ATK_YOLOV5_OBJECT_RECOGNIZE_H
#define _ATK_YOLOV5_OBJECT_RECOGNIZE_H

#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <dlfcn.h>
#include <assert.h>
#include <fcntl.h>
#include <time.h>
#include <string>
#include <sstream>
#include <ctime>


#define _BASETSD_H

#include "im2d.h"
#include "rga.h"
#include "include/drm_func.h"
#include "rga_func.h"
#include "rknn_api.h"
#include "rkmedia_api.h"
#include "postprocess.h"
#include "sample_common.h"
#include "opencv2/opencv.hpp"
#include "rockx.h"
#include "face_database.h"
#include "opencv2/freetype.hpp"

typedef struct {
    char time[40];
    char location[64];
    char type[32];
    char name[32];
    float confidence;
    char direction[32];
} DetectionLog;

struct TrackedPerson {
    int center_x;
    int center_y;
};

//std::vector<DetectionLog> detection_logs; // 存储所有检测记录
//pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

//int disp_width = 720;
//int disp_height = 1280;
//RK_U32 video_width = 2592;
//RK_U32 video_height = 1944;

static bool quit = false;

int rgb24_resize(unsigned char *input_rgb, unsigned char *output_rgb, int width,int height, int outwidth, int outheight);

static unsigned char *load_model(const char *filename, int *model_size);

static void printRKNNTensor(rknn_tensor_attr *attr);

void *rkmedia_rknn_thread(void *args);


//FACE
# define RKAIQ 1
using namespace cv;

#define True 1
#define False 0

void *rkmedia_vi_rockx_thread(void *args);
void *rkmedia_vo_rockx_thread(void *args);
void face_data_init();
static void print_usage(const RK_CHAR *name);
static void sigterm_handler(int sig);
int recognize();



#endif
