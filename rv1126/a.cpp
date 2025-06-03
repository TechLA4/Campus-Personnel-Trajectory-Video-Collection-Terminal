    std::deque<bool> movementHistory; // 移动历史窗口
    cv::Mat prevFrameGray;
    std::vector<cv::Point2f> prevCenters;

    while (true)
    {

        // 1. YOLO 检测是否存在 person
        yolov5_detect_run(ctx, frame, &detect_result_group);

        for (int i = 0; i < detect_result_group.count; i++) {
            yolov5_detect_result_t *det_result = &(detect_result_group.results[i]);
            //保存历史person框
            currentCenters.push_back(cv::Point2f(cx, cy));
        }

        // 2. 中心点和光流法移动检测 

        //  中心点位移判断
        for (const auto &center : currentCenters) {
            double moveDist = 0.0;
            if (!prevCenters.empty()) {
                double minDist = std::numeric_limits<double>::max();
                for (const auto &prevCenter : prevCenters) {
                    double dist = cv::norm(center - prevCenter);
                    if (dist < minDist) minDist = dist;
                }
                moveDist = minDist;
                    if (moveDist > config::movementThreshold) {
                isMoving = true;
                break;}
            }

        // 光流判断
         cv::calcOpticalFlowFarneback(
            prevFrameGray, gray, flow,
            config::pyrScale, config::levels, config::winsize, 
            config::iterations, config::polyN, config::polySigma, config::flags
        );
            if (flowMag > 1.0) {
                isMoving = true;
                break;}}}}


        // 更新移动历史窗口
        movementHistory.push_back(isMoving);
        if (movementHistory.size() > config::WINDOW_SIZE) {
            movementHistory.pop_front();
        }

        // 判断滑动窗口内是否有人在动
        bool anyMoving = std::any_of(
            movementHistory.begin(), movementHistory.end(),
            [](bool m) { return m; }
        );

        if (anyMoving) {
            writer.write(frame);
            totalFramesAfter++;  // 写入帧数统计
        }