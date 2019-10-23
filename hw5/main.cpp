#include<stdio.h>
#include<vector>
#include<math.h>
#include<cstring>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include "classes.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>




#define PIXEL 1000
double output[PIXEL][PIXEL][3];
char x[PIXEL][PIXEL][3];
cv::Mat A(PIXEL	, PIXEL, CV_8UC3);



std::vector<Sphere>mySphere;
std::vector<Polygon>myPolygon;


Point eye = Point( 0,40,-100 );
Vector up = Vector( 0,1,0 );
double pp[3] = { 10,10,10	};//d,w,h


Point PositionL0 = Point( 0.001,60,-60 );
Light IntensityL0 = Light( 1.0 ,1.0  ,1.0  );

Light IntensityA = Light( 1.0 ,1.0  ,1.0 );

double linD = 0.0005;
double quardD = 0.00004;

Light evaluate(Ray r, bool debug,int count) {
	r = Ray(r.getPoint()+r.getVector()*0.001, r.getVector());
	if (debug) {
	}
	count++;

	Sphere *objects;
	Polygon *objectp;
	
	double s = 10000000;
	bool isSphere;
	for (int i = 0;i < (int)mySphere.size();i++) {
		Sphere *temp = &mySphere.at(i);

		if (temp->intersecNum(r) != 0 ){
			double stemp = temp->svalue(r);
			if (stemp < s) {
				objects = temp;
				s = stemp;
				isSphere = true;
			}
		}
	}

	for (int i = 0;i < (int)myPolygon.size();i++) {
		Polygon *temp = &myPolygon.at(i);
		if (temp->intersecNum(r) != 0) {
			double stemp = temp->svalue(r);
			if (stemp < s) {
				objectp = temp;
				s = stemp;
				isSphere = false;
			}
		}
	}

	if (s > 9999990||count>4) {
		return Light();
	}

	else if(isSphere) {
		Point interPoint = objects->intersec(r);
		Vector Normal = objects->Normal(r);
		Vector Light0=(PositionL0-interPoint).Normalization();
		Vector Reflection = Normal * (2 * (Light0*Normal)) - Light0;

		Vector reflectv = Normal * (2 * (r.getVector()*(-1)*Normal)) - r.getVector()*(-1);
		Light reflect = evaluate(Ray(interPoint, reflectv), debug,count);


		bool isshowlight = true;
		Ray tolight0 = Ray(interPoint,Light0);
		for (int i = 0;i < (int)mySphere.size();i++) {
			Sphere *temp = &mySphere.at(i);
			if (objects == temp)continue;
			if (temp->intersecNum(tolight0) != 0) {
				isshowlight = false;
				break;
			}
		}
		for (int i = 0;i < (int)myPolygon.size();i++) {
			Polygon *temp = &myPolygon.at(i);
			if (temp->intersecNum(tolight0) != 0) {
				isshowlight = false;
				break;
			}
		}
		double s = objects->svalue(r);
		Light IntensityA2 = IntensityA * (1 / (1+linD*s+quardD*s*s));
		Light IntensityL02 = IntensityL0 * (1 / (1 + linD*s + quardD *s*s));
		if (isshowlight&&(Normal*Light0)>0) {
			return ((IntensityA2 * (objects->getKa()))+ (IntensityL02 * ((objects->getKd())*(Normal*Light0))) * (1.0 - (objects->getKr())) + (IntensityL02 * ((objects->getKs())*pow(Normal*Reflection,(objects->getGl())))))
				+ reflect * (objects->getKr());
		}
		else {
			return (IntensityA2 * objects->getKa()) * (1.0 - (objects->getKr())) + reflect * (objects->getKr());
		}
	}
	else {


		Point interPoint = objectp->intersec(r);
		Vector Normal = objectp->getNormal();
		Vector Light0 = (PositionL0 - interPoint).Normalization();
		Vector Reflection = Normal * (2 * (Light0*Normal)) - Light0;

		Vector reflectv = Normal * (2 * (r.getVector()*(-1)*Normal)) - r.getVector()*(-1);
		Light reflect = evaluate(Ray(interPoint, reflectv), debug, count);


		bool isshowlight = true;
		Ray tolight0 = Ray(interPoint, Light0);
		for (int i = 0;i < (int)mySphere.size();i++) {
			Sphere *temp = &mySphere.at(i);
			if (temp->intersecNum(tolight0) != 0) {
				isshowlight = false;
				break;
			}
		}
		for (int i = 0;i < (int)myPolygon.size();i++) {
			Polygon *temp = &myPolygon.at(i);
			if (objectp == temp)continue;
			if (temp->intersecNum(tolight0) != 0) {
				isshowlight = false;
				break;
			}
		}

		double s = objectp->svalue(r);
		Light IntensityA2 = IntensityA * (1 / (1 + linD*s + quardD *s*s));
		Light IntensityL02 = IntensityL0 * (1 / (1 + linD*s + quardD *s*s));
		if (isshowlight && (Normal*Light0)>0) {
			return ((IntensityA2 * (objectp->getKa())) + (IntensityL02 * ((objectp->getKd())*(Normal*Light0))) * (1.0 - (objectp->getKr())) + (IntensityL02 * ((objectp->getKs())*pow(Normal*Reflection, (objectp->getGl())))))
				+ reflect * (objectp->getKr());
		}
		else {
			return (IntensityA2 * objectp->getKa()) * (1.0 - (objectp->getKr())) + reflect * (objectp->getKr());
		}
	}
}
void cube(Point cp, double l,KKK ka,KKK kd,KKK ks) {
	double x = cp.getx();
	double y = cp.gety();
	double z = cp.getz();
	

	std::vector<Point> points0;
	std::vector<Point> points1;
	std::vector<Point> points2;
	std::vector<Point> points3;
	std::vector<Point> points4;
	std::vector<Point> points5;

	//front
	points0.push_back(Point(x+l, y-l, z - l));
	points0.push_back(Point(x+l, y+l, z - l));
	points0.push_back(Point(x-l, y+l, z - l));
	points0.push_back(Point(x-l, y-l, z - l));

	//back
	points1.push_back(Point(x-l, y-l, z + l));
	points1.push_back(Point(x-l, y+l, z + l));
	points1.push_back(Point(x+l, y+l, z + l));
	points1.push_back(Point(x+l, y-l, z + l));

	//up
	points2.push_back(Point(x+l, y+l, z - l));
	points2.push_back(Point(x+l, y + l, z + l));
	points2.push_back(Point(x-l, y + l, z + l));
	points2.push_back(Point(x-l, y + l, z - l));

	//down
	points3.push_back(Point(x-l, y-l, z - l));
	points3.push_back(Point(x-l, y - l, z + l));
	points3.push_back(Point(x+l, y - l, z + l));
	points3.push_back(Point(x+l, y - l, z - l));

	//left
	points4.push_back(Point(x+l, y+l, z - l));
	points4.push_back(Point(x + l, y-l, z - l));
	points4.push_back(Point(x + l, y-l, z + l));
	points4.push_back(Point(x + l, y+l, z + l));

	//right
	points5.push_back(Point(x-l, y-l, z - l));
	points5.push_back(Point(x - l, y + l, z - l));
	points5.push_back(Point(x - l, y + l, z + l));
	points5.push_back(Point(x - l, y - l, z + l));


	Polygon p0 = Polygon(points0, 1, 0,ka,kd,ks);
	Polygon p1 = Polygon(points1, 1,0, ka, kd, ks);
	Polygon p2 = Polygon(points2, 1, 0, ka, kd, ks);
	Polygon p3 = Polygon(points3, 1, 0, ka, kd, ks);
	Polygon p4 = Polygon(points4, 1, 0, ka, kd, ks);
	Polygon p5 = Polygon(points5, 1, 0, ka, kd, ks);

	myPolygon.push_back(p0);
	myPolygon.push_back(p1);
	myPolygon.push_back(p2);
	myPolygon.push_back(p3);
	myPolygon.push_back(p4);
	myPolygon.push_back(p5);
}

int main() {



	
	KKK ka = KKK(0.1, 0, 0);
	KKK kd = KKK(0.3, 0, 0);
	KKK ks = KKK(0.4, 0.4, 0.4);

	Sphere s0 = Sphere(0, 0, 0, 5, 1, 0,ka,kd,ks);
	Sphere s1 = Sphere(10, 10, -4, 5, 1, 0,ka, kd, ks);

	ka = KKK(0, 0, 0.1);
	kd = KKK(0, 0, 0.3);
	ks = KKK(0.4, 0.4, 0.4);
	Sphere s2 = Sphere(-10, -10, -20, 3, 1, 0, ka, kd, ks);//mirror sphere
	//down
	
	ka = KKK(0.1, 0.1, 0.1);
	kd = KKK(0.3, 0.3, 0.3);
	ks = KKK(0.4, 0.4, 0.4);

	std::vector<Point> points0;
	points0.push_back(Point(100, -15, -100));
	points0.push_back(Point(100, -15, 100));
	points0.push_back(Point(-100, -15, 100));
	points0.push_back(Point(-100,-15,-100));
	Polygon p0 = Polygon(points0,1, 0,ka,kd,ks);

	//behind
	std::vector<Point> points1;
	points1.push_back(Point(100, -100, 100));
	points1.push_back(Point(100, 100, 100));
	points1.push_back(Point(-100, 100, 100));
	points1.push_back(Point(-100, -100, 100));
	Polygon p1 = Polygon(points1, 1, 0,ka, kd, ks);

	//leftorright
	std::vector<Point> points2;
	points2.push_back(Point(50, -100, 100));
	points2.push_back(Point(50, -100, -100));
	points2.push_back(Point(50, 100, -100));
	points2.push_back(Point(50, 100, 100));
	Polygon p2 = Polygon(points2, 1, 0, ka, kd, ks);

	//leftorright
	std::vector<Point> points3;
	points3.push_back(Point(-50, -100, -100));
	points3.push_back(Point(-50, -100, 100));
	points3.push_back(Point(-50, 100, 100));
	points3.push_back(Point(-50, 100, -100));
	Polygon p3 = Polygon(points3, 1, 0, ka, kd, ks);

	//mirror1
	std::vector<Point> points4;
	points4.push_back(Point(-10, -15, -10));
	points4.push_back(Point(-10, 5, -10));
	points4.push_back(Point(-10-20/sqrt(2.0),5, -10-20 / sqrt(2.0)));
	points4.push_back(Point(-10-20 / sqrt(2.0), -15, -10-20 / sqrt(2.0)));
	Polygon p4 = Polygon(points4, 1, 0.9, ka, kd, ks);

	//mirror2
	std::vector<Point> points5;
	points5.push_back(Point(-10 + 20 / sqrt(2.0), -15, -10 - 20 / sqrt(2.0)));
	points5.push_back(Point(-10 + 20 / sqrt(2.0), 5, -10 - 20 / sqrt(2.0)));
	points5.push_back(Point(-10, 5, -10));
	points5.push_back(Point(-10, -15, -10));
	Polygon p5 = Polygon(points5, 1, 0.9, ka, kd, ks);

	

	cube(Point(30,0,0),4, ka, kd, ks);

	mySphere.push_back(s0);
	//mySphere.push_back(s1);
	mySphere.push_back(s2);
	myPolygon.push_back(p0);
	myPolygon.push_back(p1);
	myPolygon.push_back(p2);
	myPolygon.push_back(p3);
	myPolygon.push_back(p4);
	myPolygon.push_back(p5);


	for (int i = 0;i < PIXEL;i++) {
		for (int j = 0;j < PIXEL;j++) { 
			Vector z = (eye-Point()).Normalization();
			Vector x = z.cross(up).Normalization();
			Vector y = x.cross(z).Normalization();
			if (i == PIXEL / 2 && j == PIXEL / 2) {
			}
			Vector RayVector = (z*(-pp[0])+x*((double)(-PIXEL/2+i)/PIXEL*pp[1]) + y * ((double)(-PIXEL / 2+j)/PIXEL*pp[2]) ).Normalization();
			Ray r = Ray(eye,RayVector);
			Light result;
			result = evaluate(r,false, 0);

			output[j][i][0] = result.getB();
			output[j][i][1] = result.getG();
			output[j][i][2] = result.getR();
		}
	}


	for (int i = 0;i < PIXEL;i++) {
		printf("%d", i);
		for (int j = 0;j < PIXEL;j++) {
			for (int k=0;k < 3;k++) {
				x[i][j][k] = (char)((int)(output[PIXEL-i][j][k] * 256));
			}
		}
	}


	std::memcpy(A.data, x, PIXEL * PIXEL * 3 * sizeof(char));
	imwrite("./output.jpg", A);


}
