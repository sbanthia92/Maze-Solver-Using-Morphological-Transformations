#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<stdio.h>
		
using namespace cv;
using namespace std;

int main()
{	
	namedWindow("inputwindow", 1);	//create a new window
	namedWindow("outputwindow", 1);	//create a new window

	Mat image = imread("C:/Users/Shubham/Documents/MS/courses/Video processing/maze/maze3.png");	//importing the image
	//imshow("window", image);

	Mat invert, gray;
	cvtColor(image, gray, CV_BGR2GRAY);		//convert the image to grayscale
	threshold(gray, invert, 1, 255, THRESH_BINARY_INV);		//invert the grayscaled image 
	//cout << image << endl;

	vector<vector<Point>> contour;		//define a vector of points for the contours 
	Mat wall = Mat(invert.rows, invert.cols, CV_8UC1, Scalar(0));		//variable to save one wall of the image
	//cout << wall;
	findContours(invert, contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);		//find only the end points of the outermost contour
	//cout << contour.size()<<endl;
	drawContours(wall, contour, 0, CV_RGB(255,255,255), CV_FILLED);			//Draw the contour in the variable
	//imshow("window", wall);

	Mat kernel = Mat(15, 15, CV_8UC1,  Scalar(1));		//define a kernel
	Mat dilation;	
	dilate(wall, dilation, kernel, Point(-1, -1), 1, 0);		//dilate the image and store in a variable
	//imshow("window1", dilation);
	Mat erosion;
	erode(dilation, erosion, kernel, Point(-1, -1), 1, 0);		//erode the dilated image and store in a variable
	//imshow("window2", erosion);
	
	Mat path;
	absdiff(dilation, erosion, path);		//taking the absolute difference between the eroded and dilated image gives us the correct path
	//imshow("window", path);
	
	Mat path_inv = Mat(path.rows, path.cols, CV_8UC3);		
	threshold(path, path_inv, 1, 255, THRESH_BINARY_INV);		//invert the path obtained
	//imshow("window", path_inv);
	
	vector<Mat> channels;
	split(image, channels);		//split the original image into channels
	//imshow("window1", channels[0]);		//the correct path in 'path' is high, where as that in the 'path_inv' is low
	//imshow("window1", channels[1]);		//the path area in 'image' is high
	//imshow("window1", channels[2]);		//
	//cout << channels[0];

	channels[0] |= path;		//Logical OR the blue channel of the image with the path (thus, the path area would be high)
	channels[1] &= path_inv;	//Logical AND the green channel of the image with the path 
	channels[2] &= path_inv;	//Logical AND the red channel of the image with the path

	//imshow("window2", channels[0]);
	//imshow("window2", channels[1]);
	//imshow("window2", channels[2]);

	Mat output;
	merge(channels, output);		//merge all three channels into one single image	
	imshow("outputwindow", output);		//display the output with the correct path
	imshow("inputwindow", image);		//display the original image for comparison
	imwrite("output3.png", output);

	waitKey(0);
	return 0;
}


