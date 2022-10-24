
#include <iostream>
#include <glut.h>

using namespace std;

void display();
void reshape(int,int);

void init()
{
	//set background to black
	glClearColor(0.0,0.0,0.0,1.0);
}

int main(int argc,char**argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //display modes
	
	glutInitWindowPosition(200,100); //window postion
	glutInitWindowSize(400,400); //window size
	glutCreateWindow ("Ua-o_M1A1_T85"); //create the initialized window with name
	
	glutDisplayFunc(display); //initialize display function
	glutReshapeFunc(reshape); //initialize reshape function
	
	init();
	glutMainLoop();//loop at the functions

}

void display()
{
	
	glClear(GL_COLOR_BUFFER_BIT);//clear color
	
	glLoadIdentity();//clear transforms

	//Initials -> LAU

	glBegin(GL_LINE_LOOP);
		glVertex2f(-10.0f, -4.0f);
		glVertex2f(-4.0f, -4.0f);
		glVertex2f(-4.0f, -2.0f);
		glVertex2f(-8.0f, -2.0f);
		glVertex2f(-8.0f, 5.0f);
		glVertex2f(-10.0f, 5.0f);
	glEnd(); //letter L



	glBegin(GL_LINE_LOOP);
		glVertex2f(-3.0f, -4.0f);
		glVertex2f(-1.0f, -4.0f);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, -4.0f);
		glVertex2f(3.0f, -4.0f);
		glVertex2f(3.0f, 3.0f);
		glVertex2f(1.0f, 5.0f);
		glVertex2f(-1.0f, 5.0f);
		glVertex2f(-3.0f, 3.0f);
	glEnd();


	glBegin(GL_LINE_LOOP);
		glVertex2f(-1.0f, 1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(1.0f, 3.0f);
		glVertex2f(-1.0f, 3.0f);
	glEnd(); //letter A



	glBegin(GL_LINE_LOOP);
		glVertex2f(6.0f, -4.0f);
		glVertex2f(8.0f, -4.0f);
		glVertex2f(10.0f, -2.0f);
		glVertex2f(10.0f, 5.0f);
		glVertex2f(8.0f, 5.0f);
		glVertex2f(8.0f, -2.0f);
		glVertex2f(6.0f, -2.0f);
		glVertex2f(6.0f, 5.0f);
		glVertex2f(4.0f, 5.0f);
		glVertex2f(4.0f, -2.0f);			
	glEnd(); //letter U



	glutSwapBuffers(); //Send the scene to the screen
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	//size of the world
	gluOrtho2D(-11,11,-11,11);
	glMatrixMode(GL_MODELVIEW);
}