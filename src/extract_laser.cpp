#include "extract_laser.h"

static const uchar LASER_THRESHOLD                = 230; // Everything at or above this level will always be marked as the laser beam (direct red component level comparison)
static const uchar LASER_MIN_ACCEPTABLE_THRESHOLD = 180; // Everything below this level will never be marked as being the laser beam
static const uchar LASER_THRESHOLD_DECREASE_STEP  = 20;  // If in a vertical scan of the image, no laser beam is found, then the threshold will be decreased by this amount and be run again
                                                         // This dynamic decrease in threshold will continue in the selected step size until laser beam is found in the vertical scan or LASER_MIN_ACCEPTABLE_THRESHOLD reached 

// TODO add a dynamic gui slider to update the above defines to enable on-the-fly manipulation of the output image

static const uchar RED_CH    = 2;
static const uchar GREEN_CH  = 1;
static const uchar BLUE_CH   = 0;

static inline void set_rgb_values( uchar* dst, uchar r, uchar g, uchar b )
{
	dst[ RED_CH   ] = r;
	dst[ GREEN_CH ] = g;
	dst[ BLUE_CH  ] = b;
}

static inline void get_rgb_values( const uchar* const src, uchar* r, uchar* g, uchar* b )
{
	*r = src[ RED_CH   ];
	*g = src[ GREEN_CH ];
	*b = src[ BLUE_CH  ];
}

// Input parameter is original picture, output parameter can just be an empty, uninitialized Mat object
void extract_laser_from_image( const Mat& input, Mat& output )
{
	CV_Assert( input.depth() != sizeof(uchar) ); // We expect image to have depth of unsigned char
	CV_Assert( input.channels() == 3 );          // We expect three-channel images only (RGB)

	int height = input.rows;
	int width  = input.cols;

	printf( "Dimensions of input picture: Height: %d  Width: %d\n", height, width );

	// TODO, We don't really need to clone the input data like this, but it is an easy way for now to ensure output image gets its own image buffer space without overriding input image
	output = input.clone();
	output.setTo( Scalar( 0, 0, 0 ) ); // Initialize output image to solid color, 0,0,0 is black

	uchar red, green, blue;

	for( int w = 0; w < width; w++ )
	{
		int laser_threshold_static  = LASER_THRESHOLD;
		int laser_threshold_dynamic = LASER_THRESHOLD;
		int laser_found_in_vertical_scan = 0;

		do
		{
			for ( int h = 0; h < height; h++ )
			{
				const uchar*  in_ptr =  &input.at<uchar>( h, w, 0 );
				      uchar* out_ptr = &output.at<uchar>( h, w, 0 );

				get_rgb_values( in_ptr, &red, &green, &blue );

				if ( red >= laser_threshold_static && ( red > green ) && ( red > blue) ) // We only accept it as the laser if the color is more red than green or blue
				{
					set_rgb_values( out_ptr, red, green, blue ); // Keep original pixels for laser found using primary method
					laser_found_in_vertical_scan = 1;
				}
				else
				{
					if ( red >= laser_threshold_dynamic && ( (2*red > 3*green) && (2*red > 3*blue) ) ) // For secondary search method we only accept the laser if it is significantly more red than green or blue
					{
						set_rgb_values( out_ptr, blue, green, red ); // swap red/blue to indicate laser found using secondary method
						laser_found_in_vertical_scan = 1;
					}
				}
			}

			if ( laser_threshold_dynamic > LASER_THRESHOLD_DECREASE_STEP )
				laser_threshold_dynamic -= LASER_THRESHOLD_DECREASE_STEP;
		}
		while( !laser_found_in_vertical_scan && ( laser_threshold_dynamic >= LASER_MIN_ACCEPTABLE_THRESHOLD ) );
	}

	return;
}

// TODO, rewrite these pointer accesses of the in/out image. For some reason .at() on the Mat object doesn't work correctly.
// TODO, for some reason it only works for blue channel, figure out why
// 			output.at<uchar>( i, j, BLUE_CH  ) = 255;
// 			output.at<uchar>( i, j, GREEN_CH ) = 255;
// 			output.at<uchar>( i, j, RED_CH   ) = 255;