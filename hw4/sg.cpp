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

float cube[8][3] = { -5.0,-18.0,-5.0,
-5.0,-18.0,5.0,
-5.0,-8.0,5.0,
-5.0,-8.0,-5.0,
5.0,-18.0,-5.0,
5.0,-18.0,5.0,
5.0,-8.0,5.0,
5.0,-8.0,-5.0 };

float cube2[8][3] = { -5.0,-18.0,-5.0,
-5.0,-18.0,5.0,
-5.0,-8.0,5.0,
-5.0,-8.0,-5.0,
5.0,-18.0,-5.0,
5.0,-18.0,5.0,
5.0,-8.0,5.0,
5.0,-8.0,-5.0 };

int N = 15;
int M = 15;
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


double px2t[100000];
double py2t[100000];
double pz2t[100000];
double px2tt[100000];
double py2tt[100000];
double pz2tt[100000];


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
	return (1 - t)*(1 - t)*(1 - t)*b0 + 3 * t*(1 - t)*(1 - t)*b1 + 3 * t*t*(1 - t)*b2 + t * t*t*b3;
}

double cintcoord(double t, double x1, double x2, double x3, double x4) {
	return cfunc(t, x2, x2 + (x3 - x1) / 6, x3 + (x2 - x4) / 6, x3);
}

double intcoord(double t, double x1, double x2) {
	return x1 * t + x2 * (1 - t);
}

double intq0(double t, double *x1, double *x2, double *x3, double *x4) {

	double temp2 = sqrt(x1[1] * x1[1] + x1[2] * x1[2] + x1[3] * x1[3]);
	double r1x = x1[1] / temp2 * x1[0];
	double r1y = x1[2] / temp2 * x1[0];
	double r1z = x1[3] / temp2 * x1[0];
	temp2 = sqrt(x2[1] * x2[1] + x2[2] * x2[2] + x2[3] * x2[3]);
	double r2x = x2[1] / temp2 * x2[0];
	double r2y = x2[2] / temp2 * x2[0];
	double r2z = x2[3] / temp2 * x2[0];
	temp2 = sqrt(x3[1] * x3[1] + x3[2] * x3[2] + x3[3] * x3[3]);
	double r3x = x3[1] / temp2 * x3[0];
	double r3y = x3[2] / temp2 * x3[0];
	double r3z = x3[3] / temp2 * x3[0];
	temp2 = sqrt(x4[1] * x4[1] + x4[2] * x4[2] + x4[3] * x4[3]);
	double r4x = x4[1] / temp2 * x4[0];
	double r4y = x4[2] / temp2 * x4[0];
	double r4z = x4[3] / temp2 * x4[0];

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	return rang;
}

double intq1(double t, double *x1, double *x2, double *x3, double *x4) {
	double temp2 = sqrt(x1[1] * x1[1] + x1[2] * x1[2] + x1[3] * x1[3]);
	double r1x = x1[1] / temp2 * x1[0];
	double r1y = x1[2] / temp2 * x1[0];
	double r1z = x1[3] / temp2 * x1[0];
	temp2 = sqrt(x2[1] * x2[1] + x2[2] * x2[2] + x2[3] * x2[3]);
	double r2x = x2[1] / temp2 * x2[0];
	double r2y = x2[2] / temp2 * x2[0];
	double r2z = x2[3] / temp2 * x2[0];
	temp2 = sqrt(x3[1] * x3[1] + x3[2] * x3[2] + x3[3] * x3[3]);
	double r3x = x3[1] / temp2 * x3[0];
	double r3y = x3[2] / temp2 * x3[0];
	double r3z = x3[3] / temp2 * x3[0];
	temp2 = sqrt(x4[1] * x4[1] + x4[2] * x4[2] + x4[3] * x4[3]);
	double r4x = x4[1] / temp2 * x4[0];
	double r4y = x4[2] / temp2 * x4[0];
	double r4z = x4[3] / temp2 * x4[0];

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	if (rang <= 0.0000001&&rang >= -0.0000001)return 1.0;
	else return rx / rang;
}

double intq2(double t, double *x1, double *x2, double *x3, double *x4) {
	double temp2 = sqrt(x1[1] * x1[1] + x1[2] * x1[2] + x1[3] * x1[3]);
	double r1x = x1[1] / temp2 * x1[0];
	double r1y = x1[2] / temp2 * x1[0];
	double r1z = x1[3] / temp2 * x1[0];
	temp2 = sqrt(x2[1] * x2[1] + x2[2] * x2[2] + x2[3] * x2[3]);
	double r2x = x2[1] / temp2 * x2[0];
	double r2y = x2[2] / temp2 * x2[0];
	double r2z = x2[3] / temp2 * x2[0];
	temp2 = sqrt(x3[1] * x3[1] + x3[2] * x3[2] + x3[3] * x3[3]);
	double r3x = x3[1] / temp2 * x3[0];
	double r3y = x3[2] / temp2 * x3[0];
	double r3z = x3[3] / temp2 * x3[0];
	temp2 = sqrt(x4[1] * x4[1] + x4[2] * x4[2] + x4[3] * x4[3]);
	double r4x = x4[1] / temp2 * x4[0];
	double r4y = x4[2] / temp2 * x4[0];
	double r4z = x4[3] / temp2 * x4[0];

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	if (rang <= 0.0000001&&rang >= -0.0000001)return 0.0;
	else return ry / rang;
}

double intq3(double t, double *x1, double *x2, double *x3, double *x4) {
	double temp2 = sqrt(x1[1] * x1[1] + x1[2] * x1[2] + x1[3] * x1[3]);
	double r1x = x1[1] / temp2 * x1[0];
	double r1y = x1[2] / temp2 * x1[0];
	double r1z = x1[3] / temp2 * x1[0];
	temp2 = sqrt(x2[1] * x2[1] + x2[2] * x2[2] + x2[3] * x2[3]);
	double r2x = x2[1] / temp2 * x2[0];
	double r2y = x2[2] / temp2 * x2[0];
	double r2z = x2[3] / temp2 * x2[0];
	temp2 = sqrt(x3[1] * x3[1] + x3[2] * x3[2] + x3[3] * x3[3]);
	double r3x = x3[1] / temp2 * x3[0];
	double r3y = x3[2] / temp2 * x3[0];
	double r3z = x3[3] / temp2 * x3[0];
	temp2 = sqrt(x4[1] * x4[1] + x4[2] * x4[2] + x4[3] * x4[3]);
	double r4x = x4[1] / temp2 * x4[0];
	double r4y = x4[2] / temp2 * x4[0];
	double r4z = x4[3] / temp2 * x4[0];

	double rx = cintcoord(t, r1x, r2x, r3x, r4x);
	double ry = cintcoord(t, r1y, r2y, r3y, r4y);
	double rz = cintcoord(t, r1z, r2z, r3z, r4z);

	double rang = sqrt(rx*rx + ry * ry + rz * rz);

	if (rang <= 0.0000001&&rang >= -0.0000001)return 0.0;
	else return rz / rang;
}

void ccube(int ii) {

	if (ii == 4) {
		glBegin(GL_POLYGON);
		glVertex3f(cube[0][0], cube[0][1], cube[0][2]);
		glVertex3f(cube[1][0], cube[1][1], cube[1][2]);
		glVertex3f(cube[2][0], cube[2][1], cube[2][2]);
		glVertex3f(cube[3][0], cube[3][1], cube[3][2]);
		glEnd();
	}
	else if (ii == 5) {

		glBegin(GL_POLYGON);
		glVertex3f(cube[4][0], cube[4][1], cube[4][2]);
		glVertex3f(cube[5][0], cube[5][1], cube[5][2]);
		glVertex3f(cube[6][0], cube[6][1], cube[6][2]);
		glVertex3f(cube[7][0], cube[7][1], cube[7][2]);
		glEnd();
	}
	else if (ii == 3) {
		glBegin(GL_POLYGON);
		glVertex3f(cube[3][0], cube[3][1], cube[3][2]);
		glVertex3f(cube[0][0], cube[0][1], cube[0][2]);
		glVertex3f(cube[4][0], cube[4][1], cube[4][2]);
		glVertex3f(cube[7][0], cube[7][1], cube[7][2]);
		glEnd();

	}
	else {
		glBegin(GL_POLYGON);
		glVertex3f(cube[ii][0], cube[ii][1], cube[ii][2]);
		glVertex3f(cube[ii + 1][0], cube[ii + 1][1], cube[ii+ 1][2]);
		glVertex3f(cube[ii + 5][0], cube[ii + 5][1], cube[ii + 5][2]);
		glVertex3f(cube[ii + 4][0], cube[ii + 4][1], cube[ii + 4][2]);
		glEnd();
	}

}

void ccube2() {
		glBegin(GL_POLYGON);
		glVertex3f(cube2[0][0], cube2[0][1], cube2[0][2]);
		glVertex3f(cube2[1][0], cube2[1][1], cube2[1][2]);
		glVertex3f(cube2[2][0], cube2[2][1], cube2[2][2]);
		glVertex3f(cube2[3][0], cube2[3][1], cube2[3][2]);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(cube2[4][0], cube2[4][1], cube2[4][2]);
		glVertex3f(cube2[5][0], cube2[5][1], cube2[5][2]);
		glVertex3f(cube2[6][0], cube2[6][1], cube2[6][2]);
		glVertex3f(cube2[7][0], cube2[7][1], cube2[7][2]);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(cube2[3][0], cube2[3][1], cube2[3][2]);
		glVertex3f(cube2[0][0], cube2[0][1], cube2[0][2]);
		glVertex3f(cube2[4][0], cube2[4][1], cube2[4][2]);
		glVertex3f(cube2[7][0], cube2[7][1], cube2[7][2]);
		glEnd();

		for(int ii=0;ii<3;ii++){
		glBegin(GL_POLYGON);
		glVertex3f(cube2[ii][0], cube2[ii][1], cube2[ii][2]);
		glVertex3f(cube2[ii + 1][0], cube2[ii + 1][1], cube2[ii + 1][2]);
		glVertex3f(cube2[ii + 5][0], cube2[ii + 5][1], cube2[ii + 5][2]);
		glVertex3f(cube2[ii + 4][0], cube2[ii + 4][1], cube2[ii + 4][2]);
		glEnd();
	}

}




void init() {

	float light_amb[4] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diff[4] = { 0.82, 0.82, 0.82, 1.0 };
	float light_spec[4] = { 0.90, 0.90, 0.90, 1.0 };
	float light_shin = 50.8;

	glClearColor(0.4, 0.4, 0.4, 1.0);
	glClearDepth(1.0);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);


	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diff);

	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_spec);

	glLightf(GL_LIGHT0, GL_SHININESS, light_shin);
	glLightf(GL_LIGHT1, GL_SHININESS, light_shin);

}

void display() {
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadGlobalCoord();
	float light_pos0[] = { 300.0, -100.0, 200.0, 1.0 };
	float light_pos1[] = { -300.0, 500.0, -200.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);

	float ambient[4] = { 0.1, 0.2, 0.0, 1.0 };
	float diffuse[4] = { 0.4, 0.4, 0.0, 1.0 };
	float specular[4] = { 0.5, 0.4, 0.0, 1.0 };
	float shininess = 35;



	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	for (int i = 1;i < N_cs;i++) {
		for (int l = 0;l < M;l++) {
			double t = (double)l / M;
			for (int j = 0;j < N_cp + 3;j++) {
				pxt[j] = cintcoord(t, px[i - 1][j], px[i][j], px[i + 1][j], px[i + 2][j]);
				pyt[j] = cintcoord(t, py[i - 1][j], py[i][j], py[i + 1][j], py[i + 2][j]);
			}

			for (int j = 1;j < N_cp + 1;j++) {
				for (int k = 0;k < N;k++) {
					double s = (double)k / N;
					if (i != 1 || l != 0) {
						px2tt[(j - 1)*N + k] = px2t[(j - 1)*N + k];
						py2tt[(j - 1)*N + k] = py2t[(j - 1)*N + k];
						pz2tt[(j - 1)*N + k] = pz2t[(j - 1)*N + k];
					}
					px2t[(j - 1)*N + k] = bfunc(s, pxt[j - 1], pxt[j], pxt[j + 1], pxt[j + 2]);
					py2t[(j - 1)*N + k] = bfunc(s, pyt[j - 1], pyt[j], pyt[j + 1], pyt[j + 2]);
				}
			}
			if (i != 1 || l != 0) {
				px2tt[N_cp*N] = px2t[N_cp*N];
				py2tt[N_cp*N] = py2t[N_cp*N];
				pz2tt[N_cp*N] = pz2t[N_cp*N];
			}
			px2t[N_cp*N] = px2t[0];
			py2t[N_cp*N] = py2t[0];
			/*
			for (int k = 0;k < N_cp*N+1;k++) {

			glBegin(GL_POINTS);
			if (k == N_cp * N-3 )glColor3f(1,1,1);
			else glColor3f(0, 0, 1);
			glVertex3f(px2t[k], 0, py2t[k]);
			glEnd();
			}*/



			afscale = cintcoord(t, scaling[i - 1], scaling[i], scaling[i + 1], scaling[i + 2]);

			afposition[0] = cintcoord(t, position[i - 1][0], position[i][0], position[i + 1][0], position[i + 2][0]);
			afposition[1] = cintcoord(t, position[i - 1][1], position[i][1], position[i + 1][1], position[i + 2][1]);
			afposition[2] = cintcoord(t, position[i - 1][2], position[i][2], position[i + 1][2], position[i + 2][2]);


			afrot[0] = intq0(t, rotation[i - 1], rotation[i], rotation[i + 1], rotation[i + 2]);
			afrot[1] = intq1(t, rotation[i - 1], rotation[i], rotation[i + 1], rotation[i + 2]);
			afrot[2] = intq2(t, rotation[i - 1], rotation[i], rotation[i + 1], rotation[i + 2]);
			afrot[3] = intq3(t, rotation[i - 1], rotation[i], rotation[i + 1], rotation[i + 2]);



			double q[4];
			q[0] = cos(afrot[0] / 2);
			q[1] = afrot[1] * sin(afrot[0] / 2);
			q[2] = afrot[2] * sin(afrot[0] / 2);
			q[3] = afrot[3] * sin(afrot[0] / 22);


			double qi[4];
			double temp = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

			qi[0] = q[0] * (-1) / temp;
			qi[1] = q[1] / temp;
			qi[2] = q[2] / temp;
			qi[3] = q[3] / temp;

			for (int k = 0;k < N_cp*N + 1;k++) {

				double tem[4];
				double ey[4];


				ey[0] = 0;
				ey[1] = px2t[k];
				ey[2] = 0;
				ey[3] = py2t[k];
				tem[0] = ey[0] * q[0] - ey[1] * q[1] - ey[2] * q[2] - ey[3] * q[3];//q*ey
				tem[1] = ey[0] * q[1] + q[0] * ey[1] + q[2] * ey[3] - q[3] * ey[2];
				tem[2] = ey[0] * q[2] + q[0] * ey[2] + q[3] * ey[1] - q[1] * ey[3];
				tem[3] = ey[0] * q[3] + q[0] * ey[3] + q[1] * ey[2] - q[2] * ey[1];

				double te[4];
				te[0] = qi[0] * tem[0] - qi[1] * tem[1] - qi[2] * tem[2] - qi[3] * tem[3];//tem*qi
				te[1] = qi[0] * tem[1] + tem[0] * qi[1] + tem[2] * qi[3] - tem[3] * qi[2];
				te[2] = qi[0] * tem[2] + tem[0] * qi[2] + tem[3] * qi[1] - tem[1] * qi[3];
				te[3] = qi[0] * tem[3] + tem[0] * qi[3] + tem[1] * qi[2] - tem[2] * qi[1];

				te[1] *= afscale;
				te[2] *= afscale;
				te[3] *= afscale;

				te[1] += afposition[0];
				te[2] += afposition[1];
				te[3] += afposition[2];

				px2t[k] = te[1];
				py2t[k] = te[2];
				pz2t[k] = te[3];
			}
			for (int k = 0;(i != 1 || l != 0) && k < N_cp*N;k++) {

				glBegin(GL_TRIANGLES);
				glVertex3f(px2t[k], py2t[k], pz2t[k]);

				glVertex3f(px2t[k + 1], py2t[k + 1], pz2t[k + 1]);

				glVertex3f(px2tt[k], py2tt[k], pz2tt[k]);
				glEnd();

				glBegin(GL_TRIANGLES);


				glVertex3f(px2tt[k], py2tt[k], pz2tt[k]);
				glVertex3f(px2t[k + 1], py2t[k + 1], pz2t[k + 1]);
				glVertex3f(px2tt[k + 1], py2tt[k + 1], pz2tt[k + 1]);
				glEnd();

			}


		}
	}
	

	//bronze
	cube2[0][0] = 0; cube2[0][1] = 0; cube2[0][2] = 15;
	cube2[1][0] = 0; cube2[1][1] = 0; cube2[1][2] = 18;
	cube2[2][0] = 0; cube2[2][1] = 3; cube2[2][2] = 18;
	cube2[3][0] = 0; cube2[3][1] = 3; cube2[3][2] = 15;
	cube2[4][0] = 3; cube2[4][1] = 0; cube2[4][2] = 15;
	cube2[5][0] = 3; cube2[5][1] = 0; cube2[5][2] = 18;
	cube2[6][0] = 3; cube2[6][1] = 3; cube2[6][2] = 18;
	cube2[7][0] = 3; cube2[7][1] = 3; cube2[7][2] = 15;

	
	float ambient1[4] = { 0.2125, 0.1275, 0.054, 1.0 };
	float diffuse1[4] = { 0.714, 0.4284, 0.18144, 1.0 };
	float specular1[4] = { 0.393548, 0.271906, 0.166721, 1.0 };
	float shininess1 = 0.2*128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess1);

	glPushMatrix();
	glTranslatef(10, 10, 5);
	glutSolidCube(3);
	glPopMatrix();

	float ambient2[4] = { 0.25, 0.25, 0.25, 1.0 };
	float diffuse2[4] = { 0.4, 0.4, 0.4, 1.0 };
	float specular2[4] = { 0.774597, 0.774597, 0.774597, 1.0 };
	float shininess2 = 0.6 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess2);
	
	glPushMatrix();
	glTranslatef(10, 10, 10);
	glutSolidCube(3);
	glPopMatrix();


	//gold
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 3;j++) {
			cube2[i][j] -= 8;
		}
	}

	float ambient3[4] = { 0.24725,0.1995,	0.0745, 1.0 };
	float diffuse3[4] = { 0.75164,	0.60648,	0.22648, 1.0 };
	float specular3[4] = { 0.628281	,0.555802,	0.366065, 1.0 };
	float shininess3 = 0.4 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular3);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess3);
	
	glPushMatrix();
	glTranslatef(10, 10, 15);
	glutSolidCube(3);
	glPopMatrix();

	//black plastic
	for (int i = 0;i < 8;i++) {
			cube2[i][0] += 4;
		
	}

	float ambient4[4] = { 0.0,0.0,	0.0, 1.0 };
	float diffuse4[4] = { 0.01,0.01,0.01, 1.0 };
	float specular4[4] = { 0.50,	0.50,	0.50, 1.0 };
	float shininess4 = 0.25 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular4);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess4);
	
	glPushMatrix();
	glTranslatef(10, 10, 20);
	glutSolidCube(3);
	glPopMatrix();

	//yellow plastic
	for (int i = 0;i < 8;i++) {
		cube2[i][0] += 4;
	}

	float ambient5[4] = { 0.0,0.0,	0.0, 1.0 };
	float diffuse5[4] = { 0.5,0.5,0.0, 1.0 };
	float specular5[4] = { 0.60,	0.60,	0.50, 1.0 };
	float shininess5 = 0.25 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient5);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular5);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess5);
	
	glPushMatrix();
	glTranslatef(10, 10, 25);
	glutSolidCube(3);
	glPopMatrix();

	//green rubber
	for (int i = 0;i < 8;i++) {
		cube2[i][0] += 4;
	}
	

	float ambient6[4] = { 0.0,0.05,	0.0, 1.0 };
	float diffuse6[4] = { 0.4,0.5,0.4, 1.0 };
	float specular6[4] = { 0.04,	0.70,	0.04, 1.0 };
	float shininess6 = 0.78125 * 128;

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient6);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse6);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular6);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess6);

	glPushMatrix();
	glTranslatef(10, 10, 30);
	glutSolidCube(3);
	glPopMatrix();


	float ambient0[4] = { 0.0, 0.1, 0.1, 0 };
	float diffuse0[4] = { 0.0, 0.3, 0.3, 0 };
	float specular0[4] = { 0.0, 0.5, 0.5, 0 };
	float shininess0 = 35;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular0);

	if (eye[1] < -18) {
		if (eye[0] < -5) {
			if (eye[2] < -5) {
				ccube(1);
				ccube(2);
				ccube(5);
				ccube(0);
				ccube(4);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(2);
				ccube(3);
				ccube(5);
				ccube(0);
				ccube(1);
				ccube(4);
			}
			else {
				ccube(2);
				ccube(5);
				ccube(1);
				ccube(3);
				ccube(4);
				ccube(0);
			}
		}
		else if (eye[0] > 5) {
			if (eye[2] < -5) {
				ccube(2);
				ccube(1);
				ccube(4);
				ccube(3);
				ccube(0);
				ccube(5);
			}
			else if (eye[2] > 5) {
				ccube(2);
				ccube(3);
				ccube(4);
				ccube(1);
				ccube(0);
				ccube(5);
			}
			else {
				ccube(2);
				ccube(4);
				ccube(1);
				ccube(3);
				ccube(0);
				ccube(5);
			}
		}
		else  {
			if (eye[2] < -5) {
				ccube(1);
				ccube(2);
				ccube(4);
				ccube(5);
				ccube(0);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(3);
				ccube(2);
				ccube(4);
				ccube(5);
				ccube(0);
				ccube(1);
			}
			else {
				ccube(2);
				ccube(1);
				ccube(4);
				ccube(5);
				ccube(3);
				ccube(0);
			}
		}
	}
	else if (eye[1] > -8) {
		if (eye[0] < -5) {
			if (eye[2] < -5) {
				ccube(1);
				ccube(0);
				ccube(5);
				ccube(2);
				ccube(4);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(0);
				ccube(3);
				ccube(5);
				ccube(2);
				ccube(1);
				ccube(4);
			}
			else {
				ccube(0);
				ccube(5);
				ccube(1);
				ccube(3);
				ccube(4);
				ccube(2);
			}
		}
		else if (eye[0] > 5) {
			if (eye[2] < -5) {
				ccube(0);
				ccube(1);
				ccube(4);
				ccube(3);
				ccube(2);
				ccube(5);
			}
			else if (eye[2] > 5) {
				ccube(0);
				ccube(3);
				ccube(4);
				ccube(1);
				ccube(2);
				ccube(5);
			}
			else {
				ccube(0);
				ccube(4);
				ccube(1);
				ccube(3);
				ccube(2);
				ccube(5);
			}
		}
		else {
			if (eye[2] < -5) {
				ccube(1);
				ccube(0);
				ccube(4);
				ccube(5);
				ccube(2);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(3);
				ccube(0);
				ccube(4);
				ccube(5);
				ccube(2);
				ccube(1);
			}
			else {
				ccube(0);
				ccube(1);
				ccube(4);
				ccube(5);
				ccube(3);
				ccube(2);
			}
		}
	}
	else {
		if (eye[0] < -5) {
			if (eye[2] < -5) {
				ccube(1);
				ccube(5);
				ccube(2);
				ccube(0);
				ccube(4);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(5);
				ccube(3);
				ccube(2);
				ccube(0);
				ccube(1);
				ccube(4);
			}
			else {
				ccube(5);
				ccube(2);
				ccube(1);
				ccube(3);
				ccube(0);
				ccube(4);
			}
		}
		else if (eye[0] > 5) {
			if (eye[2] < -5) {
				ccube(4);
				ccube(1);
				ccube(0);
				ccube(2);
				ccube(3);
				ccube(5);
			}
			else if (eye[2] > 5) {
				ccube(4);
				ccube(3);
				ccube(2);
				ccube(0);
				ccube(1);
				ccube(5);
			}
			else {
				ccube(4);
				ccube(2);
				ccube(1);
				ccube(3);
				ccube(0);
				ccube(5);
			}
		}
		else {
			if (eye[2] < -5) {
				ccube(1);
				ccube(2);
				ccube(4);
				ccube(5);
				ccube(0);
				ccube(3);
			}
			else if (eye[2] > 5) {
				ccube(3);
				ccube(2);
				ccube(4);
				ccube(5);
				ccube(0);
				ccube(1);
			}
			else {
				ccube(2);
				ccube(1);
				ccube(4);
				ccube(5);
				ccube(3);
				ccube(0);
			}
		}
	}

	glDisable(GL_BLEND);
	
	


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
	if (templine[0] == 'B')BSPLINE = 1;
	else BSPLINE = 0;
	fscanf(fp, "%d", &N_cs);
	fscanf(fp, "%d", &N_cp);
	printf("%d %d", N_cs, N_cp);
	for (int i = 1;i < N_cs + 1;i++) {
		for (int j = 1;j < N_cp + 1;j++) {
			fscanf(fp, "%lf %lf\n", &px[i][j], &py[i][j]);
		}
		px[i][0] = px[i][N_cp];
		py[i][0] = py[i][N_cp];
		px[i][N_cp + 1] = px[i][1];
		py[i][N_cp + 1] = py[i][1];
		px[i][N_cp + 2] = px[i][2];
		py[i][N_cp + 2] = py[i][2];
		fscanf(fp, "%lf\n", &scaling[i]);
		fscanf(fp, "%lf %lf %lf %lf\n", &rotation[i][0], &rotation[i][1], &rotation[i][2], &rotation[i][3]);
		fscanf(fp, "%lf %lf %lf\n", &position[i][0], &position[i][1], &position[i][2]);
	}

	for (int j = 0;j < N_cp + 3;j++) {
		px[0][j] = 2 * px[1][j] - px[2][j];
		py[0][j] = 2 * py[1][j] - py[2][j];
	}
	scaling[0] = 2 * scaling[1] - scaling[2];
	rotation[0][0] = 2 * rotation[1][0] - rotation[2][0];
	rotation[0][1] = 2 * rotation[1][1] - rotation[2][1];
	rotation[0][2] = 2 * rotation[1][2] - rotation[2][2];
	rotation[0][3] = 2 * rotation[1][3] - rotation[2][3];
	position[0][0] = 2 * position[1][0] - position[2][0];
	position[0][1] = 2 * position[1][1] - position[2][1];
	position[0][2] = 2 * position[1][2] - position[2][2];

	for (int j = 0;j < N_cp + 3;j++) {
		px[N_cs + 1][j] = 2 * px[N_cs][j] - px[N_cs - 1][j];
		py[N_cs + 1][j] = 2 * py[N_cs][j] - py[N_cs - 1][j];
	}
	scaling[N_cs + 1] = 2 * scaling[N_cs] - scaling[N_cs - 1];
	rotation[N_cs + 1][0] = 2 * rotation[N_cs][0] - rotation[N_cs - 1][0];
	rotation[N_cs + 1][1] = 2 * rotation[N_cs][1] - rotation[N_cs - 1][1];
	rotation[N_cs + 1][2] = 2 * rotation[N_cs][2] - rotation[N_cs - 1][2];
	rotation[N_cs + 1][3] = 2 * rotation[N_cs][3] - rotation[N_cs - 1][3];
	position[N_cs + 1][0] = 2 * position[N_cs][0] - position[N_cs - 1][0];
	position[N_cs + 1][1] = 2 * position[N_cs][1] - position[N_cs - 1][1];
	position[N_cs + 1][2] = 2 * position[N_cs][2] - position[N_cs - 1][2];



	//	for (int i = 0;i < N_cs;i++) {
	//		for (int j = 0;j < N_cp;j++) {
	//			printf("%f %f\n", px[i][j], py[i][j]);
	//		}y6
	//		printf("%f\n", scaling[i]);
	//		printf("%f %f %f %f\n", rotation[i][0], rotation[i][1], rotation[i][2], rotation[i][3]);
	//		printf("%f %f %f\n", position[i][0], position[i][1], position[i][2]);



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(50, 0);
	glutCreateWindow("Example");
	init();


	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, Timer, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(glutMouse);
	glutMotionFunc(glutMotion);

	glutMainLoop();

	return 0;
}
