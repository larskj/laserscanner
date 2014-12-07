#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include "extract_laser.h"

using namespace cv;

enum image_source { IMAGE_SOURCE_STREAM, IMAGE_SOURCE_FILE };

static void show_invalid_usage_text( void )
{
	printf( "\nMissing parameters or image file not found.\n\n" );
	printf( "Usage examples:\n" );
	printf( "Start IP stream:              -> ./build_and_run.sh stream\n" );
	printf( "Process image data from file: -> ./build_and_run.sh pics/teddybears.jpg\n" );
}

int main( int argc, char** argv )
{
	const char* input_name  = argv[1];
	const char* output_name = "output/output_image.jpg";
	enum image_source source;

	VideoCapture ip_cam_capture;

	Mat3b input_image, output_image;

	if ( argc == 2 )
	{
		if ( strcmp( argv[1], "stream" ) == 0 )
			source = IMAGE_SOURCE_STREAM;
		else
			source = IMAGE_SOURCE_FILE;
	}
	else
	{
		show_invalid_usage_text();
		return -1;
	}

	switch( source )
	{
		case IMAGE_SOURCE_STREAM:
			input_name  = "http://192.168.87.105:8090/?action=stream?dummy=param.mjpg";
			ip_cam_capture.open( input_name );
			ip_cam_capture >> input_image;
			break;
		case IMAGE_SOURCE_FILE:
			input_name  = argv[1];
			input_image = imread( input_name, CV_LOAD_IMAGE_COLOR );
			break;
	}

	if( !input_image.data )
	{
		show_invalid_usage_text();
		return -1;
	}

	do
	{
		if ( source == IMAGE_SOURCE_STREAM ) // If ip stream, we update the image with the latest available
			ip_cam_capture >> input_image;

		if( input_image.empty() )
			break;

// 		imshow( "video", input_image );

		if( waitKey(30) >= 0 )
			break;

		extract_laser_from_image( input_image, output_image );

		namedWindow(  input_name, CV_WINDOW_NORMAL );
		namedWindow( output_name, CV_WINDOW_NORMAL );

		imshow(  input_name,  input_image  );
		imshow( output_name, output_image );
	}
	while ( ( source == IMAGE_SOURCE_STREAM ) && ip_cam_capture.isOpened() );

	waitKey( 0 );

	// Uncomment to record input image from stream
// 	imwrite( "pics/pic9.jpg", input_image );

	imwrite( output_name, output_image );

	printf( "All tasks done!\n" );
	printf( "The output image was saved in %s\n", output_name );
	printf( "Highlight an image and press any key to quit.\n" );
	fflush( stdout );

	return 0;
}
