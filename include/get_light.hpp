#ifndef GET_LIGHT_INC
#define GET_LIGHT_INC

extern cv::Mat current;
extern int light, led;
extern std::mutex cmtx, lmtx, t;

void get_light();

#endif