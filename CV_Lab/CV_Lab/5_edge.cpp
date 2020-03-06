#include "opencv2/core/core.hpp" //헤더파일 c++
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//6주차 //0416 //edge
	Mat src = imread("../image/Lena.jpg");

	imshow("Input", src);

	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);

	Mat edge;
	Canny(gray, edge, 50, 200, 3); //50 링킹//150 넘으면 잡음 //3 조리개
	//edge.convertTo(edge, CV_8U); //그레이 스케일

	imshow("Canny Result", edge);
	imwrite("../result_image/5_edge_canny.jpg", edge);
	waitKey(0); //안먹히면 cin.ignore(); 
				//사용자가 키보드치는 입력을 1개만 더 받음: ignore(1)->엔터키(다른 key도 됨) 기다림, 콘솔창 닫힘
	return 0;
}