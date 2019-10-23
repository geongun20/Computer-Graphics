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

/* vectors that makes the rotation and translation of the cube */
//float eye[3] = { 0.0f, 100.0f, 100.0f };
//float eye[3] = { 100.0f, 0.0f, 0.0f };
float eye[3] = { 0.0f, 0.0f, 100.0f };
float ori[3] = { 0.0f, 0.0f, 0.0f };
float rot[3] = { 0.0f, 0.0f, 0.0f };
int left = 0;
GLfloat lightPos[]  = { 0.0f, 0.0f, 1.0f };
float legR = 1;
float antR = 1;
float legAng = 15.0;
int antang = 0;
int second = 0;
int movefront = 0;
void loadGlobalCoord()
{
	glLoadIdentity();
	gluLookAt(eye[0]+100*cos((double)second/20), eye[1]+100*sin((double)second/20), eye[2], ori[0]+0.2*3*movefront-0.2*150.0, ori[1], ori[2], 0, 0, 1);
	glMultMatrixd(rotMatrix);
}

//------------------------------------------------------------------------
// Moves the screen based on mouse pressed button
//------------------------------------------------------------------------

/*void glutMotion(int x, int y)
{
	if ( leftButton ) {
		float dx = x - mousePosX;
		float dy = y - mousePosY;

		mousePosX = x;
		mousePosY = y;

		ori[0] -= dx*0.04;
		ori[1] += dy*0.04;

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
				leftButton = true;
			}
			else if ( state == GLUT_UP )
			{
				leftButton = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:break;
		case 3:break;
		default:break;
	}
	return;
}
*/
void ant(float  x, float y, float z,float ang,float r,float g,float b) {

	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glTranslatef(x,y,z);
	glRotatef(ang,0.0,0.0,1.0);
	glColor3f(   r,  g,  b );
	glPushMatrix();//body
	{
		glScalef(0.5,0.7,0.5);
		glutSolidSphere(15.0f,200,40);
		glPopMatrix();
		}
	
	glPushMatrix();//head
	{
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
		glColor3f(   r,  g,  b );
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
		glRotatef(sin((double)second)*10+60,0.0,1.0,0.0);
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
		glRotatef(-sin((double)second)*10+60.0,0.0,1.0,0.0);
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

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	loadGlobalCoord();
	glPushMatrix();
	{
		glTranslatef(0.0,0.0,-4.0);
		glColor3f(   0.5,  0.5,  0.5 );
		glLineWidth(4.0);
		glBegin(GL_LINES);
		glVertex2f(-200.0f,0.0f);
		glVertex2f(200.0f,0.0f);
		glVertex2f(-200.0f,40.0f);
		glVertex2f(200.0f,40.0f);
		glVertex2f(-200.0f,-40.0f);
		glVertex2f(200.0f,-40.0f);
		glVertex2f(-200.0f,80.0f);
		glVertex2f(200.0f,80.0f);
		glVertex2f(-200.0f,-80.0f);
		glVertex2f(200.0f,-80.0f);
		glVertex2f(-200.0f,120.0f);
		glVertex2f(200.0f,120.0f);
		glVertex2f(-200.0f,-120.0f);
		glVertex2f(200.0f,-120.0f);
		glVertex2f(-200.0f,-160.0f);
		glVertex2f(200.0f,-160.0f);
		glVertex2f(-200.0f,160.0f);
		glVertex2f(200.0f,160.0f);
		
		glVertex2f(0.0f,200.0f);
		glVertex2f(0.0f,-200.0f);
		glVertex2f(40.0f,200.0f);
		glVertex2f(40.0f,-200.0f);
		glVertex2f(-40.0f,200.0f);
		glVertex2f(-40.0f,-200.0f);
		glVertex2f(80.0f,200.0f);
		glVertex2f(80.0f,-200.0f);
		glVertex2f(-80.0f,200.0f);
		glVertex2f(-80.0f,-200.0f);
		glVertex2f(120.0f,200.0f);
		glVertex2f(120.0f,-200.0f);
		glVertex2f(-120.0f,200.0f);
		glVertex2f(-120.0f,-200.0f);
		glVertex2f(160.0f,200.0f);
		glVertex2f(160.0f,-200.0f);
		glVertex2f(-160.0f,200.0f);
		glVertex2f(-160.0f,-200.0f);
		glEnd();
		glFinish();
		glColor3f(   0.3,  0.3,  0.3 );
		glBegin(GL_QUADS);
		glVertex2f(-200.0f,-200.0f);
		glVertex2f(200.0f,-200.0f);
		glVertex2f(200.0f,200.0f);
		glVertex2f(-200.0f,200.0f);
		glEnd();
		glFinish();
		glPopMatrix();
		}
	glPushMatrix();
	glTranslatef(140.0,30.0,-4.0);
	glColor3f(   0.5,  0.2,  0.5 );
	glutSolidCylinder(2.0,30.0,200,40);
	glTranslatef(0.0,-60.0,0.0);
	glutSolidCylinder(2.0,30.0,200,40);
	glPopMatrix();
		glBegin(GL_QUADS);
	glColor3f(   0.4,  0.1,  0.4 );
		glVertex3f(140.0f,30.0f,20.0f);
		glVertex3f(140.0f,30.0f,30.0f);
		glVertex3f(140.0f,-30.0f,30.0f);
		glVertex3f(140.0f,-30.0f,20.0f);
		glEnd();
		glFinish();
ant(3*movefront-150.0,10*sin((double)movefront/10)-40,0.0,5*sin((double)left/180*3.14-1.57),0.0,0.0,0.0);
ant(1.5*movefront-60.0,10*sin((double)movefront/10)+40,0.0,5*sin((double)left/180*3.14-1.57),0.2,0.0,0.0);
	glutSwapBuffers();
}
void timer(int value)
{
	second =  second+1;
	left = (left+60)%360;
	movefront = ((movefront+3)%300);
	/* call the display callback and forces the current window to be displayed */
	glutPostRedisplay();
	glutTimerFunc(1, timer, 1);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition( 50, 0 );
	glutCreateWindow("Example");
	glClearColor(1,1,1,1);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(glutMouse);
	//glutMotionFunc(glutMotion);
	glutTimerFunc(100,timer,1);
	glutMainLoop();

	return 0;
}
