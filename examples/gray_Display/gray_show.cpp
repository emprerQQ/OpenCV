/*****************************************************************
	Description:
	-> Load an image using "imread"
	-> Transform an image from BGR to Grayscale format by using "cvtColor"
	-> Save the transformed image in a file on disk by using "imwrite"
*****************************************************************/

#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
	char* imageName = argv[1];

	Mat image;
	image = imread( imageName, 1 );  //load an image,a BGR image 

	if( argc != 2 || !image.data )
	{
		printf(" No image data \n ");
		return -1;
	}
	
	Mat gray_image;
	cvtColor( image, gray_image, CV_BGR2GRAY );
	// cvtColor( image, gray_image, CV_BGR2GRAY );
	// -> image: a source image
	// -> gray_image: a destination image, in which we will save teh converted image
	// -> a parameter that indicates what kind of transformation will be performed. Here, CV_BGR2GRAY(because of imread has BGR default channel order )

	imwrite( "./Gray_IMage.jpg", gray_image );
	// imwrite( "destination_direction/Name_gray_image", gray_image)
	// -> 1st parameter: my destination_direction/Name_gray_image, as which I want to save the gray_image 
	// -> 2st parameter: the converted image, gray_image

	namedWindow( imageName, CV_WINDOW_AUTOSIZE );// Create a window for display
	namedWindow( "Gray image", CV_WINDOW_AUTOSIZE );

	imshow( imageName, image );// Show our image inside it
	imshow( "Gray image", gray_image );

	waitKey(0); // Wait for a keystroke in the window
	//In order to display the window until the user presses a key, we use the waitkey function whose the only parameter is how long should it wait for a user input.
	// 0 : forever
	// About waitKey() ?????

	return 0;
}
