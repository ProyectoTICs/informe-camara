#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[])
{
	VideoCapture cap("rtsp://tic:proyectostic@200.14.84.100/video.pro2");

	if (cap.isOpened())
	{
		Mat actFrame, antFrame;
		Mat _kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size2i(3, 3), cv::Point2i(1, 1));

		while (true)
		{
			cap >> actFrame;

			if (!actFrame.empty())
			{
				if (!antFrame.empty())
				{
					Mat difFrame;

					subtract(actFrame, antFrame, difFrame);
					threshold(difFrame, difFrame, 15, 255, CV_THRESH_BINARY);
					morphologyEx(difFrame, difFrame, CV_MOP_OPEN, _kernel, cv::Point2i(-1, -1), 1);
					cvtColor(difFrame, difFrame, CV_BGR2GRAY);

					imshow("Movimientos", difFrame);


					vector<vector<Point> > contours;
					vector<Vec4i> hierarchy;

					int largest_area = 0;
					int largest_contour_index = 0;
					Rect bounding_rect;

					findContours(difFrame, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

					for (size_t i = 0; i < contours.size(); i++) 
					{
						double a = contourArea(contours[i], false);

						if (a > largest_area)
						{
							largest_area = a;
							largest_contour_index = i;
							bounding_rect = boundingRect(contours[i]);
						}
					}

					Mat dst = actFrame.clone();
					Scalar color(255, 255, 255);
					drawContours(dst, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
					rectangle(dst, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);

					imshow("Rectangulo Movimientos", dst);

				}

				antFrame = actFrame.clone();
			}

			imshow("Deteccion de Movimientos", actFrame);

			if (waitKey(30) == 27) break;
		}
	}

	return 0;
}


