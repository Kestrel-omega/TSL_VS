#ifndef __CAMERA_USAGE_HPP__
#define __CAMERA_USAGE_HPP__

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

// Device 설정
#define TEST 0
#define JETSON 1

using namespace cv;
using namespace std;

// 영상이 성공적으로 준비되었으면 1, 아니면 0
//int gis_video_prepared = 0;

/**
// 영상을 사용하도록 설정하는 함수
// input arg: Test 영상 이름, 기기 설정 (TEST / JETSON)
// output arg: VideoCapture 타입의 영상 사용 설정
**/
void init_camera_frame(int device);

/**
// 현재 열려 있는 비디오 프레임을 닫으며, 카메라 사용을 중지하도록 정리하는 함수
// input arg: VideoCapture 타입의 영상 정보
// output arg: 
**/
void close_camera_frame(VideoCapture cap);
#endif