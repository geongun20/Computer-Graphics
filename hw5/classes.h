#include<stdio.h>
#include<utility>
#include<math.h>
#include<vector>
class Vector {
private:
	double x;
	double y;
	double z;
public:
	Vector() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector(double xin, double yin, double zin) {
		x = xin;
		y = yin;
		z = zin;
	}
	double getx() {
		return x;
	}
	double gety() {
		return y;
	}
	double getz() {
		return z;
	}
	double length() {
		return sqrt(x*x + y * y + z * z);
	}
	double operator*(Vector in) {
		return x * in.x + y * in.y + z * in.z;
	}
	Vector operator*(double in) {
		return Vector(x * in, y * in, z * in);
	}
	Vector operator-(Vector in) {
		return Vector(x - in.x, y - in.y, z - in.z);
	}
	Vector operator+(Vector in) {
		return Vector(x + in.x, y + in.y, z + in.z);
	}

	Vector cross(Vector in) {
		return Vector(y*in.z - z * in.y, z*in.x - x * in.z, x*in.y - y * in.x);
	}
	Vector Normalization() {
		double l = length();
		return Vector(x / l, y / l, z / l);
	}
};

class Point {
private:
	double x;
	double y;
	double z;
public:
	Point() {
		x = 0;
		y = 0;
		z = 0;
	}
	Point(double xin, double yin, double zin) {
		x = xin;
		y = yin;
		z = zin;
	}
	double getx() {
		return x;
	}
	double gety() {
		return y;
	}
	double getz() {
		return z;
	}
	Point operator+(Vector in) {
		return Point(x + in.getx(), y + in.gety(), z + in.getz());
	}
	Vector operator-(Point in) {
		return Vector(x - in.x, y - in.y, z - in.z);
	}

};

class Ray {
private:
	Point p;
	Vector v;
public:
	Ray() {
		p = Point();
		v = Vector();
	}
	Ray(double px, double  py, double  pz, double  vx, double  vy, double  vz) {
		p = Point(px, py, pz);
		v = Vector(vx, vy, vz);
	}
	Ray(Point pin, Vector vin) {
		p = pin;
		v = vin;
	}
	Point getPoint() {
		return p;
	}
	Vector getVector() {
		return v;
	}
};
class KKK {
private:
	double R;
	double G;
	double B;
public:
	KKK() {
		R = 0;
		G = 0;
		B = 0;
	}
	KKK(double Rin, double Gin, double Bin) {
		R = Rin;
		G = Gin;
		B = Bin;
	}
	double getR() {
		return R;
	}
	double getG() {
		return G;
	}
	double getB() {
		return B;
	}
	KKK operator+(KKK in) {
		return KKK(R + in.getR(), G + in.getG(), B + in.getB());
	}
	KKK operator*(double in) {
		return KKK(R * in, G * in, B * in);
	}

};
class Light {
private:
	double R;
	double G;
	double B;
public:
	Light() {
		R = 0;
		G = 0;
		B = 0;
	}
	Light(double Rin, double Gin, double Bin) {
		R = Rin;
		G = Gin;
		B = Bin;
	}
	double getR() {
		return R;
	}
	double getG() {
		return G;
	}
	double getB() {
		return B;
	}
	Light operator+(Light in) {
		return Light(R + in.getR(), G + in.getG(), B + in.getB());
	}
	Light operator*(double in) {
		return Light(R * in, G * in, B * in);
	}
	Light operator*(KKK in) {
		return  Light(R * in.getR(), G * in.getG(), B * in.getB());
	}

};

class Object {
private:
	KKK Ka;
	KKK Kd;
	KKK Ks;
	int Gl;
	double Kr;

public:
	KKK getKa() {
			return Ka;
	}
	double getKr() {
		return Kr;
	}
	KKK getKd() {
			return Kd;
	}
	KKK getKs() {
		return Ks;
	}
	int getGl() {
		return Gl;
	}
	void setKr(double in) {
		Kr = in;
	}
	void setKa(KKK in) {
		Ka = in;
	}
	void setKd(KKK in) {
		Kd = in;
	}
	void setKs(KKK in) {
		Ks = in;
	}
	void setGl(int in) {
		Gl = in;
	}
	virtual int intersecNum(Ray ray) = 0;
	virtual double svalue(Ray ray) = 0;
};

class Polygon : public Object {
private:
	std::vector<Point> Points;
	double n;
	double d;
	Vector Normal;
public:
	Polygon() {
		n = 0;
		d = 0;
		setKa(KKK(0.1,0.1,0.1));
		setKd(KKK(0.3,0.3,0.3));
		setKs(KKK(0.4,0.4,0.4));
		setGl(50);
	}
	Point coord(Ray r) {
		Point P = intersec(r);
		Vector V = P - Points.at(1);
		Vector V1 = Points.at(2) - Points.at(1);
		Vector V2 = Points.at(0) - Points.at(1);
		return Point((V*V1) / V1.length()/ V1.length()*450, (V*V2) / V2.length()/ V2.length()*382,0);
	}

	Polygon(std::vector<Point> in, double nin,double krin, KKK kain, KKK kdin, KKK ksin) {
		Points = in;
		n = nin;
		Normal = (Points.at(1) - Points.at(0)).cross(Points.at(1) - Points.at(2)).Normalization();
		d = -(Normal.getx()*Points.at(0).getx() + Normal.gety()*Points.at(0).gety() + Normal.getz()*Points.at(0).getz());
		setKa(kain);
		setKd(kdin);
		setKs(ksin);
		setGl(50);
		setKr(krin);
	}


	Vector getNormal() {
		return Normal;
	}
	int intersecNum(Ray ray) {
		for (int i=0;i<(int)Points.size()-1;i++) {
			if ((((Points.at(i + 1) - Points.at(i)).cross(intersec(ray) - Points.at(i)))*Normal) > 0)return 0;
		}
		if ((((Points.at(0) - Points.at(Points.size() - 1)).cross(intersec(ray) - Points.at(Points.size() - 1)))*Normal) > 0)return 0;
		if (svalue(ray) < 0) return 0;
		else return 1;
	}
	Point intersec(Ray ray) {
		return ray.getPoint() + (ray.getVector()*svalue(ray));
	}
	double svalue(Ray ray) {
		return (-d-(Normal*((ray.getPoint())-Point())))/(Normal*(ray.getVector()));
	}
};
class Sphere : public Object {	
private:
	Point pc;
	double r;
	double n;
public:
	Sphere() {
		pc = Point();
		r = 0;
		n = 0;
		setKa(KKK(0.1, 0.1, 0.1));
		setKd(KKK(0.3, 0.3, 0.3));
		setKs(KKK(0.4, 0.4, 0.4));
		setGl(50);
	}
	Sphere(double xin, double yin, double zin, double rin, double nin,double  krin,KKK kain,KKK kdin,KKK ksin) {
		pc = Point(xin, yin, zin);
		r = rin;
		n = nin;
		setKa(kain);
		setKd(kdin);
		setKs(ksin);
		setGl(50);
		setKr(krin);
	}

	Point coord(Ray r) {
		Vector norma = Normal(r);
		return Point(norma.getz() * 450, norma.getx()*382, 0);
	}
	Point getpc() {
		return pc;
	}
	int intersecNum(Ray ray) {
		Vector delp = pc- ray.getPoint();
		double b = ray.getVector() * delp;
		double D = b * b - delp.length()*delp.length() + r * r;
		
		if (D > 0&& b - sqrt(D) > 0)return 2;
		else if(D == 0&& b - sqrt(D) > 0) return 1;

		else return 0;
	}
	Vector delp(Ray ray) {
		return pc - ray.getPoint();
	}
	double svalue(Ray ray) {
		Vector delp = pc- ray.getPoint();
		double b = ray.getVector() * delp;
		double D = b * b - delp.length()*delp.length() + r * r;
		return b - sqrt(D);
	}
	Point intersec(Ray ray) {
		return ray.getPoint() + ray.getVector() * svalue(ray);
	}
	Vector Normal(Ray ray) {
		Point intersec = ray.getPoint() + ray.getVector() * svalue(ray);
		return (intersec - pc).Normalization();
	}
};
