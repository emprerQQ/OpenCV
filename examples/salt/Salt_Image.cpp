/*****************************************************************
	Description:
	-> 椒盐噪点，它随机地将部分像素设置为白色或者黑色。传输过程中，
	   若部分像素值丢失，那么会出现这种噪点。
	-> 本例中，随机挑选若干像素，并将其设置为白色，对应0~255中的255，黑色对应0

*****************************************************************/

#include <cv.h>
#include <highgui.h>

//#include<opencv2/core/core.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include<opencv2/highgui/highgui.hpp>

#define NAME_cpp " Salt_Image "
using namespace cv;  // 避免每次都要输入cv::，因此在程序开始加入本语句

// {{{ void salt(Mat &image, int n)
//	1. 公有成员变量 cols(列) 和 rows(行)给出图像的宽和高
//	2. 成员函数at(int y, int x)可以用来存取图像元素，但是需要在编译期知道图像的数据类型，因为cv::Mat可以存放任意数据类型的元素。
//	   所以调用时，应指定数据类型：  
//		image.at<uchar>(j,i) = value;
//	   务必确保指定的数据类型要和矩阵中的数据类型相符，at本身不会进行数据类型的转换。
//	3. 对于彩色图像，每个图像由3部分组成：红色通道、绿色通道、蓝色通道。因此，一个包含彩色图像的cv::Mat会返回一个由  三个8位数组成的向量。
//	   opencv将此类向量定义为cv::Vec3b，即由三个unsigned char组成的向量：
//		image.at<cv::Vec3b>(j,i)[channel] = value;
//		channel : 颜色通道号  0  1  2
void salt(Mat &image, int n)
{
	int k;
	int i;
	int j;

	for(k=0; k<n; k++)
	{
		//rand() 是随机数生成函数
		i = rand()%image.cols;  //公有成员变量 cols(列) 和 rows(行)给出图像的宽和高
		j = rand()%image.rows;
		if(image.channels() == 1)
		{
			image.at<uchar>(j,i) = 255;
		}
		else if(image.channels() == 3)
		{
			image.at<Vec3b>(j,i)[0] = 255;
			image.at<Vec3b>(j,i)[1] = 255;
			image.at<Vec3b>(j,i)[2] = 255;
		}
	}
}
// }}}

int main( int argc, char** argv )
{
	char* imageName = argv[1];

	Mat image;
	image = imread( imageName );  //载入图片

	if( argc != 2 )//参数输入错误提示
	{
		printf( NAME_cpp "usage: ./executable_file_name Image_fullname \n ");
		return -1;
	}
	else if( !image.data )//载入图片失败
	{
		printf(" No image data. \n ");
		return -1;
	}
	
	//Mat Salt_Image;
	salt( image, 2000 );         // 1st: input image
					// 2st: number
				


//	imwrite( "./"NAME_cpp".jpg", Salt_Image );
	// imwrite( "destination_direction/Name_gray_image", gray_image)
	// -> 1st parameter: my destination_direction/Name_gray_image, as which I want to save the gray_image 
	// -> 2st parameter: the converted image, gray_image

	namedWindow( imageName, CV_WINDOW_AUTOSIZE );// Create a window for display
//	namedWindow( Salt_Image, CV_WINDOW_AUTOSIZE );

	imshow( imageName, image );// Show our image inside it
//	imshow( "Salt_Image", Salt_Image );

	waitKey(0); // Wait for a keystroke in the window
	//In order to display the window until the user presses a key, we use the waitkey function whose the only parameter is how long should it wait for a user input.
	// 0 : forever
	// About waitKey() ?????

	return 0;
}
