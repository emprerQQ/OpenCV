#include "V4L2Driver.h"
#define CAMERA_V4L2_DEBUG

CameraV4L2::CameraV4L2(const char* dev){
	fd = open(dev,O_RDWR | O_NONBLOCK,0);
	if (fd == -1){
        perror("Opening video device(modprode mxc_v4l2_capture)");
    }

	if(initCamera()){
		perror("initCamera");
	}
    if(init_mmap()){
		perror("init_mmap");
	}
	
}
CameraV4L2::~CameraV4L2(){
	close(fd);
}

int CameraV4L2::initCamera(){
    struct v4l2_capability caps;
    ZERO(caps);
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps))
    {        
		perror("Querying Capabilities");
        return 1;
    }
        struct v4l2_cropcap cropcap;
		memset(&cropcap,0,sizeof(cropcap));
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (-1 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
        {
                perror("Querying Cropping Capabilities");
                return 1;
        }
        struct v4l2_format fmt;
        ZERO(fmt);
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = IMAGE_WIDTH;
        fmt.fmt.pix.height = IMAGE_HEIGHT;
        fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
        fmt.fmt.pix.field = V4L2_FIELD_NONE;

        if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
        {
            perror("Setting Pixel Format");
            return 1;
        }
        return 0;
}
int CameraV4L2::xioctl(int fd, int request, void *arg){
	int r;
	do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);
    return r;
}
int CameraV4L2::init_mmap(){
    struct v4l2_requestbuffers req;
    ZERO(req);
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)){
        perror("Requesting Buffer");
        return 1;
    }


	for(int n_buffers = 0;n_buffers != BUFFER_COUNT;n_buffers++){
		struct v4l2_buffer buf;
    	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    	buf.memory = V4L2_MEMORY_MMAP;
    	buf.index = n_buffers;
    	if(-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf)){
    	    perror("Querying Buffer");
    	    return 1;
    	}
		frameBuffer[n_buffers].length = buf.length;
		frameBuffer[n_buffers].start = (uchar*)mmap(
		NULL,buf.length,PROT_READ | PROT_WRITE,MAP_SHARED,fd,buf.m.offset);
		if(MAP_FAILED == frameBuffer[n_buffers].start){
			perror("mmap failed\n");
			return 1;
		}
#ifdef CAMERA_V4L2_DEBUG
		printf("Length: %d\nAddress: %p\n", buf.length, frameBuffer[n_buffers].start);
		printf("Image Length: %d\n", buf.bytesused);
#endif
	}
    return 0;
}
uchar* CameraV4L2::capture_image(void){
	for(;;){
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv;
    ZERO(tv);
    tv.tv_sec = 2;
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if(-1 == r){
        perror("Waiting for Frame");
        return NULL;
    }
	uchar* data = frameRead();
	if(data){
		return data;
	}
	}
}
uchar* CameraV4L2::convertToRGB(uchar* yuyv_image){
	unsigned char* rgb_image = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT * 3]; //width and height of the image to be converted

int y;
int cr;
int cb;

double r;
double g;
double b;

for (int i = 0, j = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT * 3; i+=6,j+=4) {
    //first pixel
    y = yuyv_image[j];
    cb = yuyv_image[j+1];
    cr = yuyv_image[j+3];

    r = y + (1.4065 * (cr - 128));
    g = y - (0.3455 * (cb - 128)) - (0.7169 * (cr - 128));
    b = y + (1.7790 * (cb - 128));

    //This prevents colour distortions in your rgb image
    if (r < 0) r = 0;
    else if (r > 255) r = 255;
    if (g < 0) g = 0;
    else if (g > 255) g = 255;
    if (b < 0) b = 0;
    else if (b > 255) b = 255;

    rgb_image[i] = (unsigned char)r;
    rgb_image[i+1] = (unsigned char)g;
    rgb_image[i+2] = (unsigned char)b;

    //second pixel
    y = yuyv_image[j+2];
    cb = yuyv_image[j+1];
    cr = yuyv_image[j+3];

    r = y + (1.4065 * (cr - 128));
    g = y - (0.3455 * (cb - 128)) - (0.7169 * (cr - 128));
    b = y + (1.7790 * (cb - 128));

    if (r < 0) r = 0;
    else if (r > 255) r = 255;
    if (g < 0) g = 0;
    else if (g > 255) g = 255;
    if (b < 0) b = 0;
    else if (b > 255) b = 255;

    rgb_image[i+3] = (unsigned char)r;
    rgb_image[i+4] = (unsigned char)g;
    rgb_image[i+5] = (unsigned char)b;
}
    return rgb_image;
}
bool CameraV4L2::startCapture(){		
	struct v4l2_buffer buf;	
	for(int i = 0;i < BUFFER_COUNT;i++){	
		memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		if(-1 == xioctl(fd, VIDIOC_QBUF, &buf)){
			perror("Query Buffer");
			return false;
		}
	}
    if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type)){
        perror("Start Capture");
        return false;
    }
	return true;
}
uchar* CameraV4L2::frameRead(){
	struct v4l2_buffer buf;	
	memset(&buf,0,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
    {
        return NULL;
    }
	uchar* rgbData = convertToRGB(frameBuffer[buf.index].start);
	if(-1 == xioctl(fd, VIDIOC_QBUF, &buf)){
		perror("enqueue buffer");
		return NULL;
	}
	return rgbData;
}
