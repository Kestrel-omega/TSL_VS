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
// �������� ���� ������ Ÿ���� ���� ������ ��� ����ü
**/
typedef struct _contour_feature
{
	Point COM; // Ÿ���� �����߽�
	float angle; // Ÿ���� ����
	float major; // Ÿ���� ����
	float minor; // Ÿ���� ����
	Point head; // Ÿ���� ���� ���� ���κ�
	Point tail; // Ÿ���� ���� �ݴ� ���� ���κ�
	Scalar color; // Ÿ���� ��� ��
	int ID; // Ÿ���� ID
}contour_feature;

/**
// rgb ������ ���� �迭�� ���� ���� �迭���� ��ȯ�� �� �������� �����ϴ� �Լ�
// input arg: Mat Ÿ���� rgb ������ ���� �迭 frame_rgb
// output arg: Mat Ÿ���� �������� ����� ���� ���� �迭 frame_bin
**/
Mat convert_rgb2canny(Mat frame_rgb);

/**
// ����� �ܰ��� ���� �迭���� ��� ������ ������ ������ �����ϴ� �Լ�
// input arg: Mat Ÿ���� Mat Ÿ���� �������� ����� ���� ���� �迭 frame_bin
// output arg: vector<vector<Point>> Ÿ���� ������ ���� contour
**/
vector<vector<Point>> extract_contour(Mat frame_bin);

/**
// contour_info�� ������ dosâ�� ����ϴ� �Լ�
// input arg: Mat Ÿ���� Mat Ÿ���� �������� ����� ���� ���� �迭 frame_bin
// output arg:
**/
void print_contour_info(vector<contour_feature> contour_info, const char* str);

/**
// ����� �������� ���� ������ Ÿ���� ������ �� ���� ������ �����ϴ� �Լ�
// input arg: vector<vector<Point>> Ÿ���� ������ ���� contour
// output arg: 
extract of ellipse infomations form contour (COM, angle, major/minor axis, both ends, color)
**/
vector<contour_feature> extract_ellipse(vector<vector<Point>> contour, Mat frame_rgb);

/**
// ������ contour_feature ������ �������� ���� contour_feature ���� ���̺��ϴ� �Լ�
// input arg: vector<contour_feature> ����ü Ÿ���� ���� ������ ���� past_contour_info, ���� ������ ���� contour_info, float Ÿ�� �ð� ���� time_interval, Ÿ�� ��� ���, �Ÿ� ��� ���
// output arg: vector<contour_feature> ����ü Ÿ���� ������ ���� contour_info
// 1:1 correspondance with laser pointer ID
**/
vector<contour_feature> label_ellipse(vector<contour_feature> past_contour_info, vector<contour_feature> contour_info, float time_inteval, int method_state, int dist_calc_method);

/**
// ID ������� contour_feature ������ �������ϴ� �Լ�
// input arg: vector<contour_feature> ����ü Ÿ���� ������ ���� contour_info
// output arg: vector<contour_feature> ����ü Ÿ���� ������ ���� aligned_contour_info
// align contour_info structure with the ID order
**/
vector<contour_feature> align_contour_info(vector<contour_feature> contour_info);

/**
// ���� ��µǴ� ���� ������ �������� ������ �������� ����ϴ� �Լ�
// input arg: Mat Ÿ���� rgb ������ ���� �迭 frame_rgb, contour_feature ����ü �迭 Ÿ���� ������ ���� data
// output arg: Mat Ÿ���� rgb ������ ���� �迭 frame_rgb
**/
Mat monitoring(Mat frame_rgb, vector<contour_feature> data);

/**
// ���� ��µǴ� ������ ȭ���� ������ ������ jpeg ���Ϸ� �ش� ������ �����ϴ� �Լ�
// input arg: Mat Ÿ���� rgb ������ ���� �迭 frame_rgb, int Ÿ���� ������ ī����, char Ÿ���� ���� ����
// output arg: rgb �������� jpeg �׸� ����
**/
void save_frame2jpeg(Mat frame_rgb, int count, char frame_symbol);

#endif
