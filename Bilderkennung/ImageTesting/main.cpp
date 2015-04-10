#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

// Threshold Algorithm
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int threshold_value = 50;
int threshold_type = 0;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

// Pictures
Mat src, src_gray, dst, threshold_output;

// brightenPicture
double alpha;
int beta;

// roi size
int bild_start_x = 40;
int bild_start_y = 90;
int bild_length_x = 270;
int bild_height_y = 100;


void thresh_callback(int, void*)
{
	//Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	//threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
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
		if ((arcLength(contours[i], true) > 220) && (arcLength(contours[i], true) < 300))
		{
			float x = center.at(i).x;
			float y = center.at(i).y;

			printf("Abfalleimer - X = %f - Y = %f - Length: %.2f\n", x, y, arcLength(contours[i], true));

			if (x < bild_length_x / 2)
			{
				printf("Fahre links");

				if (x >(bild_length_x / 2 - 10))
				{
					printf(" (aber nur ein wenig)");
				}
			}
			else
			{
				printf("Fahre rechts");

				if (x < (bild_length_x / 2 + 10))
				{
					printf(" (aber nur ein wenig)");
				}
			}
		}
	}
}

Mat brightenPicture(Mat myImage)
{
	Mat image = myImage;
	Mat new_image = Mat::zeros(image.size(), image.type());

	alpha = 1;
	beta = 20;

	/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				new_image.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	return new_image;
}

Mat defineROI(Mat myImage)
{
	vector<Point> ROI_Vertices;

	Point a;
	a.x = 40;
	a.y = 80;

	Point b;
	b.x = 250;
	b.y = 80;

	Point c;
	c.x = 250;
	c.y = 200;

	Point d = (200, 200);
	d.x = 40;
	d.y = 200;

	ROI_Vertices.push_back(a);
	ROI_Vertices.push_back(b);
	ROI_Vertices.push_back(c);
	ROI_Vertices.push_back(d);

	/* ROI by creating mask for the parallelogram */
	Mat mask = cvCreateMat(240, 320, CV_8UC1);
	// Create black image with the same size as the original
	for (int i = 0; i<mask.cols; i++)
		for (int j = 0; j<mask.rows; j++)
			mask.at<uchar>(Point(i, j)) = 0;

	// Create Polygon from vertices
	vector<Point> ROI_Poly;
	approxPolyDP(ROI_Vertices, ROI_Poly, 1.0, true);

	// Fill polygon white
	fillConvexPoly(mask, &ROI_Poly[0], ROI_Poly.size(), 255, 8, 0);

	// Create new white image for result storage
	Mat imageDest = cvCreateMat(240, 320, CV_8UC3);

	//imwrite("Mat.png", mask);

	// Cut out ROI and store it in imageDest
	//image->copyTo(imageDest, mask);
	myImage.copyTo(imageDest, mask);

	imwrite("ImageDest.png", imageDest);

	return imageDest;
}

void imageProcessing()
{
	/// Load source image and convert it to gray
	src = imread("Korb_Mitte.png", 1);

	// Definiert Range of Interest (Begrenzt Bild auf das Spielfeld)
	// Mat roi(src, Rect(bild_start_x, bild_start_y, bild_length_x, bild_height_y));

	// Macht das Bild ein wenig heller (Schatten rausfiltern wenn möglich)
	//roi = brightenPicture(roi);

	src = defineROI(src);

	// Möglicherweise unnötig, da neu ROI
	src = brightenPicture(src);

	/// Macht das Bild komplett Schwarz/Weiss
	cvtColor(src, src_gray, CV_BGR2GRAY);

	blur(src_gray, src_gray, Size(3, 3));
	threshold(src_gray, threshold_output, 53, max_BINARY_value, 0);

	imwrite("BW-Pic.png", threshold_output);

	/// Create Window to show result
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, src);

	// Threshold Algorithmus
	thresh_callback(0, 0);
}

int main()
{
	imageProcessing();

	waitKey(0);
	return(0);
}

