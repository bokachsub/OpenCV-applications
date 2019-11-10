// Program that reads and displays and image. 
/*
When the user's mouse clicks on the image, read in the corresponding pixel values (blue, green, red) and write those values as text to the screen
at the mouse location.

  Display the mouse coordinates of the individual image when clicking anywhere within the three-image display

   ________________________________________________
  |                |              |               |
  |                |              |               |
  |    RECT1       |      RECT2   |      RECT3    |
  |                |              |               |
  |                |              |               |
  |                |              |               |
  |                |              |               |
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
//using namespace cv;

cv::Mat imageCanvas, image1, image2, image3, roi1, roi2, roi3;
cv::Rect rect1, rect2, rect3;
cv::Point pt(-1, -1);
bool newCoords = false;

static void onMouse(int event, int x, int y, int, void* leftCoordinate)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		pt.x = x;
		pt.y = y;

		//cv::Vec3b px;
		int x_adjusted;

		cout << "\n Left Mouse event caught at location : (" << x << "  " << y << ") \n";
		newCoords = true;

		//Get value of pixel (BGR): 		
		cv::Vec3b px = imageCanvas.at<cv::Vec3b>(cv::Point(x, y)); //big canvas px

		//since images are copied (aka hard-copied) the only way to know is to calculate the substracting on X-axis (images1 & image2)
		if (rect1.contains(cv::Point(x, y)))
		{
			cout << "\n px is inside rect1";
			x_adjusted = pt.x;

		}
		if (rect2.contains(cv::Point(x, y)))
		{
			cout << "\n px is inside rect2";
			x_adjusted = pt.x - image1.cols;
		}
		if (rect3.contains(cv::Point(x, y)))
		{
			cout << "\n px is inside rect3";
			x_adjusted = pt.x - image1.cols - image1.cols;
		}

		int blue = (int)px[0];
		int green = (int)px[1];
		int red = (int)px[2];
		string output_string = to_string(blue) + " " + to_string(green) + " " + to_string(red) + "    x:" + to_string(x_adjusted) + " y:" + to_string(pt.y);

		//grey colors look bad when text output, so using either black or white
		int textColor = (255 - (blue + green + red) / 3 < 128) ? 0 : 255;
		cout << "    Blue value is " << blue << " Green value is " << green << " Red value is " << red << "\n";
		cv::putText(imageCanvas, output_string, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(textColor, textColor, textColor), 2);
		imshow("image", imageCanvas);
	}
}


int main(int argc, char** argv)
{
	//Display 3 images in 1 frame: 
	//tut 
	image1 = cv::imread("image1.png");
	if (!image1.data) { cout << "Could not open or find the image" << std::endl; return -1; }

	image2 = cv::imread("image2.png");
	if (!image2.data) { cout << "Could not open or find the image" << std::endl; return -1; }

	image3 = cv::imread("image3.png");
	if (!image3.data) { cout << "Could not open or find the image" << std::endl; return -1; }

	int united_cols = image1.cols + image2.cols + image3.cols; //cols = x  
	int united_rows = image1.rows; //rows = y, top corner always stays the same

	imageCanvas = cv::Mat(united_rows, united_cols, CV_8UC3);

	rect1 = cv::Rect(0, 0, image1.cols, image1.rows);
	rect2 = cv::Rect(image1.cols, 0, image2.cols, image2.rows);
	rect3 = cv::Rect(image1.cols + image2.cols, 0, image3.cols, image3.rows);

	roi1 = cv::Mat(imageCanvas, rect1);
	roi2 = cv::Mat(imageCanvas, rect2);
	roi3 = cv::Mat(imageCanvas, rect3);

	image1.copyTo(roi1);
	image2.copyTo(roi2);
	image3.copyTo(roi3);

	cv::namedWindow("image");
	cv::setMouseCallback("image", onMouse);
	cv::imshow("image", imageCanvas);


	for (;;)
	{
		if (newCoords) //boolena variable - means a click happenned 
		{
			cout << "\n Processing new coordinates event";
			//refresh big image with fresh pieces
			image1.copyTo(roi1);
			image2.copyTo(roi2);
			image3.copyTo(roi3);

		} //new coordinates

		newCoords = false;
		//imshow("image", imageCanvas);

		if ((cv::waitKey(100)) == 27) break; // allow the user to break out
	}

	return 0;

}