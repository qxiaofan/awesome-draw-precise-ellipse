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

	//使用opencv中的ellipse函数在平面绘制椭圆，此处的参数为长短轴的半轴长。此绘制椭圆的函数，中心坐标为整数类型
	static Mat DrawEllipse(Mat img, double EllipseCenter_x, double EllipseCenter_y, double EllipseLong_axis, double EllipseShort_axis, double angle);

	//////////1-3、每个边界的椭圆像素分为400份，根据：I=椭圆内的像素个数h/400*150+50,将椭圆边界重新赋值，更加精确的拟合椭圆。
	static Mat ComputeBoundaryArea(vector<Point2f> boundaryPnts, Mat &dst, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);
	//////////1-4、利用opencv中的DrawEllipse函数绘制椭圆，并利用八邻域查找法找出椭圆的边界，椭圆里面的像素值赋值为200，椭圆外面的赋值为50，边界点根据面积法，通过函数ComputeBoundaryArea（）重新赋值
	static Mat DrawEllipse2ComputeBoundaryArea(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);
	//////添加高斯噪声的核大小
	static Mat DrawEllipse2ComputeBoundaryAreaAddGaussianblur(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle, double ksize, double sigmaX, double sigmaY);
	////////这个绘制椭圆的函数 中心坐标可以为double类型
	static Mat  DrawEllipseDouble(Mat &img, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle);


};

