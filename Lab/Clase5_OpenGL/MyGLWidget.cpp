// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <vector>

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
void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // escalar a més gran
      rotacio += float(45*M_PI/180);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::calculCentre(glm::vec3 min, glm::vec3 max) {
  centre = glm::vec3(float((min[0] + max[0])/2), float((min[1] + max[1])/2), float((min[2] + max[2])/2));
  radi = glm::distance(min, max)/2;
  angle = glm::asin(float(radi)/float(radi*1.5f));
}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL ();
    calculCentre(glm::vec3 (-2, -1, -2), glm::vec3 (2, 1, 2));
    viewTransform();
    projectTransform();
    
    glEnable (GL_DEPTH_TEST);
}

void MyGLWidget::modelTransform(){
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(escala));
  transform = glm::rotate (transform, rotacio, glm::vec3 (0.0, 1.0, 0.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}
void MyGLWidget::modelTransformTerra(){
  // Matriu de transformació de model
   // Matriu de transformació de model
  glm::mat4 transform (1.0);
  transform = glm::scale(transform, glm::vec3(escala));
  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::paintGL () {
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO);

    modelTransform();
    // pintem
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (VAOTerra);

    modelTransformTerra();

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::creaBuffers () 
{
  // Dades de la caseta
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 terra[6] = {
	glm::vec3( 2.0,  -1.0,  -2.0),
	glm::vec3(-2.0,  -1.0,  -2.0),
	glm::vec3(-2.0, -1.0,  2.0),
	glm::vec3( 2.0,  -1.0,  -2.0),
	glm::vec3(-2.0, -1.0,  2.0),
	glm::vec3( 2.0, -1.0,  2.0)
  };

  glm::vec3 colorTerra[6] = {
	glm::vec3(1,0,0),
	glm::vec3(0,0,1),
  glm::vec3(0,1,0),
	glm::vec3(1,0,0),
  glm::vec3(0,1,0),
	glm::vec3(1,0,0)
  };


  m.load("./models/HomerProves.obj");

  //El VAO el faig servir per en homero
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO[2];
    glGenBuffers(2, VBO);

    //VBO[0]
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_vertices (), GL_STATIC_DRAW);
    // Activem l'atribut vpatLoc
    glVertexAttribPointer(vpatLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vpatLoc);

    //VBO[1]
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData (GL_ARRAY_BUFFER,sizeof(GLfloat) * m.faces ().size () * 3 * 3,m.VBO_matdiff (), GL_STATIC_DRAW);
    // Activem l'atribut colorLoc
    glVertexAttribPointer(cpatLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(cpatLoc);

    //El VAOTerra es per dibuixar el terra
    glGenVertexArrays(1, &VAOTerra);
    glBindVertexArray(VAOTerra);

    GLuint VBOTerra[2];
    glGenBuffers(2, VBOTerra);

    //VBOTerra[0]
    glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(terra), terra, GL_STATIC_DRAW);
    // Activem l'atribut vterraLoc
    glVertexAttribPointer(vterraLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vterraLoc);

    //VBOTerra[1]
    glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorTerra), colorTerra, GL_STATIC_DRAW);
    // Activem l'atribut colorTerraLoc
    glVertexAttribPointer(colorTerraLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorTerraLoc);

    glBindVertexArray (0);
}

void MyGLWidget::carregaShaders() { // declarem-lo també en MyGLWidget.h

    BL2GLWidget::carregaShaders(); // cridem primer al mètode de BL2GLWidget
    vpatLoc = glGetAttribLocation (program->programId(), "vertex");
    vterraLoc = glGetAttribLocation (program->programId(), "vertex");
    colorTerraLoc = glGetAttribLocation (program->programId(), "color");
    cpatLoc = glGetAttribLocation (program->programId(), "color");
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::resizeGL (int w, int h) 
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
  ra = float(ample)/float(alt);
  projectTransform();
#endif
}


void MyGLWidget::viewTransform () {
// glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (glm::vec3(centre[0],centre[1],1.5f*radi), centre, glm::vec3(0,1,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}
void MyGLWidget::projectTransform () {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective (glm::atan(float(tan(angle)/ra))*2.0f, ra, float(1.5*radi-radi), float(1.5f*radi+radi));
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

MyGLWidget::~MyGLWidget() {
} 