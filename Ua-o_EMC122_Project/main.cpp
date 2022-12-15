#include <iostream>
#include <glut.h>
#include <Windows.h>
#include "imageloader.h"

using namespace std;

void display();
void reshape(int, int);
void update(int);
void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string);
long count= 0;

GLuint loadTexture(Image* image);
void processNormalKeys(unsigned char, int, int);


float angle = 0.0;// angle of rotation for the camera direction
float lx = 0.0f, lz = -1.0f;// actual vector representing the camera's direction
float cameraX = 0.0f, cameraZ = 5.0f;// XZ position of the camera
float _angle = -70.0f; // objects angle

//set materials
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 60.0 };

//set world light
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat static_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light[] = { 1.0, 1.0, 1.0 };
GLfloat light2[] = { 0.3, 0.3, 0.0 };
GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };

//Emissions
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_emission[] = {1.0, 1.0, 0.0, 0.5}; //yellow
GLfloat mat_emission2[] = {0.3, 0.3, 0.3, 1.0}; 


GLuint _textureGround;
GLuint _textureWall;
GLuint _textureRoof;
GLuint _textureDoor;
GLuint _textureWindow;
GLuint _textureBall; //The id of the texture
GLuint _textureCone;
GLUquadric* ball; //pointer quadric shape for the sphere
GLUquadric* cone;



//Initializes 3D rendering
void initRendering() {

	glClearColor(0.0, 0.0, 0.0, 1.0); //set background to black

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH); //Enable smooth shading


	Image* image_ground = loadBMP("snow.bmp");
	_textureGround = loadTexture(image_ground);
	delete image_ground;

	Image* image_wall = loadBMP("bricks2.bmp");
	_textureWall = loadTexture(image_wall);
	delete image_wall;

	Image* image_door = loadBMP("door.bmp");
	_textureDoor = loadTexture(image_door);
	delete image_door;

	Image* image_window = loadBMP("window.bmp");
	_textureWindow = loadTexture(image_window);
	delete image_window;
	
	Image* image_roof = loadBMP("roof2.bmp");
	_textureRoof = loadTexture(image_roof);
	delete image_roof;

	Image* image_sky = loadBMP("stars.bmp");
	_textureBall = loadTexture(image_sky);
	delete image_sky;
	ball = gluNewQuadric(); //declared as quadric objects

	Image* image_tree = loadBMP("lights.bmp");
	_textureCone = loadTexture(image_tree);
	delete image_tree;
	cone = gluNewQuadric(); //declared as quadric objects


}


int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	
	//Create the window
	glutCreateWindow("EMC122Project");
	initRendering();

	//Set handler functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);//controls
	glutTimerFunc(1000 / 60, update, 0); //Add a timer

	PlaySound("music.wav", NULL, SND_ASYNC); //set bg sound

	waveOutSetVolume(0, 1717986918); // set volume to Normal
	glutTimerFunc(1000 / 60, update, 0); //Add a timer

	glutMainLoop();
	return 0;
}

//Draws the 3D scene
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//camera controls
	gluLookAt(cameraX, 1.0f, cameraZ,
		cameraX + lx, 1.0f, cameraZ + lz,
		0.0f, 1.0f, 0.0f);

	//---------SKYBOX-----------------------//
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureBall);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient); //add ambient lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1,1,1); // needed to make the skybox visible
	gluQuadricTexture(ball, 1);
	glRotatef(180, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f); 
	glRotatef(_angle, 0.0f, 0.0f, 1.0f); 
	gluSphere(ball, 1000, 128, 128);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	//---------SKYBOX-----------------------//

	
	//setting world lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
	glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess*/

	glTranslatef(0.0f, 0.0f, -30.0f); //move object in -z axis to seen in display

	//------------pine tree1-------------------//
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 7.0f); //bottom
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 4.0f, 7.0f); //middle
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 9.0f, 7.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCone(4, 7, 18, 18);
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix(); 
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glTranslatef(0.0f, -1.0f, 7.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCube(2);
	glPopMatrix();
	//------------pine tree1-------------------//end

	//------------pine tree2-------------------//
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(25.0f, 0.0f, 13.0f); //bottom
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(25.0f, 4.0f, 13.0f); //middle
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(25.0f, 9.0f, 13.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCone(4, 7, 18, 18);
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix(); 
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glTranslatef(25.0f, -1.0f, 13.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCube(2);
	glPopMatrix();
	//------------pine tree2-------------------//end

	//------------pine tree3-------------------//
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-25.0f, 0.0f, 13.0f); //bottom
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-25.0f, 4.0f, 13.0f); //middle
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(5, 10, 18, 18);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureCone);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	gluQuadricTexture(cone, 1);

	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-25.0f, 9.0f, 13.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCone(4, 7, 18, 18);
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix(); 
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(0.1f, 0.1f, 0.1f);
	glTranslatef(-25.0f, -1.0f, 13.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); 
	glutSolidCube(2);
	glPopMatrix();
	//------------pine tree3-------------------//end

	//-----------------mountains----------------//
	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(24.0f, -1.0f, -40.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 21, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(35.0f, -1.0f, -40.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 21, 24, 24);
	glPopMatrix();

	
	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(12.0f, -1.0f, -40.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 17, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(-12.0f, -1.0f, -40.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 17, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(-24.0f, -1.0f, -40.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 21, 24, 24);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(-35.0f, -1.0f, -40.0f);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	glutSolidCone(20, 21, 24, 24);
	glPopMatrix();
	//-----------------mountains--------------//

	glPushMatrix();
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glTranslatef(0.0f, -2.0f, 7.0f); 
	glRotatef(270, 1.0f, 0.0f, 0.0f); //rotation animation
	glutSolidTorus(0.05, 10, 50, 50);
	glPopMatrix();

	//---------lights------------//
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(0.0f, 17.0f, -1.8f);   //topmost
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(-1.0f, 14.0f, 3.5f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(1.0f, 16.0f, -1.0f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(2.0f, 14.0f, 2.7f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(-3.0f, 11.0f, 3.5f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(3.0f, 11.0f, 3.5f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(0.0f, 11.0f, 6.5f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(0.0f, 11.0f, -2.3f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(-1.0f, 8.0f, 7.8f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(2.5f, 8.0f, 7.2f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);//add color material to object
	glRotatef(30, 1.0f, 0.0f, 0.0f); //rotation animation
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(2.5f, 8.0f, 0.0f);   //location
	glutSolidSphere(0.3, 12, 12);
	glPopMatrix();
	//---------lights------------//

	//-----------------HOUSE#1--------------//(center)
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(1.0f, 3.0f, -11.0f);
	glRotatef(0, 0.0f, 2.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//front1
	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//front2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//door
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureDoor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, 0.0f, 2.0003f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, 0.0f, 2.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//windows
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 1.5f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -2.0f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -3.5f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//back wall1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glEnd();

	//back wall2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.0f, 0.70f, 0.70f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, -4.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, 0.0f, -4.0003f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, 0.0f, -4.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//left wall
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.79f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//windows
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);;

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right roof
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureRoof);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 3.0f);
	glNormal3f(0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();

	//left roof
	glBegin(GL_QUADS);
	glNormal3f(-0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 3.0f);
	glNormal3f(-0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//below floor
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -4.0f, 2.0f);
	glVertex3f(3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, 2.0f);
	glEnd();

	glPopMatrix();
	//-----------------HOUSE#1--------------//end

	//-----------------HOUSE#2--------------//
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(-10.0f, 3.0f, -8.0f);
	glRotatef(45, 0.0f, 2.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//front1
	glBegin(GL_QUADS);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//front2
	glBegin(GL_TRIANGLES);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(0.0f, 0.89f, 0.44f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//door
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureDoor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, 0.0f, 2.0003f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, 0.0f, 2.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//windows
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 1.5f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -2.0f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -3.5f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//back wall1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glEnd();

	//back wall2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.0f, 0.70f, 0.70f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, -4.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, 0.0f, -4.0003f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, 0.0f, -4.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//left wall
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.79f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//windows
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right roof
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureRoof);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 3.0f);
	glNormal3f(0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();

	//left roof
	glBegin(GL_QUADS);
	glNormal3f(-0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 3.0f);
	glNormal3f(-0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//below floor
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -4.0f, 2.0f);
	glVertex3f(3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, 2.0f);
	glEnd();

	glPopMatrix();
	//-----------------HOUSE#2--------------//end


	//-----------------HOUSE#3--------------//
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(10.0f, 3.0f, -8.0f);
	glRotatef(-45, 0.0f, 2.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//front1
	glBegin(GL_QUADS);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//front2
	glBegin(GL_TRIANGLES);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(0.0f, 0.89f, 0.44f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//door
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureDoor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, 0.0f, 2.0003f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, 0.0f, 2.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//windows
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 1.5f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -2.0f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -3.5f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//back wall1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glEnd();

	//back wall2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.0f, 0.70f, 0.70f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, -4.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, 0.0f, -4.0003f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, 0.0f, -4.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//left wall
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.79f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//windows
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right roof
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureRoof);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 3.0f);
	glNormal3f(0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();

	//left roof
	glBegin(GL_QUADS);
	glNormal3f(-0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 3.0f);
	glNormal3f(-0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//below floor
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -4.0f, 2.0f);
	glVertex3f(3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, 2.0f);
	glEnd();

	glPopMatrix();
	//-----------------HOUSE#3--------------//end
	
	//-----------------HOUSE#4--------------//
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(12.0f, 3.0f, 1.0f);
	glRotatef(-90, 0.0f, 2.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//front1
	glBegin(GL_QUADS);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//front2
	glBegin(GL_TRIANGLES);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(0.0f, 0.89f, 0.44f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//door
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureDoor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, 0.0f, 2.0003f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, 0.0f, 2.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//windows
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 1.5f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -2.0f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -3.5f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//back wall1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glEnd();

	//back wall2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.0f, 0.70f, 0.70f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, -4.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, 0.0f, -4.0003f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, 0.0f, -4.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//left wall
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.79f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//windows
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right roof
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureRoof);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 3.0f);
	glNormal3f(0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();

	//left roof
	glBegin(GL_QUADS);
	glNormal3f(-0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 3.0f);
	glNormal3f(-0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//below floor
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -4.0f, 2.0f);
	glVertex3f(3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, 2.0f);
	glEnd();

	glPopMatrix();
	//-----------------HOUSE#4--------------//end

	//-----------------HOUSE#5--------------//
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glTranslatef(-12.0f, 3.0f, 1.0f);
	glRotatef(90, 0.0f, 2.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//front1
	glBegin(GL_QUADS);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	//glNormal3f(-0.55f, -0.74f, 0.37f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//front2
	glBegin(GL_TRIANGLES);
	//glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	//glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	//glNormal3f(0.0f, 0.89f, 0.44f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//door
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureDoor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, -4.0f, 2.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, 0.0f, 2.0003f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, 0.0f, 2.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right wall
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(0.55f, -0.74f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, -4.0f, 2.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0f, 1.0f, 2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//windows
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 1.5f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, 0.0f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -2.0f);
	glTexCoord2f(3.0f, 0.0f);			glVertex3f(3.0003f, -2.0f, -3.5f);
	glTexCoord2f(3.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//back wall1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, -4.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glEnd();

	//back wall2
	glBegin(GL_TRIANGLES);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -4.0f);
	glNormal3f(0.0f, 0.70f, 0.70f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.0f, 4.0f, -4.0f);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(1.0f, -2.5f, -4.0003f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(1.0f, 0.0f, -4.0003f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-1.0f, 0.0f, -4.0003f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//left wall
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWall);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glBegin(GL_QUADS);
	glNormal3f(-0.55f, -0.79f, 0.37f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, 2.0f);
	glNormal3f(-0.46f, -0.625f, -0.625f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, -4.0f, -4.0f);
	glNormal3f(-0.58f, 0.19f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, -4.0f);
	glNormal3f(-0.83f, 0.27f, 0.55f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0f, 1.0f, 2.0f);
	glEnd();

	//windows
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureWindow);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light2 ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light2 ); //add specular lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	//window1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 1.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, 1.5f);
	glEnd();

	//window2
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0003f, -2.0f, -3.5f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -3.5f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-3.0003f, 0.0f, -2.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//right roof
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureRoof);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glNormal3f(0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(3.0f, 1.0f, 3.0f);
	glNormal3f(0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();

	//left roof
	glBegin(GL_QUADS);
	glNormal3f(-0.68f, 0.22f, 0.68f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, 3.0f);
	glNormal3f(-0.50f, 0.16f, -0.84f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-3.0f, 1.0f, -5.0f);
	glNormal3f(0.0f, 0.625f, -0.78f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(0.0f, 4.0f, -5.0f);
	glNormal3f(0.0f, 0.8f, 0.6f);
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(0.0f, 4.0f, 3.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//below floor
	glBegin(GL_QUADS);
	glVertex3f(3.0f, -4.0f, 2.0f);
	glVertex3f(3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, -2.0f);
	glVertex3f(-3.0f, -4.0f, 2.0f);
	glEnd();

	glPopMatrix();
	//-----------------HOUSE#5--------------//end

	//-----------------GROUND----------------//
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureGround);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission2);
	
	glTranslatef(0.0f, -1.0f, 0.0f);
	glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);	
		glTexCoord2f(0 , 0);				glVertex3f(-1000.0f, -5.0f, -1000.0f);
		glTexCoord2f(1000, 0);				glVertex3f(-1000.0f, -5.0f, 1000.0f);
		glTexCoord2f(1000.0f, 1000.0f);		glVertex3f(1000.0f, -5.0f, 1000.0f);
		glTexCoord2f(0.0f, 1000.f);			glVertex3f(1000.0f, -5.0f, -1000.0f);
	
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	//-----------------GROUND----------------//end

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-6.0f, 18.0f, 10.0f, GLUT_BITMAP_HELVETICA_12,"EMC122 Project TECH DEMO");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(-4.0f, 16.0f, 10.0f, GLUT_BITMAP_HELVETICA_12,"'Snow Village'");
	glEnable(GL_LIGHTING);
	glutSwapBuffers();
}


//Called when the window is resized
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)w / (double)h, 1.0, 2000.0);
}

void update(int value) {
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(8000 / 60, update, 0);
}
//keyboard controls
void processNormalKeys(unsigned char key, int x, int y) {
	float fraction = 1.5f;


	switch (key) {
	case 'w':
		cameraX += lx * fraction;
		cameraZ += lz * fraction;
		break;
	case 's':
		cameraX -= lx * fraction;
		cameraZ -= lz * fraction;
		break;
	case 'a':
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'd':
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;

	}
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

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

