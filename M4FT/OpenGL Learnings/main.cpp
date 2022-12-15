#include <iostream>
#include <glut.h>
#include <Windows.h>
#include "imageloader.h"
#include <irr\irrKlang.h>

using namespace std;
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

void update(int);
GLuint loadTexture(Image* image);
void reshape(int, int);
void display();
void renderBitmapString(
	float x,
	float y,
	float z,
	void* font,
	char* string);

GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat directedLightPos[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 60.0 };

GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light[] = { 0.5, 0.5, 0.5 };
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat static_ambient[] = { 0.8, 0.8, 0.8, 1.0 };

//Emissions
GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };

float angle = 0; //rotation angle

GLuint _build1;
GLuint _build2;//The id of the texture
GLuint _textureMoon;
GLuint _textureBall;
GLUquadric* ball;
GLUquadric* moon;

void initRendering() {

	ISoundEngine* engine = createIrrKlangDevice();
	engine->play2D("guitar.wav", true);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	Image* image1 = loadBMP("bldg1.bmp");
	_build1 = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("bldg2.bmp");
	_build2 = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("moon.bmp");
	_textureMoon = loadTexture(image3);
	delete image3;

	Image* image_sky = loadBMP("skybox_night.bmp");
	_textureBall = loadTexture(image_sky);
	delete image_sky;

	ball = gluNewQuadric();
	moon = gluNewQuadric();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);

	glutCreateWindow("Ua-o_M2A2_Skyscrapers");
	initRendering();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000 / 60, update, 0); //add timer

	waveOutSetVolume(0, 1717986918); // set volume to Normal
	glutTimerFunc(1000 / 60, update, 0); //Add a timer


	glutMainLoop();
	return 0;
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//-------------Skybox---------------
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureBall);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient); //add ambient lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1, 1, 1); // needed to make the skybox visible
	gluQuadricTexture(ball, 1);
	glRotatef(180, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	gluSphere(ball, 1000, 128, 128);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
	glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess


	glTranslatef(0.0f, 0.0f, -15.0f);

	//--------------------------moon------------------------------------//
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureMoon);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	gluQuadricTexture(moon, 1);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
	glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(5.0f, 5.0f, -2.0f); //location
	glRotatef(angle, 0.0f, 1.0f, 1.0f); //rotation animation
	gluSphere(moon, 1.0, 24, 24);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building1-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(-6.5f, 2.0f, 3.0f);

	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.10f, -0.86f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.10f, -0.86f, 0.0f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.71f, 0.71f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-0.71f, 0.71f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//back
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//left
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.10f, -0.86f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);

	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(-0.48f, 0.48f, -0.72f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-0.71f, 0.71f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.5f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building2-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(-4.5f, 0.0f, 2.0f);

	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.10f, -0.86f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.10f, -0.86f, 0.0f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.71f, 0.71f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-0.71f, 0.71f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//back
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//left
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.10f, -0.86f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);

	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(-0.48f, 0.48f, -0.72f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(-0.71f, 0.71f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.5f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//-----------Building3-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(-2.5f, -2.0f, 2.0f);

	glBegin(GL_QUADS);
	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-2.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-2.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, -1.5f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-2.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-2.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building4-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(0.0f, -0.5f, 0.0f);

	glBegin(GL_QUADS);

	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-1.5f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(1.5f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(1.5f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.5f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.5f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.5f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, -1.5f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-1.5f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-1.5f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 0.0f);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building5-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(1.5f, -2.0f, 2.0f);

	glBegin(GL_QUADS);
	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	//glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building6-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(4.0f, 0.0f, -3.5f);

	glBegin(GL_QUADS);
	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-1.0f, -8.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-1.0f, -8.0f, -1.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.5f);


	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building7-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(5.5f, 2.0f, 0.0f);


	glBegin(GL_QUADS);
	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-1.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(2.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(2.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(2.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(2.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -2.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-1.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-1.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//-----------Building8-----------------
	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _build2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(7.0f, 4.0f, 3.0f);


	glBegin(GL_QUADS);
	//front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinate
	glNormal3f(-0.12f, -0.99f, 0.f);
	glVertex3f(-1.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.99f, 0.f);
	glVertex3f(2.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(0.12f, -0.99f, 0.0f);
	glVertex3f(2.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(2.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, 0.0f);

	//back
	glNormal3f(0.0f, 0.0f, -1.5f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.97f, -0.18f);
	glVertex3f(-1.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.12f, -0.97f, -0.18f);
	glVertex3f(2.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -2.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); //texture coordinates
	glNormal3f(-0.12f, -0.99f, 0.0f);
	glVertex3f(-1.0f, -12.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(-0.12f, -0.99f, -0.18f);
	glVertex3f(-1.0f, -12.0f, -2.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -2.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-2.5f, 3.0f, 5.0f, GLUT_BITMAP_HELVETICA_12, (char*)"Leanna Angela O. Ua-o   EMC-DAT 2A    T85");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-2.5f, 2.5f, 5.0f, GLUT_BITMAP_HELVETICA_12, (char*)"I have learned about 2D/3D computer graphics and OpenGL");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-3.5f, 2.0f, 5.0f, GLUT_BITMAP_HELVETICA_12, (char*)"This course taught me how to create graphics program and apply text and sounds,");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-3.5f, 1.5f, 5.0f, GLUT_BITMAP_HELVETICA_12, (char*)"colors, multiple textures, lighting, rotation animation, as well as creating camera");

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-3.5f, 1.0f, 5.0f, GLUT_BITMAP_HELVETICA_12, (char*)"movements and scenery using skybox.");
	glEnable(GL_LIGHTING);

	glutSwapBuffers();
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 2000.0);
}


void update(int)
{


	//looping animation logic
	angle += 0.8;
	if (angle > 360.0f)
		angle = angle - 360.0f;

	glutPostRedisplay();
	glutTimerFunc(8000 / 60, update, 0);

}

void renderBitmapString(
	float x,
	float y,
	float z,
	void* font,
	char* string) {
	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}