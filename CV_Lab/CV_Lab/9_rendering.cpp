//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/core/core.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/opencv.hpp"
//
//using namespace cv;
//using namespace std;
//
////* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
////* * * * * * * * * * for lab2 - Render the disparity in 3D space (point cloud) * * * * * * * * * * poing cloud를 사용하려면 여기서부터 주석 해제 * * * * * * * * //
////* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//template <class T>
//static void fillOcclusionInv_(Mat& src, T invalidvalue)
//{
//	int bb = 1;
//	const int MAX_LENGTH = src.cols*0.8;
//#pragma omp parallel for // openMP를 이용한 병렬 처리
//	for (int j = bb; j < src.rows - bb; j++)
//	{
//		T* s = src.ptr<T>(j);
//		const T st = s[0];
//		const T ed = s[src.cols - 1];
//		s[0] = 0;
//		s[src.cols - 1] = 0;
//		for (int i = 0; i < src.cols; i++)
//		{
//			if (s[i] == invalidvalue)
//			{
//				int t = i;
//				do
//				{
//					t++;
//					if (t > src.cols - 1)break;
//				} while (s[t] == invalidvalue);
//
//				const T dd = max(s[i - 1], s[t]);
//				if (t - i > MAX_LENGTH)
//				{
//					for (int n = 0; n < src.cols; n++)
//					{
//						s[n] = invalidvalue;
//					}
//				}
//				else
//				{
//					for (; i < t; i++)
//					{
//						s[i] = dd;
//					}
//				}
//			}
//		}
//	}
//}
//template <class T>
//static void fillOcclusion_(Mat& src, T invalidvalue)
//{
//	int bb = 1;
//	const int MAX_LENGTH = src.cols*0.5;
//#pragma omp parallel for // openMP를 이용한 병렬 처리
//	for (int j = bb; j < src.rows - bb; j++)
//	{
//		T* s = src.ptr<T>(j);
//		const T st = s[0];
//		const T ed = s[src.cols - 1];
//		s[0] = 255;
//		s[src.cols - 1] = 255;
//		for (int i = 0; i < src.cols; i++)
//		{
//			if (s[i] <= invalidvalue)
//			{
//				int t = i;
//				do
//				{
//					t++;
//					if (t > src.cols - 1)break;
//				} while (s[t] <= invalidvalue);
//
//				const T dd = min(s[i - 1], s[t]);
//				if (t - i > MAX_LENGTH)
//				{
//					for (int n = 0; n < src.cols; n++)
//					{
//						s[n] = invalidvalue;
//					}
//				}
//				else
//				{
//					for (; i < t; i++)
//					{
//						s[i] = dd;
//					}
//				}
//			}
//		}
//	}
//}
//void fillOcclusion(Mat& src, int invalidvalue, bool isInv = false)
//{	// 불규칙한 값을 조작하여 불규칙한 것을 최소화하게 다시 채움
//	if (isInv)
//	{
//		if (src.type() == CV_8U)
//		{
//			fillOcclusionInv_<uchar>(src, (uchar)invalidvalue);
//		}
//		else if (src.type() == CV_16S)
//		{
//			fillOcclusionInv_<short>(src, (short)invalidvalue);
//		}
//		else if (src.type() == CV_16U)
//		{
//			fillOcclusionInv_<unsigned short>(src, (unsigned short)invalidvalue);
//		}
//		else if (src.type() == CV_32F)
//		{
//			fillOcclusionInv_<float>(src, (float)invalidvalue);
//		}
//	}
//	else
//	{
//		if (src.type() == CV_8U)
//		{
//			fillOcclusion_<uchar>(src, (uchar)invalidvalue);
//		}
//		else if (src.type() == CV_16S)
//		{
//			fillOcclusion_<short>(src, (short)invalidvalue);
//		}
//		else if (src.type() == CV_16U)
//		{
//			fillOcclusion_<unsigned short>(src, (unsigned short)invalidvalue);
//		}
//		else if (src.type() == CV_32F)
//		{
//			fillOcclusion_<float>(src, (float)invalidvalue);
//		}
//	}
//}
//void eular2rot(double yaw, double pitch, double roll, Mat& dest)
//{
//	double theta = yaw / 180.0*CV_PI;
//	double pusai = pitch / 180.0*CV_PI;
//	double phi = roll / 180.0*CV_PI;
//
//	double datax[3][3] = { {1.0,0.0,0.0},
//	{0.0,cos(theta),-sin(theta)},
//	{0.0,sin(theta),cos(theta)} };
//	double datay[3][3] = { {cos(pusai),0.0,sin(pusai)},
//	{0.0,1.0,0.0},
//	{-sin(pusai),0.0,cos(pusai)} };
//	double dataz[3][3] = { {cos(phi),-sin(phi),0.0},
//	{sin(phi),cos(phi),0.0},
//	{0.0,0.0,1.0} };
//	Mat Rx(3, 3, CV_64F, datax);
//	Mat Ry(3, 3, CV_64F, datay);
//	Mat Rz(3, 3, CV_64F, dataz);
//	Mat rr = Rz * Rx*Ry;
//	rr.copyTo(dest);
//}
//
//void lookat(Point3d from, Point3d to, Mat& destR)
//{
//	double x = (to.x - from.x);
//	double y = (to.y - from.y);
//	double z = (to.z - from.z);
//
//	double pitch = asin(x / sqrt(x*x + z * z)) / CV_PI * 180.0;
//	double yaw = asin(-y / sqrt(y*y + z * z)) / CV_PI * 180.0;
//
//	eular2rot(yaw, pitch, 0, destR);
//}
//template <class T>
//static void projectImagefromXYZ_(Mat& image, Mat& destimage, Mat& disp, Mat& destdisp, Mat& xyz, Mat& R, Mat& t, Mat& K, Mat& dist, Mat& mask, bool isSub)
//{
//	if (destimage.empty())destimage = Mat::zeros(Size(image.size()), image.type());
//	if (destdisp.empty())destdisp = Mat::zeros(Size(image.size()), disp.type());
//
//	vector<Point2f> pt;
//	if (dist.empty()) dist = Mat::zeros(Size(5, 1), CV_32F);
//	cv::projectPoints(xyz, R, t, K, dist, pt);
//
//	destimage.setTo(0);
//	destdisp.setTo(0);
//
//#pragma omp parallel for
//	for (int j = 1; j < image.rows - 1; j++)
//	{
//		int count = j * image.cols;
//		uchar* img = image.ptr<uchar>(j);
//		uchar* m = mask.ptr<uchar>(j);
//		for (int i = 0; i < image.cols; i++, count++)
//		{
//			int x = (int)(pt[count].x + 0.5);
//			int y = (int)(pt[count].y + 0.5);
//			if (m[i] == 255)continue;
//			if (pt[count].x >= 1 && pt[count].x < image.cols - 1 && pt[count].y >= 1 && pt[count].y < image.rows - 1)
//			{
//				short v = destdisp.at<T>(y, x);
//				if (v < disp.at<T>(j, i))
//				{
//					destimage.at<uchar>(y, 3 * x + 0) = img[3 * i + 0];
//					destimage.at<uchar>(y, 3 * x + 1) = img[3 * i + 1];
//					destimage.at<uchar>(y, 3 * x + 2) = img[3 * i + 2];
//					destdisp.at<T>(y, x) = disp.at<T>(j, i);
//
//					if (isSub)
//					{
//						if ((int)pt[count + image.cols].y - y > 1 && (int)pt[count + 1].x - x > 1)
//						{
//							destimage.at<uchar>(y, 3 * x + 3) = img[3 * i + 0];
//							destimage.at<uchar>(y, 3 * x + 4) = img[3 * i + 1];
//							destimage.at<uchar>(y, 3 * x + 5) = img[3 * i + 2];
//
//							destimage.at<uchar>(y + 1, 3 * x + 0) = img[3 * i + 0];
//							destimage.at<uchar>(y + 1, 3 * x + 1) = img[3 * i + 1];
//							destimage.at<uchar>(y + 1, 3 * x + 2) = img[3 * i + 2];
//
//							destimage.at<uchar>(y + 1, 3 * x + 3) = img[3 * i + 0];
//							destimage.at<uchar>(y + 1, 3 * x + 4) = img[3 * i + 1];
//							destimage.at<uchar>(y + 1, 3 * x + 5) = img[3 * i + 2];
//
//							destdisp.at<T>(y, x + 1) = disp.at<T>(j, i);
//							destdisp.at<T>(y + 1, x) = disp.at<T>(j, i);
//							destdisp.at<T>(y + 1, x + 1) = disp.at<T>(j, i);
//						}
//						else if ((int)pt[count - image.cols].y - y < -1 && (int)pt[count - 1].x - x < -1)
//						{
//							destimage.at<uchar>(y, 3 * x - 3) = img[3 * i + 0];
//							destimage.at<uchar>(y, 3 * x - 2) = img[3 * i + 1];
//							destimage.at<uchar>(y, 3 * x - 1) = img[3 * i + 2];
//
//							destimage.at<uchar>(y - 1, 3 * x + 0) = img[3 * i + 0];
//							destimage.at<uchar>(y - 1, 3 * x + 1) = img[3 * i + 1];
//							destimage.at<uchar>(y - 1, 3 * x + 2) = img[3 * i + 2];
//
//							destimage.at<uchar>(y - 1, 3 * x - 3) = img[3 * i + 0];
//							destimage.at<uchar>(y - 1, 3 * x - 2) = img[3 * i + 1];
//							destimage.at<uchar>(y - 1, 3 * x - 1) = img[3 * i + 2];
//
//							destdisp.at<T>(y, x - 1) = disp.at<T>(j, i);
//							destdisp.at<T>(y - 1, x) = disp.at<T>(j, i);
//							destdisp.at<T>(y - 1, x - 1) = disp.at<T>(j, i);
//						}
//						else if ((int)pt[count + 1].x - x > 1)
//						{
//							destimage.at<uchar>(y, 3 * x + 3) = img[3 * i + 0];
//							destimage.at<uchar>(y, 3 * x + 4) = img[3 * i + 1];
//							destimage.at<uchar>(y, 3 * x + 5) = img[3 * i + 2];
//
//							destdisp.at<T>(y, x + 1) = disp.at<T>(j, i);
//						}
//						else if ((int)pt[count - 1].x - x < -1)
//						{
//							destimage.at<uchar>(y, 3 * x - 3) = img[3 * i + 0];
//							destimage.at<uchar>(y, 3 * x - 2) = img[3 * i + 1];
//							destimage.at<uchar>(y, 3 * x - 1) = img[3 * i + 2];
//
//							destdisp.at<T>(y, x - 1) = disp.at<T>(j, i);
//						}
//						else if ((int)pt[count + image.cols].y - y > 1)
//						{
//							destimage.at<uchar>(y + 1, 3 * x + 0) = img[3 * i + 0];
//							destimage.at<uchar>(y + 1, 3 * x + 1) = img[3 * i + 1];
//							destimage.at<uchar>(y + 1, 3 * x + 2) = img[3 * i + 2];
//
//							destdisp.at<T>(y + 1, x) = disp.at<T>(j, i);
//						}
//						else if ((int)pt[count - image.cols].y - y < -1)
//						{
//							destimage.at<uchar>(y - 1, 3 * x + 0) = img[3 * i + 0];
//							destimage.at<uchar>(y - 1, 3 * x + 1) = img[3 * i + 1];
//							destimage.at<uchar>(y - 1, 3 * x + 2) = img[3 * i + 2];
//
//							destdisp.at<T>(y - 1, x) = disp.at<T>(j, i);
//						}
//					}
//				}
//			}
//		}
//	}
//
//	if (isSub)
//	{
//		Mat image2;
//		Mat disp2;
//		destimage.copyTo(image2);
//		destdisp.copyTo(disp2);
//		const int BS = 1;
//#pragma omp parallel for
//		for (int j = BS; j < image.rows - BS; j++)
//		{
//			uchar* img = destimage.ptr<uchar>(j);
//			T* m = disp2.ptr<T>(j);
//			T* dp = destdisp.ptr<T>(j);
//			for (int i = BS; i < image.cols - BS; i++)
//			{
//				if (m[i] == 0)
//				{
//					int count = 0;
//					int d = 0;
//					int r = 0;
//					int g = 0;
//					int b = 0;
//					for (int l = -BS; l <= BS; l++)
//					{
//						T* dp2 = disp2.ptr<T>(j + l);
//						uchar* img2 = image2.ptr<uchar>(j + l);
//						for (int k = -BS; k <= BS; k++)
//						{
//							if (dp2[i + k] != 0)
//							{
//								count++;
//								d += dp2[i + k];
//								r += img2[3 * (i + k) + 0];
//								g += img2[3 * (i + k) + 1];
//								b += img2[3 * (i + k) + 2];
//							}
//						}
//					}
//					if (count != 0)
//					{
//						double div = 1.0 / count;
//						dp[i] = d * div;
//						img[3 * i + 0] = r * div;
//						img[3 * i + 1] = g * div;
//						img[3 * i + 2] = b * div;
//					}
//				}
//			}
//		}
//	}
//}
//void projectImagefromXYZ(Mat& image, Mat& destimage, Mat& disp, Mat& destdisp, Mat& xyz, Mat& R, Mat& t, Mat& K, Mat& dist, bool isSub = true, Mat& mask = Mat())
//{
//	if (mask.empty())mask = Mat::zeros(image.size(), CV_8U);
//	if (disp.type() == CV_8U)
//	{
//		projectImagefromXYZ_<unsigned char>(image, destimage, disp, destdisp, xyz, R, t, K, dist, mask, isSub);
//	}
//	else if (disp.type() == CV_16S)
//	{
//		projectImagefromXYZ_<short>(image, destimage, disp, destdisp, xyz, R, t, K, dist, mask, isSub);
//	}
//	else if (disp.type() == CV_16U)
//	{
//		projectImagefromXYZ_<unsigned short>(image, destimage, disp, destdisp, xyz, R, t, K, dist, mask, isSub);
//	}
//	else if (disp.type() == CV_32F)
//	{
//		projectImagefromXYZ_<float>(image, destimage, disp, destdisp, xyz, R, t, K, dist, mask, isSub);
//	}
//	else if (disp.type() == CV_64F)
//	{
//		projectImagefromXYZ_<double>(image, destimage, disp, destdisp, xyz, R, t, K, dist, mask, isSub);
//	}
//}
//Mat makeQMatrix(Point2d image_center, double focal_length, double baseline)
//{
//	Mat Q = Mat::eye(4, 4, CV_64F);
//	Q.at<double>(0, 3) = -image_center.x;
//	Q.at<double>(1, 3) = -image_center.y;
//	Q.at<double>(2, 3) = focal_length;
//	Q.at<double>(3, 3) = 0.0;
//	Q.at<double>(2, 2) = 0.0;
//	Q.at<double>(3, 2) = 1.0 / baseline;
//
//	return Q;
//}
//void stereoTest()
//{
//	//(1) Reading L&R images and estimating disparity map by semi-global block matching.
//	Mat image = imread("../image/l.png", 1); // 스테레오 이미지를 읽어드림
//	Mat imageR = imread("../image/r.png", 1);
//	Mat destimage;
//
//	/*int resize_k = 4; // 사진 크기가 크면 사이즈 바꾸기
//	resize(image, image, Size(image.size().width / resize_k, image.size().height / resize_k));
//	resize(imageR, imageR, Size(imageR.size().width / resize_k, imageR.size().height / resize_k));*/
//
//	StereoSGBM sgbm(1, 16 * 2, 3, 200, 255, 1, 0, 0, 0, 0, true); //SGBM으로 시차 화상을 계산함
//	Mat disp;
//	Mat destdisp;
//	Mat dispshow;
//	sgbm(image, imageR, disp); // 계산 결과의 시차는 최소 시차 -16의 값이 불규칙으로 할당됨
//	fillOcclusion(disp, 16); // 불규칙 값을 그럴싸한 값으로 재할당하는 함수
//
//	//(2)make Q matrix and reproject pixels into 3D space // 매트릭스의 구축과 시차 이미지의 3차원 공간에 투영
//	const double focal_length = 598.57;
//	const double baseline = 14.0;
//	// 3차원 공간에 투영하기 위해 Q 행렬을 사용 // 스테레오 카메라를 보정하여 Q 행렬을 얻기 위해 CV::streoRevtify에서 보완함
//	Mat Q = makeQMatrix(Point2d((image.cols - 1.0) / 2.0, (image.rows - 1.0) / 2.0), focal_length, baseline * 16);
//
//	Mat depth;
//	cv::reprojectImageTo3D(disp, depth, Q); // 시차를 3차원 공간에 투영함 
//	Mat xyz = depth.reshape(3, depth.size().area()); // 3채널의 float 배열에 저장됨
//
//	//(3) camera setting // 카메라 설정 // 행렬 설정
//	Mat K = Mat::eye(3, 3, CV_64F);
//	K.at<double>(0, 0) = focal_length;
//	K.at<double>(1, 1) = focal_length;
//	K.at<double>(0, 2) = (image.cols - 1.0) / 2.0;
//	K.at<double>(1, 2) = (image.rows - 1.0) / 2.0;
//
//	Mat dist = Mat::zeros(5, 1, CV_64F);
//	Mat R = Mat::eye(3, 3, CV_64F);
//	Mat t = Mat::zeros(3, 1, CV_64F);
//
//	Point3d viewpoint(0.0, 0.0, baseline * 10); // 관점
//	Point3d lookatpoint(0.0, 0.0, -baseline * 10.0); // 주시점
//	const double step = baseline;
//	int key = 0;
//	bool isSub = true;
//	//(4) rendering loop // 3차원 좌표에서 새로운 관점의 이미지에 점군을 투영
//	while (key != 27)
//	{
//		lookat(viewpoint, lookatpoint, R); // 가상 카메라의 방향이 업데이트됨
//		t.at<double>(0, 0) = viewpoint.x;
//		t.at<double>(1, 0) = viewpoint.y;
//		t.at<double>(2, 0) = viewpoint.z;
//
//		cout << t << endl;  // 좌표 출력
//		t = R * t;
//
//		//(5) projecting 3D point cloud to image.
//		projectImagefromXYZ(image, destimage, disp, destdisp, xyz, R, t, K, dist, isSub);
//
//		destdisp.convertTo(dispshow, CV_8U, 0.5);
//		imshow("depth - 김난희", dispshow);
//		imshow("image - 김난희", destimage);
//
//		// 위치 방향 컨트롤
//		if (key == 'f') // 양자화 억제 필터의 온 오프 전환
//		{				// default 값은 ON
//			isSub = isSub ? false : true;
//		}
//		if (key == 'a') // 아래로
//		{
//			viewpoint.y += step;
//		}
//		if (key == 's') // 위로
//		{
//			viewpoint.y -= step;
//		}
//		if (key == 'z') //오른쪽으로 
//		{
//			viewpoint.x += step;
//		}
//		if (key == 'x') //왼쪽으로
//		{
//			viewpoint.x -= step;
//		}
//		if (key == 'q') // 멀리
//		{
//			viewpoint.z += step;
//		}
//		if (key == 'w') // 가까이 
//		{
//			viewpoint.z -= step;
//		}
//		key = waitKey(1); // 실시간으로 key 값을 출력하기 위해 1이 들어감
//						  // 0은 바뀔 때마다 키값을 출력하도록 함
//	}
//}
////* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
////* * * * * * * * * * for lab2 - Render the disparity in 3D space (point cloud) * * * * * * * * * * poing cloud를 사용하려면 여기까지 주석 끝 * * * * * * * * * * //
////* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//* * * * * * * * * * for lab2 - Texture mapping on the 3D shape * * * * * * * * * * Texture mapping을 사용하려면 여기서부터 주석해제 * * * * * * * * * * * * * * //
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

///openGL 관련
#include <windows.h>
#include <glut.h>

///openCV 관련
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <strstream>
#include <functional>
#include <stdio.h>

using namespace cv;

GLuint TextureIdx[1]; //the array for our TextureIdx
Mat mSource_Bgr;	  //texture mapping할 image
GLfloat angle = 45.0; //original source의 cube 각도

/// keyboard로 3d 큐브 움직이기
// (1) 큐브 위치
float cubeX = 0.0;
float cubeY = 0.0;
float cubeZ = -4.0;
// (2) 회전
float pitch = 0.0;
float yaw = 0.0;
float roll = 0.0;


int UploadTexture(Mat image, GLuint &TextureIdx) ///The my Image to OpenGl TextureIdx function
{
	if (image.empty())
		return -1;
	glGenTextures(1, &TextureIdx);
	glBindTexture(GL_TEXTURE_2D, TextureIdx); //bind the TextureIdx to it's array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.data);
	return 0;
}
void FreeTexture(GLuint TextureIdx) ///texture mapping 후 동적할당 해제와 같음
{
	glDeleteTextures(1, &TextureIdx);
}
void special(int key, int x, int y) // 방향키에 의해 회전을 입력
{
	if (key == GLUT_KEY_UP)
	{
		pitch += 1.0;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		pitch -= 1.0;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		yaw += 1.0;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		yaw -= 1.0;
	}
}
void keyboard(unsigned char key, int x, int y) // 상하 좌우 가깝게멀리 입력 //* 주의할 점: 대문자 영문은 안됨
{
	//cout << "다음 키가 눌러졌습니다. \"" << key << "\" ASCII: " << (int)key << endl;

	//ESC 키가 눌러졌다면 프로그램 종료
	if (key == 27)
	{
		exit(0);
	}
	else if (key == 43) // 방향키 +키
	{
		roll += 1.0;
	}
	else if (key == 45) // 방향키 -키
	{
		roll -= 1.0;
	}
	else if (key == 'q') // q key - 크게, 가깝게
	{
		cubeZ += 0.1;
	}
	else if (key == 'w') // w key - 작게 ,멀리
	{
		cubeZ -= 0.1;
	}
	else if (key == 'a') // a key - 위로
	{
		cubeY += 0.1;
	}
	else if (key == 's') // s key - 아래로
	{
		cubeY -= 0.1;
	}
	else if (key == 'z') // z key - 오른쪽으로
	{
		cubeX += 0.1;
	}
	else if (key == 'x') // x key - 왼쪽으로
	{
		cubeX -= 0.1;
	}
}
void plane(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
	glLoadIdentity();                                   // Reset The View

	//아래 두 줄은 original source, 회전 불가 //angle에 의한 변화는 관찰 가능
	//glTranslatef(0.0f, .0f, -5.0f);
	//glRotatef(angle, 1.0f, 1.0f, 0.0f);

	// 이동과 회전을 적용
	glTranslatef(cubeX, cubeY, cubeZ);
	glRotatef(pitch, 1.0, 0.0, 0.0);  //x축에 대해 회전
	glRotatef(yaw, 0.0, 1.0, 0.0);    //y축에 대해 회전
	glRotatef(roll, 0.0, 0.0, 1.0);   //z축에 대해 회전

	glBindTexture(GL_TEXTURE_2D, TextureIdx[0]); //unbind the TextureIdx

	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0); //unbind the TextureIdx
}
void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //화면을 지운다. (컬러버퍼와 깊이버퍼)
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	plane(); //큐브를 그림
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //윈도우 크기로 뷰포인트 설정
	glMatrixMode(GL_PROJECTION);			  //이후 연산은 Projection Matrix에 영향을 줌. 카메라로 보이는 장면 설정
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);  //Field of view angle(단위 degrees), 윈도우의 aspect ratio, Near와 Far Plane 설정
	glMatrixMode(GL_MODELVIEW);								  //이후 연산은 ModelView Matirx에 영향을 줌. 객체 조작
}
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
//* * * * * * * * * * for lab2 - Texture mapping on the 3D shape * * * * * * * * * * // Texture mapping을 사용하려면 여기까지 주석 끝 * * * * * * * * * * //
//* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


int main(int argc, char** argv)
{
	////* * * * * * * * * * lab 1. block matching based disparity * * * * * * * * * *//
	////load stereo images -> 영상 2장
	//Mat l = imread("../image/im2.ppm"); // left  영상 
	//Mat r = imread("../image/im6.ppm"); // right 영상 right_un.png

	////image resize because big size
	////resize(l, l, size(l.size().width / 4, l.size().height / 4));
	////resize(r, r, size(r.size().width / 4, r.size().height / 4));

	//imshow("Left image - 김난희", l);
	//imshow("Right image - 김난희", r);

	//// set block matching parameters // 파라미터 세팅
	//CvStereoBMState *BMState = cvCreateStereoBMState(); // 클래스를 이용해 객체 만듦 -> 함수가 주소 return한 것을 받음
	//BMState->preFilterSize = 5;  // brightness correction, 5x5 ~ 21x21 // 객체는 ., pointer는 그 주소로 가서 화살표로 함
	//BMState->preFilterCap = 5;	// removed region after prefilter
	//BMState->SADWindowSize = 17; // sad: sum of absolute difference, window size (5x5.... 21x21) // 윈도우 사이즈 11(support 영역) 좀 크게함
	////support 영역 11->5 모호성이 커져서 오히려 잘 안나옴 // 25는 두루뭉실하지만(정밀도 떨어짐) matching은 더 잘함
	//BMState->minDisparity = 1;   // minimum disparity [pixel] // 찾고자하는 minimum 값
	//BMState->numberOfDisparities = 32; // maximum disparity [pixel] // 1~32까지 pixel 봄
	//BMState->textureThreshold = 10;  // minimum allowed
	//BMState->uniquenessRatio = 5;    // uniqueness  (removing false matching) 

	//// convert color space
	//Mat Left, Right;
	//cvtColor(l, Left, CV_RGB2GRAY); // gray로 변환
	//cvtColor(r, Right, CV_RGB2GRAY);

	//// type conversion: mat to iplimage
	//IplImage *left, *right;
	//left = &IplImage(Left);
	//right = &IplImage(Right);

	//CvMat* disparity = cvCreateMat(Left.rows, Left.cols, CV_16SC1);
	//CvMat* disparity_img = cvCreateMat(Left.rows, Left.cols, CV_8UC1); // 1~32 -> 0~255 밝기를 눈으로 볼 수 있게 바꿔줌

	//// run algorithm
	//cvFindStereoCorrespondenceBM(left, right, disparity, BMState);
	//cvNormalize(disparity, disparity_img, 0, 255, CV_MINMAX);  //normalize to display

	//// show results // stereo matching show
	//cvShowImage("disparity - 김난희", disparity_img);

	//waitKey(0);

	//* * * * * * * * * * lab 2. 3d rendering * * * * * * * * * *//
	///* * * * * (1) render the disparity in 3d space (point cloud)
	// sample of stereo matching and 3d point cloud rendering
	//stereoTest();

	/////* * * * * (2) Texture mapping on the 3D shape with openGL
	glutInit(&argc, argv);						    // GLUT 초기화
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);   // 더블 버퍼와 깊이 버퍼를 사용하도록 설정, GLUT_RGB=0x00임
	glutInitWindowSize(700, 700);				    // 윈도우의 width와 height
	glutInitWindowPosition(100, 100);			    // 윈도우의 위치 (x,y)

	glutCreateWindow("Texture mapping on the 3D shape - OpenGL Window - 김난희"); // 윈도우 생성
	glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);           // Black Background
	glClearDepth(1.0f);                             // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

	glutDisplayFunc(display); // 디스플레이 콜백 함수 등록
							  // display함수는 윈도우 처음 생성할 때와 화면 다시 그릴 필요 있을때 호출됨
	glutIdleFunc(display);

	glutReshapeFunc(reshape); // reshape 콜백 함수 등록
							  // reshape함수는 윈도우 처음 생성할 때와 윈도우 크기 변경시 호출된다.

	mSource_Bgr = imread("../image/earth.png", 1);  //The load my image to opengl TextureIdx

	UploadTexture(mSource_Bgr, TextureIdx[0]); // 직육면체에 그려질 사진을 load texture

	//키보드 콜백 함수 등록, 키보드가 눌러지면 호출된다. 
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	// GLUT event processing loop에 진입함
	// 이 함수는 리턴되지 않기 때문에 다음줄에 있는 코드가 실행되지 않음 
	glutMainLoop();

	// ESC Key를 눌러나감 // 실제로는 사용하지 않는 아래 2줄 //동적할당 해제와 같은 역할
	FreeTexture(TextureIdx[0]);           //Free our TextureIdx
	return 0;
}