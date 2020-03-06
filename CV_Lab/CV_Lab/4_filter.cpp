#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//5주차
	Mat src = imread("../image/corni_fructus.jpg");
	imshow("Original", src);

	//convert the rgb to gray
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	imshow("Gray", gray);

	//read a gray pixel
	cout << (int)gray.at<uchar>(50, 10) << endl; //문자로 보여주니까 int형으로 바꾸기

	// read a color pixel
	Vec3b pixel;
	pixel = src.at<Vec3b>(50, 10);
	cout << (int)pixel[0] << endl; //b
	cout << (int)pixel[1] << endl; //g
	cout << (int)pixel[2] << endl; //r

	int filterSz = 50;
	Mat result;
	blur(gray, result, Size(filterSz, filterSz), Point(-1, -1), BORDER_DEFAULT);

	imshow("Result", result);
	imwrite("../result_image/4_filter_blur.jpg", result);

	waitKey();
	return 0;
}