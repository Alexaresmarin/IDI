#include "NouGLWidget.h"

#include <iostream>

NouGLWidget::NouGLWidget(QWidget *parent) : MyGLWidget(parent) {
  grabKeyboard();
}

NouGLWidget::~NouGLWidget ()
{
}

void NouGLWidget::Ortho() {
  makeCurrent();
  ortho = true;
  viewTransform();
  projectTransform();
  update();
}

void NouGLWidget::Pers() {
  makeCurrent();
  ortho = false;
  viewTransform();
  projectTransform();
  update();
}

void NouGLWidget::initializeGL() {
  MyGLWidget::initializeGL();
  movx = 0;
  magenta = false;
  ortho = false;
  emit ActivarPers();
  colFocusLoc = glGetUniformLocation (program->programId(), "colFocus");
  colFocus = glm::vec3(0.8);
  glUniform3fv(colFocusLoc, 1, &colFocus[0]);  
}

void NouGLWidget::paintGL() {
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  #ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv (GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
  #endif
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //--------------------------------------------------------
  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);  
  // pintem terra
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 12);

  //--------------------------------------------------------
  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Pat);
  // pintem el Patricio
  modelTransformPatricio();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  modelTransformPatricio2();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  modelTransformPatricio3();
  glDrawArrays(GL_TRIANGLES, 0, patModel.faces().size()*3);

  //--------------------------------------------------------
  glBindVertexArray(0);
}

void NouGLWidget::iniCamera ()
{
  rav = 1.0;
  angleY = 0.0;
  angleX = float(M_PI/6.f);

  projectTransform ();
  viewTransform ();}

void NouGLWidget::iniEscena ()
{
  MyGLWidget::iniEscena();
  centreEsc = glm::vec3 (5, 0, 5);
  radiEsc = glm::distance(centreEsc, glm::vec3(0));
}

void NouGLWidget::modelTransformPatricio ()
{
  // Codi per a la TG necessària
  glm::mat4 patTG = glm::mat4(1.0f);
  patTG = glm::scale(patTG, glm::vec3(2.5*escalaPat));
  patTG = glm::translate(patTG, glm::vec3(7+movx,0,5));
  patTG = glm::rotate(patTG, -float(M_PI/2.f), glm::vec3(0,1,0));
  patTG = glm::translate(patTG, -centreBasePat);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::modelTransformPatricio2 ()
{
  // Codi per a la TG necessària
  glm::mat4 patTG = glm::mat4(1.0f);
  patTG = glm::scale(patTG, glm::vec3(2.5*escalaPat));
  patTG = glm::translate(patTG, glm::vec3(8+movx,0,5));
  patTG = glm::rotate(patTG, -float(M_PI/2.f), glm::vec3(0,1,0));
  patTG = glm::translate(patTG, -centreBasePat);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::modelTransformPatricio3 ()
{
  // Codi per a la TG necessària
  glm::mat4 patTG = glm::mat4(1.0f);
  patTG = glm::scale(patTG, glm::vec3(2.5*escalaPat));
  patTG = glm::translate(patTG, glm::vec3(9+movx,0,5));
  patTG = glm::rotate(patTG, -float(M_PI/2.f), glm::vec3(0,1,0));
  patTG = glm::translate(patTG, -centreBasePat);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &patTG[0][0]);
}

void NouGLWidget::viewTransform() {
  if (ortho) View = glm::lookAt(glm::vec3(centreEsc.x,2*radiEsc,centreEsc.z), centreEsc, glm::vec3(1,0,0));
  else {
    // Matriu de posició i orientació
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, angleX, glm::vec3(1,0,0));
    View = glm::rotate(View, -angleY, glm::vec3(0,1,0));
    View = glm::translate(View, -centreEsc);
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void NouGLWidget::projectTransform() {
  float fov, zn, zf, l, r, b, t;
  glm::mat4 Proj;  // Matriu de projecció
  
  fov = float(M_PI/3.0);
  zn = radiEsc;
  zf = 3*radiEsc;

  l = -radiEsc;
  r = radiEsc;
  b = -radiEsc;
  t = radiEsc;

  if (ortho) Proj = glm::ortho(l,r,b,t,zn,zf);
  else Proj = glm::perspective(fov, rav, zn, zf);
  
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void NouGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: {

      break;
    }
    case Qt::Key_W: {

      break;
    }
    case Qt::Key_A: {
      if (7+movx > 0) {
        movx -= 0.5;
        if (movx+7 == 0) {
          magenta = true;
          colFocus = glm::vec3(0.8,0,0.8);
          glUniform3fv(colFocusLoc, 1, &colFocus[0]); 
        }
        else {
          colFocus = glm::vec3(0.8);
          glUniform3fv(colFocusLoc, 1, &colFocus[0]); 
        }
      } 
      break;
    }
    case Qt::Key_D: {
      if (9+movx < 20) {
        movx += 0.5;
        if (movx+9 == 20) {
          magenta = true;
          colFocus = glm::vec3(0.8,0,0.8);
          glUniform3fv(colFocusLoc, 1, &colFocus[0]); 
        }
        else {
          colFocus = glm::vec3(0.8);
          glUniform3fv(colFocusLoc, 1, &colFocus[0]); 
        }
      }
      break;
    }
    case Qt::Key_C: {
        ortho = !ortho;
        if (ortho) emit ActivarOrtho();
        else emit ActivarPers();
        viewTransform();
        projectTransform();
      break;
    }
    default: {
      MyGLWidget::keyPressEvent(event);
      break;
    }
  }
  update();
}

  
