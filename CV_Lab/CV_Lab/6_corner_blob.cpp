#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/nonfree/features2d.hpp> // for Lab.2: SIFT Detector

using namespace cv;

//***** Global variables for Lab.1
Mat src, src_gray; //원본, gray image
int thresh = 80, max_thresh = 255; //threshold
String source_window = "Source image - 김난희";
String corners_window = "Corners detected - 김난희";

/// Function header
void cornerHarris_demo(int, void*);

int main(void)
{
	//***** ***** Lab.1 Simple Canny Edge Detector ***** *****//
	/// Load source image, resize 1/2 of size and convert it to gray
	src = imread("../image/Rhone_River.jpg");
	//resize(src, src, Size(src.size().width/2, src.size().height / 2));
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Create a window and a trackbar
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);

	/// Show image and keep running
	imshow(source_window, src);
	cornerHarris_demo(0, 0);

	//***** ***** Lab.2 SIFT(blob) detector ***** *****//

	/// SIFT feature detector: SIFT 객체 생성
	SiftFeatureDetector detector;

	/// Feature detection: Keypoints 객체 생성
	vector<KeyPoint> keypoints;

	/// using detect func: image에서 keypoints를 계산하고 추출, 
	/// 일부분의 keypoints 검출을 위해서는 mask 인자를 전달하면 됨.
	detector.detect(src, keypoints);

	/// draw Keypoints: image에 keypoints의 위치를 원으로 표시
	/// Scalar::all(-1)로 여러 색상으로 표시하며, DRAW_RICH_KEYPOINTS를 지정하면 크기와 방향성도 함께 표시
	drawKeypoints(src, keypoints, src, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	/// Show detected (drawn) keypoints
	imshow("SIFT Dectector/ Keypoints - 김난희", src);
	imwrite("../result_image/6_corner_blob_SIFT_Dectector_Keypoints.jpg", src);

	waitKey(0);
	return(0);
}

/** @function cornerHarris_demo */
void cornerHarris_demo(int, void*)
{
	/// 결과 image, normalize할 image, scaling할 image 선언
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(src.size(), CV_32FC1); // 결과 image를 0으로 채움, float형 선언

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04; //k는 0.04~0.06

	/// Detecting corners
	cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing: 자신이 원하는 data range로 바꿈, scale을 바꿈
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	// cornerHarris 함수에서 float형으로 바뀐 것을 다시 바꿈
	// float은 imshow 하지 못하기 때문
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners: 원으로 표시
	for (int j = 0; j < dst_norm.rows; j++) //height 만큼
	{
		for (int i = 0; i < dst_norm.cols; i++) //width 만큼
		{
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				//@input, point center(원 중심), radius(지름), 색상:0, 
				//굵기:2, 8-connected line(부드럽게), 이동량:0
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}

	/// Showing the result
	namedWindow(corners_window, CV_WINDOW_AUTOSIZE);
	imshow(corners_window, dst_norm_scaled);
	imwrite("../result_image/6_corner_blob_Corners_detected.jpg", dst_norm_scaled);
}