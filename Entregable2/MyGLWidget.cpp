#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{

}


//ALBERT CARDONA ZAERA





void MyGLWidget::iniCamera(){

    //Capsa Contenidora Escena: max-->(10,4,7); min -->(-10,0,-7)
    Vmax = glm::vec3(10,4,7);
    Vmin = glm::vec3(-10,0,-7);
    radiEscena = glm::distance(Vmin,Vmax)/2;


    ra  = 1.0;
    
    d = 2*radiEscena;
    znear = radiEscena;
    zfar = 3*radiEscena;

    FOVini = fov = 2.0f*(glm::asin(float(radiEscena)/float(d)));

    eX = float(M_PI)/4.0f;
    eY = 0;


    obs=glm::vec3(0,d,0);
    vrp=glm::vec3(0,0,0);
    up=glm::vec3(-1,0,0);

    GOL = false;
    cameraPers=1;

  viewTransform();
  projectTransform();
}




void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Patricio
  glBindVertexArray (VAO_Patr);
  patrTransform();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pilota
  if (not GOL){
    glBindVertexArray (VAO_Pil);
    pilTransform();
    glDrawArrays(GL_TRIANGLES, 0, pil.faces().size()*3);
  }
  

  // Paret1
  glBindVertexArray (VAO_Cub);
  paret1Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Paret2
  glBindVertexArray (VAO_Cub);
  paret2Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);

    // Paret3
  glBindVertexArray (VAO_Cub);
  paret3Transform();
  glDrawArrays(GL_TRIANGLES, 0, 36);


  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

    if(DoingInteractive == ROTATE and cameraPers){
        float difX = e->x() - xClick;
        float difY = yClick - e ->y() ;
        eY -= difX * factorAngleY;
        eX -= difY * factorAngleX;
        viewTransform();
    }

    xClick = e ->x();
    yClick = e ->y();
  update ();
}


void MyGLWidget::rebotaParets() 
{
    //Paret3
  if (posPilota[0] <= -9.8 + radiPil){
    dirPilota[0] = -dirPilota[0];
    posPilota[0] = -9.7+radiPil;
  }
  //Paret1
  else if (posPilota[2] <=-6.8 +radiPil){
    dirPilota[2]=-dirPilota[2];
    posPilota[2] = -6.7+radiPil;
  }
  //Paret2
  else if (posPilota[2] >= 6.8-radiPil){
    dirPilota[2]=-dirPilota[2];
    posPilota[2] = 6.7-radiPil;
  }

} 


void MyGLWidget::tractamentGol()
{
  GOL=true;
  timer.stop();
} 



void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { // moviment de la pilota
      if (posPilota[0] == 7.0)
        mourePilota ();
      break;
    }
    case Qt::Key_I: { // reinicia posició pilota
      iniciPilota ();
      GOL = false;
      break;
    }
    case Qt::Key_Left: { //moviment patricio
      if (posPorter[2] < (7-(altPorter/2))){
        posPorter[2] += 0.5;
      }
      break;
    }
    case Qt::Key_Right: { //moviment patricio
      if (posPorter[2] > -(7-(altPorter/2))){
        posPorter[2] -= 0.5;
      }
      break;
    }
    case Qt::Key_C: { //Camera pers o ortho
      cameraPers=(cameraPers+1)%2;
      viewTransform();
      projectTransform();
      break;
    }
    case Qt::Key_W: { //altura patricio
      if (altPorter < 6) altPorter++;
      break;
    }
    case Qt::Key_S: { //altura patricio
      if (altPorter > 2) altPorter--;
      break;
    }
    case Qt::Key_R: { //reiniciar
      LL2GLWidget::initializeGL();
      LL2GLWidget::iniciPilota();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersPil (){
  LL2GLWidget::creaBuffersPil();
  radiPil = ((1/escalaPil)/2)*escalaPil;
}

void MyGLWidget::patrTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posPorter);
  TG = glm::rotate(TG, float(-M_PI)/2.0f,glm::vec3(0,1,0));
  TG = glm::scale(TG,glm::vec3(escalaPatr*altPorter,escalaPatr*altPorter,escalaPatr*altPorter));
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paret1Transform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(0,0,-6.9));
  TG = glm::scale(TG,glm::vec3(20,2,0.2));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::paret2Transform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(0,0,6.9));
  TG = glm::scale(TG,glm::vec3(20,2,0.2));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::paret3Transform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(-9.9,0,0));
  TG = glm::scale(TG,glm::vec3(0.2,2,14));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);

  if(cameraPers){
    View = translate(View, glm::vec3(0,0,-d));
    View = rotate(View, eX ,glm::vec3(1,0,0));
    View = rotate(View, -eY ,glm::vec3(0,1,0));
    View = translate(View, -vrp);
  }
  else {
    View = glm::lookAt(obs,vrp,up);
  }
 
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);

  if (cameraPers){
    if(ra > 1) fov=FOVini;
    else fov = 2*atan(tan(FOVini/2)/ra);

    Proj = glm::perspective (fov, ra, znear, zfar);
  }
  else{
    if (ra >= 1) Proj = glm::ortho(-ra*radiEscena, ra*radiEscena, -radiEscena, radiEscena, znear, zfar);
    else Proj = glm::ortho(-radiEscena, radiEscena, -radiEscena/ra, radiEscena/ra, znear, zfar);
  }
    
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}