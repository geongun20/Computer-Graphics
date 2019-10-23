#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
using namespace std;

bool leftButton = false;
GLfloat mousePosX, mousePosY;

bool zoom = false;
double fov = 45.0;
double scal = 0.2;
double mrx = 0.0;
double mry = 0.0;
double mrz = 0.0;
double mrang = 0.0;
double dolly = 60.0;


int N = 15;
int M = 30;
bool BSPLINE;
int N_cs;
int N_cp;
double px[100][100];
double py[100][100];
double scaling[100];
double rotation[100][4];
double position[100][3];
double q3[4] = { 1,0.0,0.0,0.0 };
double q2[4] = { 1,0.0,0.0,0.0 };

double befposition[3];
double afposition[3];
double befscale;
double afscale;
double befrot[4];
double afrot[4];


double px2t[100][1000];
double py2t[100][1000];
double px2tt[100][1000];
double py2tt[100][1000];


double pxt[100];
double pyt[100];
double scalingt;
double rotationt[4];
double positiont[3];

GLdouble rotMatrix[16] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

/* set global variables */
int width, height;

/* vectors that makes the rotation and translation of the cube */
double eye[3] = { 0.0f, 0.0f, 100.0f };
double ori[3] = { 0.0f, 0.0f, 0.0f };
double rot[3] = { 0.0f, 0.0f, 0.0f };
double up[3] = { 0.0f, 1.0f, 0.0f };

void loadGlobalCoord()
{
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], ori[0], ori[1], ori[2], up[0], up[1], up[2]);
	glMultMatrixd(rotMatrix);
}

void glutMotion(int x, int y)
{
	if (leftButton) {

		double x2 = -(double)x + 250;
		double y2 = -(double)y + 250;

		double temp5 = x2 * x2 + y2 * y2;
		if (temp5 >= 199 * 199)x2 = x2 / sqrt(temp5) * 199;
		if (temp5 >= 199 * 199)y2 = y2 / sqrt(temp5) * 199;


		double x1 = -(double)mousePosX + 250;
		double y1 = -(double)mousePosY + 250;
		temp5 = x1 * x1 + y1 * y1;
		if (temp5 >= 199 * 199)x1 = x1 / sqrt(temp5) * 199;
		if (temp5 >= 199 * 199)y1 = y1 / sqrt(temp5) * 199;

		double z2 = sqrt(40000 - x2 * x2 - y2 * y2);
		double z1 = sqrt(40000 - x1 * x1 - y1 * y1);
		double axis[3] = { (y1*z2 - y2 * z1) / 40000,(z1*x2 - z2 * x1) / 40000,(x1*y2 - x2 * y1) / 40000 };
		double theta = atan2(sqrt(1 - (x1*x2 / 40000 + y1 * y2 / 40000 + z1 * z2 / 40000)*(x1*x2 / 40000 + y1 * y2 / 40000 + z1 * z2 / 40000)), (x1*x2 / 40000 + y1 * y2 / 40000 + z1 * z2 / 40000));

		double temp2 = sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
		double rx = axis[0] / temp2;
		double ry = axis[1] / temp2;
		double rz = axis[2] / temp2;
		double rang = theta;


		double q1[4];
		q1[0] = cos(rang / 2);
		q1[1] = rx * sin(rang / 2);
		q1[2] = ry * sin(rang / 2);
		q1[3] = rz * sin(rang / 2);

		q3[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];//q1q2
		q3[1] = q1[0] * q2[1] + q2[0] * q1[1] + q1[2] * q2[3] - q1[3] * q2[2];
		q3[2] = q1[0] * q2[2] + q2[0] * q1[2] + q1[3] * q2[1] - q1[1] * q2[3];
		q3[3] = q1[0] * q2[3] + q2[0] * q1[3] + q1[1] * q2[2] - q1[2] * q2[1];





		double qi3[4];
		double temp = q3[0] * q3[0] + q3[1] * q3[1] + q3[2] * q3[2] + q3[3] * q3[3];

		qi3[0] = q3[0] * (-1) / temp;
		qi3[1] = q3[1] / temp;
		qi3[2] = q3[2] / temp;
		qi3[3] = q3[3] / temp;

		double tem[4];
		double ey[4];


		ey[0] = 0;
		ey[1] = 0;
		ey[2] = 0;
		ey[3] = 1;
		tem[0] = ey[0] * qi3[0] - ey[1] * qi3[1] - ey[2] * qi3[2] - ey[3] * qi3[3];//qi3*eye
		tem[1] = ey[0] * qi3[1] + qi3[0] * ey[1] + qi3[2] * ey[3] - qi3[3] * ey[2];
		tem[2] = ey[0] * qi3[2] + qi3[0] * ey[2] + qi3[3] * ey[1] - qi3[1] * ey[3];
		tem[3] = ey[0] * qi3[3] + qi3[0] * ey[3] + qi3[1] * ey[2] - qi3[2] * ey[1];

		double te[4];
		te[0] = q3[0] * tem[0] - q3[1] * tem[1] - q3[2] * tem[2] - q3[3] * tem[3];//tem*q3
		te[1] = q3[0] * tem[1] + tem[0] * q3[1] + tem[2] * q3[3] - tem[3] * q3[2];
		te[2] = q3[0] * tem[2] + tem[0] * q3[2] + tem[3] * q3[1] - tem[1] * q3[3];
		te[3] = q3[0] * tem[3] + tem[0] * q3[3] + tem[1] * q3[2] - tem[2] * q3[1];



		double ey2[4];
		ey2[0] = 0;
		ey2[1] = 0;
		ey2[2] = 1;
		ey2[3] = 0;
		tem[0] = ey2[0] * qi3[0] - ey2[1] * qi3[1] - ey2[2] * qi3[2] - ey2[3] * qi3[3];//qi3*eye
		tem[1] = ey2[0] * qi3[1] + qi3[0] * ey2[1] + qi3[2] * ey2[3] - qi3[3] * ey2[2];
		tem[2] = ey2[0] * qi3[2] + qi3[0] * ey2[2] + qi3[3] * ey2[1] - qi3[1] * ey2[3];
		tem[3] = ey2[0] * qi3[3] + qi3[0] * ey2[3] + qi3[1] * ey2[2] - qi3[2] * ey2[1];

		double te2[4];
		te2[0] = q3[0] * tem[0] - q3[1] * tem[1] - q3[2] * tem[2] - q3[3] * tem[3];//tem*q3
		te2[1] = q3[0] * tem[1] + tem[0] * q3[1] + tem[2] * q3[3] - tem[3] * q3[2];
		te2[2] = q3[0] * tem[2] + tem[0] * q3[2] + tem[3] * q3[1] - tem[1] * q3[3];
		te2[3] = q3[0] * tem[3] + tem[0] * q3[3] + tem[1] * q3[2] - tem[2] * q3[1];

		eye[0] = dolly * te[1];
		eye[1] = dolly * te[2];
		eye[2] = dolly * te[3];
		up[0] = te2[1];
		up[1] = te2[2];
		up[2] = te2[3];



		loadGlobalCoord();
	}
	return;
}

void glutMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			mousePosX = x;
			mousePosY = y;
			q2[0] = q3[0];
			q2[1] = q3[1];
			q2[2] = q3[2];
			q2[3] = q3[3];
			leftButton = true;
		}
		else if (state == GLUT_UP)
		{
			leftButton = false;
		}
		break;
	case GLUT_RIGHT_BUTTON:break;
	case 3:
		if (state == GLUT_UP) {
			double temp = dolly;
			if (zoom&&fov>5) {
				fov -= 1;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(fov, 1, .1f, 500.0f);
				glMatrixMode(GL_MODELVIEW);
				glutPostRedisplay();
			}
			if (!zoom&&dolly>21) {
				dolly -= 3;
				eye[0] = dolly * eye[0] / temp;
				eye[1] = dolly * eye[1] / temp;
				eye[2] = dolly * eye[2] / temp;
			}
		}
		else if (state == GLUT_DOWN) {
			break;
		}
		break;
	case 4:
		if (state == GLUT_UP) {
			double temp = dolly;
			if (zoom&&fov<90) {
				fov += 1;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(fov, 1, .1f, 500.0f);
				glMatrixMode(GL_MODELVIEW);
				glutPostRedisplay();
			}
			if (!zoom&&dolly<450) {
				dolly += 3;
				eye[0] = dolly * eye[0] / temp;
				eye[1] = dolly * eye[1] / temp;
				eye[2] = dolly * eye[2] / temp;
			}
		}
		else if (state == GLUT_DOWN) {
			break;
		}

		break;








	default:break;
	}
	return;
}


void keyboard(unsigned char key, int x, int y) {
	double tempsize = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
	double rightv[3];
	double frontv[3] = { ori[0] - eye[0],ori[1] - eye[1],ori[2] - eye[2] };

	double temp = dolly;
	rightv[0] = frontv[1] * up[2] - frontv[2] * up[1];
	rightv[1] = frontv[2] * up[0] - frontv[0] * up[2];
	rightv[2] = frontv[0] * up[1] - frontv[1] * up[0];
	switch (key) {
	case 98://b
		eye[0] -= ori[0];
		eye[1] -= ori[1];
		eye[2] -= ori[2];
		ori[0] = 0;
		ori[1] = 0;
		ori[2] = 0;
		dolly = 1800 / fov;
		eye[0] = dolly * eye[0] / temp;
		eye[1] = dolly * eye[1] / temp;
		eye[2] = dolly * eye[2] / temp;
		break;
	case 97://a
		tempsize = sqrt(rightv[0] * rightv[0] + rightv[1] * rightv[1] + rightv[2] * rightv[2]);
		ori[0] += rightv[0] / tempsize;
		eye[0] += rightv[0] / tempsize;
		ori[1] += rightv[1] / tempsize;
		eye[1] += rightv[1] / tempsize;
		ori[2] += rightv[2] / tempsize;
		eye[2] += rightv[2] / tempsize;
		break;
	case 100://d
		tempsize = sqrt(rightv[0] * rightv[0] + rightv[1] * rightv[1] + rightv[2] * rightv[2]);
		ori[0] -= rightv[0] / tempsize;
		eye[0] -= rightv[0] / tempsize;
		ori[1] -= rightv[1] / tempsize;
		eye[1] -= rightv[1] / tempsize;
		ori[2] -= rightv[2] / tempsize;
		eye[2] -= rightv[2] / tempsize;
		break;

	case 115://s
		ori[0] += up[0] / tempsize;
		eye[0] += up[0] / tempsize;
		ori[1] += up[1] / tempsize;
		eye[1] += up[1] / tempsize;
		ori[2] += up[2] / tempsize;
		eye[2] += up[2] / tempsize;
		break;
	case 119://w
		ori[0] -= up[0] / tempsize;
		eye[0] -= up[0] / tempsize;
		ori[1] -= up[1] / tempsize;
		eye[1] -= up[1] / tempsize;
		ori[2] -= up[2] / tempsize;
		eye[2] -= up[2] / tempsize;
		break;

	case 113:
		zoom = !zoom;
		if (zoom)printf("zoommode\n");
		else printf("dollymode\n");
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

double bfunc(double t, double b0, double b1, double b2, double b3) {
	return (1 - t)*(1 - t)*(1 - t) / 6 * b0 + (3 * t*t*t - 6 * t*t + 4) / 6 * b1 + (-3 * t*t*t + 3 * t*t + 3 * t + 1) / 6 * b2 + t * t*t / 6 * b3;
}

double cfunc(double t, double b0, double b1, double b2, double b3) {
	return (1 - t)*(1 - t)*(1 - t)*b0+3*t*(1-t)*(1 - t)*b1 + 3 * t*t*(1 - t)*b2 + t*t*t*b3;
}

double cintcoord(double t, double x1, double x2, double x3, double x4) {
	return cfunc(t,x2,x2+(x3-x1)/6, x3 + (x2 - x4) / 6,x3);
}

double intcoord(double t, double x1, double x2) {
	return x1 * t + x2 * (1 - t);
}

double intq0(double t, double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3
	, double c0, double c1, double c2, double c3, double d0, double d1, double d2, double d3) {
	double temp2 = sqrt(a1 * a1 + a2 * a2 + a3 * a3);
	double r1x = a1 / temp2 * a0;
	double r1y = a2 / temp2 * a0;
	double r1z = a3 / temp2 * a0;
	temp2 = sqrt(b1 * b1 + b2 * b2 + b3 * b3);
	double r2x = b1 / temp2 * b0;
	double r2y = b2 / temp2 * b0;
	double r2z = b3 / temp2 * b0;
	temp2 = sqrt(c1 * c1 + c2 * c2 + c3 * c3);
	double r3x = c1 / temp2 * c0;
	double r3y = c2 / temp2 * c0;
	double r3z = c3 / temp2 * c0;
	temp2 = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
	double r4x = d1 / temp2 * d0;
	double r4y = d2 / temp2 * d0;
	double r4z = d3 / temp2 * d0;

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	return rang;
}

double intq1(double t, double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3
	, double c0, double c1, double c2, double c3, double d0, double d1, double d2, double d3) {
	double temp2 = sqrt(a1 * a1 + a2 * a2 + a3 * a3);
	double r1x = a1 / temp2 * a0;
	double r1y = a2 / temp2 * a0;
	double r1z = a3 / temp2 * a0;
	temp2 = sqrt(b1 * b1 + b2 * b2 + b3 * b3);
	double r2x = b1 / temp2 * b0;
	double r2y = b2 / temp2 * b0;
	double r2z = b3 / temp2 * b0;
	temp2 = sqrt(c1 * c1 + c2 * c2 + c3 * c3);
	double r3x = c1 / temp2 * c0;
	double r3y = c2 / temp2 * c0;
	double r3z = c3 / temp2 * c0;
	temp2 = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
	double r4x = d1 / temp2 * d0;
	double r4y = d2 / temp2 * d0;
	double r4z = d3 / temp2 * d0;

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	return rx / rang;
}

double intq2(double t, double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3
	, double c0, double c1, double c2, double c3, double d0, double d1, double d2, double d3) {
	double temp2 = sqrt(a1 * a1 + a2 * a2 + a3 * a3);
	double r1x = a1 / temp2 * a0;
	double r1y = a2 / temp2 * a0;
	double r1z = a3 / temp2 * a0;
	temp2 = sqrt(b1 * b1 + b2 * b2 + b3 * b3);
	double r2x = b1 / temp2 * b0;
	double r2y = b2 / temp2 * b0;
	double r2z = b3 / temp2 * b0;
	temp2 = sqrt(c1 * c1 + c2 * c2 + c3 * c3);
	double r3x = c1 / temp2 * c0;
	double r3y = c2 / temp2 * c0;
	double r3z = c3 / temp2 * c0;
	temp2 = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
	double r4x = d1 / temp2 * d0;
	double r4y = d2 / temp2 * d0;
	double r4z = d3 / temp2 * d0;

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	return ry / rang;
}

double intq3(double t, double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3
	, double c0, double c1, double c2, double c3, double d0, double d1, double d2, double d3) {
	double temp2 = sqrt(a1 * a1 + a2 * a2 + a3 * a3);
	double r1x = a1 / temp2 * a0;
	double r1y = a2 / temp2 * a0;
	double r1z = a3 / temp2 * a0;
	temp2 = sqrt(b1 * b1 + b2 * b2 + b3 * b3);
	double r2x = b1 / temp2 * b0;
	double r2y = b2 / temp2 * b0;
	double r2z = b3 / temp2 * b0;
	temp2 = sqrt(c1 * c1 + c2 * c2 + c3 * c3);
	double r3x = c1 / temp2 * c0;
	double r3y = c2 / temp2 * c0;
	double r3z = c3 / temp2 * c0;
	temp2 = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
	double r4x = d1 / temp2 * d0;
	double r4y = d2 / temp2 * d0;
	double r4z = d3 / temp2 * d0;

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	return rz / rang;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	loadGlobalCoord();
	
	for (int i = 0;i < N_cs;i++) {
		for (int l = 0;l < M;l++) {
			double t = (double)l / M;
			for (int j = 0;j < N_cp;j++) {
				pxt[j] = cintcoord(t, px[i - 1][j], px[i][j], px[i + 1][j], px[i + 2][j]);
				pyt[j] = cintcoord(t, py[i - 1][j], py[i][j], py[i + 1][j], py[i + 2][j]);
			}
			for (int j = 0;j < N_cp;j++) {
				for (int k = 0;k < N;k++) {
					px2tt[j][k] = px2t[j][k];
					py2tt[j][k] = py2t[j][k];
					px2t[j][k] = bfunc((double)k / N, pxt[j], pxt[(j + 1) % N_cp], pxt[(j + 2) % N_cp], pxt[(j + 3) % N_cp]);
					py2t[j][k] = bfunc((double)k / N, pyt[j], pyt[(j + 1) % N_cp], pyt[(j + 2) % N_cp], pyt[(j + 3) % N_cp]);
				}
			}

			if (i == 0) {
				befscale = afscale;
				befposition[0] = afposition[0];
				befposition[1] = afposition[1];
				befposition[2] = afposition[2];
				befrot[0] = afrot[0];
				befrot[1] = afrot[1];
				befrot[2] = afrot[2];
				befrot[3] = afrot[3];



				afscale = cintcoord(t, 2 * scaling[i] - scaling[i + 1], scaling[i], scaling[i + 1], scaling[i + 2]);
				afposition[0] = cintcoord(t, 2 * position[i][0] - position[i + 1][0], position[i][0], position[i + 1][0], position[i + 2][0]);
				afposition[1] = cintcoord(t, 2 * position[i][1] - position[i + 1][1], position[i][1], position[i + 1][1], position[i + 2][1]);
				afposition[2] = cintcoord(t, 2 * position[i][2] - position[i + 1][2], position[i][2], position[i + 1][2], position[i + 2][2]);
				afrot[0] = intq0(t, 2 * rotation[i][0] - rotation[i + 1][0], 2 * rotation[i][1] - rotation[i + 1][1],
					2 * rotation[i][2] - rotation[i + 1][2], 2 * rotation[i][3] - rotation[i + 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]) * 180 / 3.141592;
				afrot[1] = intq1(t, 2 * rotation[i][0] - rotation[i + 1][0], 2 * rotation[i][1] - rotation[i + 1][1],
					2 * rotation[i][2] - rotation[i + 1][2], 2 * rotation[i][3] - rotation[i + 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
				afrot[2] = intq2(t, 2 * rotation[i][0] - rotation[i + 1][0], 2 * rotation[i][1] - rotation[i + 1][1],
					2 * rotation[i][2] - rotation[i + 1][2], 2 * rotation[i][3] - rotation[i + 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
				afrot[3] = intq3(t, 2 * rotation[i][0] - rotation[i + 1][0], 2 * rotation[i][1] - rotation[i + 1][1],
					2 * rotation[i][2] - rotation[i + 1][2], 2 * rotation[i][3] - rotation[i + 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
			}

			else if (i == N_cs - 2) {
				befscale = afscale;
				afscale = cintcoord(t, scaling[i - 1], scaling[i], scaling[i + 1], 2 * scaling[i + 1] - scaling[i]);
				befposition[0] = afposition[0];
				befposition[1] = afposition[1];
				befposition[2] = afposition[2];

				afposition[0] = cintcoord(t, position[i - 1][0], position[i][0], position[i + 1][0], 2 * position[i + 1][0] - position[i][0]);
				afposition[1] = cintcoord(t, position[i - 1][1], position[i][1], position[i + 1][1], 2 * position[i + 1][1] - position[i][1]);
				afposition[2] = cintcoord(t, position[i - 1][2], position[i][2], position[i + 1][2], 2 * position[i + 1][2] - position[i][2]);


				befrot[0] = afrot[0];
				befrot[1] = afrot[1];
				befrot[2] = afrot[2];
				befrot[3] = afrot[3];
				afrot[0] = intq0(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					2 * rotation[i + 1][0] - rotation[i][0], 2 * rotation[i + 1][1] - rotation[i][1], 2 * rotation[i + 1][2] - rotation[i][2], 2 * rotation[i + 1][3] - rotation[i][3]
				) * 180 / 3.141592;
				afrot[1] = intq1(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					2 * rotation[i + 1][0] - rotation[i][0], 2 * rotation[i + 1][1] - rotation[i][1], 2 * rotation[i + 1][2] - rotation[i][2], 2 * rotation[i + 1][3] - rotation[i][3]
				);
				afrot[2] = intq2(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					2 * rotation[i + 1][0] - rotation[i][0], 2 * rotation[i + 1][1] - rotation[i][1], 2 * rotation[i + 1][2] - rotation[i][2], 2 * rotation[i + 1][3] - rotation[i][3]
				);
				afrot[3] = intq3(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					2 * rotation[i + 1][0] - rotation[i][0], 2 * rotation[i + 1][1] - rotation[i][1], 2 * rotation[i + 1][2] - rotation[i][2], 2 * rotation[i + 1][3] - rotation[i][3]
				);
			}

			else {
				befscale = afscale;
				afscale = cintcoord(t, scaling[i - 1], scaling[i], scaling[i + 1], scaling[i + 2]);
				befposition[0] = afposition[0];
				befposition[1] = afposition[1];
				befposition[2] = afposition[2];

				afposition[0] = cintcoord(t, position[i - 1][0], position[i][0], position[i + 1][0], position[i + 2][0]);
				afposition[1] = cintcoord(t, position[i - 1][1], position[i][1], position[i + 1][1], position[i + 2][1]);
				afposition[2] = cintcoord(t, position[i - 1][2], position[i][2], position[i + 1][2], position[i + 2][2]);


				befrot[0] = afrot[0];
				befrot[1] = afrot[1];
				befrot[2] = afrot[2];
				befrot[3] = afrot[3];
				afrot[0] = intq0(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]) * 180 / 3.141592;
				afrot[1] = intq1(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
				afrot[2] = intq2(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
				afrot[3] = intq3(t, rotation[i - 1][0], rotation[i - 1][1], rotation[i - 1][2], rotation[i - 1][3],
					rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3],
					rotation[i + 1][0], rotation[i + 1][1], rotation[i + 1][2], rotation[i + 1][3],
					rotation[i + 2][0], rotation[i + 2][1], rotation[i + 2][2], rotation[i + 2][3]);
			}

			glPushMatrix();
			glTranslatef(afposition[0], afposition[1], afposition[2]);

			glRotatef(afrot[0], afrot[1], afrot[2], afrot[3]);

			glScalef(afscale, afscale, afscale);
			glBegin(GL_LINE_LOOP);

			for (int j = 0;j < N_cp;j++) {
				for (int k = 0;k < N;k++) {
					glVertex3f(px2t[j][k], 0, py2t[j][k]);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
	glutSwapBuffers();
}

void resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, .1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



unsigned timeStep = 1;
void Timer(int unused)
{
	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
	glutTimerFunc(timeStep, Timer, 0);
}





int main(int argc, char **argv) {
	FILE * fp = fopen("mine.txt", "r");
	string line;
	char templine[100];
	fscanf(fp, "%s", templine);
//	printf("%s\n",templine);
	if(templine[0]=='B')BSPLINE=1;
	else BSPLINE=0;
	fscanf(fp, "%d", &N_cs);
	fscanf(fp, "%d", &N_cp);
	printf("%d %d",N_cs,N_cp);
	for (int i = 0;i < N_cs;i++) {
		for (int j = 0;j < N_cp;j++) {
			fscanf(fp, "%lf %lf\n", &px[i][j], &py[i][j]);
		}
		fscanf(fp, "%lf\n", &scaling[i]);
		fscanf(fp, "%lf %lf %lf %lf\n", &rotation[i][0], &rotation[i][1], &rotation[i][2], &rotation[i][3]);
		fscanf(fp, "%lf %lf %lf\n", &position[i][0], &position[i][1], &position[i][2]);
	}
//	for (int i = 0;i < N_cs;i++) {
//		for (int j = 0;j < N_cp;j++) {
//			printf("%f %f\n", px[i][j], py[i][j]);
//		}
//		printf("%f\n", scaling[i]);
//		printf("%f %f %f %f\n", rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3]);
//		printf("%f %f %f\n", position[i][0], position[i][1], position[i][2]);



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition( 50, 0 );
	glutCreateWindow("Example");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);

	glutMainLoop();

	return 0;
}
