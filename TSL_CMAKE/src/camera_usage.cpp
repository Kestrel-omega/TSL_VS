#include "camera_usage.hpp"


extern VideoCapture cap;

/**
// ������ ����ϵ��� �����ϴ� �Լ�
// input arg: Test ���� �̸�, ��� ���� (TEST / JETSON)
// output arg: VideoCapture Ÿ���� ���� ��� ����
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
// ���� ���� �ִ� ���� �������� ������, ī�޶� ����� �����ϵ��� �����ϴ� �Լ�
// input arg: int Ÿ������ ���ǵ� ��ġ ����
// output arg: string Ÿ���� gstreamer���� ����� pipeline ����
**/
void close_camera_frame(VideoCapture cap)
{
	cap.release();
	destroyAllWindows();
}