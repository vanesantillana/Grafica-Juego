#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include "imageloader.h"
#include "text3d.h"
#include <time.h>
#include <iostream>
using namespace std;

int score = 0;
int j = 0;
float dist=30.0f;
//bool movingUp = false;
float rotspeed = 10.0f;
float yLocation = 0.75f;	
float yRotationAngle = 0.0f;	
int total = 70;			
int arr[1000];
float lx = 0.0f, lz = -1.0f;	
float m = 0.0f;
float x = 0.0f, z = 0.0f;	
float p = -21.0f;	
int num = total - 1;
float deltaMove = 0;
int randomn;

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); 
	glBindTexture(GL_TEXTURE_2D, textureId); 
	glTexImage2D(GL_TEXTURE_2D,                
				 0,                            
				 GL_RGB,                       
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId1; //The id of the texture
GLuint _textureId2;
//GLuint _textureId3;
void changeSize(int w, int h)
{
    if (h == 0)
	h = 1;
    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void background(){
glLoadIdentity();
    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(-300.0f, 200.0f,-100.0f);
    		glTexCoord2f(0.0f,1.0f);
    glVertex3f(-300.0f, -10.0f,-100.0f);
    		glTexCoord2f(1.0f,1.0f);
    glVertex3f(300.0f, 200.0f,-100.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(300.0f, -10.0f,-100.0f);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);



}

void runner(){
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(x, 2.2f, z + ((total) * dist)-3.0); //dist camara
	//cout<<z<<" "<<z + ((total) * 30.0)-10.0<<endl;
	glRotatef(yRotationAngle*rotspeed, 1.0f, 0.0f, 0.0f);
	glutWireSphere(0.1f, 20, 20);
}

void drawSnowMan()
{

    glColor3f(1.0f, 1.0f, 1.0f);	

// Body

    glTranslatef(0.0f, yLocation, 0.0f);
    glutSolidSphere(0.75f, 20, 20);
    glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f);

// Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

// Eyes
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

// Nose
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.85f, 10, 2);

/*
    if (movingUp)		// If we are moving up
	yLocation -= 0.0005f;	// Move up along our yLocation
    else			// Otherwise
	yLocation += 0.0005f;	// Move down along our yLocation

    if (yLocation < 0.5f)	
	movingUp = false;	
    else if (yLocation > 1.5f)	
	movingUp = true;	
*/
    yRotationAngle += 0.01f;	
    if (yRotationAngle > 360.0f)	
	yRotationAngle -= 360.0f;	


}

void computePos(float deltaMove)
{
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

void drawcone(){
	glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCone(2.0f, 2.5f, 10, 2);
}

void drawshadow(){
	    glColor3f(0.0f, 0.0f, 0.0f);	
    glTranslatef(0.0f, 0.1f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);
}

//Mueve camara 
void renderScene(void)
{
    glMatrixMode(GL_MODELVIEW);
	
    if (deltaMove)
	computePos(deltaMove);

    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 3.0f, z + ((total) * dist), x + lx, 2.0f, z - 1.0f, 0.0f, 1.0f, 0.0f);	


    //Collision
    if (z < p + 1.0 && z > p - 1.0) {

	int a = (rand() % 3) - 1;
	if (x != 2.0f * arr[num]) {
	    cout << "Fin" << endl;
	    //cout << "Your score:" << score << endl;
	    //cout<<arr[num]<<"   "<<x<<"  "<<num<<endl;
	    exit(0);
	}
	rotspeed=rotspeed+0.25f;
	score = score + 1;
	num = num - 1;
	p = p - dist;
    }
    //Game end
    deltaMove = 3;//2.0f - z * 0.01f;
    //cout<<z<<"  "<<p<<"  "<<x<<"  "<<arr[num]<<"   "<<num<<endl;
    if (z < -(((total) * dist) + 20.0)) {
    	//cout << "Your score:" << score << endl;
	cout << "BIEN" << endl;
	exit(0);
    }

   	
    // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);

    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(-3.0f, 0.0f, -10000.0f);
    		glTexCoord2f(0.0f,2000.0f);
    glVertex3f(-3.0f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,2000.0f);
    glVertex3f(3.0f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(3.0f, 0.0f, -10000.0f);
    
    glEnd();
glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(-3.0f, 3.0f, -10000.0f);
    		glTexCoord2f(0.0f,2000.0f);
    glVertex3f(-3.0f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,2000.0f);
    glVertex3f(-300.0f, 3.0f, 10000.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(-300.0f, 0.0f, -10000.0f);
    
    glEnd();
glDisable(GL_TEXTURE_2D);


    glColor3f(1.0f, 1.0f, 1.0f);

    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.3f, 3.0f, -10000.0f);
    		glTexCoord2f(0.0f,2000.0f);
    glVertex3f(-2.3f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,2000.0f);
    glVertex3f(-4.0f, 3.0f, 10000.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(-4.0f, 0.0f, -10000.0f);
    
    glEnd();
glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(3.0f, 3.0f, -10000.0f);
    		glTexCoord2f(0.0f,2000.0f);
    glVertex3f(3.0f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,2000.0f);
    glVertex3f(300.0f, 3.0f, 10000.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(300.0f, 0.0f, -10000.0f);
    
    glEnd();
glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    		glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.3f, 3.0f, -10000.0f);
    		glTexCoord2f(0.0f,2000.0f);
    glVertex3f(2.3f, 0.0f, 10000.0f);
    		glTexCoord2f(1.0f,2000.0f);
    glVertex3f(4.0f, 3.0f, 10000.0f);
    		glTexCoord2f(1.0f,0.0f);
    glVertex3f(4.0f, 0.0f, -10000.0f);
    
    glEnd();
glDisable(GL_TEXTURE_2D);

    for (j = 0; j < total; j++) {

	srand(randomn+j);		//use j-1 or j+1 to change order of snowman
	arr[j] = (rand() % 3) - 1;	//-1,0,1
	//cout<<arr[j]<<"   "<<j<<endl;
	//drawtree(-5.0f, 0, j * dist);
	for (int i = -1; i < 2; i++) {	//should start from -1 to place snowmen at center
	    glPushMatrix();
	    if (arr[j] != i) {
		glTranslatef(i * 2.0, 0, j * dist);
		drawshadow();
		drawSnowMan();
		
		
	    }
	    glPopMatrix();
	}
	//drawtree(x-3,0,z);
	//drawtree(x+3,0,z);
	
    }
    
    
    
    runner();
    background();
    glutSwapBuffers();
    
}



void pressKey(int key, int xx, int yy)
{

    switch (key) {
    case GLUT_KEY_LEFT:
	if (x != -2.0f) {
	    x = x - 2.0f;
	}
	break;
    case GLUT_KEY_RIGHT:
	if (x != 2.0f) {
	    x = x + 2.0f;
	}
	break;
    case GLUT_KEY_UP:
	break;
    case GLUT_KEY_DOWN:
	break;
    }


}

void releaseKey(int key, int x, int y)
{

    switch (key) {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
	break;
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
	break;
    }
}

int main(int argc, char **argv)
{

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(250, 50);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Snow Skating");
  Image* image2 = loadBMP("snow.bmp");
	_textureId1 = loadTexture(image2);
	delete image2;

srand(time(NULL));
	randomn =  (rand() % 3) - 1;

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    glutSpecialFunc(pressKey);


    glutIgnoreKeyRepeat(1);
    glutSpecialUpFunc(releaseKey);


    glEnable(GL_DEPTH_TEST);


    glutMainLoop();

    return 1;
}

