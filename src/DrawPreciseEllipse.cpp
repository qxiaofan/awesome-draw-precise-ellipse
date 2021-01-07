#include "DrawPreciseEllipse.h"


DrawPreciseEllipse::DrawPreciseEllipse()
{
}


DrawPreciseEllipse::~DrawPreciseEllipse()
{
}

/////////:1-2、使用opencv中的ellipse函数在平面绘制椭圆，此处的参数为长短轴的半轴长。
Mat DrawPreciseEllipse::DrawEllipse(Mat img, double EllipseCenter_x, double EllipseCenter_y, double EllipseLong_axis, double EllipseShort_axis, double angle)
{

	int thickness = -2;
	int lineType = 8;
	ellipse(img,
		Point(EllipseCenter_x, EllipseCenter_y),
		Size(EllipseLong_axis, EllipseShort_axis),   ////ellipse()函数中参数轴长应该是长短轴的一半，此处将对应的参数除以二，则我们输入即可认为是长短轴轴长。
		angle,
		0,
		360,
		Scalar(1),
		thickness,
		lineType);
	return img;
}

Mat  DrawPreciseEllipse::DrawEllipseDouble(Mat &img, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle)
{
	double pivalue = acos(-1);
	angle = -angle / 180 * pivalue;
	double A = pow(semiLongAxis, 2)*pow(sin(angle), 2) + pow(semiShortAxis, 2)*pow(cos(angle), 2);
	double B = 2 * (semiLongAxis*semiLongAxis - semiShortAxis*semiShortAxis)*sin(angle)*cos(angle);
	double C = pow(semiLongAxis, 2)*pow(cos(angle), 2) + pow(semiShortAxis, 2)*pow(sin(angle), 2);
	double f = -pow(semiLongAxis, 2)*pow(semiShortAxis, 2);
	for (int j = 0; j < img.rows; j++)
	{
		for (int i = 0; i < img.cols; i++)
		{
			if ((A*pow((i - center_x), 2) + B*(i - center_x)*(j - center_y) + C*pow((j - center_y), 2) + f) <= 0)
				img.at<uchar>(j, i) = 1;
			else
			{
				continue;
			}
		}

	}

	/*namedWindow("ellipse", CV_WINDOW_AUTOSIZE);
	imshow("ellipse", img);
	waitKey();*/
	return img;
}



//////////1-3、将每个边界的椭圆像素分为400份，根据：I=椭圆内的像素个数h/400*150+50,将椭圆边界重新赋值，更加精确的拟合椭圆。
Mat DrawPreciseEllipse::ComputeBoundaryArea(vector<Point2f> boundaryPnts, Mat &dst, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle)
{
	vector<Point2f>  PixelPoints;
	/**********在C++中，cos（）等三角函数使用的是弧度制**************/
	double angleDegree;
	double pivalue = acos(-1);
	angleDegree = angle / 180 * pivalue;
	for (int i = 0; i < boundaryPnts.size(); i++)
	{
		PixelPoints.clear();
		for (double j = 0; j <= 1.0001; j = j + 0.05)
		{
			for (double k = 0; k <= 1.0001; k = k + 0.05)
			{
				PixelPoints.push_back(Point2f(boundaryPnts[i].x - 0.5 + j, boundaryPnts[i].y - 0.5 + k));
			}
		}
		double h = 0;
		for (int p = 0; p < PixelPoints.size(); p++)
		{
			/*double singleDistance = (pow((PixelPoints[p].x - center_x), 2) / pow(semiLongAxis, 2)) + (pow((PixelPoints[p].y - center_y), 2) / pow(semiShortAxis, 2));*/
			Mat R = (Mat_<double>(2, 2) << cos(angleDegree), -sin(angleDegree), sin(angleDegree), cos(angleDegree));
			Mat inv_R = R.inv();
			Mat M = (Mat_<double>(2, 1) << (PixelPoints[p].x - center_x), (PixelPoints[p].y - center_y));
			Mat U_V = inv_R*M;
			double x1 = U_V.at<double>(0, 0);
			double y1 = U_V.at<double>(1, 0);
			double singleDistance = (pow(x1, 2) / pow(semiLongAxis, 2)) + (pow(y1, 2) / pow(semiShortAxis, 2));
			if (singleDistance <= 1)
			{
				h++;
			}
			else
			{
				continue;
			}
		}
		int  w = (int)(h / 441 * 150 + 50);
		dst.at<uchar>(boundaryPnts[i].y, boundaryPnts[i].x) = w;
	}
	return dst;

}

//////////1-4、利用opencv中的DrawEllipse函数绘制椭圆，并利用八邻域查找法找出椭圆的边界，椭圆里面的像素值赋值为200，椭圆外面的赋值为50，边界点根据面积法，通过函数ComputeBoundaryArea（）重新赋值
Mat DrawPreciseEllipse::DrawEllipse2ComputeBoundaryArea(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle)
{
	vector<Point2f>  boundaryPnts;
	/////STEP1:绘制椭圆
	Mat img = Mat::zeros(length, width, CV_8UC1);
	//int angleStart = -90;
	//int angleEnd = 90;
	//double angle;
	//srand((unsigned)time(NULL));
	//angle =double(rand())/RAND_MAX*(angleEnd - angleStart) + angleStart;

	img = DrawPreciseEllipse::DrawEllipseDouble(img, center_x, center_y, semiLongAxis, semiShortAxis, angle);

	Mat Img2;
	img.copyTo(Img2);
	Mat mask = (Mat_<char>(3, 3) << 1, 1, 1, 1, 0, 1, 1, 1, 1);
	Mat dst;
	filter2D(Img2, dst, -1, mask);
	boundaryPnts.clear();
	for (int m = 0; m < dst.rows; m++)
	{
		for (int n = 0; n < dst.cols; n++)
		{
			if (dst.at<uchar>(m, n) == 0)
			{
				dst.at<uchar>(m, n) = 50;
			}
			else
			{
				if (dst.at<uchar>(m, n) == 8)
				{
					dst.at<uchar>(m, n) = 200;
				}
				else
				{
					boundaryPnts.push_back(Point2f(n, m));
				}
			}

		}
	}
	Mat dstOld;
	dst.copyTo(dstOld);
	Mat Dst = DrawPreciseEllipse::ComputeBoundaryArea(boundaryPnts, dst, center_x, center_y, semiLongAxis, semiShortAxis, angle);
	////// STEP 2  模糊  0.5
	/*GaussianBlur(Dst, Dst, Size(5, 5),0.5,0.5);*/

	//////STEP3:添加高斯噪声
	/*Mat imgNoise = Mat::zeros(length, width, CV_8UC1);
	int meanShiftValue = 50;
	randn(imgNoise, meanShiftValue, sigma);
	imgNoise = imgNoise + Dst - meanShiftValue;*/
	return Dst;
}

//////////////添加高斯噪声
Mat DrawPreciseEllipse::DrawEllipse2ComputeBoundaryAreaAddGaussianblur(int sigma, double length, double width, double center_x, double center_y, double semiLongAxis, double semiShortAxis, double angle, double ksize, double sigmaX, double sigmaY)
{
	vector<Point2f>  boundaryPnts;
	/////STEP1:绘制椭圆
	Mat img = Mat::zeros(length, width, CV_8UC1);
	//int angleStart = -90;
	//int angleEnd = 90;
	//double angle;
	//srand((unsigned)time(NULL));
	//angle =double(rand())/RAND_MAX*(angleEnd - angleStart) + angleStart;

	img = DrawPreciseEllipse::DrawEllipseDouble(img, center_x, center_y, semiLongAxis, semiShortAxis, angle);
	Mat Img2;
	img.copyTo(Img2);
	Mat mask = (Mat_<char>(3, 3) << 1, 1, 1, 1, 0, 1, 1, 1, 1);
	Mat dst;
	filter2D(Img2, dst, -1, mask);
	boundaryPnts.clear();
	for (int m = 0; m < dst.rows; m++)
	{
		for (int n = 0; n < dst.cols; n++)
		{
			if (dst.at<uchar>(m, n) == 0)
			{
				dst.at<uchar>(m, n) = 50;
			}
			else
			{
				if (dst.at<uchar>(m, n) == 8)
				{
					dst.at<uchar>(m, n) = 200;
				}
				else
				{
					boundaryPnts.push_back(Point2f(n, m));
				}
			}

		}
	}
	Mat dstOld;
	dst.copyTo(dstOld);
	Mat Dst = DrawPreciseEllipse::ComputeBoundaryArea(boundaryPnts, dst, center_x, center_y, semiLongAxis, semiShortAxis, angle);
	////STEP 2  模糊  0.5

	GaussianBlur(Dst, Dst, Size(ksize, ksize), sigmaX, sigmaY);

	//////STEP3:添加高斯噪声
	Mat imgNoise = Mat::zeros(length, width, CV_8UC1);
	int meanShiftValue = 50;
	randn(imgNoise, meanShiftValue, sigma);
	imgNoise = imgNoise + Dst - meanShiftValue;
	return imgNoise;
}

