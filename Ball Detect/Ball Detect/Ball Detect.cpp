#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

const int fps = 60;

int main(int argv, char** argc)
{
	Mat frame, newframe;
	Mat planes[3];

	VideoCapture vid(0);

	if (!vid.isOpened()) 
	{
		return -1;
	}
	
	while (vid.read(frame)) 
	{

		split(frame, planes);
		imshow("Webcam", frame);
		inRange(frame, Scalar(50, 50, 120), Scalar(150, 150, 255), newframe);
		imshow("RGB Filtered Webcam", newframe);
		cvtColor(frame, newframe, COLOR_BGR2HSV);
		inRange(newframe, Scalar(160, 100, 100), Scalar(200, 255, 255), newframe);
		imshow("Filtered Webcam", newframe);
		
		vector<vector<Point> > contours;
		findContours(newframe, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		Point2f center = { 0, 0 };
		float radius = 0;

		const float area_treshold = 100;
		float area = area_treshold;

		for (size_t i = 0; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) > area) {
				area = contourArea(contours[i]);
				minEnclosingCircle(contours[i], center, radius);
			}
		}

		if (area > area_treshold) {
			Scalar color = Scalar(0, 0, 255);
			circle(frame, center, (int) radius, color, 2);
			circle(frame, center, 1, color, 2);
		}

		/*
		GaussianBlur(newframe, newframe, Size(9, 9), 2, 2);
		vector<Vec3f> circles;
		HoughCircles(newframe, circles, HOUGH_GRADIENT, 2, newframe.rows / 4, 200, 100, 25, 100);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// draw the circle center
			circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			// draw the circle outline
			circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
		};
		*/

		imshow("Processed Webcam", frame);


		if (waitKey(1000/fps) >= 0)
		{
			break;
		}
	}
	return 1;
}
