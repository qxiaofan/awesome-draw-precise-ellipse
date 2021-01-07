#include "DrawPreciseEllipse.h"

int main()
{
	int sigma = 0;
	double length = 600;
	double width = 600;

	////////////// step 1 generate an ellipse image
	double center_x = 300.67;
	double center_y = 300.67;
	double semiLongAxis = 100;
	double semiShortAxis = 80;
	double angle = 0;

	//OpenCV���Դ�����ellipse()���Ƶ���Բ
	Mat img = Mat::zeros(length, width, CV_32FC1);
	Mat ellipse_CV = DrawPreciseEllipse::DrawEllipse(img, center_x, center_y, semiLongAxis, semiShortAxis, angle);
	Mat ellipseImg_Area = DrawPreciseEllipse::DrawEllipse2ComputeBoundaryArea(sigma, length, width, center_x, center_y, semiLongAxis, semiShortAxis, angle);
	namedWindow("���;��ȡ�", 0);
	namedWindow("���߾��ȡ�", 0);
	imshow("���;��ȡ�", ellipse_CV);
	imshow("���߾��ȡ�", ellipseImg_Area);

	waitKey();

	return 0;

}