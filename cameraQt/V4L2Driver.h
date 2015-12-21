#ifndef CAMERAV4L2
#define CAMERAV4L2
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
typedef unsigned char uchar;
const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;

template<typename T>
inline void ZERO(T& val){
    memset(&val,0,sizeof(val));
}

struct bufferType{
	uchar* start;
	int length;
};

class CameraV4L2{
	public:
		CameraV4L2(const char* dev);
		~CameraV4L2();
		bool startCapture();
		uchar* capture_image(void);//return a frame whose color space is RGB
	private:
		
        static const int BUFFER_COUNT = 4;



		int initCamera(void);
		int init_mmap();	
		uchar* frameRead();//return a frame whose color space is YCrCb
		int xioctl(int fd, int request, void *arg);
		uchar* convertToRGB(uchar* yuyvData);
		
		int fd;
		struct bufferType frameBuffer[BUFFER_COUNT];
		
		


};
#endif // CAMERAV4L2

