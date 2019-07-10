#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sstream>
#include <GL/glut.h>
#include "TextureManager.h"
//#include "text3d.h"
#include<time.h>
#include <iostream>
#include <pthread.h>
using namespace std;

int timer=0;
int score = 0;
int j = 0;
float dist=30.0f;
bool movingUp = false;
float rotspeed = 60.0f;
float yLocation = 0.75f;	// Keep track of our position on the y axis.
float yRotationAngle = 0.0f;	// The angle of rotation for our object
int total = 70;			//number of snowmen rows
int totalCoins = 30;
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
GLint sprites, segurity,snow;
GLint coin;

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

// -------- variables para el escudo --------//
bool escudo = false;
double sizea = 0.25, sizeb = 0.2;
double cont_escudo = 0;
double max_escudo = 2; //dura 2 veces mi escudo pero se vuelve mas pequenio
double uso_escudos = 0;
double total_escudos = 3; // solo puedo usar 3 escudos en total
//------------ coins -------//
int arrcoin[30];
float dist_coin=30.0f;

pthread_t thread1;


void *background_sound(void *ptr)
{
	system("canberra-gtk-play -f sonidos/theme.wav &");
}

string scores="SCORE: ";
void drawScore(){
  glPushMatrix();
  	glColor3f( 0.0f, 0.0f, 0.0f);
  glTranslatef(0,2, z + ((total) * dist)- 10.);
  stringstream ss;
  ss<<"0";
  glRasterPos2i(0, 2);
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < scores.size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)scores[i]);

  for (int i = 0; i < ss.str().size(); ++i)
  		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)ss.str()[i]);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE);
  glPopMatrix();
}

string tm="TIME: ";
void drawTime(){
  glPushMatrix();
  glTranslatef(0,2, z + ((total) * dist)- 10.);
	int t=timer/1000;
	stringstream minute;
	minute<<t/60;
	stringstream second;
	second<<t%60;
	glRasterPos2i(2, 2);
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < scores.size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)tm[i]);
	for (int i = 0; i < minute.str().size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)minute.str()[i]);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)':');
	if(t%60<10)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)'0');
	for (int i = 0; i < second.str().size(); ++i)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)second.str()[i]);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE);
  glPopMatrix();
}

void background()
{
  glColor3f( 1.0f, 1.0f, 1.0f);
  glPushMatrix();
  glTranslatef(0, 0, z + ((total) * dist)- 90);
	//glTranslatef(0.0f, -10.0f, -60);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sky);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);//coordenadas de textura
	glVertex3d(-60, -60, 0); //dl
	glTexCoord2f(0.0, 1.0f); //ul
	glVertex3d(-60, 60, 0);
	glTexCoord2f(1.0, 1.0f); //ur
	glVertex3d(60, 60, 0);
	glTexCoord2f(1.0, 0.0); //dr
	glVertex3d(60, -60, 0);
	glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  //-----------------nieve  
  glPushMatrix();
    glTranslatef(x + i, y + i, z + ((total) * dist)- 3.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, snow);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);//coordenadas de textura
    glVertex3d(-2, -5, 0); //dl
    glTexCoord2f(0.0, 8.0f); //ul
    glVertex3d(-2, 5, 0);
    glTexCoord2f(8.0,8.0f); //ur
    glVertex3d(5, 2, 0);
    glTexCoord2f(8.0, 0.0); //dr
    glVertex3d(5, -2, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();

}


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

  if(escudo && cont_escudo<max_escudo){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
	  glEnable(GL_BLEND);//utilizar transparencia
    glPushMatrix();
      glTranslatef(x, 2.1f+y +i_salto, z + ((total) * dist)- 3.0);	//2.1f- arriba o abajo, 3.0 away from camera
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, segurity);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);//arr izq
      glVertex3f(-sizea, -sizeb -0.1, -0.1);
      glTexCoord2f(0, 1); //bajo izq
      glVertex3f(-sizea, sizeb, -0.1);
      glTexCoord2f(1, 1); //bajo der
      glVertex3f(sizea, sizeb, -0.1);
      glTexCoord2f(1,0); //arr der
      glVertex3f(sizea, -sizeb -0.1, -0.1);
      glEnd();
    glPopMatrix();
  }
  else{
    escudo = false;
    cont_escudo =0;
    sizea = 0.25;
    sizeb = 0.2;
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
    glTexCoord2f(sprx*(i), p_pos_a);//arr izq
    glVertex3f(-0.4, -0.30, 0);
    glTexCoord2f(sprx*(i),p_pos_b); //bajo izq
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

void drawCoins()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, coin);
    glPushMatrix();
      glTranslatef(0,1,0);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3d(0.8, -1., 0);//abajo izquierda
      glTexCoord2f(0.0f, 1.0f);
      glVertex3d(0.8, 0.4, 0);//arriba izquierda
      glTexCoord2f(1.0f, 1.0f);
      glVertex3d(-0.8, 0.4, 0);//arriba derecha
      glTexCoord2f(1.0f, 0.0f);
      glVertex3d(-0.8, -1., 0);//abajo derecha
      glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void random_coin(){
  for (j = 0; j < totalCoins; j++) {
    glPushMatrix();
    glTranslatef(2.0, 0, j * dist);
    drawCoins();
    glPopMatrix();

  }
}

void drawSnowMan()
{
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
		glColor3f(1.0f, 1, 1);

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
    timer = glutGet(GLUT_ELAPSED_TIME);
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
        if(escudo && cont_escudo<max_escudo){
          cont_escudo +=1;
          sizea= sizea/1.5;
          sizeb= sizeb/1.5;
          cout<<"Redusco escudo "<<cont_escudo<<endl;
          if(cont_escudo==2)
            uso_escudos+=1;
        }
        else{
          exit(0);
          pthread_exit(&thread1);
          system("canberra-gtk-play -f sonidos/explosion.wav &");

        }
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
    //random_coin();

    for (j = 0; j < total; j++) {
      srand(randomn+j);		//use j-1 or j+1 to change order of snowman
	    arr[j] = (rand() % 3) - 1;	//-1,0,1
	    //cout<<arr[j]<<"   "<<j<<endl;
	    //drawtree(-5.0f, 0, j * dist);
	    for (int i = -1; i < 2; i++) {	//should start from -1 to place snowmen at center
	      glPushMatrix();
	      if (arr[j] != i) {

		      glTranslatef(i * 2.0, 0, j * dist);
					if(i==-1)
          	drawTree();
					if(i==0)
						drawSnowMan();
					if(i==1)
						drawCoins();
					/*if(arr[j]==0)
						drawSnowMan();*/
	      }
	    glPopMatrix();
	    }
    }
    runner();
    drawScore();
    drawTime();
    background();
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
  else if(key == 'e'){
    uso_escudos+=1;
    if(uso_escudos < total_escudos*2)
      escudo = !escudo;
    else
      escudo = false;
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
         cout<<"Echado"<<endl;
         break;
	break;
    }


}

int main(int argc, char **argv)
{

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("SnowRun");
    //penguin
    sprites = TextureManager::Inst()->LoadTexture("texturas/penguin.png", GL_BGRA_EXT, GL_RGBA);
    segurity = TextureManager::Inst()->LoadTexture("texturas/escudo.png", GL_BGRA_EXT, GL_RGBA);
    snow = TextureManager::Inst()->LoadTexture("texturas/snow.png", GL_BGRA_EXT, GL_RGBA);
    
    pista = TextureManager::Inst()->LoadTexture("pista.jpg", GL_BGR_EXT, GL_RGB);
    pared = TextureManager::Inst()->LoadTexture("pared.jpeg", GL_BGR_EXT, GL_RGB);
    sky = TextureManager::Inst()->LoadTexture("texturas/montania.jpg", GL_BGR_EXT, GL_RGB);
    wood = TextureManager::Inst()->LoadTexture("madera.jpg", GL_BGR_EXT, GL_RGB);
    brick = TextureManager::Inst()->LoadTexture("ladrillo.jpg", GL_RGB, GL_RGB);
    roof = TextureManager::Inst()->LoadTexture("techo.jpg", GL_RGB, GL_RGB);
    grass = TextureManager::Inst()->LoadTexture("texturas/hojasnieve.jpg", GL_BGR_EXT, GL_RGB);
    coin = TextureManager::Inst()->LoadTexture("texturas/coin.png", GL_BGRA_EXT, GL_RGBA);

    srand(time(NULL));
	  randomn = 2;// (rand() % 3) - 1;

    int iret1;
    const char *message1 = "Thread 1";
    pthread_create( &thread1, NULL, background_sound, (void*) message1);


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
