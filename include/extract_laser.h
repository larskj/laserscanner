#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <stdint.h>

using namespace cv;

void extract_laser_from_image( const Mat& input, Mat& output );
