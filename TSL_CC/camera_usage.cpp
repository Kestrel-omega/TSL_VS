#include "camera_usage.hpp"


extern VideoCapture cap;

/**
// 영상을 사용하도록 설정하는 함수
// input arg: Test 영상 이름, 기기 설정 (TEST / JETSON)
// output arg: VideoCapture 타입의 영상 사용 설정
**/
void init_camera_frame(int device)
{
	cin.tie(0);
	ios::sync_with_stdio(false);

	switch (device)
	{
	case TEST:
	{
		cout << "Test File Name ? : ";
		string file_name;
		cin >> file_name;
		cap = VideoCapture(file_name, CAP_ANY);
		break;
	}
	case JETSON:
	{
		int capture_width = 1920;
		int capture_height = 1080;
		int flip_method = 2;
		int display_width = capture_width / 2;
		int display_height = capture_height / 2;
		int framerate;

		cin >> framerate;

		string pipeline = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
			to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
			"/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
			to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";

		cap = VideoCapture(pipeline, CAP_GSTREAMER);
		cout << "Using pipeline: \r\n" << pipeline << "\r\n\r\n";
		break;
	}
	}
	
	if (!cap.isOpened())
	{
		cout << "Sys : Failed to open camera.\n";
		abort();
	}
	else cout << "Sys : Camera is successfully opened.\n";

	cout << cap.get(CAP_PROP_FPS) << "FPS\n";
	//gis_video_prepared = 1;
}

/**
// 현재 열려 있는 비디오 프레임을 닫으며, 카메라 사용을 중지하도록 정리하는 함수
// input arg: int 타입으로 정의된 장치 정보
// output arg: string 타입의 gstreamer에서 사용할 pipeline 정보
**/
void close_camera_frame(VideoCapture cap)
{
	cap.release();
	destroyAllWindows();
}