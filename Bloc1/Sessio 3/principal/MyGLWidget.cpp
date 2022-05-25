
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>


MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}
void MyGLWidget::keyPressEvent (QKeyEvent *e) {
  makeCurrent ();
  switch ( e->key() ) {
    case Qt::Key_Up:
      ty += 0.1;
      rotacion += M_PI/4;
      modelTransform();
      break;
    case Qt::Key_Down:
      ty -= 0.1;
      rotacion += M_PI/4;
      modelTransform();
      break;
    case Qt::Key_Right:
      tx += 0.1;
      rotacion += M_PI/4;
      modelTransform();
      break;
    case Qt::Key_Left:
      tx -= 0.1;
      rotacion += M_PI/4;
      modelTransform();
      break;
    case Qt::Key_S:
      sclx += 0.1;
      scly += 0.05;
      modelTransform();
      break;
    case Qt::Key_D:
      sclx -= 0.05;
      scly -= 0.1;
      modelTransform();
      break;
    default: e->ignore (); // propagar al pare
  }
  update ();
}
void MyGLWidget::modelTransform () {
  glm::mat4 TG (1.0); // Matriu de transformació, inicialment identitat
  TG = glm::translate (TG, glm::vec3 (tx, ty, 0.0));
  TG = glm::rotate (TG, rotacion, glm::vec3 (0.0, 0.0, 1.0));
  TG = glm::scale (TG, glm::vec3 (sclx, scly, 0.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::paintGL () {
  glClear (GL_COLOR_BUFFER_BIT); // Esborrem el frame-buffer
  modelTransform();
  glBindVertexArray (VAO1);
  glDrawArrays (GL_TRIANGLES, 0, 3);
  glBindVertexArray (0);
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
#endif
}

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  // Unim el VAO que hem creat a la view
  glBindVertexArray(VAO1);
  //Creem el VBO1
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  // Unim el VBO que hem creat a la view
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  // Introduim les dades al buffer (els vertex)
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  transLoc = glGetUniformLocation (program->programId(), "TG");
}
