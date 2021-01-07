#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

class DrawPreciseEllipse
{
public:
	DrawPreciseEllipse();
	~DrawPreciseEllipse();

public:

	//ʹ��opencv�е�ellipse������ƽ�������Բ���˴��Ĳ���Ϊ������İ��᳤���˻�����Բ�ĺ�������������Ϊ��������
	static Mat DrawEllipse(Mat img, double EllipseCenter_x, double EllipseCenter_y, double EllipseLong_axis, double EllipseShort_axis, double angle);

	//////////1-3��ÿ���߽����Բ���ط�Ϊ400�ݣ����ݣ�I=��Բ�ڵ����ظ���h/400*150+50,����Բ�߽����¸�ֵ�����Ӿ�ȷ�������Բ��
	static Mat ComputeBoundaryArea(vector<Point2f> boundaryPnts, Mat &dst, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);
	//////////1-4������opencv�е�DrawEllipse����������Բ�������ð�������ҷ��ҳ���Բ�ı߽磬��Բ���������ֵ��ֵΪ200����Բ����ĸ�ֵΪ50���߽������������ͨ������ComputeBoundaryArea�������¸�ֵ
	static Mat DrawEllipse2ComputeBoundaryArea(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);
	//////��Ӹ�˹�����ĺ˴�С
	static Mat DrawEllipse2ComputeBoundaryAreaAddGaussianblur(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle, double ksize, double sigmaX, double sigmaY);
	////////���������Բ�ĺ��� �����������Ϊdouble����
	static Mat  DrawEllipseDouble(Mat &img, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);


};

