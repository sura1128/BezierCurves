// CS3241Lab4.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
	int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = false;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;
bool erase = false;
	
void drawRightArrow()
{
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0,0);
		glVertex2f(100,0);
		glVertex2f(95,5);
		glVertex2f(100,0);
		glVertex2f(95,-5);
	glEnd();
}

void showControlLines() {
	glPushMatrix();
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < nPt; i++) {
		glVertex2d(ptList[i].x, ptList[i].y);
	}
	glEnd();
	glPopMatrix();
	
}

void eraseControlLines() {
	glPushMatrix();
	showControlLines();
	glPopMatrix();
}

void eraseControlPoints() {
	glPushMatrix();
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i<nPt; i++)
	{
		glVertex2d(ptList[i].x, ptList[i].y);
	}
	glEnd();
	eraseControlLines();
	glPointSize(1);
	glPopMatrix();	
	
}


int getFactorial(int x) {
	int prod = 1;
	for (int i = x; i > 0; i--) {
		prod *= i;
	}
	return prod;
}

float getCombination(int n, int i) {
	return getFactorial(n) / (getFactorial(n - i)*getFactorial(i));
}
void drawCurve(Point arr[]) {
	int n = 3;
	Point curve[34];

	Point temp;
	
	curve[0] = arr[0];
	double t = 0.03125;
	for (int j = 1; j <= 33; j++) {
		temp.x = 0;
		temp.y = 0;
		for (int i = 0; i <=n; i++) {			
			temp.x += (getCombination(n,i)*pow((1 - t), n-i)*pow(t, i))*arr[i].x;
			temp.y += (getCombination(n, i)*pow((1 - t), n-i))*(pow(t, i))*arr[i].y;
		}
		t += 0.03125;
		curve[j] = temp;
	}

	glColor3f(0, 0, 1);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 34; i++) {
		glVertex2d(curve[i].x, curve[i].y);
	}
	glEnd();
	
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if(displayControlPoints)
	{

		glPointSize(5);
		glBegin(GL_POINTS);
		for(int i=0;i<nPt; i++)
		{
				glColor3f(0,0,0);
				glVertex2d(ptList[i].x,ptList[i].y);
		}
		glEnd();
		glPointSize(1);

	}

	if(displayControlLines)
	{
		glColor3f(0,1,0);
		showControlLines();
	}

	if (erase) {
		glColor3f(1, 1, 1);
		eraseControlPoints();
	}

	if (C1Continuity) {
		
		drawCurve(ptList);
		/*for (int i = 0; i < (nPt-3); i++) {
			int p1 = 0, p2 = 1, p3 = 2, p4 = 3;
			Point temp[] = { ptList[p1], ptList[p2], ptList[p3], ptList[p4] };
			drawCurve(ptList);
		}*/
	}

	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{

	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;

	if(nPt>MAXPTNO)
	{
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}

	for(int i=0;i<nPt;i++)
	{
		file >> ptList[i].x;
		file >> ptList[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;

	for(int i=0;i<nPt;i++)
	{
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
		break;

		case 'o':
		case 'O':
			displayObjects = !displayObjects;
		break;

		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
		break;

		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
		break;

		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
		break;

		case 'e':
		case 'E':
			// Do something to erase all the control points added
			erase = !erase;
		break;

		case 27:
			exit(0);
		break;

		default:
		break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
	{
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x=x;
		ptList[nPt].y=y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "ESC: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
