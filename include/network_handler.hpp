#ifndef NET_HANDLER_INC
#define NET_HANDLER_INC

extern cv::Mat current;
extern std::mutex cmtx, n;

void network_handler();

#endif