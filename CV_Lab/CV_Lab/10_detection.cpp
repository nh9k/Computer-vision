#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
using namespace std;

/** Function Headers */
void detectAndDisplay(Mat frame);

/** Global variables */ //이곳에 support vector 등 으로 구성된 //미리 학습된 것을 가져와 사용함
String face_cascade_name = "C:/opencv24136/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "C:/opencv24136/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade; //얼굴 검출 클래스에서 객체 생성 -> 여기 안에 분류기 등 다 들어감
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";


/** @function main */
int main(void)
{
	//-- 1. Load the cascades DB (trained) //로드한 데이터 없으면 에러 띄움
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

	//-- 2. Read the test image
	Mat frame = imread("../image/Lena.jpg"); //cvQueryFrame( capture );

	//-- 3. Apply the classifier to the frame
	detectAndDisplay(frame);
	waitKey(0);
	return 0;

}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, CV_BGR2GRAY); // color를 gray로 변환
	equalizeHist(frame_gray, frame_gray); // 대조비 키워줌 // EX. 밝은 건 1, 어두운 건 0
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30)); //크기에 무관하게 detect하기 위함

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		Mat faceROI = frame_gray(faces[i]); //ROI 
		std::vector<Rect> eyes;

		//-- In each face, detect eyes //얼굴에서 눈 검출
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.05, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0); // 동그라미 그림
		}
	}
	//-- Show what you got
	imshow(window_name, frame);
	imwrite("../result_image/10_detection.jpg", frame);
}
