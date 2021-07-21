#ifndef __IMAGE_PROCESSING_HPP__
#define __IMAGE_PROCESSING_HPP__

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <queue>

// pixel
#define MIN_SIZE_CONTOUR 8 // minimum 5

// define method of state
#define NONE 0
#define VELOCITY 1
#define PROBABLITY 2

// define calculating method of distance
#define EUCLIDIAN 0
#define MANHATTAN 1

using namespace cv;
using namespace std;

/**
// 윤곽선과 가장 근접한 타원의 여러 정보를 담는 구조체
**/
typedef struct _contour_feature
{
	Point COM; // 타원의 무게중심
	float angle; // 타원의 각도
	float major; // 타원의 장축
	float minor; // 타원의 단축
	Point head; // 타원의 진행 방향 끝부분
	Point tail; // 타원의 진행 반대 방향 끝부분
	Scalar color; // 타원의 평균 색
	int ID; // 타원의 ID
}contour_feature;

/**
// rgb 색영역 영상 배열을 이진 영상 배열으로 변환한 뒤 윤곽선을 검출하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb
// output arg: Mat 타입의 윤곽선이 검출된 이진 영상 배열 frame_bin
**/
Mat convert_rgb2canny(Mat frame_rgb);

/**
// 검출된 외곽선 이진 배열에서 사용 가능한 윤곽선 정보를 추출하는 함수
// input arg: Mat 타입의 Mat 타입의 윤곽선이 검출된 이진 영상 배열 frame_bin
// output arg: vector<vector<Point>> 타입의 윤곽선 정보 contour
**/
vector<vector<Point>> extract_contour(Mat frame_bin);

/**
// contour_info의 정보를 dos창에 출력하는 함수
// input arg: Mat 타입의 Mat 타입의 윤곽선이 검출된 이진 영상 배열 frame_bin
// output arg:
**/
void print_contour_info(vector<contour_feature> contour_info, const char* str);

/**
// 추출된 윤곽선과 가장 근접한 타원을 설정한 뒤 여러 정보를 추출하는 함수
// input arg: vector<vector<Point>> 타입의 윤곽선 정보 contour
// output arg: 
extract of ellipse infomations form contour (COM, angle, major/minor axis, both ends, color)
**/
vector<contour_feature> extract_ellipse(vector<vector<Point>> contour, Mat frame_rgb);

/**
// 과거의 contour_feature 정보를 바탕으로 현재 contour_feature 정보 레이블링하는 함수
// input arg: vector<contour_feature> 구조체 타입의 과거 윤곽선 정보 past_contour_info, 현재 윤곽선 정보 contour_info, float 타입 시간 간격 time_interval, 타원 고려 방법, 거리 계산 방법
// output arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 contour_info
// 1:1 correspondance with laser pointer ID
**/
vector<contour_feature> label_ellipse(vector<contour_feature> past_contour_info, vector<contour_feature> contour_info, float time_inteval, int method_state, int dist_calc_method);

/**
// ID 순서대로 contour_feature 정보를 재정렬하는 함수
// input arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 contour_info
// output arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 aligned_contour_info
// align contour_info structure with the ID order
**/
vector<contour_feature> align_contour_info(vector<contour_feature> contour_info);

/**
// 현재 출력되는 영상에 레이저 포인터의 정보를 덧씌워서 출력하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb, contour_feature 구조체 배열 타입의 윤곽선 정보 data
// output arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb
**/
Mat monitoring(Mat frame_rgb, vector<contour_feature> data);

/**
// 현재 출력되는 영상의 화면을 프레임 단위로 jpeg 파일로 해당 폴더에 저장하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb, int 타입의 프레임 카운터, char 타입의 영상 정보
// output arg: rgb 색영역의 jpeg 그림 파일
**/
void save_frame2jpeg(Mat frame_rgb, int count, char frame_symbol);

#endif
