#ifndef VIDEO_STREAM_INC
#define VIDEO_STREAM_INC

extern int motion;
extern cv::Mat current, past;
extern std::mutex cmtx, pmtx, t, n, d;
extern time_t now;

void videoStream();

#endif