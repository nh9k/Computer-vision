#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(void)
{
	//4주차
	Mat image = imread("../image/corni_fructus.jpg"); //read an image
	imshow("RGB color image", image); //show ths image

	// split the image into 3 channels (B,G,R)
	vector<Mat> channels;
	split(image, channels); //channels의 [0] [1] [2]가 channels

	//show each channel
	imshow("Blue", channels[0]); //나오는 색상이 밝게 나옴
	imshow("Green", channels[1]);
	imshow("Red", channels[2]);
	imwrite("../result_image/3_color_blue.jpg", channels[0]);
	imwrite("../result_image/3_color_green.jpg", channels[1]);
	imwrite("../result_image/3_color_red.jpg", channels[2]);
	waitKey();

	return 0;
}