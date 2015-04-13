#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "tinyxml2.h"
#include <Windows.h>

using namespace cv;
using namespace std;
using namespace tinyxml2;


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

tinyxml2::XMLDocument xmldoc;
String picName;

int aPosX = 40;
int aPosY = 60;

int bPosX = 250;
int bPosY = 60;

int cPosX = 250;
int cPosY = 180;

int dPosX = 40;
int dPosY = 180;

// roi size
int bild_start_x = aPosX - 2;
int bild_start_y = aPosY - 2;
int bild_length_x = bPosX - aPosX + 4;
int bild_height_y = cPosY - aPosY + 4;

int length_x = bPosX - aPosX;
int steps;
char* direction;
string hotspot = "Hello";
const char* myLink;

void openURL(String richtung, int schritte);

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

			if (x < length_x / 2)
			{
				steps = (length_x / 2 - x);
				direction = "left";
				printf("Direction : Left\t");
				printf("Steps : %d", steps);
			}
			else
			{
				steps = (x - length_x / 2);
				direction = "right";
				printf("Direction : Right\t");
				printf("Steps : %d", steps);
			}
		}
	}
	openURL(direction, steps);
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
	a.x = aPosX;
	a.y = aPosY;

	Point b;
	b.x = bPosX;
	b.y = bPosY;

	Point c;
	c.x = cPosX;
	c.y = cPosY;

	Point d;
	d.x = dPosX;
	d.y = dPosY;

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
	//String picture = ConfigurationManager.AppSettings["picture"];
	
	src = imread(picName, 1);

	// Definiert Range of Interest (Begrenzt Bild auf das Spielfeld)
	// Mat roi(src, Rect(bild_start_x, bild_start_y, bild_length_x, bild_height_y));

	// Macht das Bild ein wenig heller (Schatten rausfiltern wenn möglich)
	//roi = brightenPicture(roi);

	src = defineROI(src);

	//Test
	Mat roi(src, Rect(bild_start_x, bild_start_y, bild_length_x, bild_height_y));
	roi = brightenPicture(roi);
	//Test

	// Möglicherweise unnötig, da neu ROI
	src = brightenPicture(src);

	/// Macht das Bild komplett Schwarz/Weiss
	cvtColor(roi, src_gray, CV_BGR2GRAY);

	blur(src_gray, src_gray, Size(3, 3));
	threshold(src_gray, threshold_output, 53, max_BINARY_value, 0);

	imwrite("BW-Pic.png", threshold_output);

	/// Create Window to show result
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, roi);

	// Threshold Algorithmus
	thresh_callback(0, 0);
}

void writeXMLFile()
{
	XMLNode* pRoot = xmldoc.NewElement("Root");
	xmldoc.InsertFirstChild(pRoot);

	XMLElement* pElement = xmldoc.NewElement("IntValue");
	pElement->SetText(10);
	pRoot->InsertEndChild(pElement);

	pElement = xmldoc.NewElement("FloatValue");
	pElement->SetText(0.5f);

	pRoot->InsertEndChild(pElement);

	pElement = xmldoc.NewElement("Date");
	pElement->SetAttribute("day", 26);
	pElement->SetAttribute("month", "April");
	pElement->SetAttribute("year", 2014);
	pElement->SetAttribute("dateFormat", "26/04/2014");

	pRoot->InsertEndChild(pElement);

	XMLError eResult = xmldoc.SaveFile("SavedData.xml");
}

void loadXMLFile()
{
	XMLError eResult = xmldoc.LoadFile("config.xml");
	waitKey(0);
}

void readXMLFileExample()
{
	XMLNode* pRoot = xmldoc.FirstChild();
	XMLElement * pElement = pRoot->FirstChildElement("IntValue");
	int iOutInt;
	XMLError eResult = pElement->QueryIntText(&iOutInt);

	pElement = pRoot->FirstChildElement("FloatValue");

	float fOutFloat;
	eResult = pElement->QueryFloatText(&fOutFloat);

	pElement = pRoot->FirstChildElement("Date");

	int iOutDay, iOutYear;

	eResult = pElement->QueryIntAttribute("day", &iOutDay);

	eResult = pElement->QueryIntAttribute("year", &iOutYear);

	const char * szAttributeText = nullptr;

	szAttributeText = pElement->Attribute("month");
	std::string strOutMonth = szAttributeText;
	szAttributeText = pElement->Attribute("dateFormat");
	std::string strOutFormat = szAttributeText;
}

void readXMLFile()
{
	XMLNode* pRoot = xmldoc.FirstChild();
	XMLElement * pElement = pRoot->FirstChildElement("Config");
	const char * tempName = nullptr;
	tempName = pElement->Attribute("picName");
	picName = tempName;
	tempName = pElement->Attribute("hotspot");
	hotspot = tempName;
	waitKey(0);
}

void openURL(String richtung, int schritte)
{
	String demoLink;
	String http = "http://";
	String server = hotspot;
	String filler = "/startProgramm.php?direction=";
	String filler2 = "&steps=";
	String schritt = to_string(schritte);

	demoLink = http + server + filler + direction + filler2 + schritt;
	myLink = demoLink.c_str();

	ShellExecute(NULL, "Open", myLink, NULL, NULL, 1);
}

int main()
{
	loadXMLFile();
	readXMLFile();

	imageProcessing();

	waitKey(0);
	return(0);
}

