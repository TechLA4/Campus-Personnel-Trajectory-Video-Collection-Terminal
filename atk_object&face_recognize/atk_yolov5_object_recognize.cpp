// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "atk_yolov5_object_recognize.h"
#include <sys/time.h>  // 用于获取时间戳
#include <fstream>
#include <vector>




int main(int argc, char *argv[])
{

  //-------------------初始化开始----------------
  RK_CHAR *pDeviceName_01 = "rkispp_scale0";
  RK_CHAR *pDeviceName_02 = "rkispp_scale1";
  RK_CHAR *pcDevNode = "/dev/dri/card0";
  char *iq_file_dir = "/etc/iqfiles";
  RK_S32 s32CamId = 0;
  RK_U32 u32BufCnt = 3;
  RK_U32 fps = 20;
  int ret;

  printf("\n###############################################\n");
  printf("VI CameraIdx: %d\npDeviceName: %s\nResolution: %dx%d\n\n",
          s32CamId,pDeviceName_01,video_width,video_height);

  printf("VO pcDevNode: %s\nResolution: %dx%d\n",
          pcDevNode,disp_width,disp_height);
  printf("###############################################\n\n");


  //初始化和运行图像信号处理（ISP）模块
  if (iq_file_dir) 
  {
#ifdef RKAIQ
  printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
  rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
  SAMPLE_COMM_ISP_Init(s32CamId,hdr_mode, RK_FALSE,iq_file_dir);
  SAMPLE_COMM_ISP_Run(s32CamId);
  SAMPLE_COMM_ISP_SetFrameRate(s32CamId,25);
#endif
  }


  //设置视频输入VI通道
  RK_MPI_SYS_Init();
  VI_CHN_ATTR_S vi_chn_attr_01;
  memset(&vi_chn_attr_01, 0, sizeof(vi_chn_attr_01));
  vi_chn_attr_01.pcVideoNode = pDeviceName_01;
  vi_chn_attr_01.u32BufCnt = u32BufCnt;
  vi_chn_attr_01.u32Width = video_width;
  vi_chn_attr_01.u32Height = video_height;
  vi_chn_attr_01.enPixFmt = IMAGE_TYPE_NV12;
  vi_chn_attr_01.enBufType = VI_CHN_BUF_TYPE_MMAP;
  vi_chn_attr_01.enWorkMode = VI_WORK_MODE_NORMAL;
  ret = RK_MPI_VI_SetChnAttr(s32CamId, 0, &vi_chn_attr_01);
  ret |= RK_MPI_VI_EnableChn(s32CamId, 0);
  if (ret)
  {
    printf("ERROR: create VI[0:0] error! ret=%d\n", ret);
    return 0;
  }

  //设置图像处理RGA通道
  RGA_ATTR_S stRgaAttr_01;
  memset(&stRgaAttr_01, 0, sizeof(stRgaAttr_01));
  stRgaAttr_01.bEnBufPool = RK_TRUE;
  stRgaAttr_01.u16BufPoolCnt = 5;
  stRgaAttr_01.u16Rotaion = 0;
  stRgaAttr_01.stImgIn.u32X = 0;
  stRgaAttr_01.stImgIn.u32Y = 0;
  stRgaAttr_01.stImgIn.imgType = IMAGE_TYPE_NV12;
  stRgaAttr_01.stImgIn.u32Width = video_width;
  stRgaAttr_01.stImgIn.u32Height = video_height;
  stRgaAttr_01.stImgIn.u32HorStride = video_width;
  stRgaAttr_01.stImgIn.u32VirStride = video_height;
  stRgaAttr_01.stImgOut.u32X = 0;
  stRgaAttr_01.stImgOut.u32Y = 0;
  stRgaAttr_01.stImgOut.imgType = IMAGE_TYPE_RGB888;
  stRgaAttr_01.stImgOut.u32Width = video_width;
  stRgaAttr_01.stImgOut.u32Height = video_height;
  stRgaAttr_01.stImgOut.u32HorStride = video_width;
  stRgaAttr_01.stImgOut.u32VirStride = video_height;
  ret = RK_MPI_RGA_CreateChn(0, &stRgaAttr_01);
  if (ret) 
  {
    printf("ERROR: create RGA[0:0] falied! ret=%d\n", ret);
    return -1;
  }

  //设置视频输出VO通道
  VO_CHN_ATTR_S stVoAttr = {0};
  stVoAttr.pcDevNode = "/dev/dri/card0";
  stVoAttr.emPlaneType = VO_PLANE_OVERLAY;
  stVoAttr.enImgType = IMAGE_TYPE_RGB888;
  stVoAttr.u16Zpos = 0;
  stVoAttr.stImgRect.s32X = 756;
  stVoAttr.stImgRect.s32Y = 12;
  stVoAttr.stImgRect.u32Width = 1080;
  stVoAttr.stImgRect.u32Height = 1920;
  stVoAttr.stDispRect.s32X = 0;
  stVoAttr.stDispRect.s32Y = 0;
  stVoAttr.stDispRect.u32Width = disp_width;
  stVoAttr.stDispRect.u32Height = disp_height;
  ret = RK_MPI_VO_CreateChn(0, &stVoAttr);
  if (ret)
  {
    printf("ERROR: create VO[0:0] failed! ret=%d\n", ret);
    return -1;
  }

  //将视频输入通道（VI[0:0]）绑定到图像处理RGA通道
  MPP_CHN_S stSrcChn;
  MPP_CHN_S stDestChn;
  printf("Bind VI[0:0] to RGA[0:0]....\n");
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32DevId = s32CamId;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_RGA;
  stSrcChn.s32DevId = s32CamId;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
  if (ret) 
  {
    printf("ERROR: bind VI[0:0] to RGA[0:0] failed! ret=%d\n", ret);
    return -1;
  }


  //-------------------初始化结束----------------


  //打开人脸识别的数据库
  open_db();
  printf("\ninitial face db !\n\n");

  //创建目标识别线程
  pthread_t rkmedia_rknn_tidp;
  pthread_create(&rkmedia_rknn_tidp, NULL, rkmedia_rknn_thread, NULL);

  printf("\n%s initial finish\n\n", __func__);



  //主进程循环等待
  while (!quit)
  {
    usleep(500000);
  }




  //解绑VI和RGA通道
  printf("UnBind VI[0:0] to RGA[0:0]....\n");
  stSrcChn.enModId = RK_ID_VI;
  stSrcChn.s32DevId = s32CamId;
  stSrcChn.s32ChnId = 0;
  stDestChn.enModId = RK_ID_RGA;
  stSrcChn.s32DevId = s32CamId;
  stDestChn.s32ChnId = 0;
  ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
  if (ret) 
  {
    printf("ERROR: unbind VI[0:0] to RGA[0:0] failed! ret=%d\n", ret);
    return -1;
  }


  //销毁进程
  RK_MPI_VO_DestroyChn(0);
  RK_MPI_RGA_DestroyChn(0);
  RK_MPI_VI_DisableChn(s32CamId, 0);
  
  if (iq_file_dir) 
  {
#if RKAIQ
    SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
  }
  return 0;
}


static unsigned char *load_model(const char *filename, int *model_size)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("fopen %s fail!\n", filename);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  unsigned int model_len = ftell(fp);
  unsigned char *model = (unsigned char *)malloc(model_len);
  fseek(fp, 0, SEEK_SET);

  if (model_len != fread(model, 1, model_len, fp))
  {
    printf("fread %s fail!\n", filename);
    free(model);
    return NULL;
  }
  *model_size = model_len;

  if (fp)
  {
    fclose(fp);
  }
  return model;
}


static void printRKNNTensor(rknn_tensor_attr *attr)
{
  printf("index=%d name=%s n_dims=%d dims=[%d %d %d %d] n_elems=%d size=%d "
         "fmt=%d type=%d qnt_type=%d fl=%d zp=%d scale=%f\n",
         attr->index, attr->name, attr->n_dims, attr->dims[3], attr->dims[2],
         attr->dims[1], attr->dims[0], attr->n_elems, attr->size, 0, attr->type,
         attr->qnt_type, attr->fl, attr->zp, attr->scale);
}


int rgb24_resize(unsigned char *input_rgb, unsigned char *output_rgb, 
                 int width,int height, int outwidth, int outheight)
{
  rga_buffer_t src =wrapbuffer_virtualaddr(input_rgb, width, height, RK_FORMAT_RGB_888);
  rga_buffer_t dst = wrapbuffer_virtualaddr(output_rgb, outwidth, outheight,RK_FORMAT_RGB_888);
  rga_buffer_t pat = {0};
  im_rect src_rect = {0, 0, width, height};
  im_rect dst_rect = {0, 0, outwidth, outheight};
  im_rect pat_rect = {0};
  IM_STATUS STATUS = improcess(src, dst, pat, src_rect, dst_rect, pat_rect, 0);
  if (STATUS != IM_STATUS_SUCCESS)
  {
    printf("imcrop failed: %s\n", imStrError(STATUS));
    return -1;
  }
  return 0;
}

void write_logs_to_json_file(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    fprintf(fp, "[\n");

    for (size_t i = 0; i < detection_logs.size(); ++i) {
        DetectionLog& log = detection_logs[i];
        fprintf(fp,
            "  {\n"
            "    \"time\": \"%s\",\n"
            "    \"location\": \"%s\",\n"
            "    \"type\": \"%s\",\n"
            "    \"name\": \"%s\",\n"
            "    \"similarity\": %.2f,\n"
            "    \"direction\": \"%s\"\n"
            "  }",
            log.time, log.location, log.type, log.name, log.confidence, log.direction);

        if (i != detection_logs.size() - 1) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "]\n");
    fclose(fp);
    printf("Logs written to %s\n", filename);
}




void* save_logs_periodically(void* arg) {
    while (true) {
        sleep(60);  // 等待 60 秒

        // 获取当前时间
        time_t now = time(nullptr);
        struct tm *ptm = localtime(&now);
        char filename[64];
        strftime(filename, sizeof(filename), "detection_logs_%Y%m%d_%H%M.json", ptm);

        // 写入 JSON 文件
        write_logs_to_json_file(filename);

        // 清空缓存
        pthread_mutex_lock(&log_mutex);
        detection_logs.clear();
        pthread_mutex_unlock(&log_mutex);
    }

    return nullptr;
}


void *rkmedia_rknn_thread(void *args)
{
  //分离当前线程,将线程状态改为unjoinable状态，确保资源的释放
  pthread_detach(pthread_self());

  //-------------------RKNN模型初始化--------------------------------------

  int ret;
  rknn_context ctx;
  int model_len = 0;
  unsigned char *model;
  static char *model_path = "/demo/bin/yolov5s_relu_rv1109_rv1126_out_opt.rknn";

  // 加载 RKNN 模型
  printf("Loading model ...\n");            
  model = load_model(model_path, &model_len);
  ret = rknn_init(&ctx, model, model_len, 0);
  if (ret < 0)
  {
    printf("rknn_init fail! ret=%d\n", ret);
    return NULL;
  }

  // 加载RKNN模型信息
  rknn_input_output_num io_num;
  ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
  if (ret != RKNN_SUCC)
  {
    printf("rknn_query fail! ret=%d\n", ret);
    return NULL;
  }
  printf("model input num: %d, output num: %d\n", io_num.n_input,io_num.n_output);

    // 输入信息
  printf("input tensors:\n");
  rknn_tensor_attr input_attrs[io_num.n_input];
  memset(input_attrs, 0, sizeof(input_attrs));
  for (unsigned int i = 0; i < io_num.n_input; i++)
  {
    input_attrs[i].index = i;
    ret = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]),sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC)
    {
      printf("rknn_query fail! ret=%d\n", ret);
      return NULL;
    }
    printRKNNTensor(&(input_attrs[i]));
  }

    //输出信息
  printf("output tensors:\n");
  rknn_tensor_attr output_attrs[io_num.n_output];
  memset(output_attrs, 0, sizeof(output_attrs));
  for (unsigned int i = 0; i < io_num.n_output; i++)
  {
    output_attrs[i].index = i;
    ret = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs[i]),sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC)
    {
      printf("rknn_query fail! ret=%d\n", ret);
      return NULL;
    }
    printRKNNTensor(&(output_attrs[i]));
  }

    // 获取模型输入图像的宽度和高度
  int channel = 3;
  int width = 0;
  int height = 0;
  if (input_attrs[0].fmt == RKNN_TENSOR_NCHW)
  {
      printf("model is NCHW input fmt\n");
      width = input_attrs[0].dims[0];
      height = input_attrs[0].dims[1];
  }
  else
  {
      printf("model is NHWC input fmt\n");
      width = input_attrs[0].dims[1];
      height = input_attrs[0].dims[2];
  }

  printf("model input height=%d, width=%d, channel=%d\n", height, width, channel);

  //-------------------RKNN模型初始化结束--------------------------------------



  //创建按时间写入log进程
  pthread_t log_save_tid;
  pthread_create(&log_save_tid, NULL, save_logs_periodically, NULL);
  


  //-------------------人脸识别模型初始化--------------------------------------
  //连接人脸数据库
  map<string, rockx_face_feature_t> database_face_map = FaceFeature(); 
  map<string, rockx_face_feature_t>::iterator database_iter; 

  rockx_ret_t ret_x;
  rockx_handle_t face_det_handle;
  rockx_handle_t face_5landmarks_handle;
  rockx_handle_t face_recognize_handle;//人脸识别句柄
  

  //路径和配置
  char *rockx_data = "/demo/src/rockx_data";
  rockx_config_t *config = rockx_create_config();
  rockx_add_config(config, ROCKX_CONFIG_DATA_PATH, rockx_data);

  //初始化RockX中人脸检测，先检测——再检测关键点——最后实现人脸识别
  ret_x = rockx_create(&face_det_handle, ROCKX_MODULE_FACE_DETECTION, config,sizeof(rockx_config_t));
  if (ret_x != ROCKX_RET_SUCCESS) 
  {
    printf("ERROR: init rockx module ROCKX_MODULE_FACE_DETECTION error %d\n", ret);
  }

  ret_x = rockx_create(&face_5landmarks_handle,ROCKX_MODULE_FACE_LANDMARK_5, config, sizeof(rockx_config_t));
  if (ret_x != ROCKX_RET_SUCCESS) 
  {
    printf("ERROR: init rockx module ROCKX_MODULE_FACE_LANDMARK_5 error %d\n",ret);
  }

  ret_x = rockx_create(&face_recognize_handle, ROCKX_MODULE_FACE_RECOGNIZE,config, sizeof(rockx_config_t));
  if (ret_x != ROCKX_RET_SUCCESS) 
  {
    printf("ERROR: init rockx module ROCKX_MODULE_FACE_RECOGNIZE error %d\n", ret);
    return NULL;
  }

  printf("\nFACE_RECOGNIZE initialized successfully.\n\n");

  //-------------------人脸识别模型初始化结束--------------------------------------



  //-------------------开始综合识别--------------------------------------
  while (!quit)
  {
    //处理从RGA通道获取的媒体缓冲区（MEDIA_BUFFER），并将其内容拷贝到一个DRM（Direct Rendering Manager）缓冲区中，然后对图像进行缩放，并将缩放后的图像数据设置为RKNN模型的输入
    //  获取媒体缓冲区
    MEDIA_BUFFER src_mb = NULL;
    src_mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_RGA, 0, -1);
    if (!src_mb)
    {
      printf("ERROR: RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
      break;
    }

    /*================================================================================
      =========================使用drm拷贝，可以使用如下代码===========================
      ================================================================================*/
    
    //  初始化DRM和RGA上下文
    rga_context rga_ctx;
    drm_context drm_ctx;
    memset(&rga_ctx, 0, sizeof(rga_context));
    memset(&drm_ctx, 0, sizeof(drm_context));

    // 分配DRM缓冲区
    int drm_fd = -1;
    int buf_fd = -1; // converted from buffer handle
    unsigned int handle;
    size_t actual_size = 0;
    void *drm_buf = NULL;

    drm_fd = drm_init(&drm_ctx);
    drm_buf = drm_buf_alloc(&drm_ctx, drm_fd, video_width, video_height, channel * 8, &buf_fd, &handle, &actual_size);
    
    //  拷贝数据到DRM缓冲区
    memcpy(drm_buf, (uint8_t *)RK_MPI_MB_GetPtr(src_mb) , video_width * video_height * channel);
    
    // 初始化RGA上下文并进行图像缩放
    void *resize_buf = malloc(height * width * channel);
    // init rga context
    RGA_init(&rga_ctx);
    img_resize_slow(&rga_ctx, drm_buf, video_width, video_height, resize_buf, width, height);
    uint32_t input_model_image_size = width * height * channel;


    //设置RKNN模型的输入数据
    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = input_model_image_size;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = resize_buf;
    ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
    if (ret < 0)
    {
      printf("ERROR: rknn_inputs_set fail! ret=%d\n", ret);
      return NULL;
    }

    //释放分配的资源
    free(resize_buf);
    drm_buf_destroy(&drm_ctx, drm_fd, buf_fd, handle, drm_buf, actual_size);
    drm_deinit(&drm_ctx, drm_fd);
    RGA_deinit(&rga_ctx);

    /*================================================================================
      =========================不使用drm拷贝，可以使用如下代码===========================
      ================================================================================*/
    /*
    rkMB_IMAGE_INFO ImageInfo={0};
    RK_MPI_MB_GetImageInfo(src_mb,&ImageInfo);
    uint32_t orig_image_size = RK_MPI_MB_GetSize(src_mb);
    unsigned char *orig_image_buf = (unsigned char *)RK_MPI_MB_GetPtr(src_mb);

    uint32_t input_model_image_size = width * height * channel;
    unsigned char *input_model_image_buf = (unsigned char *)malloc(input_model_image_size);
    rgb24_resize(orig_image_buf, input_model_image_buf, video_width, video_height, width, height);

    // Set Input Data
    rknn_input inputs[1];
    memset(inputs, 0, sizeof(inputs));
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = input_model_image_size;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = input_model_image_buf;
    ret = rknn_inputs_set(ctx, io_num.n_input, inputs);
    if (ret < 0)
    {
      printf("ERROR: rknn_inputs_set fail! ret=%d\n", ret);
      return NULL;
    }
    free(input_model_image_buf);
    */


    // 运行RKNN模型
    //printf("rknn_run\n");
    ret = rknn_run(ctx, nullptr);
    if (ret < 0)
    {
      printf("ERROR: rknn_run fail! ret=%d\n", ret);
      return NULL;
    }

    //  获取模型输出
    rknn_output outputs[io_num.n_output];
    memset(outputs, 0, sizeof(outputs));
    for (int i = 0; i < io_num.n_output; i++)
    {
        outputs[i].want_float = 0;
    }
    ret = rknn_outputs_get(ctx, io_num.n_output, outputs, NULL);
    if (ret < 0)
    {
      printf("ERROR: rknn_outputs_get fail! ret=%d\n", ret);
      return NULL;
    }

    //初始化 检测结果结构体
    detect_result_group_t detect_result_group;
    memset(&detect_result_group, 0, sizeof(detect_result_group));

    //获取输出张量的量化参数
    std::vector<float> out_scales;
    std::vector<uint8_t> out_zps;
    for (int i = 0; i < io_num.n_output; ++i)
    {
        out_scales.push_back(output_attrs[i].scale);
        out_zps.push_back(output_attrs[i].zp);
    }
    
    //设置后处理参数
    const float vis_threshold = 0.1; //可视化阈值，用于过滤低置信度的检测结果。
    const float nms_threshold = 0.5; //非极大值抑制（NMS）阈值，用于去除重复的检测框。
    const float conf_threshold = 0.3; //置信度阈值，用于过滤低置信度的检测结果。
    float scale_w = (float)width / video_width; //宽度和高度的缩放比例
    float scale_h = (float)height / video_height;

    //执行后处理
    post_process((uint8_t *)outputs[0].buf, (uint8_t *)outputs[1].buf, (uint8_t *)outputs[2].buf, height, width,
                 conf_threshold, nms_threshold, vis_threshold, scale_w, scale_h, out_zps, out_scales, &detect_result_group);
    

    //-------------------已识别到物体--------------------------------------

    //人物的方向
    static std::vector<TrackedPerson> prev_persons;

    // 对每一个目标进行处理
    for (int i = 0; i < detect_result_group.count; i++)
    {

     //获取当前检测结果的指针
      detect_result_t *det_result = &(detect_result_group.results[i]);


      //筛选掉置信度过低的对象
      if (detect_result_group.results[i].prop < 0.4)
      {
        continue;
      }

      //printf("%s",detect_result_group.results[i].name);

      //-------------------已检测到人物Person--------------------------------------
      if (strcmp(detect_result_group.results[i].name, "person") == 0)
      {

        printf(" \n Person Person Person Person Person Person \n");

        //获取媒体缓冲区
        //src_mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_RGA, 0, -1);
        if (!src_mb) 
        {
          printf("ERROR: RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
          break;
        }
        
        //初始化输入图像结构
        rockx_image_t input_image;
        memset(&input_image, 0, sizeof(rockx_image_t));
        rkMB_IMAGE_INFO ImageInfo={0};
        RK_MPI_MB_GetImageInfo(src_mb,&ImageInfo);
        input_image.width=ImageInfo.u32Width;   
        input_image.height=ImageInfo.u32Height;
        input_image.pixel_format = ROCKX_PIXEL_FORMAT_RGB888;
        input_image.size = RK_MPI_MB_GetSize(src_mb);
        input_image.data = (uint8_t *)RK_MPI_MB_GetPtr(src_mb);



         //对人物结果进行输出log
         // 获取当前时间
         struct timeval tv;
         gettimeofday(&tv, NULL);
         struct tm *ptm = localtime(&tv.tv_sec);
         char time_str[40];
         strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);
         const char* location = "Jing Ting";
         char direction[32] = "Unknown";

        //获取人物检测框的坐标和尺寸
        int left = det_result->box.left;
        int top = det_result->box.top;
        int right = det_result->box.right;
        int bottom = det_result->box.bottom;
        int w = (det_result->box.right - det_result->box.left) ;
        int h = (det_result->box.bottom - det_result->box.top) ;

        // 获取人物中心点
        // 计算当前人物中心点
        int current_center_x = (left + right) / 2;
        int current_center_y = (top + bottom) / 2;

        // 与上一帧位置比较计算移动方向
        if (!prev_persons.empty()) {
          int min_distance = INT_MAX;
          int closest_index = -1;

        // 寻找最近的历史人物
        for (size_t j = 0; j < prev_persons.size(); ++j) {
            int dx = current_center_x - prev_persons[j].center_x;
            int dy = current_center_y - prev_persons[j].center_y;
            int distance = dx*dx + dy*dy;
            
            if (distance < min_distance) {
                min_distance = distance;
                closest_index = j;
            }
        }

        // 设置移动阈值（50像素的平方）
        if (closest_index != -1 && min_distance < 2500) {
            int delta_x = current_center_x - prev_persons[closest_index].center_x;
            int delta_y = current_center_y - prev_persons[closest_index].center_y;

            // 判断主要移动方向
            if (abs(delta_x) > abs(delta_y)) {
                strcpy(direction, delta_x > 0 ? "East" : "West");
            } else {
                strcpy(direction, delta_y > 0 ? "South" : "North");
            }
        }
    }

        // 保存当前帧位置到临时列表
        static std::vector<TrackedPerson> current_persons;
        current_persons.push_back({current_center_x, current_center_y});

       
        // 输出log结构体
        DetectionLog log_entry;

        //log输入时间地点人物
        strncpy(log_entry.time, time_str, sizeof(log_entry.time) - 1);
        strncpy(log_entry.location, location, sizeof(log_entry.location) - 1);
        strncpy(log_entry.type, detect_result_group.results[i].name, sizeof(log_entry.type) - 1);
        strncpy(log_entry.direction, direction, sizeof(log_entry.direction) - 1);


        //初始化人脸检测结果结构
        rockx_object_array_t face_array;
        memset(&face_array, 0, sizeof(face_array));

        
        //调用人脸检测函数
        ret =rockx_face_detect(face_det_handle, &input_image, &face_array, nullptr);
        if (ret != ROCKX_RET_SUCCESS) 
        {
          printf("ERROR: rockx_face_detect error %d\n", ret);
        }

        //-------------------已检测到人脸Face--------------------------------------
        //如果检测到了人脸
        if (face_array.count > 0) 
        {
          printf("\n FACE FACE FACE FACE FACE FACE \n");
          //对每一张脸
          for (int i = 0; i < face_array.count; i++) 
          {
            //对检测到的人脸进行过滤，判断是否模糊
            int is_false_face;
            ret = rockx_face_filter(face_5landmarks_handle, &input_image,&face_array.object[i].box, &is_false_face);
            if (ret != ROCKX_RET_SUCCESS) 
            {
              printf("ERROR: rockx_face_filter error %d\n", ret);
            }
            if (is_false_face)
            {
              printf("\n Not Face !\n");
              continue;
            }
          
            //获取人脸边界框信息和置信度
            int left = face_array.object[i].box.left;
            int top = face_array.object[i].box.top;
            int right = face_array.object[i].box.right;
            int bottom = face_array.object[i].box.bottom;
            float face_confident = face_array.object[i].score;
            
            printf("facebox=(left,top,right,bottom)=(%d %d %d %d)\n", left, top, right, bottom);
            
            int w = face_array.object[i].box.right - face_array.object[i].box.left;
            int h = face_array.object[i].box.bottom - face_array.object[i].box.top;
            
            printf("w=right-left=%d\n", w);
            printf("h=right-left=%d\n", h);
            printf("face_confident=%f\n\n", face_confident);

            //边界框的边界检查和调整
            if (left < 0)
            {
              left = 0;
            }
            if (top < 0)
            { 
              top = 0;
            }
            
            while ((uint32_t)(left + w) >= input_image.width) 
            {
              w -= 16;
            }
            while ((uint32_t)(top + h) >= input_image.height) 
            {
              h -= 16;
            }

            /********************* 以下绘制矩形框和文字的代码可以选择任意一种********************/
            Mat show_img = Mat(input_image.height, input_image.width, CV_8UC3,RK_MPI_MB_GetPtr(src_mb));
            // 采用opencv来绘制矩形框,颜色格式是B、G、R
            cv::rectangle(show_img,cv::Point(left, top),cv::Point(right, bottom),cv::Scalar(0,0,255),3,8,0);
            
            //初始化最大人脸
            rockx_object_t max_face,cur_face;
            memset(&max_face, 0, sizeof(rockx_object_t));
            memset(&cur_face, 0, sizeof(rockx_object_t));
            cur_face = face_array.object[i];
            int cur_face_box_area = (cur_face.box.right - cur_face.box.left) *(cur_face.box.bottom - cur_face.box.top);
            int max_face_box_area = (max_face.box.right - max_face.box.left) *(max_face.box.bottom - max_face.box.top);
            if (cur_face_box_area > max_face_box_area) 
            {
              max_face = cur_face;
            }
            //人脸对齐
            rockx_image_t align_out_img;
            memset(&align_out_img, 0, sizeof(rockx_image_t));
            ret = rockx_face_align(face_5landmarks_handle, &input_image,&(max_face.box), nullptr, &align_out_img);
            
            if (ret != ROCKX_RET_SUCCESS) 
            {
              printf("rockx_face_align failed\n");
            }

            // 人脸比对
            rockx_face_feature_t out_feature;
            memset(&out_feature, 0, sizeof(rockx_face_feature_t));
            bool is_recognize = false;

            //提取对齐后图像的人脸特征
            rockx_face_recognize(face_recognize_handle, &align_out_img, &out_feature);
        
            rockx_image_release(&align_out_img);

            //特征比对,相似度小于或等于1.0，认为识别成功
            float similarity;// 原始欧氏距离
            float similarity_percent = (1.0f - (similarity / 2.0f)) * 100.0f; // 转换为百分比
            
            for (database_iter = database_face_map.begin();database_iter != database_face_map.end(); database_iter++) 
            {
              ret = rockx_face_feature_similarity(&database_iter->second,&out_feature, &similarity);
              printf("simple_value = %lf\n", similarity);
            
              if (similarity <= 1.2) 
              {
                is_recognize = true;
                break;
              } else 
              {
                is_recognize = false;
                continue;
              }
            }

            string person = "unkown";
            if (is_recognize == true) 
            {
              person = database_iter->first;
            } 
            else 
            {
              person = "unkown";
            }


            cv::Ptr<cv::freetype::FreeType2> ft2;
            ft2 = cv::freetype::createFreeType2();
            ft2->loadFontData( "/usr/share/fonts/source-han-sans-cn/SourceHanSansCN-Normal.otf", 0 );
            std::string similar = std::to_string(similarity_percent);
            // 采用opencv在人脸框的旁边绘制文字,颜色格式是B、G、R
            int thickness = 5;
            if(similarity <= 1.2)
            {
              ft2->putText(show_img, "识别成功: "+person, cv::Point(left, top-90), 60, Scalar(0, 255, 0), -1, 8, true );
              ft2->putText(show_img, "ED : "+similar, cv::Point(left, top-16), 60, Scalar(0, 255, 0), -1, 8, true );
            }
            else
            {
              ft2->putText(show_img, "无法识别: "+person, cv::Point(left, top-90), 60, Scalar(0, 0, 255), -1, 8, true );
              ft2->putText(show_img, "ED : "+similar, cv::Point(left, top-16), 60, Scalar(0, 0, 255), -1, 8, true );
            }
            printf("This Predict Name = %s\n", person.c_str());
            show_img.release();

            
            printf("[Time] %s [Location] %s [Type] %s [Name] %s [Similarity] %.2f [Direction] %s\n",
              time_str, location, detect_result_group.results[i].name,person, similarity_percent,direction);

            
              char personname[32];
              std::strncpy(personname, person.c_str(), sizeof(personname) - 1);
              personname[sizeof(personname) - 1] = '\0'; // 确保字符串以 null 字符结尾
          
            strncpy(log_entry.name, personname, sizeof(log_entry.name) - 1);
            log_entry.confidence = detect_result_group.results[i].prop;


            pthread_mutex_lock(&log_mutex);
            detection_logs.push_back(log_entry);
            pthread_mutex_unlock(&log_mutex);  

          }


          






















          //修正检测框的坐标和尺寸
          if (left < 0)
          {
            left = 0;
          }
          if (top < 0)
          {
            top = 0;
          }

          while ((uint32_t)(left + w) >= video_width)
          {
            w -= 16;
          }
          while ((uint32_t)(top + h) >= video_height)
          {
            h -= 16;
          }

          printf("border=(%d %d %d %d)\n", left, top, w, h);



          // 采用opencv来绘制人物矩形框,颜色格式是B、G、R
          using namespace cv;
          Mat orig_img = Mat(video_height, video_width, CV_8UC3, RK_MPI_MB_GetPtr(src_mb));//黑白灰图案
          cv::rectangle(orig_img,cv::Point(left, top),cv::Point(right, bottom),cv::Scalar(0,255,255),5,8,0);
          putText(orig_img, detect_result_group.results[i].name, Point(left, bottom+16), FONT_HERSHEY_TRIPLEX, 3, Scalar(0,0,255),4,8,0);
        }

      }

    
    }

     
    //释放RKNN输出
    rknn_outputs_release(ctx, io_num.n_output, outputs);

    

    // 图像裁剪和格式转换
    rga_buffer_t src , dst ;
    MB_IMAGE_INFO_S dst_ImageInfo = {(RK_U32)video_width, (RK_U32)video_height, (RK_U32)video_width, 
                                     (RK_U32)video_height, IMAGE_TYPE_RGB888};
    MEDIA_BUFFER dst_mb = RK_MPI_MB_CreateImageBuffer(&dst_ImageInfo, RK_TRUE, 0);
    dst = wrapbuffer_fd(RK_MPI_MB_GetFD(dst_mb), video_width, video_height,RK_FORMAT_RGB_888);
    src = wrapbuffer_fd(RK_MPI_MB_GetFD(src_mb), video_width, video_height,RK_FORMAT_RGB_888);
    
    //设置裁剪区域
    im_rect src_rect , dst_rect;
    src_rect = {0, 0, 2592, 1944};
    dst_rect = {0};
    ret = imcheck(src, dst, src_rect, dst_rect, IM_CROP);
    if (IM_STATUS_NOERROR != ret) 
    {
      printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
      break;
    }

    //执行裁剪操作
    IM_STATUS CROP_STATUS = imcrop(src, dst, src_rect);
    if (CROP_STATUS != IM_STATUS_SUCCESS)
    {
      printf("ERROR: imcrop failed: %s\n", imStrError(CROP_STATUS));
    }

    //发送处理后的图像到显示模块
    RK_MPI_SYS_SendMediaBuffer(RK_ID_VO, 0, dst_mb); // 发送到显示模块
    RK_MPI_MB_ReleaseBuffer(dst_mb);                // 释放目标缓冲区
    RK_MPI_MB_ReleaseBuffer(src_mb);                // 释放源缓冲区
    src_mb = NULL;
    dst_mb = NULL;
  }

  //关闭数据库连接
  database_face_map.erase(database_iter);
  database_face_map.erase(database_face_map.begin(), database_face_map.end());
  //free();
  rockx_destroy(face_det_handle);
  rockx_destroy(face_5landmarks_handle);
  rockx_destroy(face_recognize_handle);


  //清理资源
   if (model) 
   {
     delete model;
     model = NULL;
   }
  rknn_destroy (ctx); 
  return NULL;
}

