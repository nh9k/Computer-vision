//11주차 //0521 //stereo
#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//load stereo images -> 영상 2장
	Mat L = imread("../image/scene1.row3.col1.ppm"); // left  영상
	Mat R = imread("../image/scene1.row3.col2.ppm"); // right 영상
	imshow("Left Image", L);
	imshow("Right Image", R);
	imwrite("../result_image/9_stereo_Left.jpg", L);
	imwrite("../result_image/9_stereo_Right.jpg", R);


	// set block matching parameters // 파라미터 세팅
	CvStereoBMState *BMState = cvCreateStereoBMState(); // 클래스를 이용해 객체 만듦 -> 함수가 주소 return한 것을 받음
	BMState->preFilterSize = 5;  // brightness correction, 5x5 ~ 21x21 // 객체는 ., pointer는 그 주소로 가서 화살표로 함
	BMState->preFilterCap = 5;	// removed region after prefilter
	BMState->SADWindowSize = 11; // SAD: sum of absolute difference, window size (5x5.... 21x21) // 윈도우 사이즈 11(surport 영역) 좀 크게함
	//영역 11->5 모호성이 커져서 오히려 잘 안나옴 // 25는 두루뭉실하지만(정밀도 떨어짐) matching은 더 잘함
	BMState->minDisparity = 1;   // minimum disparity [pixel] // 찾고자하는 minimum 값
	BMState->numberOfDisparities = 32; // maximum disparity [pixel] // 1~32까지 pixel 봄
	BMState->textureThreshold = 10;  // minimum allowed
	BMState->uniquenessRatio = 5;    // uniqueness  (removing false matching) 

	// convert color space
	Mat Left, Right;
	cvtColor(L, Left, CV_RGB2GRAY); // gray로 변환
	cvtColor(R, Right, CV_RGB2GRAY);

	// type conversion: mat to IplImage
	IplImage *left, *right;
	left = &IplImage(Left);
	right = &IplImage(Right);

	CvMat* disparity = cvCreateMat(Left.rows, Left.cols, CV_16SC1);
	CvMat* disparity_img = cvCreateMat(Left.rows, Left.cols, CV_8UC1); // 1~32 -> 0~255 밝기를 눈으로 볼 수 있게 바꿔줌

	// run algorithm
	cvFindStereoCorrespondenceBM(left, right, disparity, BMState);
	cvNormalize(disparity, disparity_img, 0, 255, CV_MINMAX);  //normalize to display

	// show results // stereo matching show
	cvShowImage("Disparity", disparity_img);
	imwrite("../result_image/9_stereo_disparity.jpg", (Mat)disparity_img);

	waitKey(0);
	return 0;
}