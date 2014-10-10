#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

int main( int argc, char** argv )
{
	const char* input_name  = argv[1];
	const char* output_name = "output/output_image.jpg";
	
	Mat input_image;
	input_image = imread( input_name, 1 );

	if( argc != 2 || !input_image.data )
	{
		printf( "No image data\n" );
		printf( "Usage: ./build_and_run.sh pics/IMAG1708.jpg\n" );
		return -1;
	}

	Mat output_image;
	cvtColor( input_image, output_image, CV_BGR2GRAY );

	imwrite( output_name, output_image );

	namedWindow(  input_name, CV_WINDOW_NORMAL );
	namedWindow( output_name, CV_WINDOW_NORMAL );

	imshow(  input_name,  input_image  );
	imshow( output_name, output_image );

	waitKey(0);

	return 0;
}
