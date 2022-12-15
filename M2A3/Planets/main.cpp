#include <iostream>
#include <glut.h>
#include "imageloader.h"

void update(int);
GLuint loadTexture(Image* image);
void reshape(int, int);
void display();

float _angle = -70.0f;
float _angle2 = 0;
;


GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat directedLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat directedLightPos[] = { 0.0f, 0.0f, 1.0f, 0.0f };

//set materials
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 60.0 };

//set world light
GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

//light positions
GLfloat xlight0 = 0.0f, ylight0 = 4.5f, zlight0 = 0.0f;

//set Light #0
GLfloat light_position[] = { xlight0, ylight0, zlight0, 1.0 };
GLfloat light[] = { 0.8, 0.8, 0.0 };



//Emissions
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };

GLuint _textureSun; //The id of the texture
GLuint _textureMercury;
GLuint _textureVenus;
GLuint _textureEarth;
GLuint _textureMars;


GLUquadric* Sun; //pointer quadric shape for the planets
GLUquadric* Mercury;
GLUquadric* Venus;
GLUquadric* Earth;
GLUquadric* Mars;

//Initializes 3D rendering
void initRendering() {

	glClearColor(0.0, 0.0, 0.0, 1.0); //set background to black

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH); //Enable smooth shading

	//loading texture for the ball
	Image* Image1 = loadBMP("Sun.bmp");
	_textureSun = loadTexture(Image1);
	delete Image1;
	Image* Image2 = loadBMP("Mercury.bmp");
	_textureMercury = loadTexture(Image2);
	delete Image2;
	Image* Image3 = loadBMP("Venus.bmp");
	_textureVenus = loadTexture(Image3);
	delete Image3;
	Image* Image4 = loadBMP("Earth.bmp");
	_textureEarth = loadTexture(Image4);
	delete Image4;
	Image* Image5 = loadBMP("Mars.bmp");
	_textureMars = loadTexture(Image5);
	delete Image5;

	Sun = gluNewQuadric(); //declared as quadric objects
	Mercury = gluNewQuadric(); //declared as quadric objects
	Venus = gluNewQuadric();
	Earth = gluNewQuadric();
	Mars = gluNewQuadric();

}

int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	//Create the window
	glutCreateWindow("Ua-o_M2A3_Planets");
	initRendering();
	//Set handler functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(25, update, 0); //Add a timer
	glutMainLoop();
	return 0;
}

//Draws the 3D scene
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//setting world lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting


	glTranslatef(0.0f, 0.0f, -15.0f); //move object in -z axis to seen in display

	glPushMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

	glEnable(GL_TEXTURE_2D);

	//--------------------------Sun----------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureSun);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(Sun, 1);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
	glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glTranslatef(0.0f, 0.0f, 0.0f); //location
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); //rotation animation
	gluSphere(Sun, 2.0, 24, 24);
	glPopMatrix();
	
	//--------------------------Mercury----------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureMercury);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(Mercury, 1);

	glPushMatrix();
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); 
	glRotatef(_angle, 0.0f, 1.0f, 1.0f); //rotation animation
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTranslatef(4.0, 0.0, 0.0); //location
	gluSphere(Mercury, 0.3, 24, 24);
	glPopMatrix();

	//--------------------------Venus----------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureVenus);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(Venus, 1);

	glPushMatrix();
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); 
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); //rotation animation
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTranslatef(-5.0f, 2.0f, 0.0f); //location
	gluSphere(Venus, 0.4, 24, 24);
	glPopMatrix();

	//--------------------------Earth----------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureEarth);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(Earth, 1);

	glPushMatrix();
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); 
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); //rotation animation
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTranslatef(7.0f, 2.0, 0.0); //location
	gluSphere(Earth, 0.5, 24, 24);
	glPopMatrix();

	//--------------------------Mars----------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureMars);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(Mars, 1);

	glPushMatrix();
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); 
	glRotatef(_angle2, 0.0f, 1.0f, 1.0f); //rotation animation
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glTranslatef(-9.0f, 0.0, 1.0); //location
	gluSphere(Mars, 0.6, 24, 24);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);

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


//Called when the window is resized
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void update(int value) {
	
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}
	_angle2 += 1.0f;
	if (_angle2 > 360) {
		_angle2 -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(2000 / 60, update, 0);
}

