#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

// Global variables of Lab.2
Mat src, src_gray; //원본 이미지, gray 전환 이미지
Mat dst, detected_edges; //결과 이미지, edge 이미지
//int edgeThresh = 1; //필요없는 변수
int lowThreshold; //현재 slicing 위치 
int const max_lowThreshold = 100; //slicing 단위(범위: 0~100)
int ratio = 3; //*lowThreshold = highThreshold
int kernel_size = 3; //aperture
char window_name[] = "(Lab 2) Interactive Canny Detector - 김난희";

void CannyThreshold(int, void*)
{
	// @function CannyThreshold // @brief Trackbar callback - Canny thresholds input with a ratio 1:3

	// Reduce noise with a kernel 3x3 @input, output, kernel size
	blur(src_gray, detected_edges, Size(3, 3));

	// Canny detector @input, output, 최소 threshold1, 최대 threshold2, aperture
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0); //fill zero matrix //검은색 배경
	src.copyTo(dst, detected_edges); // mask: detected_edges //배경 위에 edge 씌움

	//show result about colorful edge detect
	imshow(window_name, dst);
}
int main()
{
	//***** ***** Lab.1 Simple Canny Edge Detector ***** *****//

	// Load an image
	src = imread("../image/colorful stock.jpg");
	resize(src, src, Size(640, 480));
	//namedWindow("Original image", CV_WINDOW_AUTOSIZE); 
	imshow("(Lab 1) Original image - 김난희", src); //show original image
	//imwrite("../result_image/5_sobel_Original.jpg", src);

	Mat gray, edge, draw; //gray 이미지, edge 결과 이미지, edge 결과를 gray로 바꿀 이미지
	cvtColor(src, gray, CV_BGR2GRAY); //원본 이미지를 gray 이미지로 바꿈
	Canny(gray, edge, 100, 200, 3); //@input, output, low threshold(linking), high threshold, aperture
	edge.convertTo(draw, CV_8U); //to convert gray scale 1채널 unsigned char
	//namedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("(Lab 1) image Using canny edge detector - 김난희", draw);
	//imwrite("../result_image/5_sobel_canny edge.jpg", draw);



	//***** ***** Lab.2 Interactive Canny Detector ***** *****//

	// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());

	// Convert the image to grayscale @input, output, BGR to GRAY
	cvtColor(src, src_gray, CV_BGR2GRAY);

	// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	// Create a Trackbar for user to enter threshold 
	// @Trackbar이름, Window이름, 현재 slicing 위치, slicing 단위, 적용할 함수
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	// Show the image //thread
	CannyThreshold(0, 0);

	//***** ***** Lab.3 Sobel Edge ***** *****//

	// Sobel Filter @input, output, depth(출력이미지 깊이조합,8비트:절단유도체, dx, dy
	Mat sobelMag, sobelDir, sobelX, sobelY;

	Sobel(gray, sobelX, CV_8U, 1, 0); //x direction
	imshow("(Lab 3) Ix using sobel mask - 김난희", sobelX);
	imwrite("../result_image/5_sobel_Ix_using_sobel_mask.jpg", sobelX);

	Sobel(gray, sobelY, CV_8U, 0, 1); //y direction
	imshow("(Lab 3) Iy using sobel mask - 김난희", sobelY);
	imwrite("../result_image/5_sobel_Iy_using_sobel_mask.jpg", sobelY);

	//magnitude(sobelX, sobelY, sobelMag);
	sobelMag = abs(sobelX) + abs(sobelY); //magnitude
	imshow("(Lab 3) Magnitude - 김난희", sobelMag);
	imwrite("../result_image/5_sobel_Magnitude.jpg", sobelMag);

	sobelX.convertTo(sobelX, CV_32F); //phase 계산을 위해 float형변환
	sobelY.convertTo(sobelY, CV_32F);
	sobelMag.convertTo(sobelMag, CV_32F);

	//cartToPolar(sobelX, sobelY, sobelMag, sobelDir);
	phase(sobelX, sobelY, sobelDir); //@input x, input y, output angle
	imshow("(Lab 3) Direction - 김난희", sobelDir); //direction

	normalize(sobelDir, sobelDir, 0, 255, NORM_MINMAX);
	imwrite("../result_image/5_sobel_Direction.jpg", sobelDir);

	// Wait until user exit program by pressing a key
	waitKey(0);
	return 0;
}

