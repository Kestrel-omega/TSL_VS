#include "image_processing.hpp"


//int gTh_1hys, gTh_2hys;

/**
// rgb 색영역 영상 배열을 이진 영상 배열으로 변환한 뒤 윤곽선을 검출하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb
// output arg: Mat 타입의 윤곽선이 검출된 이진 영상 배열 frame_bin
**/
Mat convert_rgb2canny(Mat frame_rgb)
{
	Mat frame1, frame2; // 변환된 이진 영상 배열
	cvtColor(frame_rgb, frame1, COLOR_BGR2GRAY);
	//blur(frame_bin, frame_bin, Size(3, 3));
	//Canny(frame_bin, frame_bin, 128, 255);

	blur(frame1, frame2, Size(3,3));
	//imshow("Blurred", frame2);
	//Canny(frame2, frame1, gTh_1hys, gTh_2hys);
	Canny(frame2, frame1, 128, 255);
	return frame1;
}

/**
// 검출된 외곽선 이진 배열에서 사용 가능한 arrayofarrayofPoint형태의 윤곽선 정보를 추출하는 함수
// input arg: Mat 타입의 Mat 타입의 윤곽선이 검출된 이진 영상 배열 frame_bin
// output arg: vector<vector<Point>> 타입의 윤곽선 정보 feature
**/
vector<vector<Point>> extract_contour(Mat frame_bin)
{
	vector<vector<Point>> contour; // 물체의 외곽선 정보
	findContours(frame_bin, contour, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	return contour;
}

/**
// contour_info의 정보를 dos창에 출력하는 함수
// input arg: 구조체 타입의 윤곽선 정보 contour_info, 문자열 str
// output arg: 
**/
void print_contour_info(vector<contour_feature> contour_info, const char* str)
{
	cout << str << endl;
	for (int i=0;i < contour_info.size();i++)
	{
		cout << "contour " << i << ", color : " << contour_info[i].color << ", COM : "<< contour_info[i].COM << ", head : " << contour_info[i].head << ", ID : " << contour_info[i].ID << endl;
	}
}

/**
// 추출된 윤곽선과 가장 근접한 타원을 설정한 뒤 여러 정보를 추출하는 함수
// input arg: vector<vector<Point>> 타입의 윤곽선 정보 contour
// output arg:
extract of ellipse infomations form contour (COM, angle, major/minor axis, both ends, color)
**/
vector<contour_feature> extract_ellipse(vector<vector<Point>> contour, Mat frame_rgb)
{
	vector<contour_feature> contour_info(contour.size());
	int temp_ID = 0;
	for (int i = 0; i < contour.size(); i++)
	{
		// if size of contour is too small, ignore it
		if (contour[i].size() < MIN_SIZE_CONTOUR) continue;
		// attach temporary ID to contour_info structure

		// fit ellipse from contour
		// printf("aaa\n");		
		// printf("bbb\n");
		RotatedRect ellipse_contour = fitEllipse(contour[i]);
		//printf("ccc\n");

		// get COM, Angle, Major Axis, Minor Axis from ellipse
		Point COM_contour = ellipse_contour.center;
		float angle_contour = ellipse_contour.angle / 180 * M_PI;
		float major_contour = ellipse_contour.size.width;
		float minor_contour = ellipse_contour.size.height;
		Point end1_contour, end2_contour;

		// extract both end from ellipse
		if (abs(major_contour - minor_contour) < 3)
		{
			end1_contour = COM_contour;
			end2_contour = COM_contour;
		}
		else
		{
			float coefficient = 1;
			end1_contour.x = COM_contour.x + major_contour * sin(-angle_contour) * coefficient;
			end1_contour.y = COM_contour.y + major_contour * cos(-angle_contour) * coefficient;
			end2_contour.x = COM_contour.x - major_contour * sin(-angle_contour) * coefficient;
			end2_contour.y = COM_contour.y - major_contour * cos(-angle_contour) * coefficient;
		}

		Mat ROI = frame_rgb(Rect(end1_contour, end2_contour));
		Scalar color_contour = mean(ROI);
		// cout << "Color of contour " << i << " : " << color_contour << endl;
		// apply infomations to contour_info structure
		contour_info[temp_ID].COM = COM_contour;
		// cout << "COM of contour " << i << " : " << COM_contour << endl;
		contour_info[temp_ID].angle = angle_contour;
		contour_info[temp_ID].major = major_contour;
		contour_info[temp_ID].minor = minor_contour;
		contour_info[temp_ID].head = end1_contour;
		contour_info[temp_ID].tail = end2_contour;
		contour_info[temp_ID].color = color_contour;

		temp_ID++;
	}

	contour_info.resize(temp_ID);
	// return vector of contour_info structure
	return contour_info;
}

// find mean color of contour


/**
// 과거의 contour_feature 정보를 바탕으로 현재 contour_feature 정보 레이블링
// input arg: vector<contour_feature> 구조체 타입의 과거 윤곽선 정보 past_contour_info, 현재 윤곽선 정보 contour_info, float 타입 시간 간격 time_interval, 타원 고려 방법, 거리 계산 방법
// output arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 contour_info
// 1:1 correspondance with laser pointer ID
**/
vector<contour_feature> label_ellipse(vector<contour_feature> past_contour_info, vector<contour_feature> contour_info, float time_inteval, int method_state, int dist_calc_method)
{
	size_t size_contour = contour_info.size();
	// cout << size_contour << endl;
	vector<vector<int>> dist(size_contour, vector<int>(size_contour, 0));
	vector<int> dist_end1(size_contour);
	vector<int> dist_end2(size_contour);

	// if (number of past contour != number of contour) ignore it
	if (past_contour_info.size() != size_contour) {
		printf("contour_info_k size != contour_infor_km1 size\n");
		/*while (1) {
			if (waitKey(5) >= 0) exit(-1);
		}
		return contour_info;*/
	}

	cout << endl;
	// step 1) update ellipse state
	vector<Point> past_contour_COM(size_contour);
	vector<Point> contour_end1(size_contour);
	vector<Point> contour_end2(size_contour);
	for (size_t k = 0; k < size_contour; k++)
	{
		contour_feature contour = contour_info[k];
		contour_feature past_contour = past_contour_info[k];
		switch (method_state)
		{
			// 1 : movement speed of ellipse is not considered (Assume the COM has not changed)
		case NONE:
		{
			past_contour_COM[k] = past_contour.COM;
			break;
		}

		// 2 : considering the movement speed of ellipse (x_k = x_k-1 + T*v_k-1)
		case VELOCITY:
		{
			Point velocity = { 0, };
			velocity.x = contour.COM.x - past_contour.COM.x;
			velocity.y = contour.COM.y - past_contour.COM.y;
			past_contour_COM[k] = past_contour.COM + time_inteval * velocity;
			break;
		}

		// 3 : considering the probability and movement speed of ellipse 
		case PROBABLITY:
		{
			break;
		}
		}
		// step 2) compute norm of distances
		int min_dist = 2147483647;
		int min_dist_position = 0;

		for (int j = 0;j < size_contour;j++)
		{
			switch (dist_calc_method)
			{	
				// 1 : calculate distance with euclidian method
			case EUCLIDIAN:
			{
				contour.COM = contour_info[j].COM;
				dist[k][j] = pow((contour.COM.x - past_contour.COM.x), 2) + pow((contour.COM.y - past_contour.COM.y), 2);
				break;
			}

			// 2: calculate distance with taxi method
			case MANHATTAN:
			{
				contour.COM = contour_info[j].COM;
				dist[k][j] = abs(contour.COM.x - past_contour.COM.x) + abs(contour.COM.y - past_contour.COM.y);
				break;
			}
			}
			printf("dist[%d][%d] = %d\n", k, j, (int)dist[k][j]);
			// step 3) select the ID that minimizes the distance
			if (min_dist > dist[k][j])
			{
				min_dist = dist[k][j];
				min_dist_position = j;
				// printf("min_dist = dist[%d][%d] = %d\n", k, j, (int)min_dist);
			}
			else if (min_dist == dist[k][j])
			{
				float past_angle = past_contour.angle;
				float angle1 = contour_info[j].angle;
				float angle2 = contour_info[min_dist_position].angle;
				if (past_angle - angle2 > past_angle - angle1)
				{
					min_dist = dist[k][j];
					min_dist_position = j;
				}
			}
		}
		printf("closest : %d-%d = %d\n", k, min_dist_position, (int)dist[k][min_dist_position]);
		// set selected ID to contour_info structure
		contour_info[min_dist_position].ID = k;

		// select head of ellipse
		switch (dist_calc_method)
		{
			// 1 : calculate distance with euclidian method
		case EUCLIDIAN:
		{
			dist_end1[k] = pow((contour.head.x - past_contour.COM.x), 2) + pow((contour.head.y - past_contour.COM.y), 2);
			dist_end2[k] = pow((contour.tail.x - past_contour.COM.x), 2) + pow((contour.tail.y - past_contour.COM.y), 2);

			break;
		}

		// 2: calculate distance with taxi method
		case MANHATTAN:
		{
			dist_end1[k] = abs(contour.head.x - past_contour.COM.x) + abs(contour.head.y - past_contour.COM.y);
			dist_end2[k] = abs(contour.tail.x - past_contour.COM.x) + abs(contour.tail.y - past_contour.COM.y);
			break;
		}
		}

		// head is far point between past_COM and end of ellipse
		if (dist_end1[k] < dist_end2[k])
		{
			swap(contour_info[k].head, contour_info[k].tail);
			// printf("Contour %d swapped!\n", k);
		}

	}
	return contour_info;
}
/**
// ID 순서대로 contour_feature 정보를 재정렬하는 함수
// input arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 contour_info
// output arg: vector<contour_feature> 구조체 타입의 윤곽선 정보 aligned_contour_info
// align contour_info structure with the ID order
// alert!
**/
vector<contour_feature> align_contour_info(vector<contour_feature> contour_info)
{
	size_t info_size = contour_info.size();
	vector<contour_feature> aligned_contour_info(info_size);
	for (size_t i = 0;i < info_size;i++)
	{
		int temp_ID = 0;
		for (size_t j = 0;j < info_size;j++)
		{
			if (contour_info[j].ID == i)
			{
				aligned_contour_info[i].COM = contour_info[j].COM;
				aligned_contour_info[i].angle = contour_info[j].angle;
				aligned_contour_info[i].major = contour_info[j].major;
				aligned_contour_info[i].minor = contour_info[j].minor;
				aligned_contour_info[i].head = contour_info[j].head;
				aligned_contour_info[i].tail = contour_info[j].tail;
				aligned_contour_info[i].color = contour_info[j].color;
				aligned_contour_info[i].ID = i;
			}

			
			//cout << "ID : " << i << ", " << aligned_contour_info[i].ID << ", Head : " << aligned_contour_info[i].head << endl;
		}
	}

	return aligned_contour_info;
}

/**
// 현재 출력되는 영상에 레이저 포인터의 정보를 덧씌워서 출력하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb, contour_feature 구조체 배열 타입의 윤곽선 정보 data
// output arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb
**/
Mat monitoring(Mat frame_rgb, vector<contour_feature> data)
{
	char str[100];
	for (size_t i = 0;i < data.size();i++)
	{
		circle(frame_rgb, data[i].head, 3, Scalar{ 255, 255, 255, 0 } - data[i].color, -1);
		sprintf(str, "ID : %d", (int)i);
		putText(frame_rgb, str, data[i].COM, FONT_HERSHEY_COMPLEX, 0.5, data[i].color);
	}
	return frame_rgb;
}

/**
// 현재 출력되는 영상의 화면을 프레임 단위로 jpeg 파일로 해당 폴더에 저장하는 함수
// input arg: Mat 타입의 rgb 색영역 영상 배열 frame_rgb, int 타입의 프레임 카운터, char 타입의 영상 정보
// output arg: rgb 색영역의 jpeg 그림 파일
**/
void save_frame2jpeg(Mat frame_rgb, int count, char frame_symbol)
{
	String fileName = format("frame/frame_%c_%d.jpg", frame_symbol, count);
	imwrite(fileName, frame_rgb);
}
