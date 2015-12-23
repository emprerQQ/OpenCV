/*****************************************************************
	Description:
	-> 载入图片，imread
	-> 翻转处理，flip
	-> 保存处理后的图片，imwrite
*****************************************************************/

#include <cv.h>
//#include <highgui.h>

//#include<core/core.hpp>
//#include<imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#define NAME_cpp " Flip_Image "
using namespace cv;  // 避免每次都要输入cv::，因此在程序开始加入本语句


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
	
	Mat Flip_Image;
	flip( image, Flip_Image, 0);    // 1:水平翻转
					// 0:垂直翻转
					// 负数表示既有水平又有垂直翻转


	imwrite( "./Flip_Image.jpg", Flip_Image );
	// imwrite( "destination_direction/Name_gray_image", gray_image)
	// -> 1st parameter: my destination_direction/Name_gray_image, as which I want to save the gray_image 
	// -> 2st parameter: the converted image, gray_image

	namedWindow( imageName, CV_WINDOW_AUTOSIZE );// Create a window for display
	namedWindow( "Flip_Image", CV_WINDOW_AUTOSIZE );

	imshow( imageName, image );// Show our image inside it
	imshow( "Flip_Image", Flip_Image );

	waitKey(0); // Wait for a keystroke in the window
	//In order to display the window until the user presses a key, we use the waitkey function whose the only parameter is how long should it wait for a user input.
	// 0 : forever
	// About waitKey() ?????

	return 0;
}
