#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>
#include "TextureManager.h"
//#include "text3d.h"
#include<time.h>
#include <iostream>
using namespace std;

int score = 0;
int j = 0;
float dist=30.0f;
bool movingUp = false;
float rotspeed = 60.0f;
float yLocation = 0.75f;	// Keep track of our position on the y axis.
float yRotationAngle = 0.0f;	// The angle of rotation for our object
int total = 70;			//number of snowmen rows
int arr[1000];
float lx = 0.0f, lz = -1.0f;	// actual vector representing the camera's direction
float m = 0.0f;
float x = 0.0f, z = 0.0f;	// XZ position of the camera
float y = 0.0f;
float p = -21.0f;	//26-distance of running object
int num = total - 1;
float deltaMove = 0;
int randomn;
int alto = 0.1;

//-------variables fondo -------------//
GLint sky;
GLint pista;
GLint pared;
GLint wood;
GLint brick;
GLint roof;
GLint grass;
GLint sprites;

//---------variables de Sprite -----------//
int times = 0;
int timebase = 0;
int anim = 0;
int i = 0;
double sprx = 0.08;
double spry = 0.665;
double penguin_p_a = 0.5; // parado
double penguin_p_b = 0.6;

double penguin_e_a = 0.0; // echado
double penguin_e_b = 0.1;
bool echado = false; //si apreta echado cambia

//---------variables para saltar ----------//
bool saltar = false;
double tam_salto = 0.5;
double i_salto=0;

//ventana tamaño
void changeSize(int w, int h)
{
  // Prevenir una división por cero, cuando la ventana es demasiado corta
  // (No puedes hacer una ventana de ancho cero).
    if (h == 0)
	    h = 1;
    float ratio = w * 1.0 / h;
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void runner()
{
  double p_pos_a = 0, p_pos_b = 0;
  if(echado){
    p_pos_a = penguin_e_a; p_pos_b = penguin_e_b;
    sprx=1;
  }
  else{
    p_pos_a = penguin_p_a; p_pos_b = penguin_p_b;
    sprx=0.08;
  }

  if(saltar && i_salto<tam_salto){
    i_salto += 0.1;
  }
  else{
    if(i_salto>0.1){
     i_salto -= 0.1;
    }
    saltar = false;
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	glEnable(GL_BLEND);//utilizar transparencia
	times = glutGet(GLUT_ELAPSED_TIME); // recupera el tiempo ,que paso desde el incio de programa
	int dt = times -timebase;// delta time
	timebase = times;
	anim += dt;//duracion de la animacion entre dos cambios de Sprite
	
	if (anim / 1000.0 > 0.15)// si el tiempo de animacion dura mas 0.15s cambiamos de sprite
	{
		i++;
		anim = 0.0;
	}
	
	if (i == 3) i = 0;

  glPushMatrix();
    glTranslatef(x, 2.1f+y +i_salto, z + ((total) * dist)- 3.0);	//2.1f- arriba o abajo, 3.0 away from camera
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sprites);
    glBegin(GL_QUADS);
    glTexCoord2f(sprx*(i + 0), p_pos_a);//arr izq
    glVertex3f(-0.4, -0.30, 0);
    glTexCoord2f(sprx*(i + 0),p_pos_b); //bajo izq
    glVertex3f(-0.4, 0.30, 0);
    glTexCoord2f(0.08+sprx*i, p_pos_b); //bajo der
    glVertex3f(0.4, 0.30, 0);
    glTexCoord2f(0.08+sprx*i, p_pos_a); //arr der
    glVertex3f(0.4, -0.30, 0);
    glEnd();
  glPopMatrix();
  /*
  glPushMatrix();
	GLUquadric* sphere = gluNewQuadric();
  glEnable(GL_TEXTURE_2D);
  gluQuadricTexture(sphere, true);
  glBindTexture(GL_TEXTURE_2D, penguin);
	glTranslatef(x, 2.1f+y, z + ((total) * dist)- 3.0);	//2.1f- arriba o abajo, 3.0 away from camera
  gluSphere(sphere,0.1, 50,50);
  glPopMatrix();
*/
	//cout<<z<<" "<<z + ((total) * 30.0)-10.0<<endl;
	//glRotatef(yRotationAngle*rotspeed, 1.0f, 0.0f, 0.0f);
  //glutWireSphere(0.1f, 20, 20);
  //drawPenguin();
}


void drawSnowMan()
{

    glColor3f(1.0f, 1.0f, 1.0f);	//color of snowman

// Draw Body
    glTranslatef(0.0f, yLocation, 0.0f);
    glutSolidSphere(0.75f, 20, 20);
    glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f);

// Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

// Draw Eyes
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

// Draw Nose
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.85f, 10, 2);

}


void drawTree()
{/*
  glColor3f(1.0f, 1.0f, 1.0f);	//color of snowman
  GLUquadric* tree = gluNewQuadric();
// Draw Body
  glTranslatef(0.0f, yLocation, 0.0f);
  gluCylinder(tree, 0.5f, 0.5f, 1.0f, 70 ,70); //gluCylinder(quadObj, base, top, height, slices, stacks);


  // Draw Head
      glTranslatef(0.0f, 1.0f, 0.0f);
      glutSolidSphere(0.60f, 20, 20);
*/
glPushMatrix();
glEnable(GL_TEXTURE_2D);
GLUquadric* tree = gluNewQuadric();
gluQuadricTexture (tree, true);
glBindTexture(GL_TEXTURE_2D, wood);
glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
//glBindTexture(GL_TEXTURE_2D, wood);
gluCylinder(tree, 0.5f, 0.2f, 6.0f, 64 ,64);
glPopMatrix();
glDisable(GL_TEXTURE_2D);

glPushMatrix();
glEnable(GL_TEXTURE_2D);
GLUquadric* sphere = gluNewQuadric();
gluQuadricTexture(sphere, true);
glBindTexture(GL_TEXTURE_2D, grass);
glTranslatef(0.0f, 6.4f, 0.0f);
//glutSolidSphere(2.5, 64, 64);
gluSphere(sphere,2.5, 50,50);
glPopMatrix();
glDisable(GL_TEXTURE_2D);

//glFlush();

}


void computePos(float deltaMove)
{
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

void drawshadow() // punto negro abajo de snowman
{
	  glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.1f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);
}

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
    gluLookAt(x, 3.0f, z + ((total) * dist), x + lx, 2.0f, z - 1.0f, 0.0f, 1.0f, 0.0f);	//z+1500.0f means make camera move 1500f backward,y coordinates are for inclination
    //cout<<z<<endl;
    //Collision logic
    if (z < p + 1.0 && z > p - 1.0) {
      int a = (rand() % 3) - 1;
	  if (x != 2.0f * arr[num]) {
	      exit(0);
	  }
	    rotspeed=rotspeed+0.25f;
	    score = score + 1;
	    num = num - 1;
	    p = p - dist;
    }
    //Game end logic
    deltaMove = 5;
    //cout<<z<<"  "<<p<<"  "<<x<<"  "<<arr[num]<<"   "<<num<<endl;
    if (z < -(((total) * dist) + 20.0)) {
    	cout << "Your score:" << score << endl;
	    cout << "YOU WON THE GAME" << endl;
	    exit(0);
    }


//**************************TEXTURAS*******************************************************


  //Textura de cielo falta

//---------------------------------------Textura de la pista de hielo --------------------//
    glColor3f(0.9f, 0.9f, 0.9f);
    glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, pista);
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


//--------------------largo lado izquiero pared

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, pared);
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


//-------------------Lado izquiero de la pantalla pared
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, pared);
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

 //--------------------------largo lado derecho pared
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pared);
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

//--------------------------lado derecho de la pared

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, pared);
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

//*********************************************************************************************************

    for (j = 0; j < total; j++) {
      srand(randomn+j);		//use j-1 or j+1 to change order of snowman
	    arr[j] = (rand() % 3) - 1;	//-1,0,1
	    //cout<<arr[j]<<"   "<<j<<endl;
	    //drawtree(-5.0f, 0, j * dist);
	    for (int i = -1; i < 2; i++) {	//should start from -1 to place snowmen at center
	      glPushMatrix();
	      if (arr[j] != i) {
		    glTranslatef(i * 2.0, 0, j * dist);
		    //drawshadow();
	      //drawSnowMan();
        drawTree();
	      }
	    glPopMatrix();
	    }
	//drawtree(x-3,0,z);
	//drawtree(x+3,0,z);
    }
    runner();
    glutSwapBuffers();
}

//-------funcion para reconocer letras del teclado -------------/
void processKeys(unsigned char key, int x, int y) {
  if (key == 27) // escape key 
    exit(0);
  else if (key==32){ // tecla espacio para saltar
    saltar = true;
    echado = false;
  }
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
         echado = false;
	       break;
      case GLUT_KEY_DOWN:
         echado = true;
         break;
	break;
    }


}

int main(int argc, char **argv)
{

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(250, 50);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("SnowRun");
    //penguin
    sprites = TextureManager::Inst()->LoadTexture("texturas/penguin.png", GL_BGRA_EXT, GL_RGBA);
    pista = TextureManager::Inst()->LoadTexture("pista.jpg", GL_BGR_EXT, GL_RGB);
    pared = TextureManager::Inst()->LoadTexture("pared.jpeg", GL_BGR_EXT, GL_RGB);
    sky = TextureManager::Inst()->LoadTexture("cielo.jpg", GL_BGR_EXT, GL_RGB);
    wood = TextureManager::Inst()->LoadTexture("madera.jpg", GL_BGR_EXT, GL_RGB);
    brick = TextureManager::Inst()->LoadTexture("ladrillo.jpg", GL_RGB, GL_RGB);
    roof = TextureManager::Inst()->LoadTexture("techo.jpg", GL_RGB, GL_RGB);
    grass = TextureManager::Inst()->LoadTexture("texturas/hojas.jpg", GL_BGR_EXT, GL_RGB);

    srand(time(NULL));
	  randomn = 2;// (rand() % 3) - 1;

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc( processKeys );
    glutSpecialFunc(pressKey);
    //glutIgnoreKeyRepeat(1);
    glEnable(GL_DEPTH_TEST);


    glutMainLoop();

    return 1;
}
