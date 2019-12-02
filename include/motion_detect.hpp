#ifndef MOTION_DETECT_INC
#define MOTION_DETECT_INC

extern cv::Mat current, past;
extern float sensitivity;
extern int margin, motion;
extern std::mutex cmtx, mmtx, pmtx, d;

void detect();

#endif