#ifndef VIDEO_STREAM_INC
#define VIDEO_STREAM_INC

extern cv::Mat current;
extern std::mutex cmtx, t, n;

void videoStream();

#endif