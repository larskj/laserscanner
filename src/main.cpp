#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "extract_laser.h"

using namespace cv;

int main( int argc, char** argv )
{
	const char* input_name  = argv[1];
	const char* output_name = "output/output_image.jpg";

	Mat3b input_image, output_image;
	input_image = imread( input_name, CV_LOAD_IMAGE_COLOR );

	if( argc != 2 || !input_image.data )
	{
		printf( "Missing parameters or image file not found.\n" );
		printf( "Usage example: ./build_and_run.sh pics/teddybears.jpg\n" );
		return -1;
	}

	extract_laser_from_image( input_image, output_image );

	imwrite( output_name, output_image );

	namedWindow(  input_name, CV_WINDOW_NORMAL );
	namedWindow( output_name, CV_WINDOW_NORMAL );

	printf( "All tasks done!\n" );
	printf( "The output image was saved in %s\n", output_name );
	printf( "Highlight an image and press any key to quit.\n" );
	fflush( stdout );

	imshow(  input_name,  input_image  );
	imshow( output_name, output_image );

	waitKey(0);

	return 0;
}
