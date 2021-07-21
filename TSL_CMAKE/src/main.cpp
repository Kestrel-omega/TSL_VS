#include "camera_usage.hpp"
#include "image_processing.hpp"

using namespace cv;
using namespace std;

VideoCapture cap;

vector<contour_feature> past_contour_info;
vector<vector<Point>> contour_km1;

int main() 
{
	init_camera_frame(TEST);
	

	Mat frame_rgb;
	int dummy = 0;
	int idx = 0;

	while (1)
	{
		idx++;
		printf("Frame : %d\n", idx);
		cap >> frame_rgb;
		if (frame_rgb.empty())
		{
			printf("Sys : Empty image");
			return 0;
		}
		Mat frame_bin = convert_rgb2canny(frame_rgb);
		//imshow("Binary", frame_bin);
		vector<vector<Point>> contour = extract_contour(frame_bin);
		if (contour.size() < 3) continue;
		/*if (contour.size() < 3) {

			printf("idx:%d \n", idx);
			printf("contour_km1_size: %d\n", (int)contour_km1.size());
			printf("contour_k_size: %d\n", (int)contour.size());

			while (1) {
				if (waitKey(5) >= 0) exit(-1);
			}
		}*/

		drawContours(frame_rgb, contour, -1, Scalar(0, 0, 0), 1);
		vector<contour_feature>contour_info = extract_ellipse(contour, frame_rgb);
		 //if (contour_info[0].ID != 0 || contour_info[1].ID != 1 || contour_info[2].ID != 2)
		 //{
			//printf("%d %d %d \r\n", contour_info[0].ID, contour_info[1].ID, contour_info[2].ID);
		 //system("pause");
		 //}

		if (past_contour_info.empty() == 0) // if this cycle is not first of program
		{
			contour_info = label_ellipse(past_contour_info, contour_info, 1/30, NONE, EUCLIDIAN);
		}

		print_contour_info(contour_info, "Before");
		contour_info = align_contour_info(contour_info);
		print_contour_info(contour_info, "After");
		monitoring(frame_rgb, contour_info);
		imshow("Original", frame_rgb);
		past_contour_info = contour_info;
		contour_km1 = contour;
		save_frame2jpeg(frame_rgb, idx, 'G');

		if (waitKey(5) >= 0) break;
	}

	close_camera_frame(cap);
	return 0;
}