#include<stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <algorithm>

GLdouble rotMatrix[16] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

/* set global variables */
int width, height;

bool leftButton = false;
GLfloat mousePosX, mousePosY;
bool zoom = false;
float fov=45.0;
float scal=0.2;
float mrx=0.0;
float mry=0.0;
float mrz=0.0;
float mrang=0.0;
float dolly=60.0;
/* vectors that makes the rotation and translation of the cube */
float eye[3] = { 0.0f, 0.0f, dolly};
float up[3] = { 0.0f, 1.0f, 0.0f};
float ori[3] = { 0.0f, 0.0f, 0.0f };
float rot[3] = { 0.0f, 0.0f, 0.0f };
float q3[4]={1,0.0,0.0,0.0};
float q2[4]={1,0.0,0.0,0.0};
float legAng = 15.0;
float legR = 1;
float antR = 1;
float left = 0.0;
void loadGlobalCoord()
{
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], ori[0], ori[1], ori[2], up[0], up[1], up[2]);
	glMultMatrixd(rotMatrix);
}

//------------------------------------------------------------------------
// Moves the screen based on mouse pressed button
//------------------------------------------------------------------------

void glutMotion(int x, int y)
{
	if ( leftButton ) {

float x2=-(float)x+250;
float y2=-(float)y+250;

float temp5=x2*x2+y2*y2;
if(temp5>=199*199)x2=x2/sqrt(temp5)*199;
if(temp5>=199*199)y2=y2/sqrt(temp5)*199;


float x1=-(float)mousePosX+250;
float y1=-(float)mousePosY+250;
temp5=x1*x1+y1*y1;
if(temp5>=199*199)x1=x1/sqrt(temp5)*199;
if(temp5>=199*199)y1=y1/sqrt(temp5)*199;

float z2=sqrt(40000-x2*x2-y2*y2);
float z1=sqrt(40000-x1*x1-y1*y1);
float axis[3]= {(y1*z2-y2*z1)/40000,(z1*x2-z2*x1)/40000,(x1*y2-x2*y1)/40000};
float theta = atan2(sqrt(1-(x1*x2/40000+y1*y2/40000+z1*z2/40000)*(x1*x2/40000+y1*y2/40000+z1*z2/40000)),(x1*x2/40000+y1*y2/40000+z1*z2/40000));

float temp2= sqrt(axis[0]*axis[0]+axis[1]*axis[1]+axis[2]*axis[2]);
float rx=axis[0]/temp2;
float ry=axis[1]/temp2;
float rz=axis[2]/temp2;
float rang=theta;


float q1[4];
q1[0]=cos(rang/2);
q1[1]=rx*sin(rang/2);
q1[2]=ry*sin(rang/2);
q1[3]=rz*sin(rang/2);

q3[0]=q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2]-q1[3]*q2[3];//q1q2
q3[1]=q1[0]*q2[1]+q2[0]*q1[1]+q1[2]*q2[3]-q1[3]*q2[2];
q3[2]=q1[0]*q2[2]+q2[0]*q1[2]+q1[3]*q2[1]-q1[1]*q2[3];
q3[3]=q1[0]*q2[3]+q2[0]*q1[3]+q1[1]*q2[2]-q1[2]*q2[1];





float qi3[4];
float temp = q3[0]*q3[0]+q3[1]*q3[1]+q3[2]*q3[2]+q3[3]*q3[3];

qi3[0]=q3[0]*(-1)/temp;
qi3[1]=q3[1]/temp;
qi3[2]=q3[2]/temp;
qi3[3]=q3[3]/temp;

float tem[4];
float ey[4];


ey[0]=0;
ey[1]=0;
ey[2]=0;
ey[3]=1;
tem[0]=ey[0]*qi3[0]-ey[1]*qi3[1]-ey[2]*qi3[2]-ey[3]*qi3[3];//qi3*eye
tem[1]=ey[0]*qi3[1]+qi3[0]*ey[1]+qi3[2]*ey[3]-qi3[3]*ey[2];
tem[2]=ey[0]*qi3[2]+qi3[0]*ey[2]+qi3[3]*ey[1]-qi3[1]*ey[3];
tem[3]=ey[0]*qi3[3]+qi3[0]*ey[3]+qi3[1]*ey[2]-qi3[2]*ey[1];

float te[4];
te[0]=q3[0]*tem[0]-q3[1]*tem[1]-q3[2]*tem[2]-q3[3]*tem[3];//tem*q3
te[1]=q3[0]*tem[1]+tem[0]*q3[1]+tem[2]*q3[3]-tem[3]*q3[2];
te[2]=q3[0]*tem[2]+tem[0]*q3[2]+tem[3]*q3[1]-tem[1]*q3[3];
te[3]=q3[0]*tem[3]+tem[0]*q3[3]+tem[1]*q3[2]-tem[2]*q3[1];



float ey2[4];
ey2[0]=0;
ey2[1]=0;
ey2[2]=1;
ey2[3]=0;
tem[0]=ey2[0]*qi3[0]-ey2[1]*qi3[1]-ey2[2]*qi3[2]-ey2[3]*qi3[3];//qi3*eye
tem[1]=ey2[0]*qi3[1]+qi3[0]*ey2[1]+qi3[2]*ey2[3]-qi3[3]*ey2[2];
tem[2]=ey2[0]*qi3[2]+qi3[0]*ey2[2]+qi3[3]*ey2[1]-qi3[1]*ey2[3];
tem[3]=ey2[0]*qi3[3]+qi3[0]*ey2[3]+qi3[1]*ey2[2]-qi3[2]*ey2[1];

float te2[4];
te2[0]=q3[0]*tem[0]-q3[1]*tem[1]-q3[2]*tem[2]-q3[3]*tem[3];//tem*q3
te2[1]=q3[0]*tem[1]+tem[0]*q3[1]+tem[2]*q3[3]-tem[3]*q3[2];
te2[2]=q3[0]*tem[2]+tem[0]*q3[2]+tem[3]*q3[1]-tem[1]*q3[3];
te2[3]=q3[0]*tem[3]+tem[0]*q3[3]+tem[1]*q3[2]-tem[2]*q3[1];

eye[0]=dolly*te[1];
eye[1]=dolly*te[2];
eye[2]=dolly*te[3];
up[0]=te2[1];
up[1]=te2[2];
up[2]=te2[3];



	loadGlobalCoord();
	}
	return;
}

//------------------------------------------------------------------------
// Function that handles mouse input
//------------------------------------------------------------------------
void glutMouse(int button, int state, int x, int y)
{
	switch ( button )
	{
		case GLUT_LEFT_BUTTON:
			if ( state == GLUT_DOWN )
			{
				mousePosX = x;
				mousePosY = y;
				q2[0]=q3[0];
				q2[1]=q3[1];
				q2[2]=q3[2];
				q2[3]=q3[3];
				leftButton = true;
			}
			else if ( state == GLUT_UP )
			{
				leftButton = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:break;
		case 3:
			if(state == GLUT_UP ){
				float temp=dolly;
				if(zoom&&fov>5){fov-=1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, 1, .1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();}
				if(!zoom&&dolly>21){dolly-=3;
eye[0]=dolly*eye[0]/temp;
eye[1]=dolly*eye[1]/temp;
eye[2]=dolly*eye[2]/temp;}
			}
			else if(state == GLUT_DOWN ){ break;
			}
			break;
		case 4:
			if(state == GLUT_UP ){
				float temp=dolly;
				if(zoom&&fov<90){fov+=1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, 1, .1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
	}
				if(!zoom&&dolly<450){dolly+=3;
eye[0]=dolly*eye[0]/temp;
eye[1]=dolly*eye[1]/temp;
eye[2]=dolly*eye[2]/temp;}
			}
			else if(state == GLUT_DOWN ){
				break;
			}

			break;

			
			
			
			
			
			
			
		default:break;
	}
	return;
}


void ant(float  x, float y, float z,float ang,float r,float g,float b) {

	glPushMatrix();
	glScalef(scal,scal,scal);
	glTranslatef(x,y,z);
	glColor3f(   1.0,  1.0,  1.0 );
	glPushMatrix();//body
	{
		glScalef(0.5,0.7,0.5);
		glutSolidSphere(15.0f,200,40);
		glPopMatrix();
		}
	
	glPushMatrix();//head
	{
	glColor3f(   0.3,  0.3,  0.3 );
		glScalef(0.7,0.7,0.5);
		glTranslatef(21.0,0.0,0.0);
		glutSolidSphere(15.0f,200,40);
			glPushMatrix();
			glRotatef( 60.0,0.0,1.0,0.0);
			glColor3f(   1.0,  1.0,  1.0 );
			glRotatef( 20.0,1.0,0.0,0.0);
			glutSolidCylinder(2,16.0,200,40);
			glRotatef( -40.0,1.0,0.0,0.0);
			glutSolidCylinder(2,16.0,200,40);
			glPopMatrix();
			
		glPopMatrix();
		}
	glPushMatrix();//tail
	{
		glColor3f(   1.0,  1.0,  1.0 );
		glScalef(1.2,0.7,0.5);
		glTranslatef(-20.0,0.0,0.0);
		glutSolidSphere(15.0f,200,40);
		glPopMatrix();
		}
	glPushMatrix();//left middle leg
	{
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-30.0,1.0,0.0,0.0);
		glRotatef(left,0.0,0.0,1.0);
		glRotatef(-legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,20.0,200,40);
		glPushMatrix();
		{
			glRotatef(-left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,20.0);
			glRotatef(-100.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,40.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	
	glPushMatrix();//left front leg
	{
		glRotatef(-40.0,0.0,0.0,1.0);
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-60.0,1.0,0.0,0.0);
		glRotatef(-left,0.0,0.0,1.0);
		glRotatef(-legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,30.0,200,40);
		glPushMatrix();
		{
			glRotatef(left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,30.0);
			glRotatef(-90.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,30.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//left behind leg
	{
		glRotatef(60.0,0.0,0.0,1.0);
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-60.0,1.0,0.0,0.0);
		glRotatef(-left,0.0,0.0,1.0);
		glRotatef(-legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,30.0,200,40);
		glPushMatrix();
		{
			glRotatef(left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,30.0);
			glRotatef(-90.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,30.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//right middle leg
	{
		glRotatef(180.0,0.0,0.0,1.0);
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-30.0,1.0,0.0,0.0);
		glRotatef(-left,0.0,0.0,1.0);
		glRotatef(legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,20.0,200,40);
		glPushMatrix();
		{
			glRotatef(left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,20.0);
			glRotatef(-100.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,40.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//right front leg
	{
		glRotatef(220.0,0.0,0.0,1.0);
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-60.0,1.0,0.0,0.0);
		glRotatef(left,0.0,0.0,1.0);
		glRotatef(legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,30.0,200,40);
		glPushMatrix();
		{
			glRotatef(-left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,30.0);
			glRotatef(-90.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,30.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//right behind leg
	{
		glRotatef(120.0,0.0,0.0,1.0);
		glTranslatef(0.0,5.0,-5.0);
		glRotatef(-60.0,1.0,0.0,0.0);
		glRotatef(left,0.0,0.0,1.0);
		glRotatef(legAng,1.0,0.0,0.0);
		glutSolidCylinder(legR,30.0,200,40);
		glPushMatrix();
		{
			glRotatef(-left,0.0,0.0,1.0);
			glTranslatef(0.0,0.0,30.0);
			glRotatef(-90.0,1.0,0.0,0.0);
			glutSolidCylinder(legR,30.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//antenna right
	{
		glTranslatef(20.0,0.0,5.0);
		glRotatef(60,0.0,1.0,0.0);
		glRotatef(20.0,1.0,0.0,0.0);
		glutSolidCylinder(antR,30.0,200,40);
		glPushMatrix();
		{
			glTranslatef(0.0,0.0,30.0);
			glRotatef(90.0,0.0,1.0,0.0);
			glutSolidCylinder(antR,10.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPushMatrix();//antenna left
	{
		glTranslatef(20.0,0.0,5.0);
		glRotatef(60.0,0.0,1.0,0.0);
		glRotatef(-20.0,1.0,0.0,0.0);
		glutSolidCylinder(antR,30.0,200,40);
		glPushMatrix();
		{
			glTranslatef(0.0,0.0,30.0);
			glRotatef(90.0,0.0,1.0,0.0);
			glutSolidCylinder(antR,10.0,200,40);
			glPopMatrix();
			}
		glPopMatrix();
		}
	glPopMatrix();
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	loadGlobalCoord();
	
	//glRotatef(45,0,1,1);
	ant(0.0,0.0,0.0,0.0,0.0,0.0,0.0);	
	glutSwapBuffers();
}

void resize(int w, int h) {
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)w / (GLfloat)h, .1f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	float tempsize=sqrt(up[0]*up[0]+up[1]*up[1]+up[2]*up[2]);
	float rightv[3];
	float frontv[3]={ori[0]-eye[0],ori[1]-eye[1],ori[2]-eye[2]};
	
	float temp=dolly;
	rightv[0]=frontv[1]*up[2]-frontv[2]*up[1];
	rightv[1]=frontv[2]*up[0]-frontv[0]*up[2];
	rightv[2]=frontv[0]*up[1]-frontv[1]*up[0];
	switch (key) {
	case 98://b
	eye[0]-=ori[0];
	eye[1]-=ori[1];
	eye[2]-=ori[2];
	ori[0]=0;
	ori[1]=0;
	ori[2]=0;
	dolly=1800/fov;
	eye[0]=dolly*eye[0]/temp;
	eye[1]=dolly*eye[1]/temp;
	eye[2]=dolly*eye[2]/temp;
	break;
	case 97://a
	tempsize=sqrt(rightv[0]*rightv[0]+rightv[1]*rightv[1]+rightv[2]*rightv[2]);
	ori[0]+=rightv[0]/tempsize;
	eye[0]+=rightv[0]/tempsize;
	ori[1]+=rightv[1]/tempsize;
	eye[1]+=rightv[1]/tempsize;
	ori[2]+=rightv[2]/tempsize;
	eye[2]+=rightv[2]/tempsize;
	break;
	case 100://d
	tempsize=sqrt(rightv[0]*rightv[0]+rightv[1]*rightv[1]+rightv[2]*rightv[2]);
	ori[0]-=rightv[0]/tempsize;
	eye[0]-=rightv[0]/tempsize;
	ori[1]-=rightv[1]/tempsize;
	eye[1]-=rightv[1]/tempsize;
	ori[2]-=rightv[2]/tempsize;
	eye[2]-=rightv[2]/tempsize;
	break;

	case 115://s
	ori[0]+=up[0]/tempsize;
	eye[0]+=up[0]/tempsize;
	ori[1]+=up[1]/tempsize;
	eye[1]+=up[1]/tempsize;
	ori[2]+=up[2]/tempsize;
	eye[2]+=up[2]/tempsize;
	break;
	case 119://w
	ori[0]-=up[0]/tempsize;
	eye[0]-=up[0]/tempsize;
	ori[1]-=up[1]/tempsize;
	eye[1]-=up[1]/tempsize;
	ori[2]-=up[2]/tempsize;
	eye[2]-=up[2]/tempsize;
	break;

	case 113:
		zoom =!zoom;
		if(zoom)printf("zoommode\n");
		else printf("dollymode\n");
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}


unsigned timeStep = 1;
void Timer(int unused)
{
	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
	glutTimerFunc(timeStep, Timer, 0);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
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
