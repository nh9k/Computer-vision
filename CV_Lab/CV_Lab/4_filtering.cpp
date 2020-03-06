//21611591 김난희
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdlib.h> // srand 함수 사용을 위해서
#include <time.h> // time 함수 사용을 위해서
#include <string.h>

using namespace cv;
using namespace std;

void salt_pepper(cv::Mat image, int n) // 소금-후추 잡음 첨가 함수
{
	int i, j;
	srand((int)time(NULL));

	for (int k = 0; k < n; k++)

	{
		i = rand() % image.cols; // 이미지의 열크기 내에서 랜덤 수 생성, x 좌표값
		j = rand() % image.rows; // 이미지의 행크기 내에서 랜덤 수 생성, y 좌표값

		//cout << "at (" << j << ", " << i << "), add salt or pepper!" << endl; // 랜덤하게 결정된 픽셀 위치 출력, (x, y)

		int salt_or_pepper = (rand() % 2) * 255; // 랜덤하게 0 또는 255, 0이면 후추, 255면 소금

		if (image.type() == CV_8UC1) // 그레이레벨 영상이라면
		{
			image.at<uchar>(j, i) = salt_or_pepper; // 랜덤하게 선택된 픽셀에 0 또는 255을 대입 
		}
		else if (image.type() == CV_8UC3) // 3채널 컬러 영상이라면
		{
			// 랜덤하게 선택된 픽셀에 0 또는 255을 대입, 흰색 또는 검정색을 만들기 위해 세 컬러 채널에 동일한 것이 들어감. (0, 0, 0) 또는 (255, 255, 255)
			image.at<cv::Vec3b>(j, i)[0] = salt_or_pepper; // B 채널
			image.at<cv::Vec3b>(j, i)[1] = salt_or_pepper; // G 채널
			image.at<cv::Vec3b>(j, i)[2] = salt_or_pepper; // R 채널
		}
	}
}
Mat gaussian(cv::Mat Origin, cv::Mat dst, float sigma = 30.0) //블로그에서 30.0을 넣어놓아 defalut로 설정함
{
	//---노이즈 image 생성---//
	Mat noise(Origin.size(), CV_32FC3); //노이즈만 첨가된 이미지 생성 //3채널 double
	float average = 0.0; //평균이 0이고
	float std = sigma; //표준 편차가 sigma이도록 설정

	//평균 average, 표준편차 std인 정규분포를 따르는 난수 생성 matrix 반환
	randn(noise, Scalar::all(average), Scalar::all(std));
	imshow("(Lab3) Gaussian noise Image - 김난희", noise);
	//imwrite("../result_image/4_filtering_Gaussian_noise.jpg", noise);

	Mat3b imageF_8UC3;
	noise.convertTo(imageF_8UC3, CV_8UC3, 255);
	imwrite("../result_image/4_filtering_Gaussian_noise.jpg", imageF_8UC3);

	Origin.convertTo(dst, CV_32FC3); //원본사진에서 dst image로 형변환 복사 //copyTo를 해줄 경우는 오류.
	addWeighted(dst, 1.0, noise, 1.0, .0, dst); //dst(마지막 인자) = dst * 1.0 + noise * 1.0 + .0 
	dst.convertTo(dst, Origin.type()); //합성 후에 dst 자료형을 다시 원본 사진과 같게 해줌

	return dst; //결과 image return
}
int main(int argc, char** argv)
{
	/********** 1. OpenCV-Blur **********/

	//Load an image from file
	Mat src = imread("../image/Lena.jpg");

	//show the loaded image
	imshow("(Lab1) Original Image - 김난희", src);

	//결과를 저장할 Mat 객체 선언
	Mat dst;
	char zBuffer[35];

	for (int i = 1; i < 31; i = i + 2)
	{
		//copy the text to the "zBuffer"
		_snprintf_s(zBuffer, 35, "Kernel Size : %d x %d", i, i);

		//smooth the image in the "src" and save it to "dst"
		blur(src, dst, Size(i, i), cv::Point(-1, -1), BORDER_REPLICATE); //Point(-1,-1): kernel center

		//put the text in the "zBuffer" to the "dst" image
		//사진과 함께 글자를 넣음, 내 사진에 맞게 위치와 사이즈 변경해줌
		putText(dst, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255)
		);

		//show the blurred image with the text
		imshow("(Lab1) Smoothed Image - 김난희", dst);
		imwrite("../result_image/4_filtering_smoothed_" + to_string(i) + ".jpg", dst);

		//wait for 2 seconds
		int c = waitKey(20);

		//if the "esc" key is pressed during the wait, return
		if (c == 27)
		{
			return 0;
		}
	}
	//make the "dst" image, black
	dst = Mat::zeros(src.size(), src.type());

	//copy the text to the "zBuffer"
	_snprintf_s(zBuffer, 35, "Press Any Key to Exit");

	//사진을 마치고 종료키를 눌러 종료해도 된다는 글자를 넣음
	//put the text in the "zBuffer" to the "dst" image
	putText(dst, zBuffer, Point(src.cols / 6, src.rows / 2), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));

	//show the black image with the text
	imshow("(Lab1) Smoothed Image - 김난희", dst);

	//wait for a key press infinitely
	//waitKey(0);

	/********** 2. Median Filter **********/

	//Mat src = imread("myimg.jpg");	//Load an image from file	
	imshow("(Lab2) Original Image - 김난희", src);	//Show the Original image

	//---Noise 첨가---//	
	Mat spn_img;		//salt and pepper noise를 담을 Mat 이미지 객체 선언함	
	src.copyTo(spn_img);		//원본 이미지를 복사해 원본과 별개로 그 위에 noise를 씌우기 위함

	salt_pepper(spn_img, 5000);		//원본 image에 5000알의 소금 또는 후추를 뿌림
	imshow("(Lab2) (salt and pepper) Noise Added Image - 김난희", spn_img);  //Show salt and pepper noise image
	imwrite("../result_image/4_filtering_salt&pepper_Noise_Added.jpg", spn_img);

	//---Filtering---//
	Mat MedianF_img; //noise 영상에서 median filter가 씌워진 결과를 보여줄 객체 선언
	Mat GaussianF_img; //noise 영상에서 gaussian filter가 씌워진 결과를 보여줄 객체 선언

	//char zBuffer[35]; //image에 글자를 띄울 buffer 선언

	for (int i = 1; i <= 9; i += 2) //kernel size 변화 //kernel size는 홀수로 들어갈 수 있음
	{
		medianBlur(spn_img, MedianF_img, i); //median filter를 씌움, kernel size
		GaussianBlur(spn_img, GaussianF_img, Size(i, i), 0); //gaussian filter를 씌움, kernel size, sigma 0이면 kernel size에 따라 자동 계산

		_snprintf_s(zBuffer, 35, "Kernel Size : %d x %d", i, i); // kernel size에 따라 다르게 글자를 표시함
		//사진과 함께 글자를 넣음, 내 사진에 맞게 위치와 사이즈 변경해줌
		putText(MedianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));
		putText(GaussianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));

		imshow("(Lab2) Median filtered Image - 김난희", MedianF_img); //median filter를 씌운 이미지를 보여줌
		imshow("(Lab2) Gaussian filtered Image - 김난희", GaussianF_img); //gaussian filter를 씌운 이미지를 보여줌
		imwrite("../result_image/4_filtering_salt&pepperNoise_Median_filtered_kernel_size_" + to_string(i) + ".jpg", MedianF_img);
		imwrite("../result_image/4_filtering_salt&pepperNoise_Gaussian_filtered_kernel_size_" + to_string(i) + ".jpg", GaussianF_img);

		//wait for 2 seconds
		int c = waitKey(20);
	}
	for (int i = 1; i <= 5; i += 2) //sigma 변화 //sigma는 홀수로 가능
	{
		GaussianBlur(spn_img, GaussianF_img, Size(5, 5), i, i); //gaussian filter를 씌움, kernel size, sigma

		_snprintf_s(zBuffer, 35, "Sigma : %d", i); // Sigma에 따라 다르게 글자를 표시함
		//사진과 함께 글자를 넣음, 내 사진에 맞게 위치와 사이즈 변경해줌
		putText(GaussianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));

		imshow("(Lab2) Gaussian filtered Image - 김난희", GaussianF_img); //gaussian filter를 씌운 이미지를 보여줌
		imwrite("../result_image/4_filtering_salt&pepperNoise_Gaussian_filtered_sigma_" + to_string(i) + ".jpg", GaussianF_img);

		//wait for 2 seconds
		int c = waitKey(20);
	}

	/********** 3. Gaussian Filter **********/
	//Mat src = imread("myimg.jpg");	//Load an image from file	
	imshow("(Lab3) Original Image - 김난희", src);	//Show the Original image

	//---Noise 첨가---//	
	Mat gn_img;		//gaussian noise를 담을 Mat 이미지 객체 선언함	

	gn_img = gaussian(src, gn_img, 20); //가우시안 노이즈 첨가 //sigma = 20
	imshow("(Lab3) (gaussian) Noise Added Image - 김난희", gn_img);  //Show salt and pepper noise image
	imwrite("../result_image/4_filtering_gaussian_Noise_Added.jpg", gn_img);

	//---Filtering---//
	//Mat MedianF_img; //noise 영상에서 median filter가 씌워진 결과를 보여줄 객체 선언
	//Mat GaussianF_img; //noise 영상에서 gaussian filter가 씌워진 결과를 보여줄 객체 선언

	//char zBuffer[35]; //image에 글자를 띄울 buffer 선언

	for (int i = 1; i <= 9; i += 2) //kernel size 변화 //kernel size는 홀수로 들어갈 수 있음
	{
		medianBlur(gn_img, MedianF_img, i); //median filter를 씌움, kernel size
		GaussianBlur(gn_img, GaussianF_img, Size(i, i), 0); //gaussian filter를 씌움, kernel size, sigma 0이면 kernel size에 따라 자동 계산

		_snprintf_s(zBuffer, 35, "Kernel Size : %d x %d", i, i); // kernel size에 따라 다르게 글자를 표시함
		//사진과 함께 글자를 넣음, 내 사진에 맞게 위치와 사이즈 변경해줌
		putText(MedianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));
		putText(GaussianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));

		imshow("(Lab3) Median filtered Image - 김난희", MedianF_img); //median filter를 씌운 이미지를 보여줌
		imshow("(Lab3) Gaussian filtered Image - 김난희", GaussianF_img); //gaussian filter를 씌운 이미지를 보여줌
		imwrite("../result_image/4_filtering_gaussianNoise_Median_filtered_kernel_size_" + to_string(i) + ".jpg", MedianF_img);
		imwrite("../result_image/4_filtering_gaussianNoise_Gaussian_filtered_kernel_size_" + to_string(i) + ".jpg", GaussianF_img);

		//wait for 2 seconds
		int c = waitKey(20);
	}
	for (int i = 1; i <= 5; i += 2) //sigma 변화 //sigma는 홀수로 가능
	{
		GaussianBlur(gn_img, GaussianF_img, Size(5, 5), i, i); //gaussian filter를 씌움, kernel size, sigma

		_snprintf_s(zBuffer, 35, "Sigma : %d", i); // Sigma에 따라 다르게 글자를 표시함
		//사진과 함께 글자를 넣음, 내 사진에 맞게 위치와 사이즈 변경해줌
		putText(GaussianF_img, zBuffer, Point(src.cols / 4, src.rows / 8), CV_FONT_HERSHEY_COMPLEX, 0.9, Scalar(255, 255, 255));

		imshow("(Lab3) Gaussian filtered Image - 김난희", GaussianF_img); //gaussian filter를 씌운 이미지를 보여줌
		imwrite("../result_image/4_filtering_gaussianNoise_Gaussian_filtered_sigma_" + to_string(i) + ".jpg", GaussianF_img);

		//wait for 2 seconds
		int c = waitKey(20);
	}

	//실제로는 kernel size가 5x5이고 sigma가 kernel size의 1/6배 정도로 하는데, 보이지 않으므로 100배 크게하여 보여줌
	Mat kernelX = getGaussianKernel(600, 100); //kernel size, sigma, kernel type을 넣어줌 --> 가우시안 coefficient를 얻음
	Mat kernelY = getGaussianKernel(600, 100);
	Mat GaussianK = kernelX * kernelY.t(); //x축과 y축을 합쳐서 하나의 완전한 kernel이 만들어짐 //주파수는 합이 곱하기

	normalize(GaussianK, GaussianK, 0, 0.8, NORM_MINMAX); //src, dst, alpha, beta, nomalize //  nomalize 해주며 가우시안 분포를 따르도록 함
	imshow("(Lab3) Gaussian Kernel Image - 김난희", GaussianK); //가우시안 커널을 imshow함

	normalize(GaussianK, GaussianK, 0, 255, NORM_MINMAX);
	/*
	`왜 normalize 해서 write 해주는가?`
	normalize를 해주지 않을 경우 검은색 화면으로 imwrite 됨.

	<imwrite documentation>
	https://docs.opencv.org/2.4/modules/highgui/doc/reading_and_writing_images_and_video.html?highlight=imwrite#imwrite
	The function imwrite saves the image to the specified file. 
	The image format is chosen based on the filename extension (see imread() for the list of extensions). 
	Only 8-bit (or 16-bit unsigned (CV_16U) in case of PNG, JPEG 2000, and TIFF)
	single-channel or 3-channel (with ‘BGR’ channel order) images can be saved using this function. 
	If the format, depth or channel order is different, 
	use Mat::convertTo() , and cvtColor() to convert it before saving.
	
	`imshow는 왜 되는가?`
	<imshow>
	If the image is 8-bit unsigned, it is displayed as is.
	If the image is 16-bit unsigned or 32-bit integer, the pixels are divided by 256. That is, the value range [0,255*256] is mapped to [0,255].
	If the image is 32-bit floating-point, the pixel values are multiplied by 255. That is, the value range [0,1] is mapped to [0,255].
	*/
	imwrite("../result_image/4_filtering_Gaussian_Kernel_example.jpg", GaussianK);

	waitKey(0);

	return 0;
}