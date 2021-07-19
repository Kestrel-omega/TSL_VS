#ifndef __CAMERA_USAGE_HPP__
#define __CAMERA_USAGE_HPP__

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

// Device ����
#define TEST 0
#define JETSON 1

using namespace cv;
using namespace std;

// ������ ���������� �غ�Ǿ����� 1, �ƴϸ� 0
//int gis_video_prepared = 0;

/**
// ������ ����ϵ��� �����ϴ� �Լ�
// input arg: Test ���� �̸�, ��� ���� (TEST / JETSON)
// output arg: VideoCapture Ÿ���� ���� ��� ����
**/
void init_camera_frame(int device);

/**
// ���� ���� �ִ� ���� �������� ������, ī�޶� ����� �����ϵ��� �����ϴ� �Լ�
// input arg: VideoCapture Ÿ���� ���� ����
// output arg: 
**/
void close_camera_frame(VideoCapture cap);
#endif