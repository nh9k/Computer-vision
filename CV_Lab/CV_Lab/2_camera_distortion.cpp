#include <iostream>
#include <opencv2/highgui.hpp> //imread, imshow
#include <opencv2/imgproc.hpp> //undistort

using namespace cv;

int main(void)
{
	//load an image
	Mat in = imread("../image/soccer_distortion.jpg");
	imshow("input", in);

	//set camera parameters
	float x0 = 640.0;
	float y0 = 360;
	float fx = 1108.0; //[pixel]
	float fy = 1108.0; //[pixel]

	//按眉 积己 饶 insertion operator 结辑 青纺 盲况持澜
	Mat cameraMatrix = (Mat_<float>(3, 3) << fx, 0, x0, 0, fy, y0, 0, 0, 1);

	//set distortion parameters
	float k[5];
	k[0] = -4.3287e-01; //k1
	k[1] = -5.8376e-01; //k2
	k[2] = 0.; //p1
	k[3] = 0.; //p2
	k[4] = 7.1215e-01; //k3

	//distortion 按眉 积己
	Mat distCoeff = (Mat1d(1, 5) << k[0], k[1], k[2], k[3], k[4]);

	// 搬苞 按眉 积己
	Mat result;
	undistort(in, result, cameraMatrix, distCoeff);
	imshow("result", result);
	imwrite("../result_image/2_camera_distortion_result.jpg", result);
	waitKey();

	return 0;
}