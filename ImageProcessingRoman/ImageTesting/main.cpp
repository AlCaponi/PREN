#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat img_scene_gray;
Mat img_scene;
Mat img_object;
uchar blue;
uchar green;
uchar red;
int blocker;

int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int threshold_value = 50;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

Mat src, src_gray, dst;
char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

/// Function headers
void Threshold_Demo(int, void*);

void readPictureScene(String picName, bool color)
{
	if (color == true)
	{
		img_scene = imread(picName);
	}
	if (color == false)
	{
		img_scene = imread(picName, 0);
	}
}

void getColor(Mat image, int x, int y)
{
	Vec3f intensity = image.at<Vec3f>(y, x);
	float blue = intensity.val[0];
	float green = intensity.val[1];
	float red = intensity.val[2];
}

void printValues()
{
	cout << "Blau " << blue << endl;
	cout << "Green " << green << endl;
	cout << "Rot " << red << endl;
}

void thresholdPicture()
{
	//Convert the image to Gray
	cvtColor(src, src_gray, CV_RGB2GRAY);

	//Create a window to display results
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	//Create a toolbar to choose type of Threshold
	createTrackbar(trackbar_type,
		window_name, &threshold_type,
		max_type, Threshold_Demo);

	createTrackbar(trackbar_value,
		window_name, &threshold_value,
		max_value, Threshold_Demo);

	/// Call the function to initialize
	Threshold_Demo(0, 0);

	/// Wait until user finishes program
	//while (true)
	//{
	//	int c;
	//	c = waitKey(20);
	//	if ((char)c == 27)
	//	{
	//		break;
	//	}
	//}
}

void Threshold_Demo(int, void*)
{
	//src = threshold(src_gray, src, 53, max_BINARY_value, 0);
}

void Threshold_DemoNew(int, void*)
{
	//Convert the image to Gray
	cvtColor(img_scene, src_gray, CV_RGB2GRAY);
	threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);
	imshow("Teesting", dst);
}

void testContourArea()
{
	vector<Point> contour;

	contour.push_back(Point2f(0, 0));
	contour.push_back(Point2f(10, 0));
	contour.push_back(Point2f(10, 10));
	contour.push_back(Point2f(5, 4));

	double area0 = contourArea(contour);
	vector<Point> approx;
	approxPolyDP(contour, approx, 5, true);
	double area1 = contourArea(approx);

	cout << "area0 =" << area0 << endl <<
		"area1 =" << area1 << endl <<
		"approx poly vertices" << approx.size() << endl;
}

void cropImage(Mat pic)
{

}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;



	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Get the moments
	vector<Moments> mu(contours.size());

	// Get X and Y
	int cx[100];
	int cy[100];

	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		cx[i] = int((mu[i].m10) / (mu[i].m00));
		cy[i] = int((mu[i].m01) / (mu[i].m00));
	}

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for (int i = 0; i< contours.size(); i++)
	{
		//printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
		printf(" * Contour[%d] - Area (M_00) = %.2f - X-Pos: %.2f - Y-Pos: %.2f\n", i, mu[i].m00, cx[i], cy[i]);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}
}

void thresh_callback2(int, void*)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	/// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
	}


	/// Draw polygonal contour + bonding rects + circles
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		circle(drawing, center[i], (int)radius[i], color, 2, 8, 0);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	printf("\t Info: Area and Contour Length \n");
	for (int i = 0; i< contours.size(); i++)
	{
		if ((arcLength(contours[i], true) > 600) && (i == 0))
		{
			i = 1;
		}
		float x = center.at(i).x;
		float y = center.at(i).y;

		//Point pt;

		//pt.x = x;
		//pt.y = y;

		//circle(drawing, pt, 2, red);

		printf("Kontur %d - X = %f - Y = %f - Length: %.2f\n", i, x, y, arcLength(contours[i], true));
	}
}


//int main(int argc, char** argv)
int main()
{
	/// Load source image and convert it to gray
	src = imread("Korb.png", 1);

	//threshold(src_gray, src, 53, max_BINARY_value, 0);

	Mat roi(src, Rect(20, 50, 220, 130));

	//Mat roi(src, Rect(55, 50, 230, 130));

	/// Convert image to gray and blur it
	cvtColor(roi, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	threshold(src_gray, src_gray, 53, max_BINARY_value, 0);

	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, src);

	//createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback2);
	thresh_callback2(0, 0);

	waitKey(0);
	return(0);



}