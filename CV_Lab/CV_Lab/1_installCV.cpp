#include "opencv2/opencv.hpp"//openCV lib
#include "opencv2/imgproc/imgproc.hpp" //image proc lib
using namespace cv;

int main(void)
{
	//load an image
	Mat img = imread("../image/corni_fructus.jpg");
	//show
	imshow("Image", img);
	imwrite("../result_image/1_installCV_Image.jpg", img);
	waitKey();

	return 0;
}